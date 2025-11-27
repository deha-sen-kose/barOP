#ifndef TRUSSVIS_H
#define TRUSSVIS_H
#include "../barOP/trussStructure.h"
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLine.h>
#include <vtkDoubleArray.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkScalarBarActor.h>
#include <vtkCubeSource.h>
#include <vtkAppendPolyData.h>
#include <vtkGlyph3D.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkTextProperty.h>
#include <vtkCellData.h>
#include <vtkVectorText.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkNamedColors.h>
#include <vector>
#include <string>
#include <cmath>

// For compatibility with new VTK generic data arrays.
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

struct TrussVisualization
{
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkUnstructuredGrid> ugrid;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    vtkSmartPointer<vtkActor> trussActor;
    vtkSmartPointer<vtkLookupTable> lut;
    vtkSmartPointer<vtkScalarBarActor> scalarBar;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> window;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkDoubleArray> scalarArray;

    // Constructor
    TrussVisualization(TrussStructure& trussSystem);

    // Add fixed joint glyphs
    void addFixedJoints(TrussStructure& trussSystem);

    // Update stress/strain scalar field
    void updateScalarField(TrussStructure& trussSystem,
                           std::vector<double>& displacementVec,
                           const std::string& field);

    // Start interaction
    void start();
};

#endif
