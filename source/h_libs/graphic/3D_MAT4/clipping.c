// It has been completed yet

#include <stdbool.h>

typedef struct {
    float x, y, z;
} Vec3;

// Clip line theo near/far trên trục Z (clip space, trước chia w)
// Trả về true nếu còn phần nằm trong frustum
// Nếu clip, p0 và p1 sẽ được chỉnh lại
bool clipLineNearFar(Vec3* p0, Vec3* p1, float near, float far) {
    // Giả sử near < far, z nằm trong [near, far]

    // Tính param t cắt đoạn p0->p1 với near plane
    if (p0->z > far && p1->z > far) return false; // toàn đoạn ngoài far
    if (p0->z < near && p1->z < near) return false; // toàn đoạn ngoài near

    // Hàm phụ tính giao điểm t với plane z = planeZ
    float intersectT(float z0, float z1, float planeZ) {
        return (planeZ - z0) / (z1 - z0);
    }

    // Clip near plane
    if (p0->z < near) {
        float t = intersectT(p0->z, p1->z, near);
        p0->x += (p1->x - p0->x) * t;
        p0->y += (p1->y - p0->y) * t;
        p0->z = near;
    }
    if (p1->z < near) {
        float t = intersectT(p1->z, p0->z, near);
        p1->x += (p0->x - p1->x) * t;
        p1->y += (p0->y - p1->y) * t;
        p1->z = near;
    }

    // Clip far plane
    if (p0->z > far) {
        float t = intersectT(p0->z, p1->z, far);
        p0->x += (p1->x - p0->x) * t;
        p0->y += (p1->y - p0->y) * t;
        p0->z = far;
    }
    if (p1->z > far) {
        float t = intersectT(p1->z, p0->z, far);
        p1->x += (p0->x - p1->x) * t;
        p1->y += (p0->y - p1->y) * t;
        p1->z = far;
    }

    return true;
}

float depthBuffer[WIDTH * HEIGHT];

void clearDepthBuffer() {
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        depthBuffer[i] = 1.0f; // giá trị max depth (far)
}

void putPixelDepth(int x, int y, float depth, char c) {
    int idx = y * WIDTH + x;
    if (depth < depthBuffer[idx]) { // depth nhỏ hơn là gần hơn
        depthBuffer[idx] = depth;
        putPixel(x, y, c);
    }
}

void drawLineDepth(int x0, int y0, float z0, int x1, int y1, float z1, char c) {
    // Dùng Bresenham đơn giản, kèm nội suy z theo t
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    int length = (dx > -dy) ? dx : -dy;

    for (int i = 0; ; i++) {
        float t = (length == 0) ? 0.0f : (float)i / length;
        float depth = z0 + t * (z1 - z0);

        putPixelDepth(x0, y0, depth, c);

        if (x0 == x1 && y0 == y1) break;
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


Vec3 p0 = projected[e.a];
Vec3 p1 = projected[e.b];

if (!clipLineNearFar(&p0, &p1, near, far)) continue;

// Tính tọa độ màn hình và depth
int x0 = (int)((p0.x + 1) * WIDTH / 2);
int y0 = (int)((1 - p0.y) * HEIGHT / 2);
int x1 = (int)((p1.x + 1) * WIDTH / 2);
int y1 = (int)((1 - p1.y) * HEIGHT / 2);

// depth ở đây có thể lấy z trong NDC hoặc sau projection (tính tùy cách bạn xử lý)
float z0 = (p0.z + 1) / 2.0f; // map z từ [-1,1] sang [0,1]
float z1 = (p1.z + 1) / 2.0f;

drawLineDepth(x0, y0, z0, x1, y1, z1, '*');
