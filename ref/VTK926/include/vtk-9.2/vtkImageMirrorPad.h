/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkImageMirrorPad.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkImageMirrorPad
 * @brief   Extra pixels are filled by mirror images.
 *
 * vtkImageMirrorPad makes an image larger by filling extra pixels with
 * a mirror image of the original image (mirror at image boundaries).
 */

#ifndef vtkImageMirrorPad_h
#define vtkImageMirrorPad_h

#include "vtkImagePadFilter.h"
#include "vtkImagingCoreModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class VTKIMAGINGCORE_EXPORT vtkImageMirrorPad : public vtkImagePadFilter
{
public:
  static vtkImageMirrorPad* New();
  vtkTypeMacro(vtkImageMirrorPad, vtkImagePadFilter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkImageMirrorPad() = default;
  ~vtkImageMirrorPad() override = default;

  void ComputeInputUpdateExtent(int inExt[6], int outExt[6], int wExt[6]) override;
  void ThreadedRequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector, vtkImageData*** inData, vtkImageData** outData, int ext[6],
    int id) override;

private:
  vtkImageMirrorPad(const vtkImageMirrorPad&) = delete;
  void operator=(const vtkImageMirrorPad&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
