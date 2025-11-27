#include "../include/visualization/trussVis.h"
#include <vector>

void visualizeTrussSystem(TrussStructure& trussSystem, std::vector<double>& displacementVec)
{
    vtkNew<vtkPoints> points;

    int numNodes = trussSystem.getNodes().size();
    int numElems = trussSystem.getElements().size();

    for (int i = 0; i < numNodes; i++) {


        std::vector<double> nodePosition = trussSystem.getNodes()[i]->getPosition();
        points->InsertNextPoint(nodePosition[0],nodePosition[1],nodePosition[2]);
    };

    vtkNew<vtkUnstructuredGrid> ugrid;
    ugrid->SetPoints(points);

    for (int e = 0; e < numElems; e++) {
        vtkNew<vtkLine> line;
        Node& nodeff = trussSystem.getElements()[e]->getNode1();
        Node& nodell = trussSystem.getElements()[e]->getNode2();
        line->GetPointIds()->SetId(0, nodeff.getID()-1);
        line->GetPointIds()->SetId(1, nodell.getID()-1);

        ugrid->InsertNextCell(VTK_LINE, line->GetPointIds());
    }

    vtkNew<vtkDoubleArray> elStresses;
    elStresses->SetName("Stress");
    elStresses->SetNumberOfComponents(1);

    int numElements = trussSystem.getElements().size();
    for (int e = 0; e < numElements; e++) {
        elStresses->InsertNextValue(std::abs(trussSystem.getElements()[e]->computeElStress(displacementVec)));
    };
    ugrid->GetCellData()->AddArray(elStresses);

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(ugrid);

    mapper->ScalarVisibilityOn();
    mapper->SetScalarModeToUseCellFieldData();
    mapper->SelectColorArray("Stress");   // color by stress


    vtkNew<vtkLookupTable> lut;
    lut->SetHueRange(0.667, 0.0);  // blue→red
    lut->SetNumberOfColors(256);
    lut->Build();

    mapper->SetLookupTable(lut);
    // Scale the stresses for coloring
    double minS = elStresses->GetRange()[0];
    double maxS = elStresses->GetRange()[1];
    mapper->SetScalarRange(minS, maxS);

    std::cout << "Stress range: " << minS << " - " << maxS << std::endl;

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    vtkNew<vtkScalarBarActor> scalarBar;
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetTitle("Stress (psi)");


    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->AddActor2D(scalarBar);
    renderer->SetBackground(0.1, 0.1, 0.1);

    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(renderer);

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(window);
    window->Render();
    interactor->Start();

  //return 1;
}
