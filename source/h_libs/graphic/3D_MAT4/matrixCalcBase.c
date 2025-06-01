#include "matrixCalcBase.h"
#include <stdio.h>
#include <string.h> // for memset
#include <math.h>

#ifdef MATRIX_CALC_DEBUG
#define MT_DEBUG printf
#else
#define MT_DEBUG
#endif

// Add vector
Vec3 vec3_add(Vec3 a, Vec3 b)
{
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

// Sub vector
Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

// Scale vector with scalar
Vec3 vec3_mul_scalar(float s, Vec3 v)
{
    return (Vec3){v.x * s, v.y * s, v.z * s};
}

// Dot vector (dot product)
float vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Vector normalization
Vec3 vec3_normalize(Vec3 v)
{
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0)
        return v;
    return vec3_mul_scalar(1.0f / length, v);
}

// init Unit Matrix 4x4
Mat4 mat4_identity()
{
    Mat4 mat;
    memset(&mat, 0, sizeof(Mat4));
    for (int i = 0; i < 4; i++)
    {
        mat.m[i][i] = 1.0f;
    }
    return mat;
}

// Multi two matrix 4x4
Mat4 mat4_mul(Mat4 a, Mat4 b)
{
    Mat4 result;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            result.m[row][col] = 0;
            for (int k = 0; k < 4; k++)
            {
                result.m[row][col] += a.m[row][k] * b.m[k][col];
            }
        }
    }
    return result;
}

/**** Init Translation Matrix ****/

Mat4 mat4_translation(float tx, float ty, float tz)
{
    Mat4 mat = mat4_identity();
    mat.m[0][3] = tx;
    mat.m[1][3] = ty;
    mat.m[2][3] = tz;
    return mat;
}

/**** Init Rotation Matrix ****/

Mat4 mat4_rotation_x(float angleRad)
{
    Mat4 mat = mat4_identity();
    float c = cosf(angleRad);
    float s = sinf(angleRad);

    mat.m[1][1] = c;
    mat.m[1][2] = -s;
    mat.m[2][1] = s;
    mat.m[2][2] = c;

    return mat;
}

Mat4 mat4_rotation_y(float angleRad)
{
    Mat4 mat = mat4_identity();
    float c = cosf(angleRad);
    float s = sinf(angleRad);

    mat.m[0][0] = c;
    mat.m[0][2] = s;
    mat.m[2][0] = -s;
    mat.m[2][2] = c;

    return mat;
}

Mat4 mat4_rotation_z(float angleRad)
{
    Mat4 mat = mat4_identity();
    float c = cosf(angleRad);
    float s = sinf(angleRad);

    mat.m[0][0] = c;
    mat.m[0][1] = -s;
    mat.m[1][0] = s;
    mat.m[1][1] = c;

    return mat;
}

// Reference perspective 3D -> 2D
// Input: vec3 (x,y,z), distance from camera to screen
// Output: vec3 (x', y', z') with z' to calculate in order of drawing or remove.
Vec3 projectionPerspectiveR(Vec3 point, float fov, float aspectRatio, float near, float far)
{
    float factor = 1.0f / tanf(fov / 2.0f);

    // float scaler = aspectRatio + point.z;
    // if (scaler < 0.1f) scaler = 0.1f;

    Mat4 proj = {0};
    proj.m[0][0] = factor / aspectRatio;
    proj.m[1][1] = factor;
    proj.m[2][2] = (far + near) / (near - far);
    proj.m[2][3] = (2 * far * near) / (near - far);
    proj.m[3][2] = -1.0f;
    proj.m[3][3] = 0;

    // Vec3 scalePoint = vec3_mul_scalar(0.2f, point);

    return mat4_mul_vec3(proj, point);
}

