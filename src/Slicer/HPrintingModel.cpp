#include "HPrintingModel.h"

#include "HVisualDebugging.h"

#include <map>
#include <set>
#include <stack>
#include <random>

#include <omp.h>

HPrintingModel::HPrintingModel(vtkSmartPointer<vtkRenderer> renderer)
    : renderer(renderer)
{
}

HPrintingModel::~HPrintingModel()
{
    this->ClearAll();
}

void HPrintingModel::LoadModel(const QString& fileName)
{
    auto extension = fileName.last(3).toLower();

    vtkAbstractPolyDataReader* reader = nullptr;
    if ("stl" == extension)
    {
        reader = vtkSTLReader::New();
    }
    else if ("obj" == extension)
    {
        reader = vtkOBJReader::New();
    }
    else if ("ply" == extension)
    {
        reader = vtkPLYReader::New();
    }

    reader->SetFileName(fileName.toStdString().c_str());
    reader->Update();

    vtkNew<vtkPolyDataNormals> normals;
    normals->SetInputConnection(reader->GetOutputPort());
    normals->ComputeCellNormalsOn();
    normals->Update();

    initialModelData = vtkSmartPointer<vtkPolyData>::New();
    initialModelData->DeepCopy(normals->GetOutput());

    initialModelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    initialModelMapper->SetInputConnection(reader->GetOutputPort());

    initialModelActor = vtkSmartPointer<vtkActor>::New();
    initialModelActor->SetMapper(initialModelMapper);

    double bounds[6];
    initialModelData->GetBounds(bounds);
    cout << "Model Bounds" << endl;
    cout << "xmin: " << bounds[0] << " xmax: " << bounds[1] <<
            " ymin: " << bounds[2] << " ymax: " << bounds[3] <<
            " zmin: " << bounds[4] << " zmax: " << bounds[5] << endl;
    cout << "xlength : " << bounds[1] - bounds[0] << ", ylength : " << bounds[3] - bounds[2] << ", zlength : " << bounds[5] - bounds[4] << endl;

    double center[3];
    initialModelData->GetCenter(center);
    cout << "Model Center : " << center[0] << ", " << center[1] << ", " << center[2] << endl;

    reader->Delete();
}

void HPrintingModel::ClearAll()
{
    if (nullptr != volume)
    {
        delete volume;
        volume = nullptr;
    }

    ClearRemeshedModel();

    ClearOverhangModel();

    ClearVolumeModel();

    ClearInitialModel();
}

void HPrintingModel::ClearRemeshedModel()
{
    if (nullptr != remeshedModelData)
    {
        remeshedModelData = nullptr;
    }

    if (nullptr != remeshedModelMapper)
    {
        remeshedModelMapper = nullptr;
    }

    if (nullptr != remeshedModelActor)
    {
        renderer->RemoveActor(remeshedModelActor);
        remeshedModelActor = nullptr;
    }
}

void HPrintingModel::ClearOverhangModel()
{
    if (nullptr != overhangModelDataConnectivityFilter)
    {
        overhangModelDataConnectivityFilter = nullptr;
    }

    if (nullptr != overhangModelData)
    {
        overhangModelData = nullptr;
    }

    if (nullptr != overhangModelMapper)
    {
        overhangModelMapper = nullptr;
    }

    if (nullptr != overhangModelActor)
    {
        renderer->RemoveActor(overhangModelActor);
        overhangModelActor = nullptr;
    }
}

void HPrintingModel::ClearVolumeModel()
{
    if (nullptr != volumeModelData)
    {
        volumeModelData = nullptr;
    }

    if (nullptr != volumeModelMapper)
    {
        volumeModelMapper = nullptr;
    }

    if (nullptr != volumeModelActor)
    {
        renderer->RemoveActor(volumeModelActor);
        volumeModelActor = nullptr;
    }
}

void HPrintingModel::ClearInitialModel()
{
    if (nullptr != initialModelData)
    {
        initialModelData = nullptr;
    }

    if (nullptr != initialModelMapper)
    {
        initialModelMapper = nullptr;
    }

    if (nullptr != initialModelActor)
    {
        renderer->RemoveActor(initialModelActor);
        initialModelActor = nullptr;
    }
}

