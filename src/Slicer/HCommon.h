#pragma once

#include <random>
#include <chrono>
#include <stack>
#include <set>
#include <vector>
#include <map>

#include "vtk_header_files.h"

//#define FLT_EPSILON = 1.192093e-07
//#define DBL_EPSILON = 2.220446e-16
//#define LDBL_EPSILON = 1.084202e-19
#define EPSILON 2.2204460492503131e-012

inline double Trimax(double a, double b, double c) {
	return std::max(std::max(a, b), c);
}

inline double Trimin(double a, double b, double c) {
	return std::min(std::min(a, b), c);
}

class HVector3 {
public:
    double x, y, z;

    HVector3(double* p) : x(p[0]), y(p[1]), z(p[2]) {}
    HVector3(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}

	static double Distance(const HVector3& a, const HVector3& b)
	{
		return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}

	static double DistanceSquared(const HVector3& a, const HVector3& b)
	{
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
	}

    inline HVector3 operator+(const HVector3& other) const {
        return HVector3(x + other.x, y + other.y, z + other.z);
    }

    inline HVector3 operator-(const HVector3& other) const {
        return HVector3(x - other.x, y - other.y, z - other.z);
    }

    inline HVector3 operator*(double scalar) const {
        return HVector3(x * scalar, y * scalar, z * scalar);
    }

    inline HVector3 operator/(float scalar) const {
        if (scalar == 0.0f) {
            std::cerr << "Error: Division by zero!" << std::endl;
            return *this; // You might want to handle this error better.
        }
        return HVector3(x / scalar, y / scalar, z / scalar);
    }

    inline double operator*(const HVector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    inline HVector3 operator^(const HVector3& other) const {
        return HVector3(y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    }

    inline double magnitude() {
        return std::sqrt(x * x + y * y + z * z);
    }

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

std::ostream& operator<<(std::ostream& os, HVector3 const& v);

class HColor3D {
public:
    double r, g, b;

    HColor3D(double* c) : r(c[0]), g(c[1]), b(c[2]) {}
    HColor3D(double r = 1.0, double g = 1.0, double b = 1.0) : r(r), g(g), b(b) {}

    static HColor3D RandomColor()
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

        return HColor3D { r, g, b };
    }

    inline double* rgb() { return (double*)this; }
};

class HColor3UC {
public:
    unsigned char r, g, b;

    HColor3UC(unsigned char* c) : r(c[0]), g(c[1]), b(c[2]) {}
    HColor3UC(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255) : r(r), g(g), b(b) {}

    static HColor3UC RandomColor()
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 randomEngine(seed);

        std::uniform_int_distribution<int> distribution(0, 255);

        int r = distribution(randomEngine);
        int g = distribution(randomEngine);
        int b = distribution(randomEngine);

        return HColor3UC{ (unsigned char)r, (unsigned char)g, (unsigned char)b };
    }

    inline unsigned char* rgb() { return (unsigned char*)this; }
};

class HAABB
{
public:
	HAABB(const HVector3& minPoint = { DBL_MAX, DBL_MAX, DBL_MAX }, const HVector3& maxPoint = { -DBL_MAX, -DBL_MAX, -DBL_MAX });

	inline const HVector3& GetMinPoint() const { return xyz; }
	inline const HVector3& GetMaxPoint() const { return XYZ; }
	inline const HVector3& GetCenter() const { return center; }
	inline const HVector3& GetExtents() const { return extents; }

	inline void SetMixMax(const HVector3& minPoint, const HVector3& maxPoint)
	{
		xyz = minPoint;
		XYZ = maxPoint;
		update();
	}

	inline void Expand(double x, double y, double z)
	{
		if (x < xyz.x) { xyz.x = x; }
		if (y < xyz.y) { xyz.y = y; }
		if (z < xyz.z) { xyz.z = z; }

		if (x > XYZ.x) { XYZ.x = x; }
		if (y > XYZ.y) { XYZ.y = y; }
		if (z > XYZ.z) { XYZ.z = z; }

		update();
	}

	inline void Expand(const HVector3& p)
	{
		if (p.x < xyz.x) { xyz.x = p.x; }
		if (p.y < xyz.y) { xyz.y = p.y; }
		if (p.z < xyz.z) { xyz.z = p.z; }

		if (p.x > XYZ.x) { XYZ.x = p.x; }
		if (p.y > XYZ.y) { XYZ.y = p.y; }
		if (p.z > XYZ.z) { XYZ.z = p.z; }

		update();
	}

	inline double GetXLength() const { return XYZ.x - xyz.x; }
	inline double GetYLength() const { return XYZ.y - xyz.y; }
	inline double GetZLength() const { return XYZ.z - xyz.z; }

	inline bool Contains(const HVector3& p) const
	{
		return (xyz.x <= p.x && p.x <= XYZ.x) &&
			(xyz.y <= p.y && p.y <= XYZ.y) &&
			(xyz.z <= p.z && p.z <= XYZ.z);
	}

