
#pragma once

class Vec3 {
public:
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);

    Vec3 operator+(const Vec3& other) const;
    Vec3& operator+=(const Vec3& other);

    Vec3 operator-(const Vec3& other) const;
    Vec3& operator-=(const Vec3& other);

    Vec3 operator*(float scalar) const;
    Vec3& operator*=(float scalar);

    Vec3 operator/(float scalar) const;
    Vec3& operator/=(float scalar);

    void translate(const Vec3& offset);

    float norm() const;

    float distance(const Vec3& other) const;

    float dot(const Vec3& other) const;

    Vec3 cross(const Vec3& other) const;

    bool operator==(const Vec3& other) const;
    bool operator!=(const Vec3& other) const;

    float& operator[](int index);
    const float& operator[](int index) const;
};
