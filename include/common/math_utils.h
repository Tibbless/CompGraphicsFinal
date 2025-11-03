#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>

namespace MathUtils {
    // Basic 3D vector
    struct Vec3 {
        float x, y, z;
        
        Vec3() : x(0), y(0), z(0) {}
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        
        Vec3 operator+(const Vec3& other) const {
            return Vec3(x + other.x, y + other.y, z + other.z);
        }
        
        Vec3 operator-(const Vec3& other) const {
            return Vec3(x - other.x, y - other.y, z - other.z);
        }
        
        Vec3 operator*(float scalar) const {
            return Vec3(x * scalar, y * scalar, z * scalar);
        }
        
        float length() const {
            return std::sqrt(x*x + y*y + z*z);
        }
        
        Vec3 normalize() const {
            float len = length();
            return (len > 0) ? Vec3(x/len, y/len, z/len) : Vec3();
        }
    };
    
    // 4x4 Matrix for transformations
    struct Mat4 {
        float m[16];
        
        Mat4();
        static Mat4 identity();
        static Mat4 perspective(float fov, float aspect, float near, float far);
        static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
        static Mat4 translate(const Vec3& t);
        static Mat4 rotate(float angle, const Vec3& axis);
        static Mat4 scale(const Vec3& s);
        
        Mat4 operator*(const Mat4& other) const;
    };
    
    // Utility functions
    float radians(float degrees);
    float clamp(float value, float min, float max);
}

#endif // MATH_UTILS_H