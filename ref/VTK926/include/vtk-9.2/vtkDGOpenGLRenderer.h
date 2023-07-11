/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDGOpenGLRenderer.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkDGOpenGLRenderer
 * @brief   Rendering simple DG cells (i.e., those with a fixed reference shape).
 *
 * This currently handles hexahedra and tetrahedra.
 */

#ifndef vtkDGOpenGLRenderer_h
#define vtkDGOpenGLRenderer_h

#include "vtkCellGridResponder.h" // For API.
#include "vtkOpenGLCellGridRenderRequest.h"

VTK_ABI_NAMESPACE_BEGIN
class vtkCellMetadata;
class vtkDGCell;
class vtkDGOpenGLRenderers;

class VTKRENDERINGCELLGRID_EXPORT vtkDGOpenGLRenderer
  : public vtkCellGridResponder<vtkOpenGLCellGridRenderRequest>
{
public:
  static vtkDGOpenGLRenderer* New();
  vtkTypeMacro(vtkDGOpenGLRenderer, vtkCellGridResponder<vtkOpenGLCellGridRenderRequest>);

  bool Query(vtkOpenGLCellGridRenderRequest* request, vtkCellMetadata* metadata,
    vtkCellGridResponders* caches) override;

protected:
  vtkDGOpenGLRenderer() = default;
  ~vtkDGOpenGLRenderer() override = default;

  bool DrawCells(vtkOpenGLCellGridRenderRequest* request, vtkCellMetadata* metadata);
  bool ReleaseResources(vtkOpenGLCellGridRenderRequest* request, vtkCellMetadata* metadata);

  bool DrawShapes(vtkOpenGLCellGridRenderRequest* request, vtkDGCell* metadata, int shape);

private:
  vtkDGOpenGLRenderer(const vtkDGOpenGLRenderer&) = delete;
  void operator=(const vtkDGOpenGLRenderer&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif // vtkDGOpenGLRenderer_h
// VTK-HeaderTest-Exclude: vtkDGOpenGLRenderer.h
