#pragma once
#include <cstdint>

enum perfect_skeleton {
	BONE_BOTTOM = 0,
	BONE_HEAD = 110,
	BONE_NECK = 66, // 67
	BONE_NECK2 = 8,
	BONE_CHEST = 7,
	BONE_PELVIS = 2,
	BONE_RSHOULDER = 38,
	BONE_RELBOW = 39,
	BONE_RHAND = 40,
	BONE_RTHIGH = 78,
	BONE_RKNEE = 79,
	BONE_RFOOT = 82,
	BONE_RFOOTOUT = 83,
	BONE_LSHOULDER = 9,
	BONE_LELBOW = 10,
	BONE_LHAND = 11,
	BONE_LTHIGH = 71,
	BONE_LKNEE = 72,
	BONE_LFOOT = 75,
	BONE_LFOOTOUT = 76
};

enum bone : int
{
	HumanBase = 0,
	HumanPelvis = 2,
	HumanLThigh1 = 71,
	HumanLThigh2 = 77,
	HumanLThigh3 = 72,
	HumanLCalf = 74,
	HumanLFoot2 = 75, // 73
	HumanLFoot = 76, // 86
	HumanLToe = 76,
	HumanRThigh1 = 78,
	HumanRThigh2 = 84,
	HumanRThigh3 = 79,
	HumanRCalf = 81,
	HumanRFoot2 = 82,
	HumanRFoot = 82, // 87
	HumanRToe = 83,
	HumanSpine1 = 7,
	HumanSpine2 = 5,
	HumanSpine3 = 2,
	HumanLCollarbone = 9, // 9
	HumanLUpperarm = 35,
	HumanLForearm1 = 36,
	HumanLForearm23 = 10,
	HumanLForearm2 = 34,
	HumanLForearm3 = 33,
	HumanLPalm = 32,
	HumanLHand = 11,
	HumanRCollarbone = 38, // 98
	HumanRUpperarm = 64,
	HumanRForearm1 = 65,
	HumanRForearm23 = 39,
	HumanRForearm2 = 63,
	HumanRForearm3 = 62,
	HumanRHand = 40,
	HumanRPalm = 58,
	HumanNeck = 66,
	HumanHead = 110,
	HumanLowerHead = 106,
	HumanChest = 65
};

enum offset { // all updated for v30.21
	uworld = 0x117105C8,
	game_instance = 0x1d8,
	game_state = 0x160,
	local_player = 0x38,
	player_controller = 0x30,
	acknowledged_pawn = 0x338,
	aactor = 0xA0,
	skeletal_mesh = 0x318,
	bonec = 0x600, // 0x5E8
	player_state = 0x2B0,
	root_component = 0x198,
	velocity = 0x168,
	relative_location = 0x120,
	relative_rotation = 0x138,
	current_weapon = 0xA68,
	weapon_data = 0x500,
	tier = 0x13b,
	Mesh = 0x318,
	team_index = 0x1211,
	player_array = 0x2a8,
	pawn_private = 0x308,
	component_to_world = 0x1C0,
	bone_array = 0x5B8,
	fLastSubmitTime = 0x2E8,
	fLastRenderTimeOnScreen = 0x2F0,
	location_under_reticle = 0x2560,
};
enum WeaponOffsets {
	CurrentWeapon = 0xA68,
	AmmoCount = 0xEEC,
	WeaponData = 0x510,
	Tier = 0x9A,
	ItemName = 0x40,
};
class Pointers {
public:
	uintptr_t
		uworld,
		game_instance,
		game_state,
		local_player,
		acknowledged_pawn,
		player_state,
		player_controller,
		root_component,
		skeletal_mesh,
		persistent_level,
		world_settings,
		WorldGravityZ,
		player_array,
		levels,

		player_array_size,
		current_weapon,
		current_vehicle;
	int
		team_index;
	FVector
		relative_location;

	Vector3
		LocalRelativeLocation;
}; static Pointers* pointer = new Pointers();






namespace camera
{
	FVector location, rotation;
	float fov;
}

struct camera_position_s {
	FVector location{};
	FVector rotation{};
	float fov{};
};
inline camera_position_s camera_postion{};




float target_dist = FLT_MAX;
uintptr_t  target_entity = NULL;
uintptr_t  desync_target_entity = NULL;
namespace utilities {
	class c_utilities {
	public:

