#define IMGUI_DEFINE_MATH_OPERATORS
#include "cusyto.hpp"

#include "tricks.hpp"
#include <map>

struct tab_element {
    float element_opacity;
};

using namespace ImGui;
bool Custom1337::tab(const char* label, bool selected) {
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiID id = window->GetID(label);
    ImVec2 labelSize = CalcTextSize(label);
    ImVec2 p = window->DC.CursorPos;
    ImRect totalBb(p, { p.x + labelSize.x, p.y + labelSize.y });

    ItemSize(totalBb);
    ItemAdd(totalBb, id);

    bool hovered, held;
    bool pressed = ButtonBehavior(totalBb, id, &hovered, &held);

    float textAnim = ImTricks::Animations::FastFloatLerp(std::string(label).append("tab.textAnim"), selected, 0.25f, 1.f, GetIO().DeltaTime * 4);
    float alphaAnim = ImTricks::Animations::FastFloatLerp(std::string(label).append("tab.alphaAnim"), selected, 0.f, 1.f, GetIO().DeltaTime * 4);

    window->DrawList->AddRectFilled({ totalBb.Min.x + 10, window->Pos.y + 61 }, { totalBb.Max.x - 10, window->Pos.y + 65 }, ImColor(123 / 255.f, 123 / 255.f, 233 / 255.f, alphaAnim), 10, ImDrawFlags_RoundCornersTop);

    PushStyleColor(ImGuiCol_Text, GetColorU32(ImGuiCol_Text, textAnim));
    RenderText(totalBb.Min, label);
    PopStyleColor();

    return pressed;
}
bool Custom1337::tab2(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = ImGui::CalcTextSize(name, NULL, true);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x, pos.y + label_size.y));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x + 10, rect.Max.y), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.8f : hovered ? 0.6f : 0.4f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddText(rect.Min, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), name);

    return pressed;
}

void Custom1337::beginChild(const char* name, ImVec2 size) {
    ImGuiWindow* window = GetCurrentWindow();

    ImVec2 labelSize = CalcTextSize(name);
    ImVec2 p = window->DC.CursorPos;

    BeginChild(std::string(name).append("main").c_str(), size, false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);

    window->DrawList->AddRectFilled(p, { p.x + size.x, p.y + 45 }, ImColor(25, 25, 25), GImGui->Style.ChildRounding);
    RenderText({ p.x + size.x / 2 - labelSize.x / 2, p.y + 45 / 2 - labelSize.y / 2 }, name);

    SetCursorPos({ 0, GImGui->Style.WindowPadding.y + 45 });
    BeginChild(name, { size.x, size.y - 45 - GImGui->Style.WindowPadding.y }, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
    SetCursorPosX(GImGui->Style.WindowPadding.x);
    BeginGroup();
}
void Custom1337::endChild() {
    EndGroup();
    EndChild();
    EndChild();
}

void Custom1337::renderFrame(ImVec2 start, ImVec2 end, ImColor frameColor, ImColor borderColor, float rounding) {
    PushStyleColor(ImGuiCol_Border, borderColor.Value);
    RenderFrame(start, end, frameColor, true, rounding);
    PopStyleColor();
}

bool Custom1337::Checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    float deltatime = 1.5f * ImGui::GetIO().DeltaTime;
    static std::map<ImGuiID, float> hover_animation;
    auto it_hover = hover_animation.find(id);
    if (it_hover == hover_animation.end())
    {
        hover_animation.insert({ id, 0.f });
        it_hover = hover_animation.find(id);
    }
    it_hover->second = std::clamp(it_hover->second + (3.f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -1.f)), 0.0f, 1.f);

    const ImVec4 hover_act = (ImVec4)ImColor(255, 74, 74);
    const ImVec4 hover_dis = (ImVec4)ImColor(255, 100, 100);

    static std::map<ImGuiID, ImVec4> hover_color;
    auto it_hcolor = hover_color.find(id); // defines the hover_act
    if (it_hcolor == hover_color.end())
    {
        hover_color.insert({ id, hover_dis });
        it_hcolor = hover_color.find(id);
    }
    if (*v)
    {
        ImVec4 to = hover_dis;
        if (it_hcolor->second.x != to.x)
        {
            if (it_hcolor->second.x < to.x)
                it_hcolor->second.x = ImMin(it_hcolor->second.x + deltatime, to.x);
            else if (it_hcolor->second.x > to.x)
                it_hcolor->second.x = ImMax(to.x, it_hcolor->second.x - deltatime);
        }

        if (it_hcolor->second.y != to.y)
        {
            if (it_hcolor->second.y < to.y)
                it_hcolor->second.y = ImMin(it_hcolor->second.y + deltatime, to.y);
            else if (it_hcolor->second.y > to.y)
                it_hcolor->second.y = ImMax(to.y, it_hcolor->second.y - deltatime);
        }

        if (it_hcolor->second.z != to.z)
        {
            if (it_hcolor->second.z < to.z)
                it_hcolor->second.z = ImMin(it_hcolor->second.z + deltatime, to.z);
            else if (it_hcolor->second.z > to.z)
                it_hcolor->second.z = ImMax(to.z, it_hcolor->second.z - deltatime);
        }
    }
    else
    {
        ImVec4 to = hovered ? hover_dis : hover_act;
        if (it_hcolor->second.x != to.x)
        {
            if (it_hcolor->second.x < to.x)
                it_hcolor->second.x = ImMin(it_hcolor->second.x + deltatime, to.x);
            else if (it_hcolor->second.x > to.x)
                it_hcolor->second.x = ImMax(to.x, it_hcolor->second.x - deltatime);
        }

        if (it_hcolor->second.y != to.y)
        {
            if (it_hcolor->second.y < to.y)
                it_hcolor->second.y = ImMin(it_hcolor->second.y + deltatime, to.y);
            else if (it_hcolor->second.y > to.y)
                it_hcolor->second.y = ImMax(to.y, it_hcolor->second.y - deltatime);
        }

        if (it_hcolor->second.z != to.z)
        {
            if (it_hcolor->second.z < to.z)
                it_hcolor->second.z = ImMin(it_hcolor->second.z + deltatime, to.z);
            else if (it_hcolor->second.z > to.z)
                it_hcolor->second.z = ImMax(to.z, it_hcolor->second.z - deltatime);
        }
    }

    static std::map<ImGuiID, float> filled_animation;
    auto it_filled = filled_animation.find(id);
    if (it_filled == filled_animation.end())
    {
        filled_animation.insert({ id, 0.f });
        it_filled = filled_animation.find(id);
    }
    it_filled->second = std::clamp(it_filled->second + (2.2f * ImGui::GetIO().DeltaTime * ((*v) ? 1.f : -1.f)), 0.0f, 1.f);

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    RenderNavHighlight(total_bb, id);
    RenderFrame(check_bb.Min - ImVec2(1, 1), check_bb.Max + ImVec2(1, 1), GetColorU32(ImGuiCol_FrameBgActive, it_hover->second), true, style.FrameRounding);
    RenderFrame(check_bb.Min, check_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    ImU32 check_col = GetColorU32(ImGuiCol_Text, it_filled->second);
    bool mixed_value = (ImGuiItemFlags_MixedValue) != 0;
    if (mixed_value)
    {
        ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
        window->DrawList->AddRectFilled(check_bb.Min + pad - ImVec2(4,4), check_bb.Max - pad + ImVec2(4, 4), check_col, style.FrameRounding);
    }
    const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 4.0f));
   // RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);


    if (g.LogEnabled)
        LogRenderedText(&total_bb.Min, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
    if (label_size.x > 0.0f)
        RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x + 5, check_bb.Min.y + style.FramePadding.y), (label));

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}

