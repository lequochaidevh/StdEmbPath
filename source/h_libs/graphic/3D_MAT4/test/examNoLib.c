#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 40
float NEAR = -0.1;
float FAR = -100;
typedef struct { float x,y,z; } Vec3;
typedef struct { int a,b; } Edge;

char screen[HEIGHT][WIDTH];

// Đỉnh khối lập phương (cạnh 2 đơn vị, tâm tại gốc)
Vec3 cubeVertices[8] = {
    {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1},
    {-1,-1,1},  {1,-1,1},  {1,1,1},  {-1,1,1}
};

// Cạnh nối các đỉnh
Edge cubeEdges[12] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};

void clearScreen() {
    for(int y=0; y<HEIGHT; y++)
        for(int x=0; x<WIDTH; x++)
            screen[y][x] = ' ';
}

void renderScreen() {
    printf("\x1b[H");
    for(int y=0; y<HEIGHT; y++) {
        for(int x=0; x<WIDTH; x++) {
            putchar(screen[y][x]);
        }
        putchar('\n');
    }
}

void putPixel(int x, int y, char c) {
    if (x>=0 && x<WIDTH && y>=0 && y<HEIGHT)
        screen[y][x] = c;
}

// Bresenham line
void drawLine(int x0, int y0, int x1, int y1, char c) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while(1) {
        putPixel(x0,y0,c);
        if(x0 == x1 && y0 == y1) break;
        e2 = 2*err;
        if(e2 >= dy) { err += dy; x0 += sx; }
        if(e2 <= dx) { err += dx; y0 += sy; }
    }
}

// Xoay quanh trục Y
Vec3 rotateY(Vec3 v, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    return (Vec3){
        v.x * c + v.z * s,
        v.y,
        -v.x * s + v.z * c
    };
}

// Chiếu perspective chuẩn
Vec3 project(Vec3 v, float fov_degrees, float viewerDistance) {
    float fov_rad = fov_degrees * (M_PI / 180.0f);
    float z = viewerDistance + v.z;
    if (z < 0.1f) z = 0.1f;  // tránh chia 0

    float factor = 1.0f / tanf(fov_rad / 2.0f);
    float scale = factor / z;

    return (Vec3){ v.x * scale, v.y * scale, v.z };
}

int clamp(int val, int min, int max) {
    if(val < min) return min;
    if(val > max) return max;
    return val;
}

int main() {
    float angle = 0.0f;
    float fov = 90.0f;          // Góc nhìn 90 độ
    float viewerDistance = 3.0f;

    printf("\x1b[2J");        // Xóa màn hình terminal
    printf("\x1b[?25l");       // Ẩn con trỏ

    while(1) {
        clearScreen();

        Vec3 projected[8];

        for(int i=0; i<8; i++) {
            Vec3 rotated = rotateY(cubeVertices[i], angle);
            projected[i] = project(rotated, fov, viewerDistance);
        }

        for(int i=0; i<12; i++) {
            Edge e = cubeEdges[i];

            int x0 = clamp((int)((projected[e.a].x + 1) * (WIDTH / 2)), 0, WIDTH - 1);
            int y0 = clamp((int)((1 - projected[e.a].y) * (HEIGHT / 2)), 0, HEIGHT - 1);
            int x1 = clamp((int)((projected[e.b].x + 1) * (WIDTH / 2)), 0, WIDTH - 1);
            int y1 = clamp((int)((1 - projected[e.b].y) * (HEIGHT / 2)), 0, HEIGHT - 1);

            if(x0 == x1 && y0 == y1) {
                putPixel(x0, y0, '*');
            } else {
                drawLine(x0, y0, x1, y1, '*');
            }
        }

        renderScreen();

        angle += 0.05f;
        if(angle > 2*M_PI) angle -= 2*M_PI;

        usleep(30000);
    }

    printf("\x1b[?25h");  // Hiện con trỏ lại khi thoát

    return 0;
}

