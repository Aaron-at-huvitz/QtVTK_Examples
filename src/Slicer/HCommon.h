#pragma once

class HVector3 {
public:
    double x, y, z;

    HVector3(double* p) : x(p[0]), y(p[1]), z(p[2]) {}
    HVector3(double x = 0.0f, double y = 0.0f, double z = 0.0f) : x(x), y(y), z(z) {}

    // Addition using operator overloading
    inline HVector3 operator+(const HVector3& other) const {
        return HVector3(x + other.x, y + other.y, z + other.z);
    }

    // Subtraction using operator overloading
    inline HVector3 operator-(const HVector3& other) const {
        return HVector3(x - other.x, y - other.y, z - other.z);
    }

    // Scalar multiplication using operator overloading
    inline HVector3 operator*(double scalar) const {
        return HVector3(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division using operator overloading
    inline HVector3 operator/(float scalar) const {
        if (scalar == 0.0f) {
            std::cerr << "Error: Division by zero!" << std::endl;
            return *this; // You might want to handle this error better.
        }
        return HVector3(x / scalar, y / scalar, z / scalar);
    }

    // Dot product using operator overloading
    inline double operator*(const HVector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product using operator overloading
    inline HVector3 operator^(const HVector3& other) const {
        return HVector3(y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    }

    inline double magnitude() {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Normalize the vector
    inline void normalize() {
        double length = magnitude();
        if (length != 0.0f) {
            x /= length;
            y /= length;
            z /= length;
        }
    }
};

inline double TrianglArea(const HVector3& p0, const HVector3& p1, const HVector3& p2)
{
    auto d01 = p1 - p0;
    auto d02 = p2 - p0;
    auto cross = d01 ^ d02;
    auto area = 0.5 * cross.magnitude();
    return area;
}

inline HVector3 TriangleCentroid(const HVector3& p0, const HVector3& p1, const HVector3& p2)
{
    return (p0 + p1 + p2) / 3;
}