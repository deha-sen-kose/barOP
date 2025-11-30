#include "../include/visualization/trussVis.h"

TrussVisualization::TrussVisualization(TrussStructure& trussSystem)
{
    int numNodes = trussSystem.getNodes().size();
    int numElems = trussSystem.getElements().size();

    // --- Points ---
    points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < numNodes; ++i)
    {
        std::vector<double> pos = trussSystem.getNodes()[i]->getPosition();
        points->InsertNextPoint(pos[0], pos[1], pos[2]);
    }

    // --- Grid ---
    ugrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ugrid->SetPoints(points);

    if (numElems > 0)
    {
        for (int e = 0; e < numElems; ++e)
        {
            vtkNew<vtkLine> line;
            Node& n1 = trussSystem.getElements()[e]->getNode1();
            Node& n2 = trussSystem.getElements()[e]->getNode2();
            line->GetPointIds()->SetId(0, n1.getID() - 1);
            line->GetPointIds()->SetId(1, n2.getID() - 1);
            ugrid->InsertNextCell(VTK_LINE, line->GetPointIds());
        }
    }

    // --- Mapper ---
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(ugrid);
    mapper->ScalarVisibilityOn();
    mapper->SetScalarModeToUseCellFieldData();

    // --- LUT ---
    lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetHueRange(0.667, 0.0);
    lut->SetNumberOfColors(256);
    lut->Build();
    mapper->SetLookupTable(lut);

    // --- Actor ---
    trussActor = vtkSmartPointer<vtkActor>::New();
    trussActor->SetMapper(mapper);

    // --- Scalar bar ---
    scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetWidth(0.06);
    scalarBar->SetHeight(0.35);
    scalarBar->SetPosition(0.92, 0.15);
    scalarBar->GetTitleTextProperty()->SetFontSize(8);

    // --- Renderer ---
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(trussActor);
    renderer->AddActor2D(scalarBar);
    renderer->SetBackground(0.1, 0.1, 0.1);

    // --- Render window ---
    window = vtkSmartPointer<vtkRenderWindow>::New();
    window->AddRenderer(renderer);

    // --- Interactor ---
    interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(window);
}
// --- Add fixed joints ---
void TrussVisualization::addFixedJoints(TrussStructure& trussSystem)
{

    std::map<int, bool> mp = trussSystem.getConditions();
    for (auto& node : trussSystem.getNodes())
    {
        int id = node->getID();
        int dofx = 3*id-2;
        int dofy = 3*id-1;
        int dofz = 3*id;
        std::vector<double> pos = node->getPosition();

        // X displacement fixed
        if (mp.find(dofx) != mp.end())
        {
            vtkNew<vtkVectorText> textX;
            textX->SetText("X");

            vtkNew<vtkTransform> tX;
            tX->Translate(pos[0]-1, pos[1], pos[2]);
            tX->Scale(1, 1, 1);

            vtkNew<vtkTransformPolyDataFilter> tfX;
            tfX->SetTransform(tX);
            tfX->SetInputConnection(textX->GetOutputPort());

            vtkNew<vtkPolyDataMapper> mapperX;
            mapperX->SetInputConnection(tfX->GetOutputPort());

            vtkNew<vtkActor> actorX;
            actorX->SetMapper(mapperX);
            actorX->GetProperty()->SetColor(1.0, 0.0, 0.0); // Red for X

            renderer->AddActor(actorX);
        }

        // Y displacement fixed
        if (mp.find(dofy) != mp.end())
        {
            vtkNew<vtkVectorText> textY;
            textY->SetText("Y");

            vtkNew<vtkTransform> tY;
            tY->Translate(pos[0], pos[1] + 1, pos[2]);
            tY->Scale(1, 1, 1);

            vtkNew<vtkTransformPolyDataFilter> tfY;
            tfY->SetTransform(tY);
            tfY->SetInputConnection(textY->GetOutputPort());

            vtkNew<vtkPolyDataMapper> mapperY;
            mapperY->SetInputConnection(tfY->GetOutputPort());

            vtkNew<vtkActor> actorY;
            actorY->SetMapper(mapperY);
            actorY->GetProperty()->SetColor(0.0, 1.0, 0.0); // Green for Y

            renderer->AddActor(actorY);
        }

        // Z displacement fixed
        if (mp.find(dofz) != mp.end())
        {

            vtkNew<vtkVectorText> textZ;
            textZ->SetText("Z");

            vtkNew<vtkTransform> tZ;
            tZ->Translate(pos[0] + 1, pos[1], pos[2]);
            tZ->Scale(1, 1, 1);

            vtkNew<vtkTransformPolyDataFilter> tfZ;
            tfZ->SetTransform(tZ);
            tfZ->SetInputConnection(textZ->GetOutputPort());

            vtkNew<vtkPolyDataMapper> mapperZ;
            mapperZ->SetInputConnection(tfZ->GetOutputPort());

            vtkNew<vtkActor> actorZ;
            actorZ->SetMapper(mapperZ);
            actorZ->GetProperty()->SetColor(0.0, 0.0, 1.0); // Blue for Z

            renderer->AddActor(actorZ);
        }
    }
}

// --- Update scalar field ---
void TrussVisualization::updateScalarField(TrussStructure& trussSystem,
                                           std::vector<double>& displacementVec,
                                           const std::string& field)
{
    int numElements = trussSystem.getElements().size();
    if (numElements == 0) return; // safe exit

    if (!scalarArray)
        scalarArray = vtkSmartPointer<vtkDoubleArray>::New();

    scalarArray->SetNumberOfComponents(1);
    scalarArray->SetNumberOfTuples(numElements);
    scalarArray->SetName(field.c_str());

    for (int e = 0; e < numElements; ++e)
    {
        double val = 0.0;
        if (field == "stress")
            val = std::abs(trussSystem.getElements()[e]->computeElStress(displacementVec));
        else if (field == "strain")
            val = std::abs(trussSystem.getElements()[e]->computeElStrain(displacementVec));
        scalarArray->SetValue(e, val);
    }

    scalarArray->Modified();
    ugrid->GetCellData()->SetScalars(scalarArray);

    mapper->SelectColorArray(field.c_str());

    double minV = scalarArray->GetRange()[0];
    double maxV = scalarArray->GetRange()[1];
    mapper->SetScalarRange(minV, maxV);

    scalarBar->SetTitle(field.c_str());

    window->Render();
}

// --- Start interaction ---
void TrussVisualization::start()
{
    // Add global coordinate axes and define its colors and position
    vtkNew<vtkOrientationMarkerWidget> widget;
    vtkNew<vtkAxesActor> axes;
    vtkNew<vtkNamedColors> colors;
    double rgba[4]{0.0, 0.0, 0.0, 0.0};
    colors->GetColor("Carrot", rgba);
    widget->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    widget->SetOrientationMarker(axes);
    widget->SetInteractor(interactor);
    widget->SetViewport(0.0, 0.0, 0.4, 0.4);
    widget->SetEnabled(1);
    widget->InteractiveOn();

    // Reset the camera as it automatically resets to the coord. axes
    renderer->ResetCamera();
    window->Render();           // important
    //interactor->Initialize();   // ensures widget can compute viewport sizes
    interactor->Start();
}
