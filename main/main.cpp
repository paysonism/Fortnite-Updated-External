#include "includes/includes.hpp"
#include <iostream>
#include <stdio.h>

#include <Windows.h>
#include <string>
#include "includes/utils.hpp"
#include <urlmon.h>
#include <tchar.h>
#include "Driver/driver.h"
#include "general.h"
#include <random>
#include "protect/auth.hpp"
#include <thread>
#include <cstring>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include "bytes.h"

using namespace KeyAuth;

void Print(const std::string& text) {
		std::cout << text;
}

HWND windowid = NULL;

bool CreateFileFromMemory(const std::string& desired_file_path, const char* address, size_t size)
{
	std::ofstream file_ofstream(desired_file_path.c_str(), std::ios_base::out | std::ios_base::binary);

	if (!file_ofstream.write(address, size))
	{
		file_ofstream.close();
		return false;
	}

	file_ofstream.close();
	return true;
}

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include "includes/game/game.hpp"

using namespace std;

auto name = skCrypt("SaturnFree"); // Application Name
auto ownerid = skCrypt("kKkah2e0K5"); // Owner ID
auto secret = skCrypt("078e8da2826de4e87495c2511d80045bf43e9929daa8cd3d162d6a256680dd42"); // Application Secret
auto version = skCrypt("1.0"); // Application Version
auto url = skCrypt("https://keyauth.win/api/1.2/"); // change if you're self-hosting

api KeyAuthApp(name.decrypt(), ownerid.decrypt(), secret.decrypt(), version.decrypt(), url.decrypt());

void mapdriver() {
	system("cls");
	system("color 6");
	auto drv = ("C:\\Windows\\System32\\dr.sys");
	auto mp = ("C:\\Windows\\System32\\mp.exe");

	CreateFileFromMemory(drv, reinterpret_cast<const char*>(driver_bytes), sizeof(driver_bytes));
	CreateFileFromMemory(mp, reinterpret_cast<const char*>(mapper), sizeof(mapper));

	system(("C:\\Windows\\System32\\mp.exe C:\\Windows\\System32\\dr.sys"));

	remove(("C:\\Windows\\System32\\dr.sys"));
	remove(("C:\\Windows\\System32\\mp.exe"));
}

uintptr_t process_id = 0;

INT32 find_process(LPCTSTR process_name) {
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

void currentweaponloop()
{

}

auto main() -> int
{
	KeyAuthApp.init();
	system("color 5");
	SetConsoleTitleA("Saturn IV Free FN - Made By Payson - discord.gg/saturniv");
	system("cls");
	std::cout << skCrypt("\n Saturn Free - Made By Payson\n");
	std::cout << skCrypt("\n discord.gg/saturniv");
	Sleep(5000);

	system("cls");
	std::cout << "\n Get your free 24h key in the discord!\n\n Key > ";
	string key;
	std::cin >> key;
	//KeyAuthApp.license(key); // uncomment this to use keyauth
	KeyAuthApp.data.success = true; // comment this out to use keyauth
	if (KeyAuthApp.data.success)
	{
		input();
		mapdriver();
		system("cls");
		system("color 5");
		if (!mem::find_driver())
		{
			system("color 4");
			std::cout << (skCrypt("\n Driver not loaded. Check #faq in the discord and try again!")) << std::flush;
			Sleep(8000);
			exit(0);
		}
		
		mem::find_driver();
		MessageBoxA(0, skCrypt("Launch Fortnite and Press OK in Lobby"), skCrypt("discord.gg/saturniv - github.com/paysonism"), MB_OK);
		Sleep(200);
		system("cls");
		while (windowid == NULL)
		{
			XorS(wind, "Fortnite  ");
			windowid = FindWindowA_Spoofed(0, wind.decrypt());
		}
		globals->hwnd = FindWindowA_Spoofed(NULL, "Fortnite  ");
		LI_FN(Sleep)(1000);
		mem::find_process("FortniteClient-Win64-Shipping.exe");
		system("cls");
		virtualaddy = mem::find_image();

		system("cls");

		std::cout << "\n Saturn Free - discord.gg/saturniv - github.com/paysonism\n\n Base: 0x" << virtualaddy;

		globals->width = GetSystemMetrics_Spoofed(SM_CXSCREEN); globals->height = GetSystemMetrics_Spoofed(SM_CYSCREEN);

		if (Render->Setup() != RENDER_INFORMATION::RENDER_SETUP_SUCCESSFUL) ExitProcess(0);
		if (Hook->Setup() != HOOK_INFORMATION::HOOKS_SUCCESSFUL) ExitProcess(0);
		if (Game->Setup() != GAME_INFORMATION::GAME_SETUP_SUCCESSFUL) ExitProcess(0);
		Render->Render();
	}

	if (!KeyAuthApp.data.success)
	{
		system("cls");
		system("color 4");

		std::cout << "\n Invalid Key. Follow the instructions in the server to get your free 24 hour key!\n\n Discord -> discord.gg/saturniv";
		Sleep(8000);
		exit(0);
	}
}

