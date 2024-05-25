#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include "dependencies/gui/imgui.h"
#include "dependencies/gui/imgui_internal.h"

namespace Custom1337 {
    bool tab(const char* label, bool selected);
    bool tab2(const char* name, bool boolean);
    bool Checkbox(const char* label, bool* v);
    void beginChild(const char* name, ImVec2 size);
    void endChild();

    void Hotkey(int* k, const ImVec2& size_arg = ImVec2(125, 0));
    void HelpMarker(const char* desc);

    void renderFrame(ImVec2 start, ImVec2 end, ImColor frameColor, ImColor borderColor, float rounding);
};
