#include "utils.h"
#include "../sdk/sdk.hpp"
#include "../interfaces/ModelCache.h"
#include "../interfaces/ConVar.h"
#include <memory.h>
#include "../sdk/csgo.h"
std::uint8_t* utils::PatternScan(void* module, const char* signature)
{
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(signature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scanBytes[i];
		}
	}
	return nullptr;
}
#define INRANGE( x,a,b )  ( x >= a && x <= b ) 
#define GETBITS( x )    ( INRANGE( ( x& ( ~0x20 ) ),'A','F' ) ? ( ( x&( ~0x20 ) ) - 'A' + 0xa ) : ( INRANGE( x,'0','9' ) ? x - '0' : 0) )
#define GETBYTE( x )    ( GETBITS( x[ 0 ] ) << 4 | GETBITS( x[ 1 ] ) )
unsigned long utils::pattern_scan(const char* module_name, const char* signature)
{
	MODULEINFO module_info;

	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module_name), &module_info, sizeof(MODULEINFO));

	DWORD start_addr = (DWORD)module_info.lpBaseOfDll;
	DWORD end_addr = start_addr + module_info.SizeOfImage;
	const char* pat = signature;

	DWORD first_match = 0;

	for (DWORD cur = start_addr; cur < end_addr; cur++) {
		if (!*pat) return first_match;
		if (*(PBYTE)pat == '\?' || *(BYTE*)cur == GETBYTE(pat)) {
			if (!first_match) first_match = cur;
			if (!pat[2]) return first_match;
			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
			else pat += 2;
		}
		else {
			pat = signature;
			first_match = 0;
		}
	}

	return NULL;
}
DWORD utils::pattern_mask_scan(const char* module_n, const BYTE* mask, const char* mask_string)
{
	/// Get module address
	const unsigned int module_address = reinterpret_cast<unsigned int>(GetModuleHandle(module_n));

	/// Get module information to the size
	MODULEINFO module_info;
	GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(module_address), &module_info, sizeof(MODULEINFO));

	auto IsCorrectMask = [](const unsigned char* data, const unsigned char* mask, const char* mask_string) -> bool
	{
		for (; *mask_string; ++mask_string, ++mask, ++data)
			if (*mask_string == 'x' && *mask != *data)
				return false;

		return (*mask_string) == 0;
	};

	/// Iterate until we find a matching mask
	for (unsigned int c = 0; c < module_info.SizeOfImage; c += 1)
	{
		/// does it match?
		if (IsCorrectMask(reinterpret_cast<unsigned char*>(module_address + c), mask, mask_string))
			return (module_address + c);
	}

	return 0;
}
void* utils::getInterface(const char* dllname, const char* interfacename)
{
	typedef void* (__cdecl* tCreateInterface)(const char* module, int* returnCode);
	tCreateInterface call = (tCreateInterface)(GetProcAddress(GetModuleHandleA(dllname), "CreateInterface"));
	std::string buffer = "";
	void* pClass;
	buffer.resize(50);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			buffer = interfacename;
			buffer += std::to_string(i);
			buffer += std::to_string(j);
			pClass = call(buffer.c_str(), 0);
			if (pClass)
			{
				return pClass;
			}
			else continue;
		}		
	}	
	return  pClass;//or return call(inter..., 0);
}
void* utils::getInterfaceStatic(const char* dllname, const char* interfacename)
{
	typedef void* (__cdecl* tCreateInterface)(const char* module, int* returnCode);
	tCreateInterface call = (tCreateInterface)(GetProcAddress(GetModuleHandleA(dllname), "CreateInterface"));		
	return call(interfacename, 0);
}
void utils::init_key_values(KeyValues* keyvalues, const char* key_name)
{
	using InitKeyValues_t = void(__thiscall*)(void*, const char*);
	static InitKeyValues_t InitKeyValuesEx = nullptr;
	static auto pThis = *reinterpret_cast<DWORD**>(utils::get().PatternScan(GetModuleHandle("client.dll"), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
	if (!InitKeyValuesEx)
		InitKeyValuesEx = *reinterpret_cast<InitKeyValues_t>(utils::get().PatternScan(GetModuleHandle("client.dll"), "55 8B EC 51 33 C0 C7 45"));

	InitKeyValuesEx(keyvalues, key_name);
}
void utils::load_from_buffer(KeyValues* key_values, const char* resource_name, const char* buf, void* file_sys, const char* path_id, void* eval_sym_proc, void* unk)
{
	using LoadFromBuffer_t = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
	static LoadFromBuffer_t LoadFromBufferEx = nullptr;

	if (!LoadFromBufferEx)
		LoadFromBufferEx = *reinterpret_cast<LoadFromBuffer_t>(utils::get().PatternScan(GetModuleHandle("client.dll"), "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"));

	LoadFromBufferEx(key_values, resource_name, buf, file_sys, path_id, eval_sym_proc, unk);
}
Vector utils::calculate_aim_angles(Vector to_hit, Vector my_eye)
{
	Vector deltaVec = Vector(0, 0, 0);
	deltaVec = Vector(to_hit.x - my_eye.x, to_hit.y - my_eye.y, to_hit.z - my_eye.z); 
	float lengthDeltaVec = deltaVec.Length();
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180.0 / 3.14);
	float pitch = -asin(deltaVec.z / lengthDeltaVec) * (180.0 / 3.14);
	return Vector(pitch, yaw);	
}
bool utils::screen_transform(const Vector& in, Vector& out)
{	
		static auto& w2sMatrix = g_engine->WorldToScreenMatrix();

		out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
		out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
		out.z = 0.0f;

		float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

		if (w < 0.001f) {
			
			return false;
		}

		out.x /= w;
		out.y /= w;

		return true;	
}
float utils::random_between_two_float(float min, float max)
{
	static bool once = true;
	if (once)
	{
		srand(GetTickCount() + rand() % 1000);//rand in srand = genius;
		once = false;
	}
	float to_return = (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
	if (to_return > max)to_return = max; if (to_return < min)to_return = min;
	return to_return;
}
bool utils::WorldToScreen(const Vector& in, Vector& out)
{
	if (screen_transform(in, out)) {
		int w, h;
		g_engine->GetScreenSize(w, h);

		out.x = (w / 2.0f) + (out.x * w) / 2.0f;
		out.y = (h / 2.0f) - (out.y * h) / 2.0f;

		return true;
	}
	return false;
}

bool utils::precache_model(const char* szModelName)
{	
		INetworkStringTable* m_pModelPrecacheTable = client_string_table_container->FindTable("modelprecache");
		if (m_pModelPrecacheTable)
		{
			g_model_info->FindOrLoadModel(szModelName);
			int idx = m_pModelPrecacheTable->AddString(false, szModelName);
			if (idx == INVALID_STRING_INDEX)
				return false;
		}
		return true;	
}

void utils::__CreateFont(HMODULE module, LPCSTR resource, std::string name)
{

	if (std::filesystem::exists(std::filesystem::path(name))) return;

	HGLOBAL     res_handle = NULL;
	HRSRC       res;
	char* res_data;
	DWORD       res_size;

	// We miss error handling on most of them - be careful!
	res = FindResource(module, resource, RT_RCDATA);
	if (!res) return;
	res_handle = LoadResource(module, res);
	if (!res_handle) return;
	res_data = (char*)LockResource(res_handle);
	res_size = SizeofResource(module, res);

	std::ofstream stream;
	stream.open(name, std::ios::binary);
	stream.write(res_data, res_size);
	stream.close();

	AddFontResource(name.c_str());

}

float utils::lerp()
{
	static ConVar* cl_interp = g_cvar->FindVar("cl_interp");
	static ConVar* cl_updaterate = g_cvar->FindVar("cl_updaterate");
	const int update_rate = cl_updaterate->GetInt();
	const float interp_ratio = cl_interp->GetFloat();

	float lerp = interp_ratio / update_rate;

	if (lerp <= interp_ratio)
		lerp = interp_ratio;

	return lerp;

}

bool utils::valid_backtrack(float simtime)
{
	NetworkChannel* network = g_engine->GetNetChannelInfo();
	if (!network || !csgo_data.local)
		return false;
	static ConVar* max_unlag = g_cvar->FindVar("sv_maxunlag");
	float server_time = csgo_data.local->get_tick_base() * g_global_vars->interval_per_tick;
	float delta = std::clamp(network->get_latency(0) + network->get_latency(1) + lerp(), 0.f, max_unlag->GetFloat()) - (server_time - simtime);
	return std::fabsf(delta) <= 0.2f;
}
