#ifndef MATRIX_CALC_BASE_C_H
#define MATRIX_CALC_BASE_C_H
//#define MATRIX_CALC_DEBUG
typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float m[4][4];
} Mat4;

// Add vector
Vec3 vec3_add(Vec3 a, Vec3 b);

// Sub vector
Vec3 vec3_sub(Vec3 a, Vec3 b);

// Scale vector with scalar
Vec3 vec3_mul_scalar(float s, Vec3 v);

// Dot vector (dot product)
float vec3_dot(Vec3 a, Vec3 b);

// Vector normalization
Vec3 vec3_normalize(Vec3 v);

// init Unit Matrix 4x4
Mat4 mat4_identity();

// Multi two matrix 4x4
Mat4 mat4_mul(Mat4 a, Mat4 b);

/**** Init Translation Matrix ****/
Mat4 mat4_translation(float tx, float ty, float tz);

/**** Init Rotation Matrix ****/
Mat4 mat4_rotation_x(float angleRad);
Mat4 mat4_rotation_y(float angleRad);
Mat4 mat4_rotation_z(float angleRad);

// Reference perspective 3D -> 2D
Vec3 projectionPerspectiveR(Vec3 point, float fov, float aspectRatio, float near, float far); // OpenGL
Vec3 projectionPerspectiveL(Vec3 point, float fov, float aspectRatio, float near, float far); // DirectX/Unreal

// Specify method using
static Vec3 (*projection_perspective)(Vec3, float, float, float, float) = &projectionPerspectiveR;

// Short call func (temporary adaptive)
//Vec3 (*project)(Vec3 point, float fov, float aspectRatio, float near, float far);
//project = projection_perspective;

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
Vec3 mat4_mul_vec3(Mat4 mat, Vec3 v);

int clipLineNearFar(Vec3* A, Vec3* B, float near, float far);

#endif //MATRIX_CALC_BASE_C_H
