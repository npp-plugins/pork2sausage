//this file is part of Notepad++ plugin Pork2Sausage
//Copyright (C)2025 Don HO <don.h@free.fr>
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

#include <string>
#include <iostream>
#include <fstream>

#include <vector>
#include "PluginInterface.h"
#include "Pork2Sausage.h"
#include "Process.h"
#include <locale>
#include <shlwapi.h>
#include <time.h>
#include <algorithm>

const wchar_t PLUGIN_NAME[] = L"Pork to Sausage";

#define NB_BASE_CMD 2
int nbFunc = NB_BASE_CMD; // "Edit Commands" & "About"

HINSTANCE _hInst;
NppData nppData;
FuncItem* funcItem;
wchar_t confPath[MAX_PATH] {};
const int maxNbCmd = 20;
CmdParam cmdParam[maxNbCmd];

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  reasonForCall, LPVOID /*lpReserved*/)
{
	switch (reasonForCall)
	{
		case DLL_PROCESS_ATTACH:
		{
			_hInst = (HINSTANCE)hModule;

			cmdParam[0]._pMainFunc  = launchProgram_00;
			cmdParam[1]._pMainFunc  = launchProgram_01;
			cmdParam[2]._pMainFunc  = launchProgram_02;
			cmdParam[3]._pMainFunc  = launchProgram_03;
			cmdParam[4]._pMainFunc  = launchProgram_04;
			cmdParam[5]._pMainFunc  = launchProgram_05;
			cmdParam[6]._pMainFunc  = launchProgram_06;
			cmdParam[7]._pMainFunc  = launchProgram_07;
			cmdParam[8]._pMainFunc  = launchProgram_08;
			cmdParam[9]._pMainFunc  = launchProgram_09;
			cmdParam[10]._pMainFunc = launchProgram_10;
			cmdParam[11]._pMainFunc = launchProgram_11;
			cmdParam[12]._pMainFunc = launchProgram_12;
			cmdParam[13]._pMainFunc = launchProgram_13;
			cmdParam[14]._pMainFunc = launchProgram_14;
			cmdParam[15]._pMainFunc = launchProgram_15;
			cmdParam[16]._pMainFunc = launchProgram_16;
			cmdParam[17]._pMainFunc = launchProgram_17;
			cmdParam[18]._pMainFunc = launchProgram_18;
			cmdParam[19]._pMainFunc = launchProgram_19;
		}
		break;

		case DLL_PROCESS_DETACH:
		{
			delete [] funcItem;
		}
		break;

		case DLL_THREAD_ATTACH:
		break;

		case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	nppData = notpadPlusData;

	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)confPath);
	PathAppend(confPath, L"\\pork2Sausage.ini");
	
	if (!::PathFileExists(confPath)) // pork2Sausage.ini not in %APPDATA%\Notepad++\plugins\Config\ directory
	{
		// conf file will be copied in conf dir if we find the file beside of plugin binary

		wchar_t moduleDirName[MAX_PATH] = L"pork2sausage";
		wchar_t moduleConfFilePath[MAX_PATH]{};
		::SendMessage(nppData._nppHandle, NPPM_GETPLUGINHOMEPATH, MAX_PATH, (LPARAM)moduleConfFilePath);
		::PathAppend(moduleConfFilePath, moduleDirName);
		::PathAppend(moduleConfFilePath, L"Config");
		::PathAppend(moduleConfFilePath, L"pork2sausage.ini");
		if (::PathFileExists(moduleConfFilePath))
		{
			::CopyFile(moduleConfFilePath, confPath, TRUE);
		}
		else
		{
			std::wstring msg = confPath;
			msg += L" is absent.\rPlease create the file for avoiding this message.";
			::MessageBox(nppData._nppHandle, msg.c_str(), L"Configuration file is missing", MB_OK);
		}
	}
	nbFunc += getCmdsFromConf(confPath, cmdParam, maxNbCmd);
}

