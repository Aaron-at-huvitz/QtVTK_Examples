#include "HVolume.h"

HAABB::HAABB(const HVector3& minPoint, const HVector3& maxPoint)
	: xyz(minPoint), XYZ(maxPoint)
{
	update();
}

void HAABB::InsertToPolyData(vtkPolyData* polyData)
{
    //cout << "InsertToPolyData min: " << xyz.x << ", " << xyz.y << ", " << xyz.z << endl << "\tmax: " << XYZ.x << ", " << XYZ.y << ", " << XYZ.z << endl;

	auto points = polyData->GetPoints();

	auto ixyz = points->InsertNextPoint((double*)&xyz);
	auto ixyZ = points->InsertNextPoint((double*)&xyZ);
	auto ixYz = points->InsertNextPoint((double*)&xYz);
	auto ixYZ = points->InsertNextPoint((double*)&xYZ);
	auto iXyz = points->InsertNextPoint((double*)&Xyz);
	auto iXyZ = points->InsertNextPoint((double*)&XyZ);
	auto iXYz = points->InsertNextPoint((double*)&XYz);
	auto iXYZ = points->InsertNextPoint((double*)&XYZ);

	auto polys = polyData->GetPolys();

	{
		vtkIdType ids[4] = { ixyz, ixyZ, iXyZ, iXyz };
		polys->InsertNextCell(4, ids);
	}

	{
		vtkIdType ids[4] = { ixYz, iXYz, iXYZ, ixYZ };
		polys->InsertNextCell(4, ids);
	}

	{
		vtkIdType ids[4] = { ixYz, ixYZ, ixyZ, ixyz };
		polys->InsertNextCell(4, ids);
	}

	{
		vtkIdType ids[4] = { iXyz, iXyZ, iXYZ, iXYz };
		polys->InsertNextCell(4, ids);
	}

	{
		vtkIdType ids[4] = { ixyZ, ixYZ, iXYZ, iXyZ };
		polys->InsertNextCell(4, ids);
	}

	{
		vtkIdType ids[4] = { ixyz, iXyz, iXYz, ixYz };
		polys->InsertNextCell(4, ids);
	}
}

HVector3 CalculateNormal(const HVector3& vertexA, const HVector3& vertexB, const HVector3& vertexC) {
	HVector3 edgeAB{ vertexB.x - vertexA.x, vertexB.y - vertexA.y, vertexB.z - vertexA.z };
	HVector3 edgeAC{ vertexC.x - vertexA.x, vertexC.y - vertexA.y, vertexC.z - vertexA.z };

	HVector3 normal;
	normal.x = (edgeAB.y * edgeAC.z) - (edgeAB.z * edgeAC.y);
	normal.y = (edgeAB.z * edgeAC.x) - (edgeAB.x * edgeAC.z);
	normal.z = (edgeAB.x * edgeAC.y) - (edgeAB.y * edgeAC.x);

	// Normalize the normal vector
	float length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
	normal.x /= length;
	normal.y /= length;
	normal.z /= length;

	return normal;
}

bool OverlapOnAxis(const HVector3& axis, const HVector3& minVertex, const HVector3& maxVertex, const HAABB& box) {
    // Project the triangle onto the axis
    float minIntervalA = (minVertex.x * axis.x + minVertex.y * axis.y + minVertex.z * axis.z);
    float maxIntervalA = (maxVertex.x * axis.x + maxVertex.y * axis.y + maxVertex.z * axis.z);

    auto& minPoint = box.GetMinPoint();
    auto& maxPoint = box.GetMaxPoint();

    // Project the AABB onto the axis
    float minIntervalB, maxIntervalB;
    minIntervalB = maxIntervalB = (minPoint.x * axis.x + minPoint.y * axis.y + minPoint.z * axis.z);

    float projection = (maxPoint.x * axis.x + maxPoint.y * axis.y + maxPoint.z * axis.z);
    if (projection < minIntervalB) {
        minIntervalB = projection;
    }
    else if (projection > maxIntervalB) {
        maxIntervalB = projection;
    }

    // Check for overlap
    if (maxIntervalA < minIntervalB || minIntervalA > maxIntervalB) {
        // The projections do not overlap, separating axis found
        return false;
    }

    return true;
}

