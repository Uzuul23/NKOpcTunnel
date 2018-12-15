#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shellapi.h>

#define _MIDL_USE_GUIDDEF_

#define NK_USE_SSL

#include "NkLibrary/NkLibrary.h"
#include "NkOpcTunnel/defines.h"

#include <string>
#include <list>