extern "C" __declspec(dllexport) const wchar_t* getName()
{
	return PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	bool insertSeparator = false;
	if (nbFunc > NB_BASE_CMD)
	{
		nbFunc += 1;
		insertSeparator = true;
	}
	*nbF = nbFunc;
	funcItem = new FuncItem[nbFunc];

	// User customizable commands
	int i = 0;
	for ( ; i < nbFunc - (insertSeparator?NB_BASE_CMD+1:NB_BASE_CMD) ; )
	{
		funcItem[i]._pFunc = cmdParam[i]._pMainFunc;
		lstrcpy(funcItem[i]._itemName, cmdParam[i]._cmdName);
		funcItem[i]._init2Check = false;
		funcItem[i]._pShKey = NULL;
		i++;
	}

	// Separator if necessary
	if (insertSeparator)
	{
		funcItem[i]._pFunc = NULL;
		lstrcpy(funcItem[i]._itemName, L"-SEPARATOR-");
		funcItem[i]._init2Check = false;
		funcItem[i]._pShKey = NULL;
		i++;
	}

	// Edit Commands
	funcItem[i]._pFunc = editCommands;
	lstrcpy(funcItem[i]._itemName, L"Edit User Commands");
	funcItem[i]._init2Check = false;
	funcItem[i]._pShKey = NULL;
	i++;

	// About
	funcItem[i]._pFunc = about;
	lstrcpy(funcItem[i]._itemName, L"About");
	funcItem[i]._init2Check = false;
	funcItem[i]._pShKey = NULL;

	return funcItem;
}

extern "C" __declspec(dllexport) void beNotified(SCNotification* /*notifyCode*/)
{	
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
	return TRUE;
}
#endif //UNICODE

// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// https://github.com/notepad-plus-plus/notepad-plus-plus/issues
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

HWND getCurrentScintillaHandle() {
    int currentEdit;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
	return (currentEdit == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
};

int getCmdsFromConf(const wchar_t *confPathValue, CmdParam *cmdParamValue, int /*maxNbCmd*/)
{
	wchar_t cmdNames[MAX_PATH];
	::GetPrivateProfileSectionNames(cmdNames, MAX_PATH, confPathValue);
	wchar_t* pFn = cmdNames;

	wchar_t nppConfigDir[1024];
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, 1024, (LPARAM)nppConfigDir);

	int i = 0;
	while (*pFn)
	{
		wchar_t progPath1[1024] {};
		wchar_t progPath2[1024] {};
		wchar_t progCmd[1024] {};
		wchar_t workDir1[1024] {};
		wchar_t workDir2[1024] {};

		GetPrivateProfileString(pFn, L"progPath", L"", progPath1, 1024, confPathValue);
        GetPrivateProfileString(pFn, L"progCmd", L"", progCmd, 1024, confPathValue);
        GetPrivateProfileString(pFn, L"workDir", L"", workDir1, 1024, confPathValue);

		if (progPath1[0] && progCmd[0] && workDir1[0])
		{
			::ExpandEnvironmentStrings(progPath1, progPath2, 1024);
            ::ExpandEnvironmentStrings(workDir1, workDir2, 1024);

            if (::PathIsRelative(progPath2))
            {
              ::PathCombine(progPath1, nppConfigDir, progPath2);
              ::GetFullPathName(progPath1, 1024, progPath2, NULL);
            }

            if (::PathIsRelative(workDir2))
            {
              ::PathCombine(workDir1, nppConfigDir, workDir2);
              ::GetFullPathName(workDir1, 1024, workDir2, NULL);
            }

			lstrcpy(cmdParamValue[i]._cmdName, pFn);
            lstrcpy(cmdParamValue[i]._progPath, progPath2);
            lstrcpy(cmdParamValue[i]._progCmd, progCmd);
            lstrcpy(cmdParamValue[i]._workDir, workDir2);

            // optional parameter here
			wchar_t progInput[1024];
            wchar_t progOutput[1024];
            wchar_t replaceSelection[8];
			GetPrivateProfileString(pFn, L"progInput", L"", progInput, 1024, confPathValue);
            GetPrivateProfileString(pFn, L"progOutput", L"", progOutput, 1024, confPathValue);
            GetPrivateProfileString(pFn, L"replaceSelection", L"true", replaceSelection, 8, confPathValue);
            lstrcpy(cmdParamValue[i]._progInput, progInput);
            lstrcpy(cmdParamValue[i]._progOutput, progOutput);
            if (generic_stricmp(L"false", replaceSelection) == 0)
                cmdParamValue[i]._doReplaceSelection = false;
			i++;
		}

		pFn += lstrlen(pFn) + 1;
	}
	
	return i;
}

