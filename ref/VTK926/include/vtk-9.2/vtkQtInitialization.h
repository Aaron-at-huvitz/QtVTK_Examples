/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkQtInitialization.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/
/**
 * @class   vtkQtInitialization
 * @brief   Initializes a Qt application.
 *
 *
 * Utility class that initializes Qt by creating an instance of
 * QApplication in its ctor, if one doesn't already exist.
 * This is mainly of use in ParaView with filters that use Qt in
 * their implementation - create an instance of vtkQtInitialization
 * prior to instantiating any filters that require Qt.
 */

#ifndef vtkQtInitialization_h
#define vtkQtInitialization_h

#include "vtkObject.h"
#include "vtkRenderingQtModule.h" // For export macro

class QApplication;

VTK_ABI_NAMESPACE_BEGIN

class VTKRENDERINGQT_EXPORT vtkQtInitialization : public vtkObject
{
public:
  static vtkQtInitialization* New();
  vtkTypeMacro(vtkQtInitialization, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkQtInitialization();
  ~vtkQtInitialization() override;

private:
  vtkQtInitialization(const vtkQtInitialization&) = delete;
  void operator=(const vtkQtInitialization&) = delete;

  QApplication* Application;
};

VTK_ABI_NAMESPACE_END
#endif // vtkQtInitialization_h
