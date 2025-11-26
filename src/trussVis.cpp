#include "../include/visualization/trussVis.h"
#include "../include/barOP/trussStructure.h"
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkLine.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>

void visualizeTrussSystem(TrussStructure& trussSystem)
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


    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(ugrid);
    //mapper->ScalarVisibilityOn();
    //mapper->SetScalarModeToUseCellFieldData();
    //mapper->SelectColorArray("AxialForce");   // color by force

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.1);

    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(renderer);

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(window);
    window->Render();
    interactor->Start();

  //return 1;
}
