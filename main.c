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
