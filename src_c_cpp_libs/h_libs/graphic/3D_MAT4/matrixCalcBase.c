#include "matrixCalcBase.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef MATRIX_CALC_DEBUG
    #define MT_DEBUG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define MT_DEBUG
#endif

/******************************************************************_Vec2_******************************************************************/

// Dot product Vec2
float vec2_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

// Cross product Vec2
float vec2_cross(Vec2 a, Vec2 b) {
    return a.x * b.y - a.y * b.x;
}

float vec2_length(Vec2 v) {
    return sqrtf(v.x*v.x + v.y*v.y);
}

// Add 2 Vec2
Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 result = {a.x + b.x, a.y + b.y};
    return result;
}

// Sub Vec2
Vec2 vec2_sub(Vec2 a, Vec2 b) {
    Vec2 result = {a.x - b.x, a.y - b.y};
    return result;
}

// Scale Vec2
Vec2 vec2_mul_scalar(float s, Vec2 v) {
    Vec2 result = {v.x * s, v.y * s};
    return result;
}

// Normalize Vec2
Vec2 vec2_normalize(Vec2 v) {
    float length = vec2_length(v);
    if (length == 0) return v;
    Vec2 result = {v.x / length, v.y / length};
    return result;
}

Vec2 project_to_screen(Vec3 pointNDC, int screenWidth, int screenHeight) {
    Vec2 screenPos;
    screenPos.x = (pointNDC.x + 1.0f) * 0.5f * screenWidth;
    screenPos.y = (1.0f - (pointNDC.y + 1.0f) * 0.5f) * screenHeight;
    return screenPos;
}

Vec2 vec2_direction(Vec2 from, Vec2 to) {
    return vec2_normalize(vec2_sub(to, from));
}

// Check relative direction of vecB with vecA
eDir3s check_relative_direction(Vec2 a, Vec2 b, const char* nameA, const char* nameB) {
    float cross = vec2_cross(a, b);
    if (cross > 0) {
        MT_DEBUG("Vector %s is to the LEFT of vector %s.\n", nameB, nameA);
        return eMT_D_LEFT;
    } else if (cross < 0) {
        MT_DEBUG("Vector %s is to the RIGHT of vector %s.\n", nameB, nameA);
        return eMT_D_RIGHT;
    } else {
        MT_DEBUG("Vector %s and %s are COLLINEAR \
			(pointing in the same or opposite direction).\n", nameA, nameB);
        return eMT_D_COLLINEAR;
    } 
}
/******************************************************************_Vec2_******************************************************************/


/******************************************************************_Vec3_******************************************************************/

// Dot vector (dot product)
float vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec2_convert_to_vec3(Vec2 v2) {
    return (Vec3){v2.x, v2.y, 0.0f};
}

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

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    Vec3 result = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return result;
}

// Vector normalization
Vec3 vec3_normalize(Vec3 v)
{
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0)
        return v;
    return vec3_mul_scalar(1.0f / length, v);
}

// Reference perspective 3D -> 2D
// Input: vec3 (x,y,z), distance from camera to screen
// Output: vec3 (x', y', z') with z' to calculate in order of drawing or remove.
Vec3 projectionPerspectiveR(Vec3 point, float fov, float aspectRatio, float near, float far)
{ // OpenGL
    float factor = 1.0f / tanf(fov / 2.0f);

    Mat4 proj = {0};
    proj.m[0][0] = factor / aspectRatio;
    proj.m[1][1] = factor;
    proj.m[2][2] = (far + near) / (near - far);
    proj.m[2][3] = (2 * far * near) / (near - far);
    proj.m[3][2] = -1.0f;
    proj.m[3][3] = 0;

    return mat4_mul_vec3(proj, point);
}

Vec3 projectionPerspectiveL(Vec3 point, float fov, float aspectRatio, float near, float far)
{ // DirectX/Unreal
    float factor = 1.0f / tanf(fov / 2.0f);

    Mat4 proj = {0};
    proj.m[0][0] = factor / aspectRatio;
    proj.m[1][1] = factor;
    proj.m[2][2] = far / (far - near);
    proj.m[2][3] = (-far * near) / (far - near);
    proj.m[3][2] = 1;
    proj.m[3][3] = 0;

    return mat4_mul_vec3(proj, point);
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



/******************************************************************_Vec3_******************************************************************/

/******************************************************************_Mat4_******************************************************************/

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


/******************************************************************_Mat4_******************************************************************/

// Non release

// Func move clip 1 A point approach plane clipZ
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

void cartesian_to_spherical(Vec3 v, float* r, float* phi, float* psi) {
    *r = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (*r == 0) {
        *phi = 0;
        *psi = 0;
    } else {
        *phi = acosf(v.z / *r);            // polar angle
        *psi = atan2f(v.y, v.x);           // azimuthal angle
        if (*psi < 0) *psi += 2 * M_PI;    // ensure psi place in [0, 2pi)
    }
}

// Convert (phi & psi) to vec3
Vec3 _no_spherical_to_cartesian(float r, float phi, float psi) {
    Vec3 v;
    float sin_phi = sinf(phi);
    v.x = r * sin_phi * cosf(psi);
    v.y = r * sin_phi * sinf(psi);
    v.z = r * cosf(phi);
    return v;
}

Vec3 spherical_to_cartesian(float phi, float psi) {
    Vec3 v;
    float sin_phi = sinf(phi);
    v.x = sin_phi * cosf(psi);
    v.y = sin_phi * sinf(psi);
    v.z = cosf(phi);
    return v;
}

// Convert spherical angles (phi, psi) to a 3D direction vector
Vec3 angles_to_vec3(viewVec2 angles) {
    Vec3 res;
    res.x = cos(angles.psi) * cos(angles.phi);
    res.y = cos(angles.psi) * sin(angles.phi);
    res.z = sin(angles.psi);
    return res;
}

typedef struct {
    float psi;
    float phi;
} Vec2angle;

// Right-handed coordinate system --- OpenGL 
Vec3 angles_to_vec3_opengl(Vec2angle angles) {
    Vec3 v;
    float azimuth = angles.phi;
    float elevation = angles.psi;
    float cos_elev = cosf(elevation);
    v.x = cos_elev * cosf(azimuth);
    v.y = cos_elev * sinf(azimuth);
    v.z = sinf(elevation);
    return v;
}

// Left-handed coordinate system --- DirectX
Vec3 angles_to_vec3_directx(Vec2angle angles) {
    Vec3 v;
    float azimuth = angles.phi;
    float elevation = angles.psi;
    float cos_elev = cosf(elevation);
    v.x = sinf(azimuth) * cos_elev;
    v.y = sinf(elevation);
    v.z = cosf(azimuth) * cos_elev;
    return v;
}

