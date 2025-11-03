#include "common/math_utils.h"
#include <cstring>
#include <cmath>

namespace MathUtils {

float radians(float degrees) {
    return degrees * 3.14159265359f / 180.0f;
}

float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Mat4 implementation
Mat4::Mat4() {
    std::memset(m, 0, sizeof(m));
}

Mat4 Mat4::identity() {
    Mat4 result;
    result.m[0] = 1.0f;
    result.m[5] = 1.0f;
    result.m[10] = 1.0f;
    result.m[15] = 1.0f;
    return result;
}

Mat4 Mat4::perspective(float fov, float aspect, float near, float far) {
    Mat4 result;
    float tanHalfFov = std::tan(fov / 2.0f);
    
    result.m[0] = 1.0f / (aspect * tanHalfFov);
    result.m[5] = 1.0f / tanHalfFov;
    result.m[10] = -(far + near) / (far - near);
    result.m[11] = -1.0f;
    result.m[14] = -(2.0f * far * near) / (far - near);
    
    return result;
}

Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
    Vec3 f = (center - eye).normalize();
    Vec3 s = Vec3(
        f.y * up.z - f.z * up.y,
        f.z * up.x - f.x * up.z,
        f.x * up.y - f.y * up.x
    ).normalize();
    Vec3 u = Vec3(
        s.y * f.z - s.z * f.y,
        s.z * f.x - s.x * f.z,
        s.x * f.y - s.y * f.x
    );
    
    Mat4 result = Mat4::identity();
    result.m[0] = s.x;
    result.m[4] = s.y;
    result.m[8] = s.z;
    result.m[1] = u.x;
    result.m[5] = u.y;
    result.m[9] = u.z;
    result.m[2] = -f.x;
    result.m[6] = -f.y;
    result.m[10] = -f.z;
    result.m[12] = -(s.x * eye.x + s.y * eye.y + s.z * eye.z);
    result.m[13] = -(u.x * eye.x + u.y * eye.y + u.z * eye.z);
    result.m[14] = f.x * eye.x + f.y * eye.y + f.z * eye.z;
    
    return result;
}

Mat4 Mat4::translate(const Vec3& t) {
    Mat4 result = identity();
    result.m[12] = t.x;
    result.m[13] = t.y;
    result.m[14] = t.z;
    return result;
}

Mat4 Mat4::rotate(float angle, const Vec3& axis) {
    Mat4 result = identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1.0f - c;
    
    Vec3 a = axis.normalize();
    
    result.m[0] = c + a.x * a.x * t;
    result.m[1] = a.y * a.x * t + a.z * s;
    result.m[2] = a.z * a.x * t - a.y * s;
    
    result.m[4] = a.x * a.y * t - a.z * s;
    result.m[5] = c + a.y * a.y * t;
    result.m[6] = a.z * a.y * t + a.x * s;
    
    result.m[8] = a.x * a.z * t + a.y * s;
    result.m[9] = a.y * a.z * t - a.x * s;
    result.m[10] = c + a.z * a.z * t;
    
    return result;
}

Mat4 Mat4::scale(const Vec3& s) {
    Mat4 result = identity();
    result.m[0] = s.x;
    result.m[5] = s.y;
    result.m[10] = s.z;
    return result;
}

Mat4 Mat4::operator*(const Mat4& other) const {
    Mat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i * 4 + j] = 0;
            for (int k = 0; k < 4; k++) {
                result.m[i * 4 + j] += m[i * 4 + k] * other.m[k * 4 + j];
            }
        }
    }
    return result;
}

} // namespace MathUtils