bool TriangleAABBIntersection(const HTriangle& triangle, const HAABB& box) {
    // Convert the AABB into convex planes
    HVector3 faceNormals[6] = {
        {1.0f, 0.0f, 0.0f},  // Right
        {-1.0f, 0.0f, 0.0f}, // Left
        {0.0f, 1.0f, 0.0f},  // Top
        {0.0f, -1.0f, 0.0f}, // Bottom
        {0.0f, 0.0f, 1.0f},  // Front
        {0.0f, 0.0f, -1.0f}  // Back
    };

    // Check for overlap on each potential separating axis
    for (int i = 0; i < 6; i++) {
        if (!OverlapOnAxis(faceNormals[i], triangle.v0, triangle.v2, box)) {
            return false;
        }
    }

    // Calculate the triangle's normal
    HVector3 triangleNormal = CalculateNormal(triangle.v0, triangle.v1, triangle.v2);

    // Check for overlap with triangle's normal
    if (!OverlapOnAxis(triangleNormal, triangle.v0, triangle.v2, box)) {
        return false;
    }

    return true;
}






float Max(float a, float b, float c) {
    return std::max(std::max(a, b), c);
}

float Min(float a, float b, float c) {
    return std::min(std::min(a, b), c);
}

bool BoxIntersectsTriangle(const HAABB& bounds, const HVector3& triangle0, const HVector3& triangle1, const HVector3& triangle2) {
    auto box_center = bounds.GetCenter();
    auto box_extents = bounds.GetExtents();

    HVector3 v0 = { triangle0.x - box_center.x, triangle0.y - box_center.y, triangle0.z - box_center.z };
    HVector3 v1 = { triangle1.x - box_center.x, triangle1.y - box_center.y, triangle1.z - box_center.z };
    HVector3 v2 = { triangle2.x - box_center.x, triangle2.y - box_center.y, triangle2.z - box_center.z };

    // Compute edge vectors for triangle
    HVector3 f0 = { triangle1.x - triangle0.x, triangle1.y - triangle0.y, triangle1.z - triangle0.z };
    HVector3 f1 = { triangle2.x - triangle1.x, triangle2.y - triangle1.y, triangle2.z - triangle1.z };
    HVector3 f2 = { triangle0.x - triangle2.x, triangle0.y - triangle2.y, triangle0.z - triangle2.z };

    //// region Test axes a00..a22 (category 3)

    // Test axis a00
    HVector3 a00 = { 0, -f0.z, f0.y };
    float p0 = v0.x * a00.x + v0.y * a00.y + v0.z * a00.z;
    float p1 = v1.x * a00.x + v1.y * a00.y + v1.z * a00.z;
    float p2 = v2.x * a00.x + v2.y * a00.y + v2.z * a00.z;
    float r = box_extents.y * std::abs(f0.z) + box_extents.z * std::abs(f0.y);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a01
    HVector3 a01 = { 0, -f1.z, f1.y };
    p0 = v0.x * a01.x + v0.y * a01.y + v0.z * a01.z;
    p1 = v1.x * a01.x + v1.y * a01.y + v1.z * a01.z;
    p2 = v2.x * a01.x + v2.y * a01.y + v2.z * a01.z;
    r = box_extents.y * std::abs(f1.z) + box_extents.z * std::abs(f1.y);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a02
    HVector3 a02 = { 0, -f2.z, f2.y };
    p0 = v0.x * a02.x + v0.y * a02.y + v0.z * a02.z;
    p1 = v1.x * a02.x + v1.y * a02.y + v1.z * a02.z;
    p2 = v2.x * a02.x + v2.y * a02.y + v2.z * a02.z;
    r = box_extents.y * std::abs(f2.z) + box_extents.z * std::abs(f2.y);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a10
    HVector3 a10 = { f0.z, 0, -f0.x };
    p0 = v0.x * a10.x + v0.y * a10.y + v0.z * a10.z;
    p1 = v1.x * a10.x + v1.y * a10.y + v1.z * a10.z;
    p2 = v2.x * a10.x + v2.y * a10.y + v2.z * a10.z;
    r = box_extents.x * std::abs(f0.z) + box_extents.z * std::abs(f0.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a11
    HVector3 a11 = { f1.z, 0, -f1.x };
    p0 = v0.x * a11.x + v0.y * a11.y + v0.z * a11.z;
    p1 = v1.x * a11.x + v1.y * a11.y + v1.z * a11.z;
    p2 = v2.x * a11.x + v2.y * a11.y + v2.z * a11.z;
    r = box_extents.x * std::abs(f1.z) + box_extents.z * std::abs(f1.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a12
    HVector3 a12 = { f2.z, 0, -f2.x };
    p0 = v0.x * a12.x + v0.y * a12.y + v0.z * a12.z;
    p1 = v1.x * a12.x + v1.y * a12.y + v1.z * a12.z;
    p2 = v2.x * a12.x + v2.y * a12.y + v2.z * a12.z;
    r = box_extents.x * std::abs(f2.z) + box_extents.z * std::abs(f2.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a20
    HVector3 a20 = { -f0.y, f0.x, 0 };
    p0 = v0.x * a20.x + v0.y * a20.y + v0.z * a20.z;
    p1 = v1.x * a20.x + v1.y * a20.y + v1.z * a20.z;
    p2 = v2.x * a20.x + v2.y * a20.y + v2.z * a20.z;
    r = box_extents.x * std::abs(f0.y) + box_extents.y * std::abs(f0.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a21
    HVector3 a21 = { -f1.y, f1.x, 0 };
    p0 = v0.x * a21.x + v0.y * a21.y + v0.z * a21.z;
    p1 = v1.x * a21.x + v1.y * a21.y + v1.z * a21.z;
    p2 = v2.x * a21.x + v2.y * a21.y + v2.z * a21.z;
    r = box_extents.x * std::abs(f1.y) + box_extents.y * std::abs(f1.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a22
    HVector3 a22 = { -f2.y, f2.x, 0 };
    p0 = v0.x * a22.x + v0.y * a22.y + v0.z * a22.z;
    p1 = v1.x * a22.x + v1.y * a22.y + v1.z * a22.z;
    p2 = v2.x * a22.x + v2.y * a22.y + v2.z * a22.z;
    r = box_extents.x * std::abs(f2.y) + box_extents.y * std::abs(f2.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    //// endregion

    //// region Test the three axes corresponding to the face normals of AABB b (category 1)

    // Exit if...
    // ... [-extents.X, extents.X] and [Min(v0.X,v1.X,v2.X), Max(v0.X,v1.X,v2.X)] do not overlap
    if (Max(v0.x, v1.x, v2.x) < -box_extents.x || Min(v0.x, v1.x, v2.x) > box_extents.x)
        return false;

    // ... [-extents.Y, extents.Y] and [Min(v0.Y,v1.Y,v2.Y), Max(v0.Y,v1.Y,v2.Y)] do not overlap
    if (Max(v0.y, v1.y, v2.y) < -box_extents.y || Min(v0.y, v1.y, v2.y) > box_extents.y)
        return false;

    // ... [-extents.Z, extents.Z] and [Min(v0.Z,v1.Z,v2.Z), Max(v0.Z,v1.Z,v2.Z)] do not overlap
    if (Max(v0.z, v1.z, v2.z) < -box_extents.z || Min(v0.z, v1.z, v2.z) > box_extents.z)
        return false;

    //// endregion

    //// region Test separating axis corresponding to triangle face normal (category 2)

    HVector3 plane_normal = { f0.y * f1.z - f0.z * f1.y, f0.z * f1.x - f0.x * f1.z, f0.x * f1.y - f0.y * f1.x };
    float plane_distance = std::abs(plane_normal.x * v0.x + plane_normal.y * v0.y + plane_normal.z * v0.z);

    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    r = box_extents.x * std::abs(plane_normal.x) + box_extents.y * std::abs(plane_normal.y) + box_extents.z * std::abs(plane_normal.z);

    // Intersection occurs when plane distance falls within [-r,+r] interval
    if (plane_distance > r)
        return false;

    //// endregion

    return true;
}