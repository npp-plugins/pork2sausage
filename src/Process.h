//this file is part of notepad++
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

#ifndef PROCESSUS_H
#define PROCESSUS_H

#include <windows.h>
#include <string>


enum progType {WIN32_PROG, CONSOLE_PROG};


class Process 
{
public:
    Process(progType pt = WIN32_PROG) : _type(pt) {};
    Process(const wchar_t* cmd, const wchar_t* args, const wchar_t* cDir, progType pt = WIN32_PROG)
		: _type(pt), _stdoutStr(L""), _stderrStr(L""), _hPipeOutR(NULL),
		_hPipeErrR(NULL), _hProcess(NULL), _hProcessThread(NULL),
		_command(cmd), _args(args), _curDir(cDir), _bProcessEnd(TRUE)
	{};

	BOOL run();

	const wchar_t * getStdout() const {
		return _stdoutStr.c_str();
	};
	
	const wchar_t * getStderr() const {
		return _stderrStr.c_str();
	};

	int getExitCode() const {
		return _exitCode;
	};

	bool hasStdout() {
		return (_stdoutStr.compare(L"") != 0);
	};

	bool hasStderr() {
		return (_stderrStr.compare(L"") != 0);
	};
 
protected:
    progType _type;

	// LES ENTREES
    std::wstring _command;
	std::wstring _args;
	std::wstring _curDir;
	
	// LES SORTIES
	std::wstring _stdoutStr;
	std::wstring _stderrStr;
	int _exitCode;

	// LES HANDLES
    HANDLE _hPipeOutR;
	HANDLE _hPipeErrR;
	HANDLE _hProcess;
	HANDLE _hProcessThread;

	BOOL	_bProcessEnd;

    //UINT _pid;   // process ID assigned by caller
	
	static DWORD WINAPI staticListenerStdOut(void * myself){
		((Process *)myself)->listenerStdOut();
		return 0;
	};
	static DWORD WINAPI staticListenerStdErr(void * myself) {
		((Process *)myself)->listenerStdErr();
		return 0;
	};
	static DWORD WINAPI staticWaitForProcessEnd(void * myself) {
		((Process *)myself)->waitForProcessEnd();
		return 0;
	};

	void listenerStdOut();
	void listenerStdErr();
	void waitForProcessEnd();

	void error(const wchar_t* txt2display, BOOL & returnCode, int errCode);
};

#endif //PROCESSUS_H

