#define M_PI 3.14159265358979323846264338327950288419716939937510

class Vector2
{
public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(double _x, double _y) : x(_x), y(_y) {}
	~Vector2() {}
	double x, y;
};

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	~Vector3() {}
	double x, y, z;
	inline double dot(Vector3 v) { return x * v.x + y * v.y + z * v.z; }
	inline double distance(Vector3 v) { return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0))); }
	Vector3 operator-(Vector3 v) { return Vector3(x - v.x, y - v.y, z - v.z); }
};



struct FQuat { double x, y, z, w; };
struct FTransform
{
	FQuat rotation;
	Vector3 translation;
	uint8_t pad1c[0x8];
	Vector3 scale3d;
	uint8_t pad2c[0x8];

	D3DMATRIX to_matrix_with_scale()
	{
		D3DMATRIX m{};

		const Vector3 Scale
		(
			(scale3d.x == 0.0) ? 1.0 : scale3d.x,
			(scale3d.y == 0.0) ? 1.0 : scale3d.y,
			(scale3d.z == 0.0) ? 1.0 : scale3d.z
		);

		const double x2 = rotation.x + rotation.x;
		const double y2 = rotation.y + rotation.y;
		const double z2 = rotation.z + rotation.z;
		const double xx2 = rotation.x * x2;
		const double yy2 = rotation.y * y2;
		const double zz2 = rotation.z * z2;
		const double yz2 = rotation.y * z2;
		const double wx2 = rotation.w * x2;
		const double xy2 = rotation.x * y2;
		const double wz2 = rotation.w * z2;
		const double xz2 = rotation.x * z2;
		const double wy2 = rotation.w * y2;

		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;
		m._11 = (1.0f - (yy2 + zz2)) * Scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * Scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * Scale.z;
		m._32 = (yz2 - wx2) * Scale.z;
		m._23 = (yz2 + wx2) * Scale.y;
		m._21 = (xy2 - wz2) * Scale.y;
		m._12 = (xy2 + wz2) * Scale.x;
		m._31 = (xz2 + wy2) * Scale.z;
		m._13 = (xz2 - wy2) * Scale.x;
		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

inline D3DMATRIX matrix_multiplication(D3DMATRIX pm1, D3DMATRIX pm2)
{
	D3DMATRIX pout{};
	pout._11 = pm1._11 * pm2._11 + pm1._12 * pm2._21 + pm1._13 * pm2._31 + pm1._14 * pm2._41;
	pout._12 = pm1._11 * pm2._12 + pm1._12 * pm2._22 + pm1._13 * pm2._32 + pm1._14 * pm2._42;
	pout._13 = pm1._11 * pm2._13 + pm1._12 * pm2._23 + pm1._13 * pm2._33 + pm1._14 * pm2._43;
	pout._14 = pm1._11 * pm2._14 + pm1._12 * pm2._24 + pm1._13 * pm2._34 + pm1._14 * pm2._44;
	pout._21 = pm1._21 * pm2._11 + pm1._22 * pm2._21 + pm1._23 * pm2._31 + pm1._24 * pm2._41;
	pout._22 = pm1._21 * pm2._12 + pm1._22 * pm2._22 + pm1._23 * pm2._32 + pm1._24 * pm2._42;
	pout._23 = pm1._21 * pm2._13 + pm1._22 * pm2._23 + pm1._23 * pm2._33 + pm1._24 * pm2._43;
	pout._24 = pm1._21 * pm2._14 + pm1._22 * pm2._24 + pm1._23 * pm2._34 + pm1._24 * pm2._44;
	pout._31 = pm1._31 * pm2._11 + pm1._32 * pm2._21 + pm1._33 * pm2._31 + pm1._34 * pm2._41;
	pout._32 = pm1._31 * pm2._12 + pm1._32 * pm2._22 + pm1._33 * pm2._32 + pm1._34 * pm2._42;
	pout._33 = pm1._31 * pm2._13 + pm1._32 * pm2._23 + pm1._33 * pm2._33 + pm1._34 * pm2._43;
	pout._34 = pm1._31 * pm2._14 + pm1._32 * pm2._24 + pm1._33 * pm2._34 + pm1._34 * pm2._44;
	pout._41 = pm1._41 * pm2._11 + pm1._42 * pm2._21 + pm1._43 * pm2._31 + pm1._44 * pm2._41;
	pout._42 = pm1._41 * pm2._12 + pm1._42 * pm2._22 + pm1._43 * pm2._32 + pm1._44 * pm2._42;
	pout._43 = pm1._41 * pm2._13 + pm1._42 * pm2._23 + pm1._43 * pm2._33 + pm1._44 * pm2._43;
	pout._44 = pm1._41 * pm2._14 + pm1._42 * pm2._24 + pm1._43 * pm2._34 + pm1._44 * pm2._44;
	return pout;
}

inline D3DMATRIX to_matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radpitch = (rot.x * M_PI / 180);
	float radyaw = (rot.y * M_PI / 180);
	float radroll = (rot.z * M_PI / 180);
	float sp = sinf(radpitch);
	float cp = cosf(radpitch);
	float sy = sinf(radyaw);
	float cy = cosf(radyaw);
	float sr = sinf(radroll);
	float cr = cosf(radroll);
	D3DMATRIX matrix{};
	matrix.m[0][0] = cp * cy;
	matrix.m[0][1] = cp * sy;
	matrix.m[0][2] = sp;
	matrix.m[0][3] = 0.f;
	matrix.m[1][0] = sr * sp * cy - cr * sy;
	matrix.m[1][1] = sr * sp * sy + cr * cy;
	matrix.m[1][2] = -sr * cp;
	matrix.m[1][3] = 0.f;
	matrix.m[2][0] = -(cr * sp * cy + sr * sy);
	matrix.m[2][1] = cy * sr - cr * sp * sy;
	matrix.m[2][2] = cr * cp;
	matrix.m[2][3] = 0.f;
	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;
	return matrix;
}

struct FNRot
{
	double a;
	char pad_0008[24];
	double b;
	char pad_0028[424];
	double c;
};

namespace fracture::sdk
{
	class Fortnite
	{
	public: 

