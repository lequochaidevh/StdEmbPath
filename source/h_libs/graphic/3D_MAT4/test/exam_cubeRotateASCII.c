//build method 1: gcc exam_cubeRotateASCII.c ../matrixCalcBase.c -o exam -lm -g
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "../matrixCalcBase.h"

#define WIDTH 80
#define HEIGHT 40

float NEAR = 0.1f;
float FAR = 100.0f;
char screen[HEIGHT][WIDTH];

typedef struct
{
    int a, b; // vertices 2 to make edge
} Edge;

Vec3 cubeVertices[8] = {
    {-1, -1, -1}, // 0
    {1, -1, -1},  // 1
    {1, 1, -1},   // 2
    {-1, 1, -1},  // 3
    {-1, -1, 1},  // 4
    {1, -1, 1},   // 5
    {1, 1, 1},    // 6
    {-1, 1, 1},   // 7
};

Edge cubeEdges[12] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, // bottom face
    {4, 5},
    {5, 6},
    {6, 7},
    {7, 4}, // top face
    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7} // connective edges
};

void clearScreen()
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            screen[y][x] = ' ';
}

void renderScreen()
{
    printf("\x1b[H"); // move to start point in terminal
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            putchar(screen[y][x]);
        }
        putchar('\n');
    }
}

void putPixel(int x, int y, char c)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        screen[y][x] = c;
    }
}

// Bresenham line
void drawLine(int x0, int y0, int x1, int y1, char c)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1)
    {
        putPixel(x0, y0, c);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

Vec3 rotateY_nolib(Vec3 v, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    return (Vec3){
        v.x * c + v.z * s,
        v.y,
        -v.x * s + v.z * c};
}

Vec3 rotateY_lib(Vec3 v, float angle)
{
    Mat4 ry_mat4 = mat4_rotation_y(angle);
    Vec3 result = mat4_mul_vec3(ry_mat4, v);
    return result;
}

Vec3 (*rotateY)(Vec3, float) = &rotateY_lib;
Vec3 rotateX(Vec3 v, float angle)
{
    Mat4 rx_mat4 = mat4_rotation_x(angle);
    Vec3 result = mat4_mul_vec3(rx_mat4, v);
    return result;
}


Vec3 project_nolib(Vec3 v, float fov_degrees, float viewerDistance)
{
    float fov_rad = fov_degrees * (M_PI / 180.0f);
    float z = viewerDistance + v.z;
    if (z < 0.1f)
        z = 0.1f; // tránh chia 0

    float factor = 1.0f / tanf(fov_rad / 2.0f);
    float scale = factor / z;

    return (Vec3){v.x * scale, v.y * scale, v.z};
}
// Refer perspective
Vec3 project_lib(Vec3 v, float fov_degrees, float viewerDistance)
{
    float fov_rad = fov_degrees * (M_PI / 180.0f);
    float aspectRatio = 1; //(float)WIDTH / (float)HEIGHT;
    float near = NEAR;
    float far = FAR;
    Vec3 translatedPoint = {v.x, v.y, v.z - viewerDistance};
    //printf("z = %.3f  --- ", translatedPoint.z);
    Vec3 result = projectionPerspectiveR(translatedPoint, fov_rad, aspectRatio, near, far);

    return result;
}

Vec3 (*project)(Vec3, float, float) = &project_lib;

int clamp(int val, int min, int max)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

int test()
{
    float angle = 0.0f;
    float fov = 90.0f; // View 90
    float viewerDistance = 2.8f;

    printf("\x1b[2J");   // Clear terminal
    printf("\x1b[?25l"); // Hiden cursor
    int break_time = 0;
    while (break_time < 90)
    {
        clearScreen();
        Vec3 projected[8];
        printf("\n");
        for (int i = 0; i < 8; i++)
        {
            Vec3 rotated = rotateY(cubeVertices[i], angle);
            rotated = rotateX(rotated, angle);
            projected[i] = project(rotated, fov, viewerDistance);
        }

        for (int i = 0; i < 12; i++)
        {
            Edge e = cubeEdges[i];

            int x0 = clamp((int)((projected[e.a].x + 1) * (WIDTH / 2)), 0, WIDTH - 1);
            int y0 = clamp((int)((1 - projected[e.a].y) * (HEIGHT / 2)), 0, HEIGHT - 1);
            int x1 = clamp((int)((projected[e.b].x + 1) * (WIDTH / 2)), 0, WIDTH - 1);
            int y1 = clamp((int)((1 - projected[e.b].y) * (HEIGHT / 2)), 0, HEIGHT - 1);

            if (x0 == x1 && y0 == y1)
            {
                putPixel(x0, y0, '*');
            }
            else
            {
                drawLine(x0, y0, x1, y1, '*');
            }
        }

        renderScreen();

        angle += 0.05f;
        if (angle > 2 * M_PI)
            angle -= 2 * M_PI;

        usleep(80 * 1000);
        break_time++;
    }

    printf("\x1b[?25h"); // Appear cursor before exit
}

int main()
{
    test();
    return 0;
}