		bool debug = 0; //debugs offsets

		auto InScreen(FVector2d screen_position) -> bool {

			if (screen_position.x > 0 && screen_position.x < globals->width && screen_position.y > 0 && screen_position.y < globals->height)
				return true;
			else
				return false;

		}

		auto IsShootable(FVector lur, FVector wl) -> bool {

			if (lur.x >= wl.x - 20 && lur.x <= wl.x + 20 && lur.y >= wl.y - 20 && lur.y <= wl.y + 20 && lur.z >= wl.z - 30 && lur.z <= wl.z + 30)
				return true;
			else
				return false;

		}

		auto IsVisible(uintptr_t skeletal_mesh) -> bool {
			auto last_submit = read<float>(skeletal_mesh + offset::fLastSubmitTime);
			auto last_render = read<float>(skeletal_mesh + offset::fLastRenderTimeOnScreen);
			const float fVisionTick = 0.06f;
			bool visible = last_render + fVisionTick >= last_submit;
			return visible;
		}

		static auto GetBoneLocation(uintptr_t skeletal_mesh, int bone_index) -> FVector {

			uintptr_t bone_array = read<uintptr_t>(skeletal_mesh + offset::bone_array);
			if (bone_array == NULL) bone_array = read<uintptr_t>(skeletal_mesh + offset::bone_array + 0x10);
			ftransform bone = read<ftransform>(bone_array + (bone_index * 0x60));
			ftransform component_to_world = read<ftransform>(skeletal_mesh + offset::component_to_world);
			D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
			return FVector(matrix._41, matrix._42, matrix._43);
		}

		static auto GetBoneLocation3(uintptr_t skeletal_mesh, int bone_index) -> Vector3 {

			uintptr_t bone_array = read<uintptr_t>(skeletal_mesh + offset::bone_array);
			if (bone_array == NULL) bone_array = read<uintptr_t>(skeletal_mesh + offset::bone_array + 0x10);
			ftransform bone = read<ftransform>(bone_array + (bone_index * 0x60));
			ftransform component_to_world = read<ftransform>(skeletal_mesh + offset::component_to_world);
			D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
			return Vector3(matrix._41, matrix._42, matrix._43);
		}

		struct FMinimalViewInfo
		{
			FVector Location, Rotation;
			float FOV;
		};

		struct CamewaDescwipsion
		{
			Vector3 Location;
			Vector3 Rotation;
			float FieldOfView;
		};

		static auto UpdateCamera() -> void
		{
			auto location_pointer = read<uintptr_t>(pointer->uworld + 0x110);
			auto rotation_pointer = read<uintptr_t>(pointer->uworld + 0x120);

			struct FNRotation
			{
				double a; //0x0000
				char pad_0008[24]; //0x0008
				double b; //0x0020
				char pad_0028[424]; //0x0028
				double c; //0x01D0
			}tpmrotation;

			tpmrotation.a = read<double>(rotation_pointer);
			tpmrotation.b = read<double>(rotation_pointer + 0x20);
			tpmrotation.c = read<double>(rotation_pointer + 0x1d0);

			camera::rotation.x = asin(tpmrotation.c) * (180.0 / M_PI);
			camera::rotation.y = ((atan2(tpmrotation.a * -1, tpmrotation.b) * (180.0 / M_PI)) * -1) * -1;
			camera::rotation.z = 0;
			camera::location = read<FVector>(location_pointer);
			camera::fov = read<float>(pointer->player_controller + 0x394) * 90.f;
		}

		struct cdecrypt
		{
			Vector3 location;
			Vector3 rotation;
			float fov;
		};
		__forceinline auto viewpoint() -> cdecrypt
		{
			uintptr_t cachedgworld = read<uintptr_t>(globals->imagebase + offset::uworld);
			uintptr_t cachedgameinstance = read<uintptr_t>(cachedgworld + offset::game_instance);
			uintptr_t cachedlocalplayers = read<uintptr_t>(cachedgameinstance + offset::local_player);
			uintptr_t cachedlocalplayer = read<uintptr_t>(cachedlocalplayers);
			uintptr_t cachedplayercontroller = read<uintptr_t>(cachedlocalplayer + offset::player_controller);

			cdecrypt camera;

			auto locationp = read<uintptr_t>(cachedgworld + 0x110);
			auto rotationp = read<uintptr_t>(cachedgworld + 0x120);

			struct fnrotation
			{
				double a;
				double b;
				double c;
			}fnrotation;

			fnrotation.a = read<double>(rotationp);
			fnrotation.b = read<double>(rotationp + 0x20);
			fnrotation.c = read<double>(rotationp + 0x1d0);

			camera.location = read<Vector3>(locationp);
			camera.rotation.x = asin(fnrotation.c) * (180.0 / M_PI);
			camera.rotation.y = ((atan2(fnrotation.a * -1, fnrotation.b) * (180.0 / M_PI)) * -1) * -1;
			camera.fov = read<float>((uintptr_t)cachedplayercontroller + 0x394) * 90.f;

			return camera;
		}
		


