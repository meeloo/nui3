/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
#include "nui.h"
#include "nuiInit.h"
#include "nuiDbg.h"
#include "MainWindow.h"

#include <string.h>
#include <stdio.h>

#include "nglConsole.h"
#include "nuiText.h"

#include "nuiNativeResourceVolume.h"

// Custom Widgets:
#include "Main/ConfigViewController.h"
#include "Main/WidgetTreeController.h"
// End of Custom Widgets:


NGL_APP_CREATE(nuiApp);
/*
_NGL_GLOBALS(nuiApp)
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  if (App)
  {
    return 1;
  }
  else
  {
    _NGL_GLOBALS_INIT(nuiApp)
    NGL_APP_TYPE* pBase = new NGL_APP_TYPE;
    App = pBase;
    int ret = pBase->WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    pBase->ExeLoop();
    delete gpKernel;
    return ret;
  }
}
*/

nuiApp::nuiApp()
{
}

nuiApp::~nuiApp()
{
  if (win)
    win->Release();
  win = NULL;
}

void nuiApp::OnExit (int Code)
{
  GetPreferences().Save();
  
  if (win)
    win->Release();
  win = NULL;

  nuiUninit();
}

void nuiApp::OnInit()
{
  nuiInit(NULL);

  mpDebugger = new nuiDebugger();
  
  uint Width = 0, Height = 0;
  bool HasSize = false;
  mFullVersion = true;
  bool IsFullScreen = false;
  bool DebugObject = false;
  bool DebugInfo = false;
  bool ShowFPS = false;
  bool PartialRedraw = true;
//  nuiRenderer Renderer = eDirect3D;
  nuiRenderer Renderer = eOpenGL2;
//  nuiRenderer Renderer = eSoftware;

  // Accept NGL default options
  ParseDefaultArgs();

  GetLog().UseConsole(true);
  GetLog().SetLevel("font", 100);

  // Manual
  if ( (GetArgCount() == 1) &&
       ((!GetArg(0).Compare("-h")) || (!GetArg(0).Compare("--help"))) )
  {
    NGL_OUT("no params\n");
    Quit (0);
    return;
  }
  
  // Parse args
  int i = 0;
  while (i < GetArgCount())
  {
    nglString arg = GetArg(i);
    if ((!arg.Compare("--size") || !arg.Compare("-s")) && ((i+1) < GetArgCount()))
    {
      int w, h;

      std::string str(GetArg(i+1).GetStdString());
      sscanf(str.c_str(), "%dx%d", &w, &h);
      if (w > 0) Width  = w;
      if (h > 0) Height = h;
      HasSize = true;
      i++;
    }
    else if (!arg.Compare("--fullversion") || !arg.Compare("-full")) mFullVersion = true;
    else if (!arg.Compare("--showfps") || !arg.Compare("-fps")) ShowFPS = true;
    else if (!arg.Compare("--fullscreen") || !arg.Compare("-f")) IsFullScreen = true;
    else if (!arg.Compare("--debugobject") || !arg.Compare("-d")) DebugObject = true;
    else if (!arg.Compare("--debuginfo") || !arg.Compare("-i")) DebugInfo = true;
    else if (!arg.Compare("--partialredraw") || !arg.Compare("-p")) PartialRedraw = true;
    else if (!arg.Compare("--renderer") || !arg.Compare("-r")) 
    {
      arg = GetArg(i+1);
      if (!arg.Compare("opengl")) Renderer = eOpenGL2;
      else if (!arg.Compare("direct3d")) Renderer = eDirect3D;
      else if (!arg.Compare("software")) Renderer = eSoftware;
      i++;
    }
    i++;
  }
  
  nuiMainWindow::SetRenderer(Renderer);

  if (!HasSize)
  {
    if (IsFullScreen)
    {
      nglVideoMode current_mode;

      Width = current_mode.GetWidth();
      Height = current_mode.GetHeight();
    }
    else
    {
      Width = 1024;
      Height = 768;
    }
  }


  RegisterCustomWidgets();

/*
  Width = 512;
  Height = 512;
*/

  /* Create the nglWindow (and thus a GL context, don't even try to
   *   instantiate the gui (or nglFont) before the nuiWin !)
   */
  nuiContextInfo ContextInfo(nuiContextInfo::StandardContext3D);
  nglWindowInfo Info;

  Info.Flags = IsFullScreen ? nglWindow::FullScreen : 0;
  Info.Width = Width;
  Info.Height = Height;
  Info.Pos = nglWindowInfo::ePosCenter;
  Info.Title = "nuiDbg - A GUI debugger for NUI";
  Info.XPos = 0;
  Info.YPos = 0;
        
  win = new MainWindow(ContextInfo,Info);
  if ((!win) || (win->GetError()))
  {
    if (win) 
      NGL_OUT("Error: cannot create window (%s)\n", win->GetErrorStr());
    Quit (1);
    return;
  }
  win->Acquire();
  win->DBG_SetMouseOverInfo(DebugInfo);
  win->DBG_SetMouseOverObject(DebugObject);
  win->SetState(nglWindow::eShow);
  win->EnablePartialRedraw(PartialRedraw);
/*
  nuiImage* pImage = new nuiImage(win, nglPath("../data/nui.png"));
  if (pImage)
  {
    pImage->SetPosition(nuiFill);
    pImage->SetUseAlpha(true);
  }
  */
}


MainWindow* nuiApp::GetMainWindow()
{
	return win;
}

Preferences& nuiApp::GetPreferences()
{
  return mPreferences;
}

nuiDebugger& nuiApp::GetDebugger() const
{
  return *mpDebugger;
}


void nuiApp::RegisterCustomWidgets()
{
  NUI_ADD_WIDGET_CREATOR(ConfigViewController);
  NUI_ADD_WIDGET_CREATOR(WidgetTreeController);
}


bool nuiApp::IsFullVersion()
{
  return mFullVersion;
}




nuiApp* GetApp()
{
  return ((nuiApp*)App);
}


MainWindow* GetMainWindow()
{
	return ((nuiApp*)App)->GetMainWindow();
}


Preferences& GetPreferences()
{
  return ((nuiApp*)App)->GetPreferences();
}

nuiDebugger& GetDebugger()
{
  return ((nuiApp*)App)->GetDebugger();
}
