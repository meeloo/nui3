/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"


class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();
  void OnClose();
  
protected:

  nuiWidget* Tutorial_Buttons();
  nuiWidget* Tutorial_ToggleButtons();
  nuiWidget* Tutorial_RadioButtons1();
  nuiWidget* Tutorial_RadioButtons2();
  
  nuiWidget* CreateTestDelChildren();
  void OnCreateView(const nuiEvent& rEvent);
  
  
  void OnButtonPressed(const nuiEvent& rEvent);
  void OnTogglePressed(const nuiEvent& rEvent);
  void OnRadioPressed(const nuiEvent& rEvent);
  
  bool LoadCSS(const nglPath& rPath);
  
private:
  
  nuiLabel* mpLabel;
  nuiEventSink<MainWindow> mEventSink;
};

