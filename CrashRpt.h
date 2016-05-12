#ifndef _CRASHRPT_H_
#define _CRASHRPT_H_

//ϵͳͷ�ļ�
#include "exception"

//////////////////////////////////////////////////////////////////////////

//���ѻָ�������
class CCrashRpt
{
	//��������
public:
	//���캯��
	CCrashRpt();
	//��������
	virtual ~CCrashRpt(void);

public:
	//��������
	static bool Execute(LPCTSTR lpFileNameEXE, LPCTSTR lpParameters = NULL);

private:
	//VC-MFC�쳣
	static LONG WINAPI GEHExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);
	//C++�쳣
	static void terminate_function();

private:
	//����ת���ļ�
	static void CreateMiniDump(EXCEPTION_POINTERS * pep = NULL);
};

//////////////////////////////////////////////////////////////////////////

#endif // _CRASHRPT_H_