Vec3 projectionPerspectiveL(Vec3 point, float fov, float aspectRatio, float near, float far)
{ // DirectX/Unreal
    float f = 1.0f / tanf(fov / 2.0f);

    Mat4 proj = {0};
    proj.m[0][0] = f / aspectRatio;
    proj.m[1][1] = f;
    proj.m[2][2] = far / (far - near);
    proj.m[2][3] = (-far * near) / (far - near);
    proj.m[3][2] = 1;
    proj.m[3][3] = 0;

    // Vector4 to computing (x, y, z, w)
    float x = point.x;
    float y = point.y;
    float z = point.z;
    float w = 1.0f;

    float px = proj.m[0][0] * x + proj.m[0][1] * y + proj.m[0][2] * z + proj.m[0][3] * w;
    float py = proj.m[1][0] * x + proj.m[1][1] * y + proj.m[1][2] * z + proj.m[1][3] * w;
    float pz = proj.m[2][0] * x + proj.m[2][1] * y + proj.m[2][2] * z + proj.m[2][3] * w;
    float pw = proj.m[3][0] * x + proj.m[3][1] * y + proj.m[3][2] * z + proj.m[3][3] * w;

    if (pw != 0)
    {
        px /= pw;
        py /= pw;
        pz /= pw;
    }

    return (Vec3){px, py, pz};
}

/*
    Projection Perspective method

 * Right-Handed Coordinate System (OpenGL)

          Y (Up)
          |
          |
          |
          |
          O--------- X (Right)
         /
        /
       Z (Forward)

       Calculate method:
       proj.m[0][0] = f / aspectRatio;
       proj.m[1][1] = f;
       proj.m[2][2] = (far + near) / (near - far);
       proj.m[2][3] = (2 * far * near) / (near - far);
       proj.m[3][2] = -1;
       proj.m[3][3] = 0;

 * Left-Handed Coordinate System (DirectX/Unreal)

          Y (Up)
          |
          |
          |
          |
          O--------- X (Right)
         /
        /
       -Z (Forward)

       Calculate method:
       proj.m[0][0] = f / aspectRatio;
       proj.m[1][1] = f;
       proj.m[2][2] = far / (far - near);
       proj.m[2][3] = (-far * near) / (far - near);
       proj.m[3][2] = 1;
       proj.m[3][3] = 0;



*/
// Multi mat4 with vec3
Vec3 mat4_mul_vec3(Mat4 mat, Vec3 v)
{
    float x = v.x * mat.m[0][0] + v.y * mat.m[0][1] + v.z * mat.m[0][2] + mat.m[0][3];
    float y = v.x * mat.m[1][0] + v.y * mat.m[1][1] + v.z * mat.m[1][2] + mat.m[1][3];
    float z = v.x * mat.m[2][0] + v.y * mat.m[2][1] + v.z * mat.m[2][2] + mat.m[2][3];
    float w = v.x * mat.m[3][0] + v.y * mat.m[3][1] + v.z * mat.m[3][2] + mat.m[3][3];

    if (w != 0.0f)
    {
        x /= w;
        y /= w;
        z /= w;
    }

    return (Vec3){x, y, z};
}

// Hàm con clip 1 điểm A về gần plane clipZ
void clipPoint(Vec3 *P, Vec3 *Q, float clipZ)
{
    float t = (clipZ - P->z) / (Q->z - P->z);
    P->x += t * (Q->x - P->x);
    P->y += t * (Q->y - P->y);
    P->z = clipZ;
}

int clipLineNearFar(Vec3 *A, Vec3 *B, float near, float far)
{
    float zA = A->z;
    float zB = B->z;
    MT_DEBUG("\nzB = %f  ", zB);
    if (zA > near && zB > near)
        return 0;
    MT_DEBUG("\nzA = %f  ", zA);
    // Nếu cả hai điểm đều sau far plane thì loại đoạn
    if (zA < -far && zB < -far)
        return 0;
    
    // if (zA > -near) clipPoint(A, B, -near);
    // else if (zA < -far) clipPoint(A, B, -far);

    // if (zB > -near) clipPoint(B, A, -near);
    // else if (zB < -far) clipPoint(B, A, -far);
    // MT_DEBUG("zA = ",);
    return 1;
}