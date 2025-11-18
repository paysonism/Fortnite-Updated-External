#pragma once
#include "../security/spoofer.h"

uintptr_t virtualaddy;

#define code_rw CTL_CODE(FILE_DEVICE_UNKNOWN, 0x532, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_ba CTL_CODE(FILE_DEVICE_UNKNOWN, 0x576, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_security 0xAF2B94E7
#define code_get_guarded_region CTL_CODE(FILE_DEVICE_UNKNOWN, 0x47, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _rw {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct _ba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _ga {
	INT32 security;
	ULONGLONG* address;
} ga, * pga;

namespace mem {
	HANDLE driver_handle;
	INT32 process_id;

	bool find_driver() {
		HKSSPFF;

		driver_handle = CreateFileW((L"\\\\.\\\zixywashere6767"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
			return false;

		return true;

		// Made By Payson
		// github.com/paysonism
	}

	void read_physical(PVOID address, PVOID buffer, DWORD size) {
		HKSSPFF;

		_rw arguments = { 0 };

		arguments.security = code_security;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = FALSE;

		DeviceIoControl(driver_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	void write_physical(PVOID address, PVOID buffer, DWORD size) {
		HKSSPFF;

		_rw arguments = { 0 };

		arguments.security = code_security;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = TRUE;

		DeviceIoControl(driver_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	uintptr_t find_image() {
		HKSSPFF;

		uintptr_t image_address = { NULL };
		_ba arguments = { NULL };

		arguments.security = code_security;
		arguments.process_id = process_id;
		arguments.address = (ULONGLONG*)&image_address;

		DeviceIoControl(driver_handle, code_ba, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return image_address;
	}

	uintptr_t get_guarded_region() {
		HKSSPFF;

		uintptr_t guarded_region_address = { NULL };
		_ga arguments = { NULL };

		arguments.security = code_security;
		arguments.address = (ULONGLONG*)&guarded_region_address;

		DeviceIoControl(driver_handle, code_get_guarded_region, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return guarded_region_address;
	}

	INT32 find_process(LPCTSTR process_name) {
		HKSSPFF;

		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, process_name)) {
					CloseHandle(hsnap);
					process_id = pt.th32ProcessID;
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}
		CloseHandle(hsnap);

		return { NULL };
	}
}

template <typename T>
T read(uint64_t address) {
	HKSSPFF;

	T buffer{ };
	mem::read_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

template <typename T>
T write(uint64_t address, T buffer) {
	HKSSPFF;

	mem::write_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

bool is_valid(const uint64_t adress)
{
	HKSSPFF;

	if (adress <= 0x400000 || adress == 0xCCCCCCCCCCCCCCCC || reinterpret_cast<void*>(adress) == nullptr || adress >
		0x7FFFFFFFFFFFFFFF) {
		return false;
	}
	return true;
}
template<typename T>
bool read_array(uintptr_t address, T out[], size_t len)
{
	HKSSPFF;

	for (size_t i = 0; i < len; ++i)
	{
		out[i] = read<T>(address + i * sizeof(T));
	}
	return true; // you can add additional checks to verify successful reads
}

template<typename T>
bool read_array2(uint64_t address, T* out, size_t len)
{
	HKSSPFF;

	if (!mem::driver_handle || mem::driver_handle == INVALID_HANDLE_VALUE)
	{
		if (!mem::find_driver())
		{
			return false;
		}
	}

	if (!out || len == 0)
	{
		return false;
	}

	for (size_t i = 0; i < len; ++i)
	{
		if (!is_valid(address + i * sizeof(T)))
		{
			return false;
		}

		out[i] = read<T>(address + i * sizeof(T));
	}
	return true;
}