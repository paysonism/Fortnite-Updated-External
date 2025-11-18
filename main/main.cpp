/*
* This currently uses v1 of my IOCTL driver as it has write functionality. If you want to upgrade to v2 and keep write functionality you 
* can do that yourself. It's not that hard.
* 
* This is a really shitty source that I made for my free fn a year ago but just thought I'd release an updated version for those of you
* who want it.
* 
* Updated by zixy
* Made by Payson - https://github.com/paysonism
*/

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

auto name = skCrypt("if u sell this ur weird*"); // Application Name
auto ownerid = skCrypt("uwu"); // Owner ID
auto secret = skCrypt("uwu"); // Application Secret
auto version = skCrypt("1.0"); // Application Version
auto url = skCrypt("https://keyauth.win/api/1.2/"); // change if you're self-hosting

api KeyAuthApp(name.decrypt(), ownerid.decrypt(), secret.decrypt(), version.decrypt(), url.decrypt());
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
bool DownloadToMemory(const std::string& url, std::vector<uint8_t>& outData) {
	HINTERNET hInternet = InternetOpenA("memDownloader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hInternet) return false;

	HINTERNET hUrlFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!hUrlFile) {
		InternetCloseHandle(hInternet);
		return false;
	}

	BYTE buffer[4096];
	DWORD bytesRead = 0;

	while (InternetReadFile(hUrlFile, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
		outData.insert(outData.end(), buffer, buffer + bytesRead);
	}

	InternetCloseHandle(hUrlFile);
	InternetCloseHandle(hInternet);
	return !outData.empty();
}
void mapdriver() {
	system("cls");
	std::cout << skCrypt(" Mapping Driver!\n");
	Sleep(1000);

	std::vector<uint8_t> driverData;
	std::vector<uint8_t> kdmapperData;
	if (!DownloadToMemory("https://files.catbox.moe/a5aczk.sys", driverData)) {
		std::cout << skCrypt(" Failed to download driver!\n");
		return;
	}

	if (!DownloadToMemory("https://files.catbox.moe/wvsaqx.bin", kdmapperData)) {
		std::cout << skCrypt(" Failed to download mapper!\n");
		return;
	}

	wchar_t tempPath[MAX_PATH];
	GetTempPathW(MAX_PATH, tempPath);
	std::wstring kdmapperPath = std::wstring(tempPath) + L"m.exe";

	std::ofstream kdmapperFile(kdmapperPath, std::ios::binary);
	if (!kdmapperFile.is_open()) {
		std::cout << skCrypt(" Failed to save to disk! (Antivirus?)\n");
		return;
	}
	kdmapperFile.write(reinterpret_cast<const char*>(kdmapperData.data()), kdmapperData.size());
	kdmapperFile.close();

	std::wstring driverPath = std::wstring(tempPath) + L"dr.sys";
	std::ofstream driverFile(driverPath, std::ios::binary);
	if (!driverFile.is_open()) {
		std::cout << skCrypt(" Failed to save to disk! (Antivirus?)\n");
		return;
	}
	driverFile.write(reinterpret_cast<const char*>(driverData.data()), driverData.size());
	driverFile.close();

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	std::wstring cmdLine = L"\"" + kdmapperPath + L"\" \"" + driverPath + L"\"";

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	std::wstring wcmdLine = std::wstring(cmdLine.begin(), cmdLine.end());

	if (!CreateProcessW(nullptr, wcmdLine.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
		ShowWindow(GetConsoleWindow(), SW_RESTORE);
		std::cout << skCrypt(" Failed to launch kdmapper!\n");
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	system("cls");

	ShowWindow(GetConsoleWindow(), SW_RESTORE);

	std::filesystem::remove(kdmapperPath);
	std::filesystem::remove(driverPath);

	std::cout << skCrypt(" Done!\n");
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
	//KeyAuthApp.init();
	system("color 5");
	SetConsoleTitleA("Saturn IV Free FN - Made By Payson - discord.gg/saturniv");
	system("cls");
	std::cout << skCrypt("\n Saturn Free - Made By Payson\n");
	std::cout << skCrypt("\n discord.gg/saturniv");
	Sleep(5000);

	system("cls");
	//std::cout << "\n Get your free 24h key in the discord!\n\n Key > ";
	//string key;
	//std::cin >> key;
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
	//	MessageBoxA(0, skCrypt("Launch Fortnite and Press OK in Lobby"), skCrypt("discord.gg/saturniv - github.com/paysonism"), MB_OK);
		Sleep(200);
		system("cls");
		while (windowid == NULL)
		{
			XorS(wind, "Fortnite  ");
			windowid = FindWindowA_Spoofed(0, wind.decrypt());
		}

		globals->hwnd = FindWindowA_Spoofed(NULL, "Fortnite  ");
		LI_FN(Sleep)(1000);
		if (!mem::find_process("FortniteClient-Win64-Shipping.exe"))
		{
			system("color 4");
			std::cout << (skCrypt("\n Fortnite process not found! Please launch Fortnite and try again.")) << std::flush;
			Sleep(5000);
			exit(0);
		}
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

