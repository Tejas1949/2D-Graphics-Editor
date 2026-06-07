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
/* ==========================================
 * Canvas Drawing Algorithms
 * ========================================== */
/**
 * Fills the canvas with the background character '_'
 */
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