		class UWorld
		{
		private:
			uintptr_t world;

			// caching
		public:
			auto cache() -> void
			{
				world = read<uintptr_t>(virtualaddy + 0x14942840);
			}

			auto uworld() -> uintptr_t { return world; }
		};

		class UGameInstance
		{
		private:
			uintptr_t gi = 0;
			uintptr_t lp = 0;

		public:
			auto cache(uintptr_t world) -> void
			{
				gi = read<uintptr_t>(world + 0x228);
				lp = read<uintptr_t>(read<uintptr_t>(gi + 0x38));
			}

			auto GameInstance() -> uintptr_t { return gi; }
			auto LocalPlayers() -> uintptr_t { return lp; }
		};

		class AGameState
		{
		private:
			uintptr_t gs = 0;
			uintptr_t pa = 0;
			int pc = 0;

		public:
			
			auto cache(uintptr_t world) -> void
			{
				gs = read<uintptr_t>(world + 0x1B0);
				pa = read<uintptr_t>(gs + 0x2c0);
				pc = read<int>(gs + (0x2c0 + sizeof(uintptr_t)));
			}

			auto GameSate() -> uintptr_t { return gs; }
			auto PlayerArray() -> uintptr_t { return pa; }
			auto PlayerCount() -> int { return pc; }
		};

		class APlayer
		{
		private:
			uintptr_t state = 0;
			uintptr_t pawn = 0;
			uintptr_t mesh = 0;
			int teamID = -1;
			Vector3 location = { 0, 0, 0 };

		public:

			enum Bones
			{
				HEAD = 67,
				NECK = 66,

				LEFT_SHOULDER = 9,
				LEFT_ELBOW = 10,
				LEFT_HAND = 11,

				RIGHT_SHOULDER = 38,
				RIGHT_ELBOW = 39,
				RIGHT_HAND = 40,

				PELVIS = 2,

				LEFT_HIP = 71,
				LEFT_KNEE = 72,
				LEFT_FOOT = 75,

				RIGHT_HIP = 78,
				RIGHT_KNEE = 79,
				RIGHT_FOOT = 82
			};

			bool IsValid() const {
				return state && pawn && mesh;
			}

			auto cache(uintptr_t playerarray, int i) -> void
			{
				state = read<uintptr_t>(playerarray + (i * sizeof(uintptr_t)));
				pawn = read<uintptr_t>(state + 0x320);
				mesh = read<uintptr_t>(pawn + 0x328);
				teamID = read<int>(state + 0x1291);
			}

			auto PlayerState() -> uintptr_t { return state; }
			auto PlayerPawn() -> uintptr_t { return pawn; }
			auto PlayerMesh() -> uintptr_t { return mesh; }
			auto PlayerTeamID() -> int { return teamID; }

			auto get_bone(uintptr_t mesh, int bone_index) -> Vector3
			{
				uintptr_t bone_array = read<uintptr_t>(mesh + 0x5E8);
				if (bone_array == 0) bone_array = read<uintptr_t>(mesh + 0x5F8);
				FTransform bone = read<FTransform>(bone_array + (bone_index * 0x60));
				FTransform component_to_world = read<FTransform>(mesh + 0x1E0);
				D3DMATRIX matrix = matrix_multiplication(bone.to_matrix_with_scale(), component_to_world.to_matrix_with_scale());
				return Vector3(matrix._41, matrix._42, matrix._43);
			}

			auto is_visible(uintptr_t uworld) -> bool
			{
				auto Seconds = read<double>(uworld + 0x180);
				auto LastRenderTime = read<float>(mesh + 0x2f0);
				return Seconds - LastRenderTime <= 0.06f;
			}
		};
		
		class ALocalPLayer : public APlayer
		{
		private:
			uintptr_t controller = 0;
			uintptr_t local_pawn = 0;
			uintptr_t root_component = 0;
			int local_teamID = -1;
			Vector3 local_location = { 0, 0, 0 };

		public:
			 
			auto cache(uintptr_t localplayer)
			{
				controller = read<uintptr_t>(localplayer + 0x30);
				local_pawn = read<uintptr_t>(controller + 0x350);
				root_component = read<uintptr_t>(local_pawn + 0x1B0);
				local_location = read<Vector3>(root_component + 0x138);
				uintptr_t player_state = read<uintptr_t>(local_pawn + 0x2C8);
				local_teamID = read<int>(player_state + 0x1291);
			}

			auto LocalPlayerController() -> uintptr_t { return controller; }
			auto LocalPlayerPawn() -> uintptr_t { return local_pawn; }
			auto RootComponent() -> uintptr_t { return root_component; }
			auto LocalLocation() const -> Vector3 { return local_location; }
			auto LocalTeamID() const -> int { return local_teamID; }
		};
	};
}
