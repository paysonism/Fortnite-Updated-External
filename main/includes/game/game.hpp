#pragma once
#include <random>
#include <winnt.h>

#define SPOOF_FUNC CallSpoofer::SpoofFunction spoof(_AddressOfReturnAddress());
#ifdef _KERNEL_MODE
#define SPOOF_CALL(ret_type,name) (CallSpoofer::SafeCall<ret_type,std::remove_reference_t<decltype(*name)>>(name))
#else
#define SPOOF_CALL(name) (CallSpoofer::SafeCall(name))
#endif

__forceinline std::string weapon(uint64_t CurrentWeapon) {
	std::string PlayersWeaponName = "";

	uint64_t weapondata = read<uint64_t>(CurrentWeapon + offset::weapon_data);
	uint64_t ItemName = read<uint64_t>(weapondata + 0x40);
	if (!ItemName) return "";

	uint64_t FData = read<uint64_t>(ItemName + 0x28);
	int FLength = read<int>(ItemName + 0x30);

	if (FLength > 0 && FLength < 50) {
		wchar_t* WeaponBuffer = new wchar_t[FLength];
		read_array(FData, WeaponBuffer, FLength);
		std::wstring wstr_buf(WeaponBuffer);
		PlayersWeaponName.append(std::string(wstr_buf.begin(), wstr_buf.end()));

		delete[] WeaponBuffer;
	}
	return PlayersWeaponName;
}

class entity {
public:
	uintptr_t
		actor,
		skeletal_mesh,
		root_component,
		player_state;
	int
		team_index;
	int
		kills;
};

class item {
public:
	uintptr_t
		Actor;

	std::string
		Name;
	bool
		isVehicle,
		isChest,
		isPickup,
		isAmmoBox;
	float
		distance;


};

int enemyID = 0;
int visennemy = 0;
int InFovEnemy = 0;
std::vector<item> item_pawns;
std::vector<entity> entity_list;
std::vector<entity> temporary_entity_list;
void RadarRange(float* x, float* y, float range)
{
	if (fabs((*x)) > range || fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}

	}
}
void CalcRadarPoint(FVector vOrigin, int& screenx, int& screeny)
{
	FVector vAngle = camera_postion.rotation;
	auto fYaw = vAngle.y * M_PI / 180.0f;
	float dx = vOrigin.x - camera_postion.location.x;
	float dy = vOrigin.y - camera_postion.location.y;

	//x' = x * cos(p) - y * sin(p)
	//y' = y * sin(p) - y * -cos(p)
	float fsin_yaw = sinf(fYaw);
	float fminus_cos_yaw = -cosf(fYaw);

	float x = dy * fminus_cos_yaw + dx * fsin_yaw;
	x = -x;
	float y = dx * fminus_cos_yaw - dy * fsin_yaw;

	float range = (float)radar_range * 1000.f;

	RadarRange(&x, &y, range);

	ImVec2 DrawPos = ImVec2(radar_pos_x, radar_pos_y);
	ImVec2 DrawSize = ImVec2(radar_size, radar_size);

	int rad_x = (int)DrawPos.x;
	int rad_y = (int)DrawPos.y;

	float r_siz_x = DrawSize.x;
	float r_siz_y = DrawSize.y;

	int x_max = (int)r_siz_x + rad_x - 5;
	int y_max = (int)r_siz_y + rad_y - 5;

	screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
	screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

	if (screenx > x_max)
		screenx = x_max;

	if (screenx < rad_x)
		screenx = rad_x;

	if (screeny > y_max)
		screeny = y_max;

	if (screeny < rad_y)
		screeny = rad_y;
}
void render_radar_main() {
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(radar_pos_x, radar_pos_y), ImVec2(radar_pos_x + radar_size, radar_pos_y + radar_size), ImGui::GetColorU32({ 0.13f, 0.13f, 0.13f, 0.f }), 0.f, 0);
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(radar_pos_x + (radar_size / 2), radar_pos_y + (radar_size / 2)), radar_size / 2, ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), 64, 0.f);

	ImVec2 center = ImVec2(radar_pos_x + (radar_size / 2), radar_pos_y + (radar_size / 2));
	float orangeCircleRadius = 2.3f;
	ImGui::GetBackgroundDrawList()->AddCircleFilled(center, orangeCircleRadius, ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), 12);
}
void add_to_radar(FVector WorldLocation, float fDistance, bool visible) {
	int ScreenX, ScreenY = 0;
	CalcRadarPoint(WorldLocation, ScreenX, ScreenY);

	if (!visible)
	{
		ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(ScreenX, ScreenY), 6.f, ImGui::GetColorU32({ 1.f, 0.f, 0.f, 1.f }), 12);
	}
	else
	{
		ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(ScreenX, ScreenY), 6.f, ImGui::GetColorU32({ 0.f, 1.f, 0.f, 1.f }), 12);
	}

	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ScreenX, ScreenY), 6.f, ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 12, 1.f);
}
inline std::wstring MBytesToWString(const char* lpcszString)
{


	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}
