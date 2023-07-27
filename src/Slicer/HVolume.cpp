#include "HVolume.h"

//HVolume::HVolume(double voxelSize, const HVector3& minPoint, const HVector3& maxPoint)
	//	: HAABB(minPoint, maxPoint), voxelSize(voxelSize)
	//{
	//	InitializeVTK();
	//}

HVolume::HVolume(double voxelSize, vtkSmartPointer<vtkPolyData> initialModelData)
	: voxelSize(voxelSize)
{
	double bounds[6];
	initialModelData->GetBounds(bounds);

	double halfVoxelSize = voxelSize * 0.5;

	Expand(bounds[0] - halfVoxelSize, bounds[2] - halfVoxelSize, bounds[4] - halfVoxelSize);
	Expand(bounds[1] + halfVoxelSize, bounds[3] + halfVoxelSize, bounds[5] + halfVoxelSize);
	//Expand(bounds[0], bounds[2], bounds[4]);
	//Expand(bounds[1], bounds[3], bounds[5]);

	Initialize(initialModelData);
}

void HVolume::Initialize(vtkSmartPointer<vtkPolyData> initialModelData)
{
	modelData = initialModelData;

	auto cellDatas = modelData->GetCellData();
	auto cellNormals = cellDatas->GetNormals();

	auto cells = modelData->GetPolys();
	auto noc = cells->GetNumberOfCells();
	auto points = modelData->GetPoints();
	auto nop = points->GetNumberOfPoints();

	double rX = GetXLength() / voxelSize;
	double rY = GetYLength() / voxelSize;
	double rZ = GetZLength() / voxelSize;

	double drX, drY, drZ;
	double mrX = modf(rX, &drX);
	double mrY = modf(rY, &drY);
	double mrZ = modf(rZ, &drZ);

	resolutionX = ceil(rX);
	resolutionY = ceil(rY);
	resolutionZ = ceil(rZ);

	if (mrX == 0) resolutionX += 1;
	if (mrY == 0) resolutionY += 1;
	if (mrZ == 0) resolutionZ += 1;

	cout << xyz.x << ", " << xyz.y << ", " << xyz.z << ", "
		<< XYZ.x << ", " << XYZ.y << ", " << XYZ.z << endl;

	cout << "Voxel Size: " << voxelSize << endl;
	cout << "Resolution x: " << resolutionX << ", y: " << resolutionY << ", z: " << resolutionZ << endl;

	voxels.resize(resolutionX * resolutionY * resolutionZ);

	for (int z = 0; z < resolutionZ; z++)
	{
		for (int y = 0; y < resolutionY; y++)
		{
			for (int x = 0; x < resolutionX; x++)
			{
				auto min = HVector3({ x * voxelSize + xyz.x, y * voxelSize + xyz.y, z * voxelSize + xyz.z });
				auto max = HVector3({ (x + 1) * voxelSize + xyz.x, (y + 1) * voxelSize + xyz.y, (z + 1) * voxelSize + xyz.z });

				auto& voxel = GetVoxel(x, y, z);
				voxel.SetMixMax(min, max);
				//voxel.InsertToPolyData(voxelsPolyData);
			}
		}
	}

	int maxCellCount = 0;
	for (int i = 0; i < noc; i++)
	{
		auto cell = modelData->GetCell(i);
		auto pi0 = cell->GetPointId(0);
		auto pi1 = cell->GetPointId(1);
		auto pi2 = cell->GetPointId(2);
		HVector3 p0, p1, p2;
		points->GetPoint(pi0, (double*)&p0);
		points->GetPoint(pi1, (double*)&p1);
		points->GetPoint(pi2, (double*)&p2);

		auto centroid = TriangleCentroid(p0, p1, p2);

		HVector3 normal(cellNormals->GetTuple(i));

		HAABB taabb;
		taabb.Expand(p0);
		taabb.Expand(p1);
		taabb.Expand(p2);

		auto tminIndex = GetIndex(taabb.GetMinPoint());
		auto tmaxIndex = GetIndex(taabb.GetMaxPoint());

		bool intersected = false;
#pragma omp parallel for
		for (int z = tminIndex.z; z <= tmaxIndex.z; z++)
		{
#pragma omp parallel for
			for (int y = tminIndex.y; y <= tmaxIndex.y; y++)
			{
#pragma omp parallel for
				for (int x = tminIndex.x; x <= tmaxIndex.x; x++)
				{
					auto& voxel = GetVoxel(x, y, z);
					if (voxel.IsOccupied()) {
						intersected = true;
						//continue;
					}

					if (voxel.IntersectsTriangle(p0, p1, p2, normal)) {
						//cout << "Intersects : " << resolutionZ * z + resolutionY * y + x << endl;
						voxel.SetOccupied(true);
						voxel.SetCellId(i);
						intersected = true;

						if (maxCellCount < voxel.GetCellIds().size())
							maxCellCount = voxel.GetCellIds().size();
					}

					auto dir = centroid - voxel.GetCenter();
					auto distance = dir.magnitude();
					if (distance < voxel.GetMinimumDistance())
					{
						voxel.SetMinimumDistance(distance);
						voxel.SetMinimumDistanceCellId(i);
					}
				}
			}
		}

		if (intersected == false)
		{
			cout << "No intersects cellIndex : " << i << endl;
			cout << "p0 : " << p0.x << ", " << p0.y << ", " << p0.z << endl;
			cout << "p1 : " << p1.x << ", " << p1.y << ", " << p1.z << endl;
			cout << "p2 : " << p2.x << ", " << p2.y << ", " << p2.z << endl;
			cout << "tminIndex : " << tminIndex.x << ", " << tminIndex.y << ", " << tminIndex.z << endl;
			cout << "tmaxIndex : " << tmaxIndex.x << ", " << tmaxIndex.y << ", " << tmaxIndex.z << endl << endl;
		}
	}

	cout << "Maximum cell count in a voxel: " << maxCellCount << endl;
}
