#pragma once

#include <random>
#include <chrono>

class HVector3 {
public:
    double x, y, z;

    HVector3(double* p) : x(p[0]), y(p[1]), z(p[2]) {}
    HVector3(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}

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

    inline double* xyz() { return (double*)this; }
};

inline double TriangleArea(const HVector3& p0, const HVector3& p1, const HVector3& p2)
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

class HColor3 {
public:
    double r, g, b;

    HColor3(double* c) : r(c[0]), g(c[1]), b(c[2]) {}
    HColor3(double r = 1.0, double g = 1.0, double b = 1.0) : r(r), g(g), b(b) {}

    static HColor3 RandomColor()
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 randomEngine(seed);

        std::uniform_int_distribution<int> distribution(0, 255);

        int ir = distribution(randomEngine);
        double r = (double)ir / 255.0;

        int ig = distribution(randomEngine);
        double g = (double)ig / 255.0;

        int ib = distribution(randomEngine);
        double b = (double)ib / 255.0;

        return HColor3 { r, g, b };
    }

    inline double* rgb() { return (double*)this; }

protected:
    static HColor3 colors[27];
};
