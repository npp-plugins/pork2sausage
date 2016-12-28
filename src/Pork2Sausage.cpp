//this file is part of Notepad++ plugin Pork2Sausage
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

#include <string>
#include <iostream>
#include <fstream>
//#include <ios>
//#include <ifstream>

#include <vector>
#include "PluginInterface.h"
#include "Pork2Sausage.h"
#include "Process.h"
#include <shlwapi.h>
#include <time.h>

const TCHAR PLUGIN_NAME[] = TEXT("Pork to Sausage");

#define NB_BASE_CMD 2
int nbFunc = NB_BASE_CMD; // "Edit Commands" & "About"

HINSTANCE _hInst;
NppData nppData;
FuncItem *funcItem;
generic_string confPath;
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
	TCHAR confDir[MAX_PATH];
	confDir[0] = '\0';
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)confDir);
	confPath = confDir;
	confPath += TEXT("\\pork2Sausage.ini");
	
	if (!::PathFileExists(confPath.c_str()))
	{
		generic_string msg = confPath + TEXT(" is absent.");
		::MessageBox(nppData._nppHandle, confPath.c_str(), TEXT("Not present"), MB_OK);
	}
	nbFunc += getCmdsFromConf(confPath.c_str(), cmdParam, maxNbCmd);
}



extern "C" __declspec(dllexport) const TCHAR * getName()
{
	return PLUGIN_NAME;
}
/*
extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	*nbF = nbFunc;
	return funcItem;
}
*/

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
		lstrcpy(funcItem[i]._itemName, TEXT("-SEPARATOR-"));
		funcItem[i]._init2Check = false;
		funcItem[i]._pShKey = NULL;
		i++;
	}

	// Edit Commands
	funcItem[i]._pFunc = editCommands;
	lstrcpy(funcItem[i]._itemName, TEXT("Edit User Commands"));
	funcItem[i]._init2Check = false;
	funcItem[i]._pShKey = NULL;
	i++;

	// About
	funcItem[i]._pFunc = about;
	lstrcpy(funcItem[i]._itemName, TEXT("About"));
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
// http://sourceforge.net/forum/forum.php?forum_id=482781
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

int getCmdsFromConf(const TCHAR *confPathValue, CmdParam *cmdParamValue, int /*maxNbCmd*/)
{
	TCHAR cmdNames[MAX_PATH];
	::GetPrivateProfileSectionNames(cmdNames, MAX_PATH, confPathValue);
	TCHAR *pFn = cmdNames;

	int i = 0;
	while (*pFn)
	{
		TCHAR progPath[1024];
        TCHAR progCmd[1024];
        TCHAR workDir[1024];

		GetPrivateProfileString(pFn, TEXT("progPath"), TEXT(""), progPath, 1024, confPathValue);
        GetPrivateProfileString(pFn, TEXT("progCmd"), TEXT(""), progCmd, 1024, confPathValue);
        GetPrivateProfileString(pFn, TEXT("workDir"), TEXT(""), workDir, 1024, confPathValue);

		if (progPath[0] && progCmd[0] && workDir[0])
		{
			lstrcpy(cmdParamValue[i]._cmdName, pFn);
            lstrcpy(cmdParamValue[i]._progPath, progPath);
            lstrcpy(cmdParamValue[i]._progCmd, progCmd);
            lstrcpy(cmdParamValue[i]._workDir, workDir);

            // optional parameter here
			TCHAR progInput[1024];
            TCHAR progOutput[1024];
            TCHAR replaceSelection[8];
			GetPrivateProfileString(pFn, TEXT("progInput"), TEXT(""), progInput, 1024, confPathValue);
            GetPrivateProfileString(pFn, TEXT("progOutput"), TEXT(""), progOutput, 1024, confPathValue);
            GetPrivateProfileString(pFn, TEXT("replaceSelection"), TEXT("true"), replaceSelection, 8, confPathValue);
            lstrcpy(cmdParamValue[i]._progInput, progInput);
            lstrcpy(cmdParamValue[i]._progOutput, progOutput);
            if (generic_stricmp(TEXT("false"), replaceSelection) == 0)
                cmdParamValue[i]._doReplaceSelection = false;
			i++;
		}

		pFn += lstrlen(pFn) + 1;
	}
	
	return i;
}

