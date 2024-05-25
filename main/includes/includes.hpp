#pragma once

#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES
#include "../general.h"
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <d3d9.h>
#include <d3d11.h>
#include <dwmapi.h>
#include <TlHelp32.h>

#include "../includes/hexarray.hpp"


#include "../includes/hooks/hooks.hpp"

#include "../includes/game/g_math.hpp"

#include "../includes/globals.hpp"

#include "../dependencies/gui/imgui.h"
#include "../dependencies/gui/imgui_impl_dx11.h"
#include "../dependencies/gui/imgui_impl_win32.h"

#include "../includes/utilities/utilities.hpp"

#include "../includes/game/game.hpp"

#include "../dependencies/gui/burbank.hpp"
#include "../dependencies/gui/fontsinfracted.h"

#include "../includes/render/render.hpp"