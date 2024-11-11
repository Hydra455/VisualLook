// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include "injector/injector.hpp"
#include "ini.h"

float VTRed, VTGreen, VTBlue, VTBlackBloom, VTColorBloom, VTSaturation;
DWORD VTCodeCaveExit = 0x73E361;
DWORD VTecx;

void __declspec(naked) VTCodeCave()
{
	_asm
	{
		mov VTecx, ecx

		mov eax, [edi + 0x08]

		mov ecx, VTRed
		mov	dword ptr ds : [eax + 0xC0] , ecx //red

		mov ecx, VTGreen
		mov	dword ptr ds : [eax + 0xc4] , ecx //green

		mov ecx, VTBlue
		mov	dword ptr ds : [eax + 0xc8] , ecx //blue

		mov ecx, VTColorBloom
		mov dword ptr ds : [eax + 0xd0] , ecx //color bloom

		mov ecx, VTSaturation
		mov dword ptr ds : [eax + 0xd4] , ecx //saturation

		mov ecx, VTBlackBloom
		mov	dword ptr ds : [eax + 0xdc] , ecx //black bloom

		fld dword ptr[eax + 0xC4]

		mov ecx, VTecx
		jmp VTCodeCaveExit
	}
}

void Main()
{
	mINI::INIFile file("VisualLook.ini");
	mINI::INIStructure ini;
	file.read(ini);

	std::string& redValue = ini["Main"]["Red"];
	std::string& greenValue = ini["Main"]["Green"];
	std::string& blueValue = ini["Main"]["Blue"];
	std::string& blackBloomValue = ini["Main"]["BlackBloom"];
	std::string& bloomValue = ini["Main"]["ColorBloom"];
	std::string& saturationValue = ini["Main"]["Saturation"];

	VTRed = std::stof(redValue);
	VTGreen = std::stof(greenValue);
	VTBlue = std::stof(blueValue);

	VTBlackBloom = std::stof(blackBloomValue);
	VTColorBloom = std::stof(bloomValue);

	VTSaturation = std::stof(saturationValue);

	injector::MakeRangedNOP(0x73E358, VTCodeCaveExit, true);
	injector::MakeJMP(0x73E358, VTCodeCave, true);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		Main();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}