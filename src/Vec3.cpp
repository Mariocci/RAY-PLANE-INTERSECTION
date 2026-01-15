#include "Vec3.h"
#include <cmath>
#include <stdexcept>

Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) {}

Vec3::Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}
Vec3& Vec3::operator+=(const Vec3& other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}
Vec3& Vec3::operator-=(const Vec3& other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

Vec3 Vec3::operator*(float scalar) const {
    return Vec3(this->x * scalar, this->y * scalar, this->z * scalar);
}
Vec3& Vec3::operator*=(float scalar) {
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
}

Vec3 Vec3::operator/(float scalar) const {
    return Vec3(this->x / scalar, this->y / scalar, this->z / scalar);
}
Vec3& Vec3::operator/=(float scalar) {
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
    return *this;
}

void Vec3::translate(const Vec3& offset) {
    this->x += offset.x;
    this->y += offset.y;
    this->z += offset.z;
}

float Vec3::norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

float Vec3::distance(const Vec3& other) const {
    return std::sqrt((x - other.x) * (x - other.x) +
                     (y - other.y) * (y - other.y) +
                     (z - other.z) * (z - other.z));
}

float Vec3::dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3& other) const {
    return Vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

bool Vec3::operator==(const Vec3& other) const {
    return x == other.x && y == other.y && z == other.z;
}
bool Vec3::operator!=(const Vec3& other) const {
    return !(*this == other);
}

float& Vec3::operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    throw std::out_of_range("Vec3 index out of range");
}
const float& Vec3::operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    throw std::out_of_range("Vec3 index out of range");
}