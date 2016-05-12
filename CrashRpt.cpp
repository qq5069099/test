#include "StdAfx.h"
#include "CrashRpt.h"
#include <DbgHelp.h>

CCrashRpt *g_pCrashRpt = NULL;

//////////////////////////////////////////////////////////////////////////

//�쳣�¼�
LONG WINAPI CCrashRpt::GEHExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo) 
{
	if (!g_pCrashRpt)
		return EXCEPTION_EXECUTE_HANDLER;

	try
	{
		DWORD dwPID = GetCurrentProcessId();
		CreateMiniDump(ExceptionInfo);
//		ExitProcess(0);

		//���ִ���ļ�
		TCHAR szModuleFile[MAX_PATH] = {0};
		GetModuleFileName(0, szModuleFile, MAX_PATH);
		Execute(szModuleFile);

		return EXCEPTION_EXECUTE_HANDLER;					//���в��ܸ�
	}
	catch (...)
	{
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

//C++�쳣
void CCrashRpt::terminate_function()
{
	if (!g_pCrashRpt) return;

	try
	{
		DWORD dwPID = GetCurrentProcessId();
		CreateMiniDump(NULL);
//		terminate();

		//���ִ���ļ�
		TCHAR szModuleFile[MAX_PATH] = {0};
		GetModuleFileName(0, szModuleFile, MAX_PATH);
		Execute(szModuleFile);
	}
	catch (...)
	{
	}
}

//���캯��
CCrashRpt::CCrashRpt()
{
	try
	{
		LPTOP_LEVEL_EXCEPTION_FILTER pExceptionFilter = SetUnhandledExceptionFilter(CCrashRpt::GEHExceptionFilter);
		::terminate_function pTerminateFunction = set_terminate(CCrashRpt::terminate_function);

		g_pCrashRpt = this;
	}
	catch (...)
	{

	}
}

//��������
CCrashRpt::~CCrashRpt(void)
{
}

//��������
bool CCrashRpt::Execute(LPCTSTR lpFileNameEXE, LPCTSTR lpParameters/* = NULL*/)
{
	HINSTANCE nInst = ShellExecute(0, _T("open"), lpFileNameEXE, lpParameters, NULL, SW_SHOWNORMAL);
	return (LONGLONG)nInst >= (LONGLONG)32;
}

//����ת���ļ�
void CCrashRpt::CreateMiniDump(EXCEPTION_POINTERS * pep/* = NULL*/) 
{
	CTime time = CTime::GetTickCount();
	CString strFileName = time.Format(_T("%Y-%m-%d-%H-%M-%S.dmp"));
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL || hFile == INVALID_HANDLE_VALUE) return;

	MINIDUMP_EXCEPTION_INFORMATION mdei;
	mdei.ThreadId           = GetCurrentThreadId();
	mdei.ExceptionPointers  = pep;
	mdei.ClientPointers     = FALSE;
	MINIDUMP_TYPE mdt       = MiniDumpNormal;
	BOOL rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
		hFile, mdt, (pep != NULL) ? &mdei : NULL, NULL, NULL);

	CloseHandle(hFile);
}

//ΪC#���
void __stdcall CreateCrashRpt()
{
	CCrashRpt *pCrashRpt = new CCrashRpt;
}