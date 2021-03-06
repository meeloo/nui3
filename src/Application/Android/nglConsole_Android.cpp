/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "nui.h"
#include "nglApplication.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "android/log.h"

#define NGL_CONSOLE_PROMPT "> "


nglConsole::nglConsole(bool IsVisible)
{
  LOGI("Android console ctor");
  Setup(); // Portable code init

  mIsVisible = IsVisible;

  if (mIsVisible)
    LOGI(NGL_CONSOLE_PROMPT);
  LOGI("OK2");
}

nglConsole::~nglConsole()
{
}


/*
 * Public methods
 */

void nglConsole::Show (bool IsVisible)
{
  mIsVisible = IsVisible;
}


/*
 * Internals
 */

void nglConsole::OnOutput (const nglString& rText)
{
  // 'char' mode : string buffer is considered to use the locale's encoding
  //__android_log_print(ANDROID_LOG_INFO, "nui", "%s", rText.GetChars());
  LOGI("%s", rText.GetChars());
}

#define IN_BUFFER_SIZE 1024

void nglConsole::OnEvent(uint Flags)
{
  char buffer[IN_BUFFER_SIZE+1];
  int count;
}
