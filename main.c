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
/* ==========================================
 * Interactive Input Validation Helpers
 * ========================================== */
int get_int(const char *prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("   Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("   Out of bounds. Must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        return value;
    }
}
void prompt_shape_details(Shape *s) {
    printf("   Select Shape Type:\n");
    printf("     1. Line\n");
    printf("     2. Rectangle\n");
    printf("     3. Circle\n");
    printf("     4. Triangle\n");
    int choice = get_int("   Choice (1-4): ", 1, 4);
    switch (choice) {
        case 1:
            s->type = SHAPE_LINE;
            s->data.line.x1 = get_int("   Enter Point 1 X (0-59): ", 0, COLS - 1);
            s->data.line.y1 = get_int("   Enter Point 1 Y (0-19): ", 0, ROWS - 1);
            s->data.line.x2 = get_int("   Enter Point 2 X (0-59): ", 0, COLS - 1);
            s->data.line.y2 = get_int("   Enter Point 2 Y (0-19): ", 0, ROWS - 1);
            break;
        case 2:
            s->type = SHAPE_RECTANGLE;
            s->data.rect.x1 = get_int("   Enter Corner 1 X (0-59): ", 0, COLS - 1);
            s->data.rect.y1 = get_int("   Enter Corner 1 Y (0-19): ", 0, ROWS - 1);
            s->data.rect.x2 = get_int("   Enter Corner 2 X (0-59): ", 0, COLS - 1);
            s->data.rect.y2 = get_int("   Enter Corner 2 Y (0-19): ", 0, ROWS - 1);
            break;
        case 3:
            s->type = SHAPE_CIRCLE;
            s->data.circle.cx = get_int("   Enter Center X (0-59): ", 0, COLS - 1);
            s->data.circle.cy = get_int("   Enter Center Y (0-19): ", 0, ROWS - 1);
            s->data.circle.r = get_int("   Enter Radius (0-40): ", 0, 40);
            break;
        case 4:
            s->type = SHAPE_TRIANGLE;
            s->data.triangle.x1 = get_int("   Enter Vertex 1 X (0-59): ", 0, COLS - 1);
            s->data.triangle.y1 = get_int("   Enter Vertex 1 Y (0-19): ", 0, ROWS - 1);
            s->data.triangle.x2 = get_int("   Enter Vertex 2 X (0-59): ", 0, COLS - 1);
            s->data.triangle.y2 = get_int("   Enter Vertex 2 Y (0-19): ", 0, ROWS - 1);
            s->data.triangle.x3 = get_int("   Enter Vertex 3 X (0-59): ", 0, COLS - 1);
            s->data.triangle.y3 = get_int("   Enter Vertex 3 Y (0-19): ", 0, ROWS - 1);
            break;
    }
}
/* ==========================================
 * Main Loop
 * ========================================== */
int main(void) {
    canvas_clear();
    shapes_render();
    while (1) {
        printf("\n========================================\n");
        printf("       2D GRAPHICS EDITOR (C-CLI)\n");
        printf("========================================\n");
        printf("  1. Display Canvas\n");
        printf("  2. Add Object\n");
        printf("  3. Delete Object\n");
        printf("  4. Modify Object\n");
        printf("  5. Clear All Objects\n");
        printf("  6. Exit\n");
        printf("----------------------------------------\n");
        int menu_choice = get_int("Enter option (1-6): ", 1, 6);
        switch (menu_choice) {
            case 1:
                shapes_render();
                canvas_display();
                break;
            case 2:
                if (shape_count >= MAX_SHAPES) {
                    printf("   Error: Maximum object limit reached (%d).\n", MAX_SHAPES);
                } else {
                    printf("\n--- Adding New Object ---\n");
                    prompt_shape_details(&shapes[shape_count]);
                    shape_count++;
                    shapes_render();
                    printf("   Object added successfully.\n");
                }
                break;
            case 3:
                printf("\n");
                shapes_print_list();
                if (shape_count > 0) {
                    int del_idx = get_int("   Enter object number to delete: ", 1, shape_count);
                    shapes_delete(del_idx - 1);
                    shapes_render();
                    printf("   Object deleted successfully.\n");
                }
                break;
            case 4:
                printf("\n");
                shapes_print_list();
                if (shape_count > 0) {
                    int mod_idx = get_int("   Enter object number to modify: ", 1, shape_count);
                    printf("\n--- Modifying Object [%d] ---\n", mod_idx);
                    prompt_shape_details(&shapes[mod_idx - 1]);
                    shapes_render();
                    printf("   Object modified successfully.\n");
                }
                break;
            case 5:
                shape_count = 0;
                shapes_render();
                printf("   All objects cleared. Canvas reset.\n");
                break;
            case 6:
                printf("   Exiting. Thank you for using the 2D Graphics Editor!\n");
                return 0;
        }
    }
    return 0;
}