	inline bool Intersects(const HAABB& other) const
	{
		if ((xyz.x > other.XYZ.x) && (xyz.y > other.XYZ.y) && (xyz.z > other.XYZ.z)) { return false; }
		if ((XYZ.x < other.xyz.x) && (XYZ.y < other.xyz.y) && (XYZ.z < other.xyz.z)) { return false; }

		if (Contains(other.xyz)) return true;
		if (Contains(other.xyZ)) return true;
		if (Contains(other.xYz)) return true;
		if (Contains(other.xYZ)) return true;
		if (Contains(other.Xyz)) return true;
		if (Contains(other.XyZ)) return true;
		if (Contains(other.XYz)) return true;
		if (Contains(other.XYZ)) return true;

		if (other.Contains(xyz)) return true;
		if (other.Contains(xyZ)) return true;
		if (other.Contains(xYz)) return true;
		if (other.Contains(xYZ)) return true;
		if (other.Contains(Xyz)) return true;
		if (other.Contains(XyZ)) return true;
		if (other.Contains(XYz)) return true;
		if (other.Contains(XYZ)) return true;
	}

	void InsertToPolyData(vtkSmartPointer<vtkPolyData> polyData);

	bool IntersectsTriangle(const HVector3& tp0, const HVector3& tp1, const HVector3& tp2);
	bool IntersectsTriangle(const HVector3& tp0, const HVector3& tp1, const HVector3& tp2, const HVector3& tn);

protected:
	HVector3 center = { 0.0,  0.0,  0.0 };
	HVector3 extents = { 0.0,  0.0,  0.0 };
	HVector3 xyz = { DBL_MAX,  DBL_MAX,  DBL_MAX };
	HVector3 xyZ = { DBL_MAX,  DBL_MAX, -DBL_MAX };
	HVector3 xYz = { DBL_MAX, -DBL_MAX,  DBL_MAX };
	HVector3 xYZ = { DBL_MAX, -DBL_MAX, -DBL_MAX };
	HVector3 Xyz = { -DBL_MAX,  DBL_MAX,  DBL_MAX };
	HVector3 XyZ = { -DBL_MAX,  DBL_MAX, -DBL_MAX };
	HVector3 XYz = { -DBL_MAX, -DBL_MAX,  DBL_MAX };
	HVector3 XYZ = { -DBL_MAX, -DBL_MAX, -DBL_MAX };

	inline void update()
	{
		xYz.x = xyz.x; xYz.y = XYZ.y; xYz.z = xyz.z;
		xyZ.x = xyz.x; xyZ.y = xyz.y; xyZ.z = XYZ.z;
		xYZ.x = xyz.x; xYZ.y = XYZ.y; xYZ.z = XYZ.z;
		Xyz.x = XYZ.x; Xyz.y = xyz.y; Xyz.z = xyz.z;
		XYz.x = XYZ.x; XYz.y = XYZ.y; XYz.z = xyz.z;
		XyZ.x = XYZ.x; XyZ.y = xyz.y; XyZ.z = XYZ.z;

		center.x = (xyz.x + XYZ.x) * 0.5;
		center.y = (xyz.y + XYZ.y) * 0.5;
		center.z = (xyz.z + XYZ.z) * 0.5;
		extents.x = XYZ.x - center.x;
		extents.y = XYZ.y - center.y;
		extents.z = XYZ.z - center.z;
	}
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

inline void ShowActor(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkActor> actor, bool bShow)
{
	if (nullptr != actor)
	{
		actor->SetVisibility(bShow);
		renderer->GetRenderWindow()->Render();
	}
}

inline void ToggleActorVisibility(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkActor> actor)
{
	if (nullptr != actor)
	{
		actor->SetVisibility(!actor->GetVisibility());
		renderer->GetRenderWindow()->Render();
	}
}

inline void ToggleActorRepresentation(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkActor> actor)
{
	if (nullptr != actor)
	{
		auto mode = actor->GetProperty()->GetRepresentation();
		mode += 1;
		if (mode > VTK_SURFACE)
		{
			mode = VTK_POINTS;
		}
		actor->GetProperty()->SetRepresentation(mode);
		renderer->GetRenderWindow()->Render();
	}
}

void GetCellPoints(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId, HVector3& p0, HVector3& p1, HVector3& p2);
HVector3 GetCellCenter(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId);

void GetNeighborCellIds(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId, std::list<vtkIdType>& neighborCellIds);
void GetConnectedCellIdsFromCell(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId, std::set<vtkIdType>& connectedCellIds);
void GetConnectedCellIdsFromCellWithInDistance(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId, double distance, std::set<vtkIdType>& connectedCellIds);
void GetConnectedCellIds(vtkSmartPointer<vtkPolyData> polyData, std::vector<std::set<vtkIdType>>& connectedCellIds, std::vector<int>& cellGroupIds, std::vector<double>& cellGroupAreas); 

vtkSmartPointer<vtkPolyData> GetOverhangPolyData(vtkSmartPointer<vtkPolyData> modelData, double angleThreshhold);
vtkSmartPointer<vtkPolyData> FilterSmallCells(vtkSmartPointer<vtkPolyData> modelData, double areaThreshhold);
void SplitPolyData(vtkSmartPointer<vtkPolyData> modelData, std::vector<vtkSmartPointer<vtkPolyData>>& result, std::vector<double>& groupAreas);
void SplitPolyDataWithRemesh(vtkSmartPointer<vtkPolyData> modelData, double remeshEdgeLength, std::vector<vtkSmartPointer<vtkPolyData>>& result, std::vector<double>& groupAreas);
std::vector<std::tuple<double, vtkIdType>> GetOverhangCellIds(vtkSmartPointer<vtkPolyData> polyData);
