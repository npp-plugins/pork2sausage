//this file is part of Notepad++ plugin Pork2Sausage
//Copyright (C)2022 Don HO <don.h@free.fr>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#pragma once

#define VERSION_VALUE "2.3\0"
#define VERSION_DIGITALVALUE 2, 3, 0, 0

#define IDD_ABOUTBOX 250

#ifndef IDC_STATIC 
#define IDC_STATIC -1
#endif

#define NppMainEntry wWinMain
#define generic_strtol wcstol
#define generic_strncpy wcsncpy
#define generic_stricmp wcsicmp
#define generic_strncmp wcsncmp
#define generic_strnicmp wcsnicmp
#define generic_strncat wcsncat
#define generic_strchr wcschr
#define generic_atoi _wtoi
#define generic_itoa _itow
#define generic_atof _wtof
#define generic_strtok wcstok
#define generic_strftime wcsftime
#define generic_fprintf fwprintf
#define generic_sscanf swscanf
#define generic_fopen _wfopen
#define generic_fgets fgetws
#define generic_stat _wstat
#define generic_sprintf swprintf
#define COPYDATA_FILENAMES COPYDATA_FILENAMESW


typedef std::basic_string<TCHAR> generic_string;

struct CmdParam {
	TCHAR _cmdName[menuItemSize] {};
	PFUNCPLUGINCMD _pMainFunc = nullptr;
	TCHAR _progPath[1024] {};
	TCHAR _progCmd[1024] {};
	TCHAR _workDir[1024] {};
	
	TCHAR _progInput[1024] {};
	TCHAR _progOutput[1024] {};
	bool _doReplaceSelection = true;
};



void launchProgram(const CmdParam & cmdParamValue);

int getCmdsFromConf(const TCHAR *confPathValue, CmdParam *cmdParamValue, int maxNbCmd);

void launchProgram_00();
void launchProgram_01();
void launchProgram_02();
void launchProgram_03();
void launchProgram_04();
void launchProgram_05();
void launchProgram_06();
void launchProgram_07();
void launchProgram_08();
void launchProgram_09();
void launchProgram_10();
void launchProgram_11();
void launchProgram_12();
void launchProgram_13();
void launchProgram_14();
void launchProgram_15();
void launchProgram_16();
void launchProgram_17();
void launchProgram_18();
void launchProgram_19();

void editCommands();
void about();

