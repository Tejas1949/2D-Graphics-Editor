#include <stdio.h>
#include <stdlib.h>
#define ROWS 20
#define COLS 60
#define MAX_SHAPES 100

typedef enum {
    SHAPE_LINE,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE
} ShapeType;
typedef struct {
    int x1, y1;
    int x2, y2;
} LineParams;
typedef struct {
    int x1, y1;
    int x2, y2;
} RectParams;
typedef struct {
    int cx, cy;
    int r;
} CircleParams;
typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriangleParams;
typedef struct {
    ShapeType type;
    union {
        LineParams line;
        RectParams rect;
        CircleParams circle;
        TriangleParams triangle;
    } data;
} Shape;
Shape shapes[MAX_SHAPES];
int shape_count = 0;
// Global canvas buffer
char canvas[ROWS][COLS];

void canvas_clear(void) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            canvas[y][x] = '_';
        }
    }
}
/**
 * Displays the canvas on the standard output with grid coordinate headers.
 */
void canvas_display(void) {
    printf("\n");
    // Print column coordinate headers (tens digit)
    printf("   ");
    for (int x = 0; x < COLS; x++) {
        if (x % 10 == 0) {
            printf("%d", x / 10);
        } else {
            printf(" ");
        }
    }
    printf("\n");
    // Print column coordinate headers (ones digit)
    printf("   ");
    for (int x = 0; x < COLS; x++) {
        printf("%d", x % 10);
    }
    printf("\n");
    // Print rows with row header
    for (int y = 0; y < ROWS; y++) {
        printf("%2d ", y);
        for (int x = 0; x < COLS; x++) {
            putchar(canvas[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}
void canvas_draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;
    while (1) {
        if (x0 >= 0 && x0 < COLS && y0 >= 0 && y0 < ROWS) {
            canvas[y0][x0] = '*';
        }
        if (x0 == x1 && y0 == y1) {
            break;
        }
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}
/**
 * Draws a rectangle outline with corners at (x1, y1) and (x2, y2).
 */
void canvas_draw_rect(int x1, int y1, int x2, int y2) {
    canvas_draw_line(x1, y1, x2, y1);
    canvas_draw_line(x2, y1, x2, y2);
    canvas_draw_line(x2, y2, x1, y2);
    canvas_draw_line(x1, y2, x1, y1);
}
/**
 * Helper to draw 8-way symmetric points for circle outline.
 */
static void draw_circle_points(int cx, int cy, int x, int y) {
    int points[8][2] = {
        {cx + x, cy + y}, {cx - x, cy + y}, {cx + x, cy - y}, {cx - x, cy - y},
        {cx + y, cy + x}, {cx - y, cy + x}, {cx + y, cy - x}, {cx - y, cy - x}
    };
    for (int i = 0; i < 8; i++) {
        int px = points[i][0];
        int py = points[i][1];
        if (px >= 0 && px < COLS && py >= 0 && py < ROWS) {
            canvas[py][px] = '*';
        }
    }
}
**
 * Draws a circle outline using Midpoint Circle algorithm.
 */
void canvas_draw_circle(int cx, int cy, int r) {
    if (r < 0) return;
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    draw_circle_points(cx, cy, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        draw_circle_points(cx, cy, x, y);
    }
}
/**
 * Draws a triangle outline by connecting three vertices.
 */
void canvas_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    canvas_draw_line(x1, y1, x2, y2);
    canvas_draw_line(x2, y2, x3, y3);
    canvas_draw_line(x3, y3, x1, y1);
}
/* ==========================================
 * Shape List Operations
 * ========================================== */
void shapes_print_list(void) {
    if (shape_count == 0) {
        printf("   No objects in the list.\n");
        return;
    }
    printf("--- Current Objects ---\n");
    for (int i = 0; i < shape_count; i++) {
        printf("  [%d] ", i + 1);
        switch (shapes[i].type) {
            case SHAPE_LINE:
                printf("Line: (%d, %d) to (%d, %d)\n",
                       shapes[i].data.line.x1, shapes[i].data.line.y1,
                       shapes[i].data.line.x2, shapes[i].data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                printf("Rectangle: Corner 1 (%d, %d), Corner 2 (%d, %d)\n",
                       shapes[i].data.rect.x1, shapes[i].data.rect.y1,
                       shapes[i].data.rect.x2, shapes[i].data.rect.y2);
                break;
            case SHAPE_CIRCLE:
                printf("Circle: Center (%d, %d), Radius %d\n",
                       shapes[i].data.circle.cx, shapes[i].data.circle.cy,
                       shapes[i].data.circle.r);
                break;
            case SHAPE_TRIANGLE:
                printf("Triangle: V1(%d, %d), V2(%d, %d), V3(%d, %d)\n",
                       shapes[i].data.triangle.x1, shapes[i].data.triangle.y1,
                       shapes[i].data.triangle.x2, shapes[i].data.triangle.y2,
                       shapes[i].data.triangle.x3, shapes[i].data.triangle.y3);
                break;
        }
    }
}
void shapes_render(void) {
    canvas_clear();
    for (int i = 0; i < shape_count; i++) {
        switch (shapes[i].type) {
            case SHAPE_LINE:
                canvas_draw_line(shapes[i].data.line.x1, shapes[i].data.line.y1,
                                 shapes[i].data.line.x2, shapes[i].data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                canvas_draw_rect(shapes[i].data.rect.x1, shapes[i].data.rect.y1,
                                 shapes[i].data.rect.x2, shapes[i].data.rect.y2);
                break;
            case SHAPE_CIRCLE:
                canvas_draw_circle(shapes[i].data.circle.cx, shapes[i].data.circle.cy,
                                   shapes[i].data.circle.r);
                break;
            case SHAPE_TRIANGLE:
                canvas_draw_triangle(shapes[i].data.triangle.x1, shapes[i].data.triangle.y1,
                                     shapes[i].data.triangle.x2, shapes[i].data.triangle.y2,
                                     shapes[i].data.triangle.x3, shapes[i].data.triangle.y3);
                break;
        }
    }
}
void shapes_delete(int index) {
    if (index < 0 || index >= shape_count) {
        return;
    }
    for (int i = index; i < shape_count - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    shape_count--;
}
