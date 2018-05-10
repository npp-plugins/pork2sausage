//This file is part of the Notepad++ plugin "Pork to Sausage"
//Copyright (C)2010 Don HO <donho@altern.org>
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

#ifndef NPP_TOOLS_H
	#define NPP_TOOLS_H
	#define VERSION_VALUE "2.1\0"
	#define VERSION_DIGITALVALUE 2, 1, 0, 0
	#define IDD_ABOUTBOX 250

	#ifndef IDC_STATIC 
		#define IDC_STATIC -1
	#endif //IDC_STATIC

	#ifdef UNICODE
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
	#else
		#define NppMainEntry WinMain
		#define generic_strtol strtol
		#define generic_strncpy strncpy
		#define generic_stricmp stricmp
		#define generic_strncmp strncmp
		#define generic_strnicmp strnicmp
		#define generic_strncat strncat
		#define generic_strchr strchr
		#define generic_atoi atoi
		#define generic_itoa itoa
		#define generic_atof atof
		#define generic_strtok strtok
		#define generic_strftime strftime
		#define generic_fprintf fprintf
		#define generic_sscanf sscanf
		#define generic_fopen fopen
		#define generic_fgets fgets
		#define generic_stat _stat
		#define generic_sprintf sprintf
		#define COPYDATA_FILENAMES COPYDATA_FILENAMESA
	#endif //UNICODE

	typedef std::basic_string<TCHAR> generic_string;

	struct CmdParam {
		TCHAR _cmdName[nbChar];
		PFUNCPLUGINCMD _pMainFunc;
		TCHAR _progPath[1024];
		TCHAR _progCmd[1024];
		TCHAR _workDir[1024];
		TCHAR _progInput[1024];
		TCHAR _progOutput[1024];
		bool _doReplaceSelection;
		CmdParam() : _doReplaceSelection(true){
			_cmdName[0] = '\0';
			_progPath[0] = '\0';
			_progCmd[0] = '\0';
			_workDir[0] = '\0';
			_progInput[0] = '\0';
			_progOutput[0] = '\0';
		};
	};

	void launchProgram(const CmdParam & cmdParam);

	int getCmdsFromConf(const TCHAR *confPath, CmdParam *cmdParam, int maxNbCmd);

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

#endif //NPP_TOOLS_H