struct commandParam
{
    generic_string _prgramPath;
    generic_string _param;
    generic_string _prgramDir;
};

void replaceStr(generic_string & str, generic_string str2BeReplaced, generic_string replacement)
{
	size_t pos = str.find(str2BeReplaced);

	if (pos != str.npos)
		str.replace(pos, str2BeReplaced.length(), replacement);
};

void launchProgram(const CmdParam & cmdParamValue)
{
    const TCHAR *programPath = cmdParamValue._progPath; 
    const TCHAR *param = cmdParamValue._progCmd;
    const TCHAR *programDir = cmdParamValue._workDir;
    const TCHAR *progInput = cmdParamValue._progInput;
    const TCHAR *progOutput = cmdParamValue._progOutput;
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
    if (asciiTextLen == 0) return;

    char * pAsciiText = new char[asciiTextLen+1];
    
    ::SendMessage(hCurrScintilla, SCI_GETSELTEXT, 0, (LPARAM)pAsciiText);

    TCHAR newProgramDir[MAX_PATH];
    lstrcpy(newProgramDir, programPath);
    ::PathRemoveFileSpec(newProgramDir);

    const TCHAR *pProgramDir = programDir?programDir:newProgramDir;

    generic_string paramInput = param;
    generic_string progInputStr = progInput?progInput:TEXT("");
    generic_string progOutputStr = progOutput?progOutput:TEXT("");
    generic_string selectedText = TEXT("\"");

//todo : convert from correct encoding
#ifdef UNICODE
    std::string inputA = pAsciiText;
    std::wstring inputW(inputA.begin(), inputA.end());
    inputW.assign(inputA.begin(), inputA.end());
    selectedText += inputW;
#else
    selectedText += pAsciiText;
#endif
    selectedText += TEXT("\"");

    const int temBufLen = 32;
	TCHAR tmpbuf[temBufLen];
	time_t ltime = time(0);
	struct tm *today;

	today = localtime(&ltime);
	generic_strftime(tmpbuf, temBufLen, TEXT("%Y-%m-%d_%H%M%S"), today);

	generic_string timeStampString = tmpbuf;

    replaceStr(paramInput, TEXT("$(TIMESTAMP)"), timeStampString);
    replaceStr(paramInput, TEXT("$(SELECTION)"), selectedText);
    if (progInputStr != TEXT(""))
    {
        replaceStr(progInputStr, TEXT("$(TIMESTAMP)"), timeStampString);
    }

    if (progOutputStr != TEXT(""))
    {
        replaceStr(progOutputStr, TEXT("$(TIMESTAMP)"), timeStampString);
    }

    if (progInputStr != TEXT(""))
    {
       	FILE *f = generic_fopen(progInputStr.c_str(), TEXT("wb"));
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
        if (progOutputStr != TEXT("") && doReplace)
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
                ::MessageBox(NULL, TEXT("The file is invalide"), progOutputStr.c_str(), MB_OK);
            }
        }
        // otherwise, we look in stdout
        else if (program.hasStdout() && doReplace)
        {
            
#ifdef UNICODE
            std::wstring outputW = program.getStdout();
            std::string output(outputW.begin(), outputW.end());
            output.assign(outputW.begin(), outputW.end());
            pOutput = output.c_str();
#else
            pOutput = program.getStdout();
#endif
            ::SendMessage(hCurrScintilla, SCI_REPLACESEL, 0, (LPARAM)pOutput);
	        ::SendMessage(hCurrScintilla, SCI_SETSEL, start, start+strlen(pOutput));
        }
        else
        {
            if (doReplace)
                ::MessageBox(NULL, TEXT("No output"), TEXT("Problem"), MB_OK);
        }
    }
    else
    {
        ::MessageBox(NULL, program.getStderr(), TEXT("Error"), MB_OK);
    }
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
	if (!::PathFileExists(confPath.c_str()))
	{
		generic_string msg = confPath + TEXT("is not present.\\rPlease create this file manually.");
		::MessageBox(nppData._nppHandle, msg.c_str(), TEXT("Configuration file is absent"), MB_OK);
		return;
	}
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)confPath.c_str());
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