inline std::string WStringToUTF8(const wchar_t* lpwcszWString)
{


	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}
void DrawCorneredBox(int x, int y, int w, int h, ImColor color, int thickness)
{

	ImDrawList* Drawlist = ImGui::GetBackgroundDrawList();

	float line_w = (w / 8);
	float line_h = (h / 8);
	Drawlist->AddLine(ImVec2(x, y), ImVec2(x, y + line_h), color, thickness);
	Drawlist->AddLine(ImVec2(x, y), ImVec2(x + line_w, y), color, thickness);
	Drawlist->AddLine(ImVec2(x + w - line_w, y), ImVec2(x + w, y), color, thickness);
	Drawlist->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + line_h), color, thickness);
	Drawlist->AddLine(ImVec2(x, y + h - line_h), ImVec2(x, y + h), color, thickness);
	Drawlist->AddLine(ImVec2(x, y + h), ImVec2(x + line_w, y + h), color, thickness);
	Drawlist->AddLine(ImVec2(x + w - line_w, y + h), ImVec2(x + w, y + h), color, thickness);
	Drawlist->AddLine(ImVec2(x + w, y + h - line_h), ImVec2(x + w, y + h), color, thickness);
}
inline void DrawString(float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImColor(color), text.c_str());
}

#define FNAMEPOOL_OFFSET 0x11400C80

uintptr_t RootComponent(uintptr_t actor)
{
	return read<uintptr_t>(actor + 0x198);
}
FVector GetLocation(uintptr_t actor)
{
	return read<FVector>(RootComponent(actor) + 0x128);
}

enum GAME_INFORMATION : int {
	GAME_THREAD_FAILED = 0,
	GAME_SETUP_SUCCESSFUL = 1
};

using namespace std;

ImColor visible_color;
namespace game {
	class c_game {
	public:

		auto Setup() -> GAME_INFORMATION {
			std::thread(game::c_game::CacheData).detach();
			return GAME_INFORMATION::GAME_SETUP_SUCCESSFUL;
		}
		auto isVisible(uintptr_t test) -> bool
		{
			float fLastSubmitTime = read<float>(offset::skeletal_mesh + offset::fLastSubmitTime);
			float fLastRenderTimeOnScreen = read<float>(offset::skeletal_mesh + offset::fLastRenderTimeOnScreen);
			const float fVisionTick = 0.06f;
			bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
			return bVisible;
		}

		void DrawBox(int x, int y, int w, int h, ImColor color, int thickness) {
			ImDrawList* Drawlist = ImGui::GetBackgroundDrawList();

			Drawlist->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 3.0f, 0, thickness);
		}

