/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSubdivideTetra.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkSubdivideTetra
 * @brief   subdivide one tetrahedron into twelve for every tetra
 *
 * This filter subdivides tetrahedra in an unstructured grid into twelve tetrahedra.
 */

#ifndef vtkSubdivideTetra_h
#define vtkSubdivideTetra_h

#include "vtkFiltersModelingModule.h" // For export macro
#include "vtkUnstructuredGridAlgorithm.h"

VTK_ABI_NAMESPACE_BEGIN
class VTKFILTERSMODELING_EXPORT vtkSubdivideTetra : public vtkUnstructuredGridAlgorithm
{
public:
  static vtkSubdivideTetra* New();
  vtkTypeMacro(vtkSubdivideTetra, vtkUnstructuredGridAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkSubdivideTetra();
  ~vtkSubdivideTetra() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkSubdivideTetra(const vtkSubdivideTetra&) = delete;
  void operator=(const vtkSubdivideTetra&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