void Custom1337::HelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

const char* const KeyNames[] = {
    "Unknown",
    "VK_LBUTTON",
    "VK_RBUTTON",
    "VK_CANCEL",
    "VK_MBUTTON",
    "VK_XBUTTON1",
    "VK_XBUTTON2",
    "Unknown",
    "VK_BACK",
    "VK_TAB",
    "Unknown",
    "Unknown",
    "VK_CLEAR",
    "VK_RETURN",
    "Unknown",
    "Unknown",
    "VK_SHIFT",
    "VK_CONTROL",
    "VK_MENU",
    "VK_PAUSE",
    "VK_CAPITAL",
    "VK_KANA",
    "Unknown",
    "VK_JUNJA",
    "VK_FINAL",
    "VK_KANJI",
    "Unknown",
    "VK_ESCAPE",
    "VK_CONVERT",
    "VK_NONCONVERT",
    "VK_ACCEPT",
    "VK_MODECHANGE",
    "VK_SPACE",
    "VK_PRIOR",
    "VK_NEXT",
    "VK_END",
    "VK_HOME",
    "VK_LEFT",
    "VK_UP",
    "VK_RIGHT",
    "VK_DOWN",
    "VK_SELECT",
    "VK_PRINT",
    "VK_EXECUTE",
    "VK_SNAPSHOT",
    "VK_INSERT",
    "VK_DELETE",
    "VK_HELP",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "VK_LWIN",
    "VK_RWIN",
    "VK_APPS",
    "Unknown",
    "VK_SLEEP",
    "VK_NUMPAD0",
    "VK_NUMPAD1",
    "VK_NUMPAD2",
    "VK_NUMPAD3",
    "VK_NUMPAD4",
    "VK_NUMPAD5",
    "VK_NUMPAD6",
    "VK_NUMPAD7",
    "VK_NUMPAD8",
    "VK_NUMPAD9",
    "VK_MULTIPLY",
    "VK_ADD",
    "VK_SEPARATOR",
    "VK_SUBTRACT",
    "VK_DECIMAL",
    "VK_DIVIDE",
    "VK_F1",
    "VK_F2",
    "VK_F3",
    "VK_F4",
    "VK_F5",
    "VK_F6",
    "VK_F7",
    "VK_F8",
    "VK_F9",
    "VK_F10",
    "VK_F11",
    "VK_F12",
    "VK_F13",
    "VK_F14",
    "VK_F15",
    "VK_F16",
    "VK_F17",
    "VK_F18",
    "VK_F19",
    "VK_F20",
    "VK_F21",
    "VK_F22",
    "VK_F23",
    "VK_F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_NUMLOCK",
    "VK_SCROLL",
    "VK_OEM_NEC_EQUAL",
    "VK_OEM_FJ_MASSHOU",
    "VK_OEM_FJ_TOUROKU",
    "VK_OEM_FJ_LOYA",
    "VK_OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_LSHIFT",
    "VK_RSHIFT",
    "VK_LCONTROL",
    "VK_RCONTROL",
    "VK_LMENU",
    "VK_RMENU"
};
static const int KeyCodes[] = {
    0x0,  //Undefined
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07, //Undefined
    0x08,
    0x09,
    0x0A, //Reserved
    0x0B, //Reserved
    0x0C,
    0x0D,
    0x0E, //Undefined
    0x0F, //Undefined
    0x10,
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16, //IME On
    0x17,
    0x18,
    0x19,
    0x1A, //IME Off
    0x1B,
    0x1C,
    0x1D,
    0x1E,
    0x1F,
    0x20,
    0x21,
    0x22,
    0x23,
    0x24,
    0x25,
    0x26,
    0x27,
    0x28,
    0x29,
    0x2A,
    0x2B,
    0x2C,
    0x2D,
    0x2E,
    0x2F,
    0x30,
    0x31,
    0x32,
    0x33,
    0x34,
    0x35,
    0x36,
    0x37,
    0x38,
    0x39,
    0x3A, //Undefined
    0x3B, //Undefined
    0x3C, //Undefined
    0x3D, //Undefined
    0x3E, //Undefined
    0x3F, //Undefined
    0x40, //Undefined
    0x41,
    0x42,
    0x43,
    0x44,
    0x45,
    0x46,
    0x47,
    0x48,
    0x49,
    0x4A,
    0x4B,
    0x4C,
    0x4B,
    0x4E,
    0x4F,
    0x50,
    0x51,
    0x52,
    0x53,
    0x54,
    0x55,
    0x56,
    0x57,
    0x58,
    0x59,
    0x5A,
    0x5B,
    0x5C,
    0x5D,
    0x5E, //Rservered
    0x5F,
    0x60, //Numpad1
    0x61, //Numpad2
    0x62, //Numpad3
    0x63, //Numpad4
    0x64, //Numpad5
    0x65, //Numpad6
    0x66, //Numpad7
    0x67, //Numpad8
    0x68, //Numpad8
    0x69, //Numpad9
    0x6A,
    0x6B,
    0x6C,
    0x6D,
    0x6E,
    0x6F,
    0x70, //F1
    0x71, //F2
    0x72, //F3
    0x73, //F4
    0x74, //F5
    0x75, //F6
    0x76, //F7
    0x77, //F8
    0x78, //F9
    0x79, //F10
    0x7A, //F11
    0x7B, //F12
    0x7C, //F13
    0x7D, //F14
    0x7E, //F15
    0x7F, //F16
    0x80, //F17
    0x81, //F18
    0x82, //F19
    0x83, //F20
    0x84, //F21
    0x85, //F22
    0x86, //F23
    0x87, //F24
    0x88, //Unkown
    0x89, //Unkown
    0x8A, //Unkown
    0x8B, //Unkown
    0x8C, //Unkown
    0x8D, //Unkown
    0x8E, //Unkown
    0x8F, //Unkown
    0x90,
    0x91,
    0x92, //OEM Specific
    0x93, //OEM Specific
    0x94, //OEM Specific
    0x95, //OEM Specific
    0x96, //OEM Specific
    0x97, //Unkown
    0x98, //Unkown
    0x99, //Unkown
    0x9A, //Unkown
    0x9B, //Unkown
    0x9C, //Unkown
    0x9D, //Unkown
    0x9E, //Unkown 
    0x9F, //Unkown
    0xA0,
    0xA1,
    0xA2,
    0xA3,
    0xA4,
    0xA5
};

void Custom1337::Hotkey(int* k, const ImVec2& size_arg) {

    static bool waitingforkey = false;
    if (waitingforkey == false) {
        if (ImGui::Button(KeyNames[*(int*)k], size_arg))
            waitingforkey = true;
    }
    else if (waitingforkey == true) {
        ImGui::Button("...", size_arg);

        for (auto& Key : KeyCodes)
        {
            if (GetAsyncKeyState(Key)) {
                *(int*)k = Key;
                waitingforkey = false;
            }
        }
    }
}
