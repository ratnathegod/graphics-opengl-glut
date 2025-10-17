// math.hpp - tiny 3D math, GLU-free perspective/lookAt
#pragma once
#include <cmath>
#define PI 3.14159265358979323846f
#define DEG2RAD (PI/180.0f)

struct Vec3 { float x,y,z; };
inline Vec3 operator-(Vec3 a, Vec3 b){ return {a.x-b.x,a.y-b.y,a.z-b.z}; }
inline Vec3 operator+(Vec3 a, Vec3 b){ return {a.x+b.x,a.y+b.y,a.z+b.z}; }
inline Vec3 operator*(Vec3 a, float s){ return {a.x*s,a.y*s,a.z*s}; }

inline float dot(Vec3 a, Vec3 b){ return a.x*b.x + a.y*b.y + a.z*b.z; }
inline Vec3  cross(Vec3 a, Vec3 b){
    return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
}
inline Vec3  norm(Vec3 v){ float L=std::sqrt(dot(v,v)); return (L>0)? v*(1.0f/L) : v; }
inline float clamp(float v, float lo, float hi){ return v<lo?lo:(v>hi?hi:v); }

struct Mat4 {
    // Column-major to match OpenGL
    float m[16];
    static Mat4 identity(){ Mat4 r{}; r.m[ 0]=r.m[5]=r.m[10]=r.m[15]=1; return r; }
};

// Create perspective projection matrix (GLU-free)
inline Mat4 perspective(float fovyRad, float aspect, float n, float f)
{
    float s = 1.0f / std::tan(fovyRad*0.5f);
    Mat4 P{};
    P.m[0] = s/aspect;
    P.m[5] = s;
    P.m[10]= (f+n)/(n-f);
    P.m[11]= -1.0f;
    P.m[14]= (2.0f*f*n)/(n-f);
    return P;
}

// Create view matrix from eye position looking at target (GLU-free)
inline Mat4 lookAt(Vec3 eye, Vec3 target, Vec3 up)
{
    Vec3 f = norm(target - eye);
    Vec3 s = norm(cross(f, up));
    Vec3 u = cross(s, f);

    Mat4 V = Mat4::identity();
    V.m[0]= s.x; V.m[4]= s.y; V.m[8 ]= s.z;
    V.m[1]= u.x; V.m[5]= u.y; V.m[9 ]= u.z;
    V.m[2]=-f.x; V.m[6]=-f.y; V.m[10]=-f.z;

    // translation: {-dot(s,eye), -dot(u,eye), +dot(f,eye)}
    V.m[12] = - (s.x*eye.x + s.y*eye.y + s.z*eye.z);
    V.m[13] = - (u.x*eye.x + u.y*eye.y + u.z*eye.z);
    V.m[14] = + (f.x*eye.x + f.y*eye.y + f.z*eye.z);
    return V;
}