		/*void RadarRange(float* x, float* y, float range)
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
		}*/

		/*void CalcRadarPoint(FVector vOrigin, int& screenx, int& screeny)
		{
			FVector vAngle = camera_postion.rotation;
			auto fYaw = vAngle.y * M_PI / 180.0f;
			float dx = vOrigin.x - camera_postion.location.x;
			float fy = vOrigin.y - camera_postion.location.y;

			float fsin_yaw = sinf((fYaw));
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


		}*/

		__forceinline auto matrix(Vector3 rotation, Vector3 origin = Vector3(0, 0, 0))
		{
			double radPitch = (rotation.x * double(M_PI) / 180.f);
			double radYaw = (rotation.y * double(M_PI) / 180.f);
			double radRoll = (rotation.z * double(M_PI) / 180.f);

			double SP = sinf(radPitch);
			double CP = cosf(radPitch);
			double SY = sinf(radYaw);
			double CY = cosf(radYaw);
			double SR = sinf(radRoll);
			double CR = cosf(radRoll);

			D3DMATRIX matrix;
			matrix.m[0][0] = CP * CY;
			matrix.m[0][1] = CP * SY;
			matrix.m[0][2] = SP;
			matrix.m[0][3] = 0.f;

			matrix.m[1][0] = SR * SP * CY - CR * SY;
			matrix.m[1][1] = SR * SP * SY + CR * CY;
			matrix.m[1][2] = -SR * CP;
			matrix.m[1][3] = 0.f;

			matrix.m[2][0] = -(CR * SP * CY + SR * SY);
			matrix.m[2][1] = CY * SR - CR * SP * SY;
			matrix.m[2][2] = CR * CP;
			matrix.m[2][3] = 0.f;

			matrix.m[3][0] = origin.x;
			matrix.m[3][1] = origin.y;
			matrix.m[3][2] = origin.z;
			matrix.m[3][3] = 1.f;

			return matrix;
		}
		static auto WorldToScreen(FVector worldlocation) -> FVector2d {


			D3DMATRIX tempMatrix = Matrix(camera::rotation);

			FVector vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
			FVector vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
			FVector vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

			FVector vDelta = worldlocation - camera::location;
			FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

			if (vTransformed.z < 1.f)
				vTransformed.z = 1.f;
			return FVector2d((globals->width / 2.0f) + vTransformed.x * (((globals->width / 2.0f) / tanf(camera::fov * (float)M_PI / 360.f))) / vTransformed.z, (globals->height / 2.0f) - vTransformed.y * (((globals->width / 2.0f) / tanf(camera::fov * (float)M_PI / 360.f))) / vTransformed.z);

		}
		int screen_width1;
		int screen_heigh1t;


		

		
		
		
		
		ftransform GetBoneIndex(uint64_t mesh, int index)
		{
			uint64_t bonearray = read<uint64_t>(mesh + 0x4a8);
			if (!bonearray) bonearray = read<uint64_t>(mesh + 0x4a8 + 0x10);
			return read<ftransform>(bonearray + (index * 0x30));
		}

		Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
		{
			ftransform bone = GetBoneIndex(mesh, id);
			ftransform ComponentToWorld = read<ftransform>(mesh + offset::component_to_world);

			D3DMATRIX Matrix;
			Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

			return Vector3(Matrix._41, Matrix._42, Matrix._43);
		}






		int Width = GetSystemMetrics(SM_CXSCREEN);
		int Height = GetSystemMetrics(SM_CYSCREEN);

