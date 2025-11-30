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

/**
 * Visualization structure
 */
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

    /**
     * Structure initializer.
     * @param trussSystem A TrusStructure instance
     * @see TrussStructure
     */
    TrussVisualization(TrussStructure& trussSystem);

    /**
     * Adds fixed joint indicators on window.
     * Adds X, Y and/or Z letters to indicate which direction is fixed.
     * @param trussSystem A TrusStructure instance
     * @see TrussStructure
     */
    void addFixedJoints(TrussStructure& trussSystem);

    /**
     * Updates the field put into the truss structure.
     * Color grades the engineering strain and stress fields.
     * @param trussSystem A TrusStructure instance
     * @param displacementVec Complete displacement solution
     * @param field Field name. Available options are: "stress" and "strain"
     * @see TrussStructure
     */
    void updateScalarField(TrussStructure& trussSystem,
                           std::vector<double>& displacementVec,
                           const std::string& field);

    /**
     * Starts the visualitzation process
     */
    void start();
};

#endif
