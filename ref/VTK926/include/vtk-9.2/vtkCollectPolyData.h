/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCollectPolyData.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkCollectPolyData
 * @brief   Collect distributed polydata.
 *
 * This filter has code to collect polydat from across processes onto node 0.
 * Collection can be turned on or off using the "PassThrough" flag.
 */

#ifndef vtkCollectPolyData_h
#define vtkCollectPolyData_h

#include "vtkFiltersParallelModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

VTK_ABI_NAMESPACE_BEGIN
class vtkMultiProcessController;
class vtkSocketController;

class VTKFILTERSPARALLEL_EXPORT vtkCollectPolyData : public vtkPolyDataAlgorithm
{
public:
  static vtkCollectPolyData* New();
  vtkTypeMacro(vtkCollectPolyData, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * By default this filter uses the global controller,
   * but this method can be used to set another instead.
   */
  virtual void SetController(vtkMultiProcessController*);
  vtkGetObjectMacro(Controller, vtkMultiProcessController);
  ///@}

  ///@{
  /**
   * When this filter is being used in client-server mode,
   * this is the controller used to communicate between
   * client and server.  Client should not set the other controller.
   */
  virtual void SetSocketController(vtkSocketController*);
  vtkGetObjectMacro(SocketController, vtkSocketController);
  ///@}

  ///@{
  /**
   * To collect or just copy input to output. Off (collect) by default.
   */
  vtkSetMacro(PassThrough, vtkTypeBool);
  vtkGetMacro(PassThrough, vtkTypeBool);
  vtkBooleanMacro(PassThrough, vtkTypeBool);
  ///@}

protected:
  vtkCollectPolyData();
  ~vtkCollectPolyData() override;

  vtkTypeBool PassThrough;

  // Data generation method
  int RequestUpdateExtent(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  vtkMultiProcessController* Controller;
  vtkSocketController* SocketController;

private:
  vtkCollectPolyData(const vtkCollectPolyData&) = delete;
  void operator=(const vtkCollectPolyData&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