void replaceStr(std::wstring & str, std::wstring str2BeReplaced, std::wstring replacement)
{
	size_t pos = str.find(str2BeReplaced);

	if (pos != str.npos)
		str.replace(pos, str2BeReplaced.length(), replacement);
};

void launchProgram(const CmdParam & cmdParamValue)
{
    const wchar_t* programPath = cmdParamValue._progPath; 
    const wchar_t* param = cmdParamValue._progCmd;
    const wchar_t* programDir = cmdParamValue._workDir;
    const wchar_t* progInput = cmdParamValue._progInput;
    const wchar_t* progOutput = cmdParamValue._progOutput;
    bool doReplace = cmdParamValue._doReplaceSelection;

	HWND hCurrScintilla = getCurrentScintillaHandle();

    size_t start = ::SendMessage(hCurrScintilla, SCI_GETSELECTIONSTART, 0, 0);
	size_t end = ::SendMessage(hCurrScintilla, SCI_GETSELECTIONEND, 0, 0);
	if (end < start)
	{
		size_t tmp = start;
		start = end;
		end = tmp;
	}

	size_t asciiTextLen = end - start;
	char * pAsciiText = nullptr;
	if (asciiTextLen > 0)
	{
		pAsciiText = new char[asciiTextLen + 1];
		::SendMessage(hCurrScintilla, SCI_GETSELTEXT, 0, (LPARAM)pAsciiText);
	}
    wchar_t newProgramDir[MAX_PATH];
    lstrcpy(newProgramDir, programPath);
    ::PathRemoveFileSpec(newProgramDir);

    const wchar_t *pProgramDir = programDir?programDir:newProgramDir;

    std::wstring paramInput = param;
    std::wstring progInputStr = progInput ? progInput : L"";
    std::wstring progOutputStr = progOutput ? progOutput : L"";

//todo : convert from correct encoding
    std::string inputA = pAsciiText ? pAsciiText : "";
    std::wstring inputW;
	std::transform(inputA.begin(), inputA.end(), std::back_inserter(inputW), [](char c) {
		return (wchar_t)c;
	});

    std::wstring selectedText = inputW;
	std::wstring quotedSelectedText = L"\"" + selectedText + L"\"";

    const int temBufLen = 32;
	wchar_t tmpbuf[temBufLen];
	time_t ltime = time(0);
	struct tm *today;

	today = localtime(&ltime);
	generic_strftime(tmpbuf, temBufLen, L"%Y-%m-%d_%H%M%S", today);

	std::wstring timeStampString = tmpbuf;

    replaceStr(paramInput, L"$(TIMESTAMP)", timeStampString);
    replaceStr(paramInput, L"$(SELECTION)", quotedSelectedText);
    if (!progInputStr.empty())
    {
        replaceStr(progInputStr, L"$(TIMESTAMP)", timeStampString);
    }

    if (!progOutputStr.empty())
    {
        replaceStr(progOutputStr, L"$(TIMESTAMP)", timeStampString);
    }

    if (!progInputStr.empty())
    {
       	FILE *f = generic_fopen(progInputStr.c_str(), L"wb");
	    if (f)
	    {
            fwrite(selectedText.c_str(), sizeof((selectedText.c_str())[0]), selectedText.length(), f);
		    fflush(f);
		    fclose(f);
	    }
    }
    Process program(programPath, paramInput.c_str(), pProgramDir, CONSOLE_PROG);
	program.run();

    if (!program.hasStderr())
    {
        const char *pOutput = NULL;
        size_t pOutputLen = 0;
        // If progOutput is defined, then we search the file to read
        if (!progOutputStr.empty() && doReplace)
        { 
            if (::PathFileExists(progOutputStr.c_str()))
            {
                // open the file for binary reading
                std::ifstream file;
                file.open(progOutputStr.c_str(), std::ios_base::binary);
                std::vector<byte> fileContent;
                if (file.is_open())
                {
                    // get the length of the file
                    file.seekg(0, std::ios::end);
                    pOutputLen = static_cast<size_t>(file.tellg());
                    file.seekg(0, std::ios::beg);

                    // create a vector to hold all the bytes in the file
                    fileContent.resize(pOutputLen, 0);

                    // read the file
                    file.read(reinterpret_cast<char*>(&fileContent[0]), (std::streamsize)pOutputLen);

                    // close the file
                    file.close();

                    pOutput = reinterpret_cast<const char*>(&fileContent[0]);
                }
                const char errMsg[] = "ERROR: NO FILE CONTENT";
                if (!pOutput || !(pOutput[0]))
                {
                    pOutput = errMsg;
                    pOutputLen = strlen(errMsg);
                }
                ::SendMessage(hCurrScintilla, SCI_CLEAR, 0, 0);
                ::SendMessage(hCurrScintilla, SCI_ADDTEXT, pOutputLen, (LPARAM)pOutput);
	            ::SendMessage(hCurrScintilla, SCI_SETSEL, start, start+pOutputLen);

            }
            else
            {
                ::MessageBox(NULL, L"The file is invalid", progOutputStr.c_str(), MB_OK);
            }
        }
        // otherwise, we look in stdout
        else if (program.hasStdout() && doReplace)
        {
            std::wstring outputW = program.getStdout();
			std::string output;
			std::transform(outputW.begin(), outputW.end(), std::back_inserter(output), [](wchar_t c) {
				return (char)c;
				}
			);

            pOutput = output.c_str();

            ::SendMessage(hCurrScintilla, SCI_REPLACESEL, 0, (LPARAM)pOutput);
	        ::SendMessage(hCurrScintilla, SCI_SETSEL, start, start+strlen(pOutput));
        }
        else
        {
            if (doReplace)
                ::MessageBox(NULL, L"No output", L"Problem", MB_OK);
        }
    }
    else
    {
        ::MessageBox(NULL, program.getStderr(), L"Error", MB_OK);
    }

	if (asciiTextLen > 0)
		delete [] pAsciiText;
}