		float zoom;

		Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
		{
			Vector3 Screenlocation = Vector3(0, 0, 0);
			Vector3 Camera;

			auto chain69 = read<uintptr_t>(offset::local_player + 0xA8);
			uint64_t chain699 = read<uintptr_t>(chain69 + 8);

			Camera.x = read<float>(chain699 + 0x7F8);
			//printf("Camera.x :0x%llX\n", Camera.x);
			Camera.y = read<float>(offset::root_component + 0x12C);

			float test = asin(Camera.x);
			float degrees = test * (180.0 / M_PI);
			Camera.x = degrees;

			if (Camera.y < 0)
				Camera.y = 360 + Camera.y;

			D3DMATRIX tempMatrix = Matrix2(Camera);
			Vector3 vAxisX, vAxisY, vAxisZ;

			vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
			vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
			vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

			uint64_t chain = read<uint64_t>(offset::local_player + 0x70);
			uint64_t chain1 = read<uint64_t>(chain + 0x98);
			uint64_t chain2 = read<uint64_t>(chain1 + 0x140);
			//printf("chain2 :0x%llX\n", chain2);

			Vector3 vDelta = WorldLocation - read<Vector3>(chain2 + 0x10);
			Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

			if (vTransformed.z < 1.f)
				vTransformed.z = 1.f;

			zoom = read<float>(chain699 + 0x590);
			//printf("zoom %f\n", zoom);

			float FovAngle = 80.0f / (zoom / 1.19f);
			float ScreenCenterX = Width / 2.0f;
			float ScreenCenterY = Height / 2.0f;

			Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
			Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

			return Screenlocation;
		}

		auto cursor_to(float x, float y) -> void {
			FVector center(globals->width / 2, globals->height / 2, 0);
			FVector target(0, 0, 0);

			if (x != 0) {
				if (x > center.x) {
					target.x = -(center.x - x);
					target.x /= (globals->smooth);
					if (target.x + center.x > center.x * 2)
						target.x = 0;
				}

				if (x < center.x) {
					target.x = x - center.x;
					target.x /= (globals->smooth);
					if (target.x + center.x < 0)
						target.x = 0;
				}
			}
			if (y != 0) {
				if (y > center.y) {
					target.y = -(center.y - y);
					target.y /= (globals->smooth);
					if (target.y + center.y > center.y * 2)
						target.y = 0;
				}

				if (y < center.y) {
					target.y = y - center.y;
					target.y /= (globals->smooth);
					if (target.y + center.y < 0)
						target.y = 0;
				}
			}

			const float snapThreshold = 1.0f;
			if (std::abs(target.x) < snapThreshold) {
				target.x = 0;
			}
			if (std::abs(target.y) < snapThreshold) {
				target.y = 0;
			}
			HKSSPFF;
			Inject->set_cursor_position(target.x, target.y);

		}

		auto cursor_to2(float x, float y) -> void {
			FVector center(globals->width / 2, globals->height / 2, 0);
			FVector target(0, 0, 0);

			target.x = (x > center.x) ? -(center.x - x) : x - center.x;
			target.x /= globals->smooth;
			target.x = (target.x + center.x > center.x * 2 || target.x + center.x < 0) ? 0 : target.x;

			target.y = (y > center.y) ? -(center.y - y) : y - center.y;
			target.y /= globals->smooth;
			target.y = (target.y + center.y > center.y * 2 || target.y + center.y < 0) ? 0 : target.y;

			FVector Angles(-target.y / globals->smooth, target.x / globals->smooth, 0);


			const float snapThreshold = 1.0f;
			if (std::abs(target.x) < snapThreshold) {
				target.x = 0;
			}
			if (std::abs(target.y) < snapThreshold) {
				target.y = 0;
			}


			//write<FVector>(pointer->player_controller + 0x520, Angles);
			//write<FVector>(pointer->player_controller + 0x528, Angles);


		}

		FVector calc_angle(FVector LocalPos, FVector WorldPos) {
			FVector RelativePos = WorldPos - LocalPos;
			float yaw = atan2(RelativePos.y, RelativePos.x) * 180.0f / M_PI;
			float pitch = -((acos(RelativePos.z / LocalPos.Distance(WorldPos)) * 180.0f / M_PI) - 90.0f);
			return FVector(pitch, yaw, 0);
		}