void HPrintingModel::Voxelize(vtkSmartPointer<vtkPolyData> modelData, double voxelSize)
{
    ClearVolumeModel();

    vtkNew<vtkPolyData> polyData;
    vtkNew<vtkPoints> points;
    polyData->SetPoints(points);

    volume = new HVolume(voxelSize, modelData);
    auto voxels = volume->GetVoxels();
    auto resolutionX = volume->GetResolutionX();
    auto resolutionY = volume->GetResolutionY();
    auto resolutionZ = volume->GetResolutionZ();

    vtkNew<vtkDoubleArray> scales;
    scales->SetNumberOfComponents(1);
    scales->SetName("Scales");
    //polyData->GetPointData()->SetScalars(scales);
    polyData->GetPointData()->AddArray(scales);

    vtkNew<vtkUnsignedCharArray> colors;
    colors->SetName("Colors");
    colors->SetNumberOfComponents(4);
    //polyData->GetPointData()->SetScalars(colors);
    polyData->GetPointData()->AddArray(colors);

    for (int z = 0; z < resolutionZ; z++)
    {
        for (int y = 0; y < resolutionY; y++)
        {
            for (int x = 0; x < resolutionX; x++)
            {
                auto& voxel = voxels[resolutionX * resolutionY * z + resolutionX * y + x];
                if (voxel.IsOccupied())
                {
                    voxel.GetCenter();
                    points->InsertNextPoint((double*)&(voxel.GetCenter()));

                    scales->InsertNextValue(1.0);

                    unsigned char c[] = { 255, 0, 0, 255 };
                    colors->InsertNextTypedTuple(c);
                }
                else
                {
                    voxel.GetCenter();
                    points->InsertNextPoint((double*)&(voxel.GetCenter()));

                    unsigned char c[] = { 0, 255, 255, 50 };
                    colors->InsertNextTypedTuple(c);

                    scales->InsertNextValue(0.1);
                }
            }
        }
    }

    vtkNew<vtkCubeSource> cubeSource;
    cubeSource->SetXLength(voxelSize);
    cubeSource->SetYLength(voxelSize);
    cubeSource->SetZLength(voxelSize);
    cubeSource->Update();

    volumeModelData = vtkSmartPointer<vtkGlyph3D>::New();
    volumeModelData->SetSourceConnection(cubeSource->GetOutputPort());
    volumeModelData->SetInputData(polyData);
    volumeModelData->SetScaleModeToScaleByScalar();
    volumeModelData->SetColorModeToColorByScalar();
    //https://vtk.org/doc/nightly/html/classvtkGlyph3D.html
    /*
    Warning
    The scaling of the glyphs is controlled by the ScaleFactor ivar multiplied by the scalar
    value at each point (if VTK_SCALE_BY_SCALAR is set), or multiplied by the vector magnitude
    (if VTK_SCALE_BY_VECTOR is set), Alternatively (if VTK_SCALE_BY_VECTORCOMPONENTS is set),
    the scaling may be specified for x,y,z using the vector components.
    The scale factor can be further controlled by enabling clamping using the Clamping ivar.
    If clamping is enabled, the scale is normalized by the Range ivar, and then multiplied
    by the scale factor. The normalization process includes clamping the scale value between (0,1).
    Typically this object operates on input data with scalar and/or vector data.
    However, scalar and/or vector aren't necessary, and it can be used to copy data from
     a single source to each point. In this case the scale factor can be used to uniformly
     scale the glyphs.
    The object uses "vector" data to scale glyphs, orient glyphs, and/or index into a table of glyphs.
    You can choose to use either the vector or normal data at each input point.
    Use the method SetVectorModeToUseVector() to use the vector input data, and SetVectorModeToUseNormal()
    to use the normal input data.
    If you do use a table of glyphs, make sure to set the Range ivar to make sure the index
    into the glyph table is computed correctly.
    You can turn off scaling of the glyphs completely by using the Scaling ivar.
    You can also turn off scaling due to data (either vector or scalar)
    by using the SetScaleModeToDataScalingOff() method.
    You can set what arrays to use for the scalars, vectors, normals, and color scalars
    by using the SetInputArrayToProcess methods in vtkAlgorithm. The first array is scalars,
    the next vectors, the next normals and finally color scalars.
    */

    volumeModelData->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Scales");
    volumeModelData->SetInputArrayToProcess(3, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Colors");
    //volumeModelData->ScalingOff();
    volumeModelData->Update();

    volumeModelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    volumeModelMapper->SetInputConnection(volumeModelData->GetOutputPort());

    volumeModelActor = vtkSmartPointer<vtkActor>::New();
    volumeModelActor->SetMapper(volumeModelMapper);
    volumeModelActor->GetProperty()->SetRepresentationToWireframe();
    //volumeModelActor->GetProperty()->SetColor(0.0, 0.5, 0.0);
    volumeModelActor->GetProperty()->SetAmbient(1.0);
    volumeModelActor->GetProperty()->SetDiffuse(0.0);

    renderer->AddActor(volumeModelActor);
}

void HPrintingModel::AnalyzeOverhang()
{
    ClearOverhangModel();

    overhangModelData = vtkSmartPointer<vtkPolyData>::New();

    std::vector<vtkIdType> cellsToCreate;
    std::vector<HVector3> cellNormalsToCreate;

    auto cellDatas = initialModelData->GetCellData();
    auto cellNormals = cellDatas->GetNormals();
    for (size_t i = 0; i < initialModelData->GetNumberOfCells(); i++)
    {
        auto normal = cellNormals->GetTuple(i);

        double gravity[3] = { 0.0, 0.0, -1.0 };
        auto angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(normal, gravity));
        //if (angle < 45.0) angle = 45.0;

        if (angle < 45.0)
        {
            auto cell = initialModelData->GetCell(i);
            double p0[3], p1[3], p2[3];
            cell->GetPoints()->GetPoint(0, p0);
            cell->GetPoints()->GetPoint(1, p1);
            cell->GetPoints()->GetPoint(2, p2);

            cellsToCreate.push_back(i);
            cellNormalsToCreate.push_back({ normal[0], normal[1], normal[2] });
        }
    }

    std::map<vtkIdType, vtkIdType> pointIdMapping;

    vtkNew<vtkFloatArray> overhangIntensity;
    overhangIntensity->SetNumberOfComponents(1);
    overhangIntensity->SetName("OverhangIntensity");

    vtkNew<vtkPoints> points;
    overhangModelData->SetPoints(points);
    vtkNew<vtkCellArray> polys;
    overhangModelData->SetPolys(polys);
    for (size_t i = 0; i < cellsToCreate.size(); i++)
    {
        auto oldCellId = cellsToCreate[i];
        auto oldCellNormal = cellNormalsToCreate[i];

        auto oldCell = initialModelData->GetCell(oldCellId);
        auto pi0 = oldCell->GetPointId(0);
        auto pi1 = oldCell->GetPointId(1);
        auto pi2 = oldCell->GetPointId(2);
        double p0[3], p1[3], p2[3];
        if (pointIdMapping.count(pi0) == 0) {
            initialModelData->GetPoint(pi0, p0);
            auto npi0 = points->InsertNextPoint(p0);
            pointIdMapping[pi0] = npi0;
        }
        if (pointIdMapping.count(pi1) == 0) {
            initialModelData->GetPoint(pi1, p1);
            auto npi1 = points->InsertNextPoint(p1);
            pointIdMapping[pi1] = npi1;
        }
        if (pointIdMapping.count(pi2) == 0) {
            initialModelData->GetPoint(pi2, p2);
            auto npi2 = points->InsertNextPoint(p2);
            pointIdMapping[pi2] = npi2;
        }

        vtkIdType pids[3]{ pointIdMapping[pi0], pointIdMapping[pi1], pointIdMapping[pi2] };
        polys->InsertNextCell(3, pids);

        auto& newNormal = cellNormalsToCreate[i];
        double gravity[3] = { 0.0, 0.0, -1.0 };
        auto angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors((double*)&newNormal, gravity));

        overhangIntensity->InsertNextValue(angle);
    }

    overhangModelData->GetCellData()->SetScalars(overhangIntensity);

    overhangModelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    overhangModelMapper->SetInputData(overhangModelData);
    overhangModelMapper->ScalarVisibilityOn();
    overhangModelMapper->SetColorModeToMapScalars();
    overhangModelMapper->SetScalarModeToUseCellData();

    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->AddRGBPoint(0, 1.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(45, 1.0, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(90, 1.0, 1.0, 1.0);
    overhangModelMapper->SetLookupTable(colorTransferFunction);

    overhangModelActor = vtkSmartPointer<vtkActor>::New();
    overhangModelActor->SetMapper(overhangModelMapper);

    renderer->AddActor(overhangModelActor);
    initialModelActor->VisibilityOff();



    for (size_t i = 0; i < overhangIntensity->GetNumberOfValues(); i++)
    {
        auto angle = overhangIntensity->GetValue(i);
        if (angle < 5)
        {
            auto cell = overhangModelData->GetCell(i);
            auto points = cell->GetPoints();
            double p0[3], p1[3], p2[3];
            points->GetPoint(0, p0);
            points->GetPoint(1, p1);
            points->GetPoint(2, p2);

            auto center = TriangleCentroid(p0, p1, p2);
            HVisualDebugging::AddSphere(center, 0.1, 255, 0, 0);
        }
        else if (angle < 15)
        {
            auto cell = overhangModelData->GetCell(i);
            auto points = cell->GetPoints();
            double p0[3], p1[3], p2[3];
            points->GetPoint(0, p0);
            points->GetPoint(1, p1);
            points->GetPoint(2, p2);

            auto center = TriangleCentroid(p0, p1, p2);
            HVisualDebugging::AddSphere(center, 0.07, 255, 0, 255);
        }
        else if (angle < 30)
        {
            auto cell = overhangModelData->GetCell(i);
            auto points = cell->GetPoints();
            double p0[3], p1[3], p2[3];
            points->GetPoint(0, p0);
            points->GetPoint(1, p1);
            points->GetPoint(2, p2);

            auto center = TriangleCentroid(p0, p1, p2);
            HVisualDebugging::AddSphere(center, 0.05, 255, 255, 0);
        }
        else if (angle < 45)
        {
            auto cell = overhangModelData->GetCell(i);
            auto points = cell->GetPoints();
            double p0[3], p1[3], p2[3];
            points->GetPoint(0, p0);
            points->GetPoint(1, p1);
            points->GetPoint(2, p2);

            auto center = TriangleCentroid(p0, p1, p2);
            HVisualDebugging::AddSphere(center, 0.03, 0, 255, 0);
        }
    }

    

    //{
    //    overhangModelDataConnectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
    //    overhangModelDataConnectivityFilter->SetInputData(overhangModelData);
    //    overhangModelDataConnectivityFilter->SetExtractionModeToAllRegions();
    //    overhangModelDataConnectivityFilter->ColorRegionsOn();
    //    overhangModelDataConnectivityFilter->Update();

    //    int numberOfRegions = overhangModelDataConnectivityFilter->GetNumberOfExtractedRegions();
    //    cout << "Number of extracted regions: " << numberOfRegions << endl;

    //    vtkNew<vtkLookupTable> lut;
    //    lut->SetNumberOfTableValues(std::max(numberOfRegions, 10));
    //    lut->Build();

    //    // Fill in a few known colors, the rest will be generated if needed
    //    vtkNew<vtkNamedColors> colors;
    //    lut->SetTableValue(0, colors->GetColor4d("Gold").GetData());
    //    lut->SetTableValue(1, colors->GetColor4d("Banana").GetData());
    //    lut->SetTableValue(2, colors->GetColor4d("Tomato").GetData());
    //    lut->SetTableValue(3, colors->GetColor4d("Wheat").GetData());
    //    lut->SetTableValue(4, colors->GetColor4d("Lavender").GetData());
    //    lut->SetTableValue(5, colors->GetColor4d("Flesh").GetData());
    //    lut->SetTableValue(6, colors->GetColor4d("Raspberry").GetData());
    //    lut->SetTableValue(7, colors->GetColor4d("Salmon").GetData());
    //    lut->SetTableValue(8, colors->GetColor4d("Mint").GetData());
    //    lut->SetTableValue(9, colors->GetColor4d("Peacock").GetData());

    //    if (numberOfRegions > 9)
    //    {
    //        std::mt19937 mt(4355412); // Standard mersenne_twister_engine
    //        std::uniform_real_distribution<double> distribution(.4, 1.0);
    //        for (auto i = 10; i < numberOfRegions; ++i)
    //        {
    //            lut->SetTableValue(i, distribution(mt), distribution(mt), distribution(mt), 1.0);
    //        }
    //    }

    //    // Visualize
    //    vtkNew<vtkPolyDataMapper> mapper;
    //    mapper->SetInputConnection(overhangModelDataConnectivityFilter->GetOutputPort());
    //    /*mapper->SetScalarRange(connectivityFilter->GetOutput()
    //        ->GetPointData()
    //        ->GetArray("RegionId")
    //        ->GetRange());*/
    //    mapper->SetScalarRange(0, numberOfRegions - 1);
    //    mapper->SetLookupTable(lut);
    //    mapper->Update();

    //    //double bounds[6];
    //    //overhangModelActor->GetBounds(bounds);
    //    vtkNew<vtkActor> actor;
    //    actor->SetMapper(mapper);
    //    //actor->SetPosition(0, 0, -(bounds[5] - bounds[4]) * 2);
    //    //renderer->AddActor(actor);
    //}

    //{
    //    auto tempPoly = overhangModelDataConnectivityFilter->GetOutput();
    //    //auto cellData = tempPoly->GetCellData();
    //    //cout << "cellData : " << cellData << endl;
    //    //cout << "numberOfValues : " << cellData->GetScalars()->GetNumberOfValues() << endl;
    //    //// Angle info of cells
    //    ////for (size_t i = 0; i < cellData->GetScalars()->GetNumberOfValues(); i++)
    //    ////{
    //    ////    cout << cellData->GetScalars()->GetVariantValue(i).ToDouble() << endl;
    //    ////}

    //    std::map<int, std::vector<int>> pointIdRegionIdMapping;
    //    auto pointData = tempPoly->GetPointData();
    //    cout << "pointData : " << pointData << endl;
    //    cout << "numberOfValues : " << pointData->GetScalars()->GetNumberOfValues() << endl;
    //    // Region index of points
    //    for (size_t i = 0; i < pointData->GetScalars()->GetNumberOfValues(); i++)
    //    {
    //        pointIdRegionIdMapping[pointData->GetScalars()->GetVariantValue(i).ToInt()].push_back(i);
    //        //cout << pointData->GetScalars()->GetVariantValue(i).ToInt() << endl;
    //    }

    //    vtkNew<vtkPolyDataConnectivityFilter> tempCon;
    //    tempCon->SetInputData(tempPoly);

    //    tempCon->SetExtractionModeToPointSeededRegions();
    //    //tempCon->AddSeed(10000);
    //    tempCon->AddSeed(pointIdRegionIdMapping[0][0]);
    //    tempCon->Update();

    //    vtkNew<vtkPolyDataMapper> tempMapper;
    //    tempMapper->SetInputConnection(tempCon->GetOutputPort());
    //    tempMapper->ScalarVisibilityOff();
    //    //tempMapper->SetScalarModeToDefault();
    //    //tempMapper->UseLookupTableScalarRangeOff();
    //    
    //    vtkNew<vtkActor> tempActor;
    //    tempActor->SetMapper(tempMapper);
    //    double color[3] = { 0.0, 1.0, 0.0 };
    //    //vtkNew<vtkNamedColors> colors;
    //    //colors->GetColor("Lime", color);

    //    tempActor->GetProperty()->SetColor(color[0], color[1], color[2]);

    //    renderer->AddActor(tempActor);
    //}

    //auto bounds = initialModelData->GetBounds();
    //auto xLen = bounds[1] - bounds[0];
    //auto yLen = bounds[3] - bounds[2];
    //auto zLen = bounds[5] - bounds[4];
    //auto maxLen = std::min(std::min(xLen, yLen), zLen);
    //auto voxelSize = maxLen / 100.0;
    //Voxelize(overhangModelData, voxelSize);
}

void HPrintingModel::AnalyzeIsland()
{
    std::map<vtkIdType, std::set<vtkIdType>> linkedPoints;
    std::map<vtkIdType, vtkIdType> lowestVertex;

    auto noc = initialModelData->GetNumberOfCells();
    for (size_t i = 0; i < noc; i++)
    {
        auto cell = initialModelData->GetCell(i);
        auto points = cell->GetPointIds();

        auto pi0 = points->GetId(0);
        auto pi1 = points->GetId(1);
        auto pi2 = points->GetId(2);

        if (linkedPoints.count(pi0) == 0) {
            linkedPoints[pi0] = std::set<vtkIdType>();
        }
        linkedPoints[pi0].insert(pi1);
        linkedPoints[pi0].insert(pi2);

        if (linkedPoints.count(pi1) == 0) {
            linkedPoints[pi1] = std::set<vtkIdType>();
        }
        linkedPoints[pi1].insert(pi0);
        linkedPoints[pi1].insert(pi2);

        if (linkedPoints.count(pi2) == 0) {
            linkedPoints[pi2] = std::set<vtkIdType>();
        }
        linkedPoints[pi2].insert(pi0);
        linkedPoints[pi2].insert(pi1);
    }

    auto nop = initialModelData->GetNumberOfPoints();
    for (size_t i = 0; i < nop; i++)
    {
        if (linkedPoints.count(i) == 0)
        {
            cout << "??? i: " << i << endl;
            continue;
        }
        else
        {
            auto& lps = linkedPoints[i];
            double position[3];
            initialModelData->GetPoint(i, position);
            auto zmin = position[2];
            auto zminIndex = i;
            int sameZCount = 0;
            for (auto& pi : lps)
            {
                double p[3];
                initialModelData->GetPoint(pi, p);
                if (p[2] < zmin) {
                    zminIndex = pi;
                    zmin = p[2];
                }
            }
            lowestVertex[i] = zminIndex;
        }
    }

    std::set<vtkIdType> islandPoints;
    for (auto& kvp : linkedPoints)
    {
        auto currentId = kvp.first;
        auto nextId = lowestVertex[currentId];
        bool store = false;
        while (currentId != nextId)
        {
            currentId = nextId;
            nextId = lowestVertex[currentId];
        }
        islandPoints.insert(currentId);
    }

    for (auto& i : islandPoints)
    {
        double p[3];
        initialModelData->GetPoint(i, p);
        //HVisualDebugging::AddSphere(p, 0.5, 0, 0, 255);
        HVisualDebugging::AddArrow(p, 0, 0, 255);
    }

    cout << "Total Island Points : " << islandPoints.size() << endl;
}

double HPrintingModel::GetLongestEdgeLength()
{
    if (nullptr != initialModelData)
    {
        double edgeLength = 0.0;

        auto numberOfPolys = initialModelData->GetNumberOfCells();
        for (size_t i = 0; i < numberOfPolys; i++)
        {
            auto cell = initialModelData->GetCell(i);
            auto pi0 = cell->GetPointId(0);
            auto pi1 = cell->GetPointId(1);
            auto pi2 = cell->GetPointId(2);

            double p0[3], p1[3], p2[3];
            initialModelData->GetPoint(pi0, p0);
            initialModelData->GetPoint(pi1, p1);
            initialModelData->GetPoint(pi2, p2);

            auto ll0 = vtkMath::Distance2BetweenPoints(p0, p1);
            auto ll1 = vtkMath::Distance2BetweenPoints(p1, p2);
            auto ll2 = vtkMath::Distance2BetweenPoints(p2, p0);

            if (edgeLength < ll0) edgeLength = ll0;
            if (edgeLength < ll1) edgeLength = ll1;
            if (edgeLength < ll2) edgeLength = ll2;
        }

        return sqrt(edgeLength);
    }

    return -1.0;
}

void HPrintingModel::Remesh(double edgeLength)
{
    vtkNew<vtkAdaptiveSubdivisionFilter> subdivisionFilter;
    subdivisionFilter->SetInputData(initialModelData);
    subdivisionFilter->SetMaximumEdgeLength(edgeLength);
    subdivisionFilter->Update();

    if (nullptr != remeshedModelMapper)
    {
        remeshedModelMapper = nullptr;
    }

    if (nullptr != initialModelActor)
    {
        renderer->RemoveActor(initialModelActor);
        initialModelActor = nullptr;
    }

    if (nullptr != remeshedModelActor)
    {
        renderer->RemoveActor(remeshedModelActor);
        remeshedModelActor = nullptr;
    }

    remeshedModelData = vtkSmartPointer<vtkPolyData>::New();
    remeshedModelData->DeepCopy(subdivisionFilter->GetOutput());

    remeshedModelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    remeshedModelMapper->SetInputData(remeshedModelData);

    remeshedModelActor = vtkSmartPointer<vtkActor>::New();
    remeshedModelActor->SetMapper(remeshedModelMapper);
}

void GetNeighborCellIds(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId, std::list<vtkIdType>& neighborCellIds)
{
    vtkNew<vtkIdList> cellPointIds;
    polyData->GetCellPoints(cellId, cellPointIds);

    for (vtkIdType i = 0; i < cellPointIds->GetNumberOfIds(); i++)
    {
        vtkNew<vtkIdList> idList;
        idList->InsertNextId(cellPointIds->GetId(i));

        // get the neighbors of the cell
        vtkNew<vtkIdList> ncIds;

        polyData->GetCellNeighbors(cellId, idList, ncIds);

        for (vtkIdType j = 0; j < ncIds->GetNumberOfIds(); j++)
        {
            neighborCellIds.push_back(ncIds->GetId(j));
        }
    }
}

void HPrintingModel::Pick(double x, double y)
{
    vtkNew<vtkCellPicker> picker;

    picker->PickFromListOn();
    //picker->AddPickList(initialModelActor);
    if (nullptr != overhangModelActor)
    {
        picker->AddPickList(overhangModelActor);
    }
    picker->SetTolerance(0.0005);

    picker->Pick(x, y, 0, renderer);
    auto pickedActor = picker->GetActor();
    if (nullptr != pickedActor)
    {
        auto pickPosition = picker->GetPickPosition();
        auto cellId = picker->GetCellId();



        //auto subId = picker->GetSubId();

        //cout << "cellId: " << cellId << " subId: " << subId << endl;
        //cout << "pickPosition: " << pickPosition[0] << ", " << pickPosition[1] << ", " << pickPosition[2] << endl;

        //HVisualDebugging::AddSphere(pickPosition, 0.25, 255, 0, 0);
        //HVisualDebugging::AddLine(renderer->GetActiveCamera()->GetPosition(), pickPosition, 0, 0, 255);

        //std::set<vtkIdType> visited;
        //auto currentCellId = cellId;
        //std::stack<vtkIdType> nextCellIds;
        //nextCellIds.push(cellId);
        //while (nextCellIds.size() != 0)
        //{
        //    currentCellId = nextCellIds.top();
        //    nextCellIds.pop();

        //    visited.insert(currentCellId);

        //    std::list<vtkIdType> neighbors;
        //    GetNeighborCellIds(overhangModelData, currentCellId, neighbors);

        //    for (auto& n : neighbors)
        //    {
        //        if (visited.count(n) == 0)
        //        {
        //            nextCellIds.push(n);
        //        }
        //    }
        //}

        //auto totalArea = 0.0;

        //for (auto& selectedCellId : visited)
        //{
        //    auto scell = overhangModelData->GetCell(selectedCellId);
        //    auto points = scell->GetPoints();
        //    double p0[3], p1[3], p2[3];
        //    points->GetPoint(0, p0);
        //    points->GetPoint(1, p1);
        //    points->GetPoint(2, p2);

        //    HVisualDebugging::AddTriangle(p0, p1, p2, 255, 0, 0);

        //    auto area = TrianglArea(p0, p1, p2);
        //    totalArea += area;

        //    //HVisualDebugging::AddLine(p0, p1, 255, 0, 0);
        //    //HVisualDebugging::AddLine(p1, p2, 255, 0, 0);
        //    //HVisualDebugging::AddLine(p2, p0, 255, 0, 0);
        //}

        //cout << "Total AREA : " << totalArea << endl;
    }
}

std::vector<vtkIdType> HPrintingModel::GetConnectedCellIds(vtkSmartPointer<vtkPolyData> modelData, vtkIdType cellId)
{
    std::vector<vtkIdType> output;

    vtkNew<vtkPolyDataConnectivityFilter> filter;
    filter->SetInputData(modelData);
    filter->SetExtractionModeToCellSeededRegions();
    filter->AddSeed(cellId);
    filter->Update();

    return output;
}

void HPrintingModel::ShowModel(vtkSmartPointer<vtkActor> actor, bool bShow)
{
    if (nullptr != actor)
    {
        actor->SetVisibility(bShow);
        renderer->GetRenderWindow()->Render();
    }
}

void HPrintingModel::ToggleModelVisibility(vtkSmartPointer<vtkActor> actor)
{
    if (nullptr != actor)
    {
        actor->SetVisibility(!actor->GetVisibility());
        renderer->GetRenderWindow()->Render();
    }
}

void HPrintingModel::ToggleModelRepresentation(vtkSmartPointer<vtkActor> actor)
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