		static auto CacheData() -> void {
			for (;; ) {
				temporary_entity_list.clear();
				pointer->uworld = read<uintptr_t>(virtualaddy + offset::uworld);
				pointer->game_instance = read<uintptr_t>(pointer->uworld + offset::game_instance);
				pointer->levels = read<uintptr_t>(pointer->levels + 0x170);
				pointer->game_state = read<uintptr_t>(pointer->uworld + offset::game_state);
				pointer->local_player = read<uintptr_t>(read<uintptr_t>(pointer->game_instance + offset::local_player));
				pointer->player_controller = read<uintptr_t>(pointer->local_player + offset::player_controller);
				pointer->acknowledged_pawn = read<uintptr_t>(pointer->player_controller + offset::acknowledged_pawn);
				pointer->current_weapon = read<uintptr_t>(pointer->acknowledged_pawn + offset::current_weapon);
				pointer->skeletal_mesh = read<uintptr_t>(pointer->acknowledged_pawn + offset::skeletal_mesh);
				pointer->player_state = read<uintptr_t>(pointer->acknowledged_pawn + offset::player_state);
				pointer->root_component = read<uintptr_t>(pointer->acknowledged_pawn + offset::root_component);
				pointer->team_index = read<int>(pointer->player_state + offset::team_index);
				pointer->relative_location = read<FVector>(pointer->root_component + offset::relative_location);
				pointer->player_array = read<uintptr_t>(pointer->game_state + offset::player_array);
				pointer->player_array_size = read<int>(pointer->game_state + (offset::player_array + sizeof(uintptr_t)));
				if (Utilities->debug)
				{
					std::printf(" [uworld] -> %I64d\n\n", pointer->uworld);
					std::printf(" [game_instance] -> %I64d\n\n", pointer->game_instance);
					std::printf(" [levels] -> %I64d\n\n", pointer->levels);
					std::printf(" [game_state] -> %I64d\n\n", pointer->game_state);
					std::printf(" [local_player] -> %I64d\n\n", pointer->local_player);
					std::printf(" [player_controller] -> %I64d\n\n", pointer->player_controller);
					std::printf(" [acknowledged_pawn] -> %I64d\n\n", pointer->acknowledged_pawn);
					std::printf(" [current_weapon] -> %I64d\n\n", pointer->current_weapon);
					std::printf(" [skeletal_mesh] -> %I64d\n\n", pointer->skeletal_mesh);
					std::printf(" [player_state] -> %I64d\n\n", pointer->player_state);
					std::printf(" [root_component] -> %I64d\n\n", pointer->root_component);
					std::printf(" [team_index] -> %I64d\n\n", pointer->team_index);
					std::printf(" [relative_location.x] -> %I64d\n", pointer->relative_location.x);
					std::printf(" [relative_location.y] -> %I64d\n", pointer->relative_location.y);
					std::printf(" [relative_location.z] -> %I64d\n\n", pointer->relative_location.z);
					std::printf(" [player_array] -> %I64d\n", pointer->player_array);
					std::printf(" [player_array_size] -> %I64d\n", pointer->player_array_size);
				}

				for (int i = 0; i < pointer->player_array_size; ++i) {
					auto player_state = read<uintptr_t>(pointer->player_array + (i * sizeof(uintptr_t)));
					auto current_actor = read<uintptr_t>(player_state + offset::pawn_private);
					if (current_actor == pointer->acknowledged_pawn) continue;
					auto skeletalmesh = read<uintptr_t>(current_actor + offset::skeletal_mesh);
					if (!skeletalmesh) continue;
					auto base_bone = Utilities->GetBoneLocation(skeletalmesh, bone::HumanBase);
					if (base_bone.x == 0 && base_bone.y == 0 && base_bone.z == 0) continue;
					if (!Utilities->InScreen(Utilities->WorldToScreen(Utilities->GetBoneLocation(skeletalmesh, bone::HumanPelvis)))) continue;
					auto is_despawning = (read<char>(current_actor + 0x758) >> 3);
					if (is_despawning) continue;
					if (pointer->acknowledged_pawn) {
						auto team_index = read<int>(player_state + offset::team_index);
						if (pointer->team_index == team_index) continue;
					}
					entity cached_actors{};
					cached_actors.actor = current_actor;
					cached_actors.skeletal_mesh = read<uintptr_t>(current_actor + offset::skeletal_mesh);
					cached_actors.root_component = read<uintptr_t>(current_actor + offset::root_component);
					cached_actors.player_state = read<uintptr_t>(current_actor + offset::player_state);
					cached_actors.team_index = read<int>(cached_actors.player_state + offset::team_index);
					temporary_entity_list.push_back(cached_actors);
				}
				entity_list.clear();
				entity_list = temporary_entity_list;
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		}

		auto ActorLoop() -> void {
			target_dist = FLT_MAX;
			target_entity = NULL;
			desync_target_entity = NULL;
			Utilities->UpdateCamera();
			if (pointer->acknowledged_pawn) {
				uint64_t player_weapon = read<uint64_t>(pointer->acknowledged_pawn + 0xa20); // 	struct AFortWeapon* CurrentWeapon;
				if (is_valid(player_weapon)) {
					uint64_t weapon_data = read<uint64_t>(player_weapon + 0x4a8);	//struct UFortWeaponItemDefinition* WeaponData;
					if (is_valid(weapon_data)) {
						uint64_t ftext_ptr = read<uint64_t>(weapon_data + 0x90);
						if (is_valid(ftext_ptr)) {
							uint64_t ftext_data = read<uint64_t>(ftext_ptr + 0x28);
							int ftext_length = read<int>(ftext_ptr + 0x30);
							if (ftext_length > 0 && ftext_length < 50) {
								wchar_t* ftext_buf = new wchar_t[ftext_length];
								//read(reinterpret_cast<void*>(ftext_data), reinterpret_cast<uint8_t*>(ftext_buf), ftext_length * sizeof(wchar_t));
								std::wstring wstr_buf(ftext_buf);
								wchar_t* WeaponName = ftext_buf;
								delete[] ftext_buf;
							}
						}
					}
				}
			}

			for (auto& cached_entity : entity_list) {
				auto root_location = Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanBase);
				auto root_screen = Utilities->WorldToScreen(root_location);
				auto head_location = Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanHead);
				auto head_screen = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanHead));
				auto head_screen1 = Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanHead);
				FVector2d head_box = Utilities->WorldToScreen(FVector(head_screen1.x, head_screen1.y, head_screen1.z + 15));
				FVector2d head_2d = Utilities->WorldToScreen(FVector(head_location.x, head_location.y, head_location.z + 20));
				float box_height = abs(head_screen.y - root_screen.y);
				float box_width = box_height * 0.50f;
				float distance = camera::location.Distance(root_location) / 100;

				if (globals->aimbot) {
					auto dx = head_screen.x - (globals->width / 2);
					auto dy = head_screen.y - (globals->height / 2);
					auto dist = sqrtf(dx * dx + dy * dy);

					if (dist < globals->field_of_view && dist < target_dist) {
						target_dist = dist;
						target_entity = cached_entity.actor;
					}
				}
				if (globals->radarrrrrr && globals->enableeenigger)
				{

					render_radar_main();

				}
				if (globals->biscoteiloveu && globals->enableeenigger)
				{
					std::string platform_str = Utilities->platform(cached_entity.player_state);
					ImVec2 text_size = ImGui::CalcTextSize(platform_str.c_str());
					DrawString(globals->fontsize, head_box.x - (text_size.x / 2), head_box.y - 25, ImColor(255, 255, 255), false, true, platform_str.c_str());

				}
				/*if (globals->biscoteiloveu) {

					if (offset::acknowledged_pawn && globals->biscoteiloveu)
					{
                     #define u uintptr_t
						DWORD_PTR test_platform = read<DWORD_PTR>((u)cached_entity.player_state + 0x438);
						wchar_t platform[64];
					 //(test_platform, (PVOID)platform, sizeof(platform));
						std::wstring balls_sus(platform);
						std::string str_platform(balls_sus.begin(), balls_sus.end());
						ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(str_platform.c_str());

						if (str_platform.find(("XBL")) != std::string::npos || str_platform.find(("XSX")) != std::string::npos) {
							char XboxText[] = "XBOX";
							ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(XboxText);
							DrawString(offset::acknowledged_pawn, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, XboxText);

						}
						else if (str_platform.find(("PSN")) != std::string::npos || str_platform.find(("PS5")) != std::string::npos) {
							char PSNText[] = "PSN";
							ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(PSNText);
							DrawString (offset::acknowledged_pawn, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, PSNText);
						}
						else if (str_platform.find(("WIN")) != std::string::npos) {
							char WINText[] = "WIN";
							ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(WINText);
							DrawString(offset::acknowledged_pawn,  head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, WINText);
						}
						else if (str_platform.find(("SWH")) != std::string::npos || str_platform.find(("SWT")) != std::string::npos) {
							char SWHText[] = "SWITCH";
							ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(SWHText);
							DrawString(offset::acknowledged_pawn, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, SWHText);
						}
						else if (str_platform.find(("MAC")) != std::string::npos) {
							char MACText[] = "MAC";
							ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(MACText);
							DrawString(offset::acknowledged_pawn, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, MACText);
						}
						else if (str_platform.find(("LNX")) != std::string::npos) {
							char LINUXText[] = "LINUX";
							ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(LINUXText);
							DrawString(offset::acknowledged_pawn, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, LINUXText);
						}
						else if (str_platform.find(("IOS")) != std::string::npos) {
							char IOSText[] = "IOS";
							ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(IOSText);
							DrawString(offset::acknowledged_pawn, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, IOSText);
						}
						else if (str_platform.find(("AND")) != std::string::npos) {
							char ANDText[] = "ANDROID";
							ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(ANDText);
							DrawString(20, head_box.x + 5, head_box.y, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, ANDText);
						}
						else {
							DrawString(offset::acknowledged_pawn, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, str_platform.c_str());
						}
					}
				}*/

				enemyID += 1; ImColor visible_color_box; ImColor visible_color_skel; ImColor visible_color_Snapline; ImColor visible_color_Text; ImColor visible_color_Text1;

				if (distance <= 10) { InFovEnemy += 1; }

				ImColor VisibleBox;
				ImColor InvisibleBOxbiscote;
				ImColor distancecolor;

				VisibleBox = ImVec4(globals->g_color_Visible[0], globals->g_color_Visible[1], globals->g_color_Visible[2], 1.0f);
				InvisibleBOxbiscote = ImVec4(globals->g_color_invisible[0], globals->g_color_invisible[1], globals->g_color_invisible[2], 1.0f);
				distancecolor = ImVec4(globals->distancecolor[0], globals->distancecolor[1], globals->distancecolor[2], 1.0f);



				ImColor visibleColor;
				ImColor InivisibleColor;
				if (globals->DoVisibleCheck)
				{
					visibleColor = Utilities->IsVisible(cached_entity.skeletal_mesh) ? ImColor(VisibleBox) : ImColor(InvisibleBOxbiscote);
				}
				else
				{
					visibleColor = Utilities->IsVisible(cached_entity.skeletal_mesh) ? ImColor(VisibleBox) : ImColor(InvisibleBOxbiscote);
				}
				struct WeaponInformation
				{
					int32_t ammo_count;
					int32_t max_ammo;

					BYTE tier;
					std::string weapon_name;
					std::string buildplan;

				};
				if (globals->weapon_esp && globals->enableeenigger)
				{
				  if(offset::acknowledged_pawn)
				  {
					  WeaponInformation held_weapon{ };

					  uint64_t player_weapon = read <uint64_t>(WeaponOffsets::CurrentWeapon);

					  if (is_valid)(player_weapon);
					  {
						  held_weapon.ammo_count = read <int32_t>(player_weapon + WeaponOffsets::AmmoCount);
						  uint64_t weapon_data = read <uint64_t>(player_weapon + WeaponOffsets::WeaponData);

						  if (is_valid(weapon_data))
						  {
							  held_weapon.tier = read <BYTE>(weapon_data + WeaponOffsets::Tier);
							  uint64_t ftext_ptr = read <uint64_t>(weapon_data + WeaponOffsets::ItemName);

							  if (is_valid(ftext_ptr))
							  {
								  uint64_t ftext_data = read <uint64_t>(ftext_ptr + 0x28);
								  int ftext_length = read <int>(ftext_ptr + 0x30);
								  if (ftext_length > 0 && ftext_length < 50)
								  {
									  wchar_t* ftext_buf = new wchar_t[ftext_length];

									  // read_array(ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
									  std::wstring wstr_buf(ftext_buf);
									  held_weapon.weapon_name = std::string(wstr_buf.begin(), wstr_buf.end());
									  delete[] ftext_buf;
								  }
							  }
						  }
					  }

				  }
				}
				if (globals->magicbullet) {

						uintptr_t bullet = read<uintptr_t>(offset::root_component + 0652);
						write<bool>(bullet + 0xD3ADB33FCAF, true);
				}
				if (globals->radarrrrrr)
				{
					//render_radar_main();
				}
				if (globals->smvskele && globals->enableeenigger)
				{
					char players_found[256];
					sprintf_s(players_found, (("Players Rendered: %d")), entity_list.size());
					DrawString(15, 5, 18, ImColor(255, 255, 255), false, true, players_found);
				}
				uintptr_t CurrentWeapon = 0xa20;
				bool pid = false;
				if (globals->norecoilnigger)
				{
					write<float>(pointer->player_controller + 0x68, -1);
				}
				auto player = read<uintptr_t>(offset::player_array +  0x8);
				auto CurrentActor = read<uintptr_t>(player + 0x308);//PawnPrivate
				uint64_t CurrentActorMesh = read<uint64_t>(CurrentActor + offset::Mesh);
				float NotVisColor[3] = { 0.0f, 1.0f, 1.0f }; // Cyan color
				float VisColor[3] = { 1.0f, 0.58f, 0.0f };   // Orange color
				

				/*if (globals->radarrrrrr)
				{

					add_t(root_bone,187, Utilities->IsVisible)

				}*/

			       if (globals->corner && globals->enableeenigger)
				   {
					auto root_box1 = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, 0));
					float CornerHeight = abs(head_box.y - root_box1.y);
					float CornerWidth = box_height * 0.50f;
					if (isVisible(CurrentActorMesh)) {
						DrawBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, ImColor(VisColor[0] * 255, VisColor[1] * 255, VisColor[2] * 255), globals->box_thick);
					}
					if (isVisible(CurrentActorMesh)) {
						DrawBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, ImColor(NotVisColor[0] * 255, NotVisColor[1] * 255, NotVisColor[2] * 255), globals->box_thick);
					}
				}
				if (globals->carflyyy) {
					//make it later
				}
				if (globals->bounding_box && globals->enableeenigger) {
					auto root_box1 = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, 0));

					float CornerHeight = abs(head_box.y - root_box1.y);
					float CornerWidth = box_height * 0.50f;
					if (globals->outlinebox)
					{
						DrawBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, ImColor(0, 0, 0), globals->box_thick + 2.5);
					}
					DrawBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, visibleColor, globals->box_thick);
				}
				ImDrawList* draw_list = ImGui::GetForegroundDrawList();

				if (globals->snaplin && globals->enableeenigger) {
					draw_list->AddLine(ImVec2(globals->width / 2, 0), ImVec2(head_2d.x, head_2d.y), visibleColor, 1.4f);
				}

				int CloseRangeDistanceID = 0;
				float CloseRangeDistance = 50.f;
				int enemyID = 0;
				int enemyIDvis = 0;

				if (enemyID != 0)
					enemyID = 0;

				if (CloseRangeDistanceID != 0)
					CloseRangeDistanceID = 0;

				if (enemyIDvis != 0)
					enemyIDvis = 0;

				/* // old skeleton code (its meh)
				if (globals->skeleton && globals->enableeenigger) {
					auto neck = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanHead));
					auto chest = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanNeck));
					auto left_shoulder = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, 9));
					auto left_elbow = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLForearm23));
					auto left_hand = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLHand));
					auto right_shoulder = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, 38));
					auto right_elbow = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRForearm23));
					auto right_hand = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRHand));
					auto pelvis = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanPelvis));
					auto left_hip = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLThigh1));
					auto left_knee = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLThigh3));
					auto left_foot = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLFoot2));
					auto right_hip = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRThigh1));
					auto right_knee = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRThigh3));
					auto right_foot = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRFoot2));

					if (globals->outlines_skel) {
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(neck.x, neck.y), ImVec2(chest.x, chest.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(right_shoulder.x, right_shoulder.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(chest.x, chest.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_shoulder.x, right_shoulder.y), ImVec2(right_elbow.x, right_elbow.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(pelvis.x, pelvis.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_hip.x, left_hip.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_hip.x, left_hip.y), ImVec2(left_knee.x, left_knee.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_hip.x, right_hip.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_hip.x, right_hip.y), ImVec2(right_knee.x, right_knee.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_foot.x, right_foot.y), ImVec2(right_knee.x, right_knee.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);

					}
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(neck.x, neck.y), ImVec2(chest.x, chest.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(right_shoulder.x, right_shoulder.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(chest.x, chest.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_shoulder.x, right_shoulder.y), ImVec2(right_elbow.x, right_elbow.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(pelvis.x, pelvis.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_hip.x, left_hip.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_hip.x, left_hip.y), ImVec2(left_knee.x, left_knee.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_hip.x, right_hip.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_hip.x, right_hip.y), ImVec2(right_knee.x, right_knee.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(neck.x, neck.y), ImVec2(chest.x, chest.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(right_shoulder.x, right_shoulder.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(chest.x, chest.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), visibleColor, globals->skeleton_thick);
				}*/
				if (globals->skeleton && globals->enableeenigger) {

					auto neck = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_HEAD));
					auto chest = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_NECK));
					auto left_shoulder = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_LSHOULDER));
					auto left_elbow = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_LELBOW));
					auto left_hand = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_LHAND));
					auto right_shoulder = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_RSHOULDER));
					auto right_elbow = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_RELBOW));
					auto right_hand = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_RHAND));
					auto pelvis = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_PELVIS));
					auto left_hip = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_LTHIGH));
					auto left_knee = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_LKNEE));
					auto left_foot = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_LFOOT));
					auto right_hip = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_RTHIGH));
					auto right_knee = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_RKNEE));
					auto right_foot = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, perfect_skeleton::BONE_RFOOT));

					if (globals->outlines_skel) {
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(neck.x, neck.y), ImVec2(chest.x, chest.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(right_shoulder.x, right_shoulder.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(chest.x, chest.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_shoulder.x, right_shoulder.y), ImVec2(right_elbow.x, right_elbow.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(pelvis.x, pelvis.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_hip.x, left_hip.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_hip.x, left_hip.y), ImVec2(left_knee.x, left_knee.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_hip.x, right_hip.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_hip.x, right_hip.y), ImVec2(right_knee.x, right_knee.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
					}
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(neck.x, neck.y), ImVec2(chest.x, chest.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(right_shoulder.x, right_shoulder.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(chest.x, chest.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_shoulder.x, right_shoulder.y), ImVec2(right_elbow.x, right_elbow.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(pelvis.x, pelvis.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_hip.x, left_hip.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_hip.x, left_hip.y), ImVec2(left_knee.x, left_knee.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_hip.x, right_hip.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_hip.x, right_hip.y), ImVec2(right_knee.x, right_knee.y), visibleColor, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), visibleColor, globals->skeleton_thick);
				}

				if (globals->distance && globals->enableeenigger) {
					std::string distance_string = "" + std::to_string((int)distance) + "m";
					ImVec2 text_size = ImGui::CalcTextSize(distance_string.c_str());
					if (globals->text_outline) {
						DrawString(globals->font_size, root_screen.x - (text_size.x / 2), root_screen.y + 5, ImColor(255, 255,255), false, true, distance_string.c_str());
					}
					else {
						DrawString(globals->font_size, root_screen.x - (text_size.x / 2), root_screen.y + 5, distancecolor, false, false, distance_string.c_str());
					}
				}
				if (globals->corneredboxes && globals->enableeenigger)
				{
					auto root_box1 = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, 0));

					float CornerHeight = abs(head_box.y - root_box1.y);
					float CornerWidth = box_height * 0.50f;
					if (globals->outlinebox) {
						DrawCorneredBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, ImColor(0, 0, 0), globals->box_thick + 3.5);
					}
					DrawCorneredBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, visibleColor, globals->box_thick);
				}
				//&&&//
				/*void trigger() 
				{
					INPUT    Input = { 0 };
					// left down 
					Input.type = INPUT_MOUSE;
					Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
					::SendInput(1, &Input, sizeof(INPUT));
					// left up
					std::this_thread::sleep_for(std::chrono::milliseconds(11));
					::ZeroMemory(&Input, sizeof(INPUT));
					Input.type = INPUT_MOUSE;
					Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
					::SendInput(1, &Input, sizeof(INPUT));
				}*/
				if (globals->triggerbot)
				{
					if (GetAsyncKeyState_Spoofed(globals->triggerbot_key))
					{
					
						//std::thread t(globals->triggerbot);

						// Wait for the thread to finish
						//t.join();

					}
				}

				if (globals->headbox && globals->enableeenigger) {
					FVector2d head_2d = Utilities->WorldToScreen(FVector(head_location.x, head_location.y, head_location.z));

					FVector delta = head_location - camera::location;
					float distance = delta.length();

					const float constant_circle_size = 5;

					float circle_radius = constant_circle_size * (globals->height / (2.0f * distance * tanf(camera::fov * (float)M_PI / 360.f)));

					float y_offset = +12.0f;

					int segments = 50;
					float thickness = 2.0f;

					if (globals->outlines_skel)
					{
						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, globals->skeleton_thick + 2);
					}
					ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, visibleColor, segments, globals->skeleton_thick);
				}
				__forceinline auto platform(uintptr_t PlayerState) -> std::string;
				{
					SPOOF_FUNC;

					uintptr_t test_platform = read<uintptr_t>(offset::player_state + 0x438);
					wchar_t platform[64]{ 0 };
					//read(reinterpret_cast<PVOID>(test_platform), reinterpret_cast<uint8_t*>(platform), sizeof(platform));
					std::wstring platform_wstr(platform);
					std::string platform_str(platform_wstr.begin(), platform_wstr.end());
					//return platform;
				}

				ImColor box_filled;
				ImColor box_full;
				ImColor skeleton;
				ImColor text;
				ImColor snaplines;
				ImFont* UsernamePlatformFont;
				/*if (globals->platform) // name + platform
				{
					std::string username_str = get_player_name(offset::player_state);
					std::string platform_str = platform(offset::player_state);

					ImVec2 text_size1 = ImGui::CalcTextSize(platform_str.c_str());
					ImVec2 text_size2 = ImGui::CalcTextSize(platform_str.c_str());

					ImVec2 text_size1 = ImGui::CalcTextSize(username_str.c_str());
					FVector2d textPosition(text_size1.x, text_size2.y);

					std::string combinedText = (" + platform_str + ");
					DrawString(Misc::fontsize, head_box.x - 20 - (text_size2.y / 2), head_box.y - 25, text, false, true, combinedText.c_str());
				}*/
				if (globals->show_name && globals->enableeenigger)
				{
					auto username = Utilities->get_player_name(cached_entity.player_state);

					ImVec2 text_size = ImGui::CalcTextSize(username.c_str());

					if (globals->text_outline)
					{
						DrawString(globals->font_size, head_box.x - (text_size.x / 2) + (globals->font_size / 4), head_box.y + 10, ImColor(255, 255, 255, 255), false, true, username.c_str());
					}
					else
					{
						DrawString(globals->font_size, head_box.x - (text_size.x / 2) + (globals->font_size / 4), head_box.y + 10, ImColor(255, 255, 255, 255), false, false, username.c_str());
					}
				}
				if (globals->g_username && globals->enableeenigger) {

					if (pointer->player_array_size > 1)
					{
						static float g_usernamecolor[3] = { 1.00f, 1.00f, 1.00f };
						static float g_font_size = 15.5;


						if (globals->g_username && globals->enableeenigger)
						{
							std::string username_str = "[ID:" + std::to_string(cached_entity.team_index) + " | " + Utilities->get_player_name(cached_entity.player_state) + " (" + std::to_string(cached_entity.kills) + ")" "]";
							ImVec2 text_size = ImGui::CalcTextSize(username_str.c_str());
							DrawString(g_font_size, head_box.x - (text_size.x / 2), head_box.y - 20, ImColor(g_usernamecolor[0], g_usernamecolor[1], g_usernamecolor[2]), false, true, username_str.c_str());
						}
						else
						{
							std::string username_str = Utilities->get_player_name(cached_entity.player_state) + " (" + std::to_string(cached_entity.kills) + ")";
							ImVec2 text_size = ImGui::CalcTextSize(username_str.c_str());
							DrawString(g_font_size, head_box.x - (text_size.x / 2), head_box.y - 20, ImColor(g_usernamecolor[0], g_usernamecolor[1], g_usernamecolor[2]), false, true, username_str.c_str());
						}
					}
				}
				if (globals->vsync)
				{
					auto root_component = read<uintptr_t>(offset::aactor + offset::root_component);
					auto relative_location1 = read<FVector>(root_component + offset::relative_location);
					auto team_index = read<int>(player_state + offset::team_index);
					auto kills1 = read<int>(player_state + 0x1264); //IntProperty Class FortniteGame.FortPlayerStateAthena.KillScore
				}
				/*if (globals->show_name )
				{
				  if(globals->player_array_size)
				  {
				  
					  if (globals->show_name)
					  {
						  static float g_font_size = 15.5;
						  static float g_usernamecolor[3] = { 1.00f, 1.00f, 1.00f };

						  if (globals->show_name)
						  {
							  std::string username_str = "[ID:" + std::to_string(cached_entity.team_index) + " | " + Utilities->get_player_name(cached_entity.player_state) + " (" + std::to_string(cached_entity.kills) + ")" "]";
							  ImVec2 text_size = ImGui::CalcTextSize(username_str.c_str());
							  DrawString(g_font_size, head_box.x - (text_size.x / 2), head_box.y - 20, ImColor(g_usernamecolor[0], g_usernamecolor[1], g_usernamecolor[2]), false, true, username_str.c_str());
						  }
						  else
						  {
							  std::string username_str = Utilities->get_player_name(cached_entity.player_state) + " (" + std::to_string(cached_entity.kills) + ")";
							  ImVec2 text_size = ImGui::CalcTextSize(username_str.c_str());
							  DrawString(g_font_size, head_box.x - (text_size.x / 2), head_box.y - 20, ImColor(g_usernamecolor[0], g_usernamecolor[1], g_usernamecolor[2]), false, true, username_str.c_str());
						  }
					  }
				  }
				}*/

				if (enemyID != 0)
					enemyID = 0;

				if (InFovEnemy != 0)
					InFovEnemy = 0;

				if (visennemy != 0)
					visennemy = 0;

			}

			std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
			std::uniform_int_distribution<int> distribution(0, 3);
			int randomHitbox = distribution(rng);
			if (target_entity && globals->aimbot) {
				auto test = read<uintptr_t>(target_entity + offset::root_component);
				FVector Velocity = read<FVector>(pointer->root_component + offset::velocity); //offset::velocity

				auto closest_mesh = read<uint64_t>(target_entity + offset::skeletal_mesh);

				uint64_t currentactormesh = read<uint64_t>(target_entity + 0x280);

				FVector hitbox3;
				Vector3 predhitbox;
				FVector hitbox;
				float projectileSpeed;//0;	
				float projectileGravityScale;//0;

				hitbox3 = Utilities->GetBoneLocation(closest_mesh, bone::HumanHead);
				predhitbox = Utilities->GetBoneLocation3(closest_mesh, bone::HumanHead);

				if (globals->HeadBone) {
					hitbox = Utilities->GetBoneLocation(closest_mesh, bone::HumanHead);
				}
				else if (globals->chestboone) {
					hitbox = Utilities->GetBoneLocation(closest_mesh, bone::HumanChest);
				}
				else if (globals->random) {
					switch (randomHitbox) {
					case 0:
						hitbox = Utilities->GetBoneLocation(closest_mesh, bone::HumanChest);
						break;
					case 1:
						hitbox = Utilities->GetBoneLocation(closest_mesh, bone::HumanLowerHead);
						break;
					case 2:
						hitbox = Utilities->GetBoneLocation(closest_mesh, bone::HumanChest);
						break;
					}
				}

				auto rootHead = Utilities->GetBoneWithRotation(closest_mesh, bone::HumanHead);
				Vector3 rootHeadOut = Utilities->ProjectWorldToScreen(rootHead);
				if (hitbox.x != 0 || hitbox.y != 0 && (Utilities->GetCrossDistance(hitbox.x, hitbox.y, globals->width / 2, globals->height / 2) <= globals->field_of_view))
				{
					if (Utilities->IsVisible(closest_mesh)) {
						if (GetAsyncKeyState_Spoofed(globals->aimbot_key))
						{

							FVector2d hitbox_screen = Utilities->WorldToScreen(hitbox);
							/*
							if (globals->prediction) {

								float projectileSpeed = 50000;
								float projectileGravityScale = 3.5f;

								auto closest_mesh2 = read<std::uint64_t>(target_entity + offset::skeletal_mesh);

								float distance = camera::location.Distance(hitbox);

								auto test = read<uintptr_t>(target_entity + offset::root_component);

								FVector Velocity = read<FVector>(test + 0x168);

								FVector Predictor = Utilities->Prediction(hitbox, Velocity, distance, projectileSpeed);

								FVector2d hitbox_screen_predict = Utilities->WorldToScreen(Predictor);

								Utilities->cursor_to(hitbox_screen_predict.x, hitbox_screen_predict.y - ((2 * projectileGravityScale) / 2));

							}
							else
								Utilities->cursor_to(hitbox_screen.x, hitbox_screen.y);
							*/

							Vector3 velocityy;

							std::string weapon;

							if (globals->prediction && weapon == ("Reaper Sniper Rifle"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 50000, 3.5);

								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else if (globals->prediction && weapon == ("HandCannon"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 6000, 2);

								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else if (globals->prediction && weapon == ("Warforged Assault Rifle"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 80000, 3.5);

								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else if (globals->prediction && weapon == ("Tactical Assault Rifle"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 80000, 3.5);


								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else if (globals->prediction && weapon == ("Nemesis AR"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 80000, 3.5);

								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else if (globals->prediction && weapon == ("Huntress DMR"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 96000, 2.5);

								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else if (globals->prediction && weapon == ("DrumGun"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 75000, 3);

								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else if (globals->prediction && weapon == ("Harbinger SMG"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 70000, 3);

								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else if (globals->prediction && weapon == ("Thunder Burst SMG"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 70000, 3);

								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else if (globals->prediction && weapon == ("Ranger Pistol"))
							{
								auto dist = camera::location.Distance(hitbox) / 100;
								auto target = Utilities->PredictPlayerPosition(predhitbox, dist, velocityy, 60000, 2);

								hitbox_screen.x = Utilities->ProjectWorldToScreen(target).x;
								hitbox_screen.y = Utilities->ProjectWorldToScreen(target).y;
							}
							else
							{
								Utilities->cursor_to(hitbox_screen.x, hitbox_screen.y);
							}
						}
					}
				}

			}
			else {
				target_dist = FLT_MAX;
				target_entity = NULL;
			}
		}
	};

} static game::c_game* Game = new game::c_game();