		static void normalize(FVector& in)
		{
			while (in.x > 180.0f)
				in.x -= 360.0f;

			while (in.x < -180.0f)
				in.x += 360.0f;

			while (in.y > 180.0f)
				in.y -= 360.0f;

			while (in.y < -180.0f)
				in.y += 360.0f;

			in.z = 0;
		}

		__forceinline double Clamp(double value, double min, double max)
		{
			if (value < min) {
				return min;
			}
			else if (value > max) {
				return max;
			}
			else {
				return value;
			}
		}

		FVector Prediction(FVector TargetPosition, FVector ComponentVelocity, float player_distance, float ProjectileSpeed = 239)
		{
			float gravity = 3.5;
			float TimeToTarget = player_distance / ProjectileSpeed;
			float bulletDrop = abs(gravity) * (TimeToTarget * TimeToTarget) * 0.5;
			return FVector
			{
			TargetPosition.x + TimeToTarget * ComponentVelocity.x,
			TargetPosition.y + TimeToTarget * ComponentVelocity.y,
			TargetPosition.z + TimeToTarget * ComponentVelocity.z + bulletDrop
			};
		}

		float custom_fabsf(float x) {

			__m128 x_vec = _mm_set_ss(x);
			x_vec = _mm_and_ps(x_vec, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
			return _mm_cvtss_f32(x_vec);
		}

		Vector3 PredictPlayerPosition(Vector3 CurrentLocation, float Distance, Vector3 Velocity, float speed, float gravity)
		{
			Vector3 CalculatedPosition = CurrentLocation;
			if (!speed) return CalculatedPosition;

			float TimeToTarget = Distance / custom_fabsf(speed);
			float CalculatedBulletDrop = (((-2800) * 0.5f * (TimeToTarget))) * 10;

			CalculatedPosition.x += Velocity.x * (TimeToTarget) * 120;
			CalculatedPosition.y += Velocity.y * (TimeToTarget) * 120;
			CalculatedPosition.z += Velocity.z * (TimeToTarget) * 120;
			CalculatedPosition.z += custom_fabsf((-49000 / 50) * gravity) / 2.0f * (TimeToTarget * 25);

			return CalculatedPosition;
		}

		__forceinline auto platform(uintptr_t PlayerState) -> std::string
		{

			uintptr_t test_platform = read<uintptr_t>(PlayerState + 0x438);
			wchar_t platform[64]{ 0 };
			//mem::read_physical(reinterpret_cast<PVOID>(test_platform), reinterpret_cast<uint8_t*>(platform), sizeof(platform));
			std::wstring platform_wstr(platform);
			std::string platform_str(platform_wstr.begin(), platform_wstr.end());
			return platform_str;
		}

		auto get_player_name(uintptr_t player_state) -> std::string
		{
			auto fstring = read<__int64>(player_state + 0xAB0);
			auto length = read<int>(fstring + 16);

			auto v6 = (__int64)length;

			if (!v6) return std::string(skCrypt("NPC"));

			auto IsABot = read<char>(player_state + 0x29A & 0x8);

			if (IsABot) return std::string(skCrypt("AI"));

			auto ftext = (uintptr_t)read<__int64>(fstring + 8);

			wchar_t* buffer = new wchar_t[length];
			//read(PVOID(static_cast<ULONGLONG>(ftext)), buffer, length * sizeof(wchar_t));

			char v21;
			int v22;
			int i;

			int v25;
			UINT16* v23;

			v21 = v6 - 1;
			if (!(UINT32)v6)
				v21 = 0;
			v22 = 0;
			v23 = (UINT16*)buffer;
			for (i = (v21) & 3; ; *v23++ += i & 7)
			{
				v25 = v6 - 1;
				if (!(UINT32)v6)
					v25 = 0;
				if (v22 >= v25)
					break;
				i += 3;
				++v22;
			}

			std::wstring username{ buffer };
			delete[] buffer;
			return std::string(username.begin(), username.end());
		}

		static float powf_(float _X, float _Y) {
			return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))));
		}
		static float sqrtf_(float _X) {
			return (_mm_cvtss_f32(_mm_sqrt_ps(_mm_set_ss(_X))));
		}
		static double GetCrossDistance(double x1, double y1, double x2, double y2) {
			return sqrtf(powf((x2 - x1), 2) + powf_((y2 - y1), 2));
		}

	};
} static utilities::c_utilities* Utilities = new utilities::c_utilities(); 
