#include <Windows.h>


namespace PlayerColor {
	float BoxVisible[4] = { 1.f, 0.1f, 0.7f, 1.0f };
	float BoxNotVisible[4] = { 0.f, 0.0f, 0.0f, 1.0f };

	float SkeletonVisible[4] = { 1.f, 0.1f, 0.7f, 1.0f };
	float SkeletonNotVisible[4] = { 0.f, 0.0f, 0.0f, 1.0f };

	float targetcol[4] = { 1.f, 0, 0, 1.0f };
	
	float accentcolor[4] = { 1.f, 1.f, 1.f, 1.0f };

	float SnaplineVisible[4];
	float SnaplineNotVisible[4];

	float TopTextVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };
	float TopTextNotVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };

	float BottomTextVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };
	float BottomTextNotVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };




}
namespace Misc
{

	inline bool enable_menu = 0;
	inline int Selection = 1;

	inline bool fps = 1;
	inline bool Teamcheck = 1;
	inline bool VSYNC = 0;
	inline bool KillCount = 0;
	inline bool playerid = 0;
	inline bool playercount = 0;
	float fontsize = 14;

	inline bool Crosshair = 0;
	inline bool weapon_cfg = 1;


	inline int skeleton_thick = 1.5f;
	inline int box_thick = 1.5f;
	inline int line_thick = 1.5;
	inline int HeadSized = 1;

	// colors

	static float box_visible[3] = { 0.f, 1.f, 0.f };
	static float box_invisible[3] = { 1.f, 0.f, 0.f };

	static float box_filled_visible[3] = { 0.f, 1.f, 0.f };
	static float box_filled_invisible[3] = { 1.f, 0.f, 0.f };

	static float skeleton_visible[3] = { 0.f, 1.f, 0.f };
	static float skeleton_invisible[3] = { 1.f, 0.f, 0.f };

	static float text_visible[3] = { 1.0f, 1.0f, 0.0f };
	static float text_invisible[3] = { 1.0f, 1.0f, 0.0f };

	static float snaplines_visible[3] = { 0.f, 1.f, 0.f };
	static float snaplines_invisible[3] = { 1.f, 0.f, 0.f };

}
class c_globals {
public:
	int width, height;
	bool render_menu = 0;
	bool crsm = 0;
	HWND hwnd = 0;

	bool bounding_box = 0;
	bool rounded;
	bool headbox = 0;
	bool radarrrrrr;
	bool g_username = false;
	bool enableeenigger;
	bool corneredboxes = false;
	uintptr_t imagebase;

	int radar_pos_x = 20.f;
	int radar_pos_y = 50.f;
	int radar_size = 200.f;
	int radar_range = 60.f;
	bool outlinebox = 1;
	bool outlineskele = 1;
	bool magicbullet = 0;
	bool carflyyy = 0;
	bool corner = false;
	bool norecoilnigger = 0;


	bool distance = 0;
	bool outline = 1;
	bool platform = false;
	bool text_outline = 0;
	bool weapon_esp = 0;
	bool biscoteiloveu = 1;
	bool fontsize;

	float font_size = 16;
	bool filledboxesssssss = false;
	int add = 3;
	bool head = 0;
	int Smoothing = 1;
	bool aimbot = 1;

	bool HeadBone = 1;
	bool chestboone = 1;
	bool target_line = 0;
	bool target_text = 0;
	bool player_info = 0;

	bool ChestBone = 0;
	bool random = 0;
	bool chests = 0;





	bool snaplin = 0;
	bool smvskele = false;
	bool vehicle = 0;
	bool Lamma = 0;
	int max_distance = 1000;
	int max_distance1 = 1000;
	int max_distance2 = 1000;
	int max_distance3 = 1000;
	int max_distance4 = 1000;


	int smooth = 3.0f;
	int triggerbot_key = VK_LMENU;
	int field_of_view = 100.0f;
	bool rendercrosshair = 0;
	bool rendercrosshairefk = 0;


	bool normal_fov = 1;
	bool weapon_cfg = 1;
	int aimbot_bone = 0;
	bool prediction = 1;

	bool color_crosshair = 1;
	bool box_color = 1;

	//Skeleton
	bool skeleton = 0;
	bool outlines_skel = 1;

	//Name
	bool show_name = 0;
	int player_array_size = false;
	
	int aimbot_key = VK_LMENU;
	//int triggerbot_key;
	bool showSelectKeyModal;
	bool DoVisibleCheck = 1;
	bool keyselected;
	bool triggerbot = 1;
	int custom_delay = 1;
	int maximum_distance = 20;
	bool has_clicked;
	std::chrono::steady_clock::time_point tb_begin;
	std::chrono::steady_clock::time_point tb_end;
	int tb_time_since;

	int Linethick = 2.f;

	float box_thick = 1.f, skeleton_thick = 1.f;
	bool vsync = false;

	float g_color_Visible[4] = { 0 , 255, 255 }; // { 87 / 255.0f, 173 / 255.0f, 17 / 255.0f };
	float  g_color_invisible[4] = { 255, 0, 255 }; // { 173 / 255.0f, 17 / 255.0f, 17 / 255.0f };
	float distancecolor[3] = {255, 255, 255};

}; static c_globals* globals = new c_globals();

inline float radar_pos_x = 20.f;
inline float radar_pos_y = 50.f;
inline float radar_size = 200.f;
inline float radar_range = 60.f;


static float g_platform_color[3] = { 1.00f, 1.00f, 1.00f };

namespace Important1
{
	class func
	{
	public:
		// Player Name

		auto get_player_name(__int64 player_state) -> std::string
		{
			//SPOOF_FUNC;

			int pNameLength;
			_WORD* pNameBufferPointer;
			int i;
			char v25;
			int v26;
			int v29;

			char16_t* pNameBuffer;

			uintptr_t pNameStructure = read<uintptr_t>(player_state + 0xAB0);
			if (is_valid(pNameStructure)) {
				pNameLength = read<int>(pNameStructure + 0x10);
				if (pNameLength <= 0)
					return "AI";

				pNameBuffer = new char16_t[pNameLength];
				uintptr_t pNameEncryptedBuffer = read <uintptr_t>(pNameStructure + 0x8);
				if (is_valid(pNameEncryptedBuffer)) {
					// read_array(pNameEncryptedBuffer, pNameBuffer, pNameLength);

					v25 = pNameLength - 1;
					v26 = 0;
					pNameBufferPointer = (_WORD*)pNameBuffer;

					for (i = (v25) & 3;; *pNameBufferPointer++ += i & 7) {
						v29 = pNameLength - 1;
						if (!(_DWORD)pNameLength)
							v29 = 0;

						if (v26 >= v29)
							break;

						i += 3;
						++v26;
					}

					std::u16string temp_wstring(pNameBuffer);
					delete[] pNameBuffer;
					return std::string(temp_wstring.begin(), temp_wstring.end());
				}
			}
		}
	// prediction

		FVector Anis_Target_Prediction(FVector target, FVector targetVelocity, float projectileSpeed, float projectileGravityScale, float distance)
		{
			//SPOOF_FUNC;

			float horizontalTime = distance / projectileSpeed;
			float verticalTime = distance / projectileSpeed;

			target.x += targetVelocity.x * horizontalTime;
			target.y += targetVelocity.y * horizontalTime;
			target.z += targetVelocity.z * verticalTime + abs(-980 * projectileGravityScale) * 0.5f * (verticalTime * verticalTime);

			return target;
		}
	};  static Important1::func* Important = new Important1::func();
}