BOOL CALLBACK dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM /*lParam*/) 
{
	switch (message) 
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
            {
                case IDCLOSE :
			    {
					::EndDialog(hwnd, 0);
					return  TRUE;
				}
			}
			return FALSE;
	}
	return FALSE;
}

void launchProgram_00() {launchProgram( cmdParam[0] );}
void launchProgram_01() {launchProgram( cmdParam[1] );}
void launchProgram_02() {launchProgram( cmdParam[2] );}
void launchProgram_03() {launchProgram( cmdParam[3] );}
void launchProgram_04() {launchProgram( cmdParam[4] );}
void launchProgram_05() {launchProgram( cmdParam[5] );}
void launchProgram_06() {launchProgram( cmdParam[6] );}
void launchProgram_07() {launchProgram( cmdParam[7] );}
void launchProgram_08() {launchProgram( cmdParam[8] );}
void launchProgram_09() {launchProgram( cmdParam[9] );}
void launchProgram_10() {launchProgram( cmdParam[10]);}
void launchProgram_11() {launchProgram( cmdParam[11]);}
void launchProgram_12() {launchProgram( cmdParam[12]);}
void launchProgram_13() {launchProgram( cmdParam[13]);}
void launchProgram_14() {launchProgram( cmdParam[14]);}
void launchProgram_15() {launchProgram( cmdParam[15]);}
void launchProgram_16() {launchProgram( cmdParam[16]);}
void launchProgram_17() {launchProgram( cmdParam[17]);}
void launchProgram_18() {launchProgram( cmdParam[18]);}
void launchProgram_19() {launchProgram( cmdParam[19]);}


void editCommands()
{
	if (!::PathFileExists(confPath))
	{
		std::wstring msg = confPath;
		msg += L" is not present.\rPlease create this file manually.";
		::MessageBox(nppData._nppHandle, msg.c_str(), L"Configuration file is absent", MB_OK);
		return;
	}
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)confPath);
}


void about()
{
	HWND hSelf = ::CreateDialogParam(_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), nppData._nppHandle, (DLGPROC)dlgProc, (LPARAM)NULL);
		    
	// Go to center
	RECT rc;
	::GetClientRect(nppData._nppHandle, &rc);
	POINT center;
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	center.x = rc.left + w/2;
	center.y = rc.top + h/2;
	::ClientToScreen(nppData._nppHandle, &center);

	RECT dlgRect;
	::GetClientRect(hSelf, &dlgRect);
	int x = center.x - (dlgRect.right - dlgRect.left)/2;
	int y = center.y - (dlgRect.bottom - dlgRect.top)/2;

	::SetWindowPos(hSelf, HWND_TOP, x, y, (dlgRect.right - dlgRect.left), (dlgRect.bottom - dlgRect.top), SWP_SHOWWINDOW);
}
