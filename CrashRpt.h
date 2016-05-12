#ifndef _CRASHRPT_H_
#define _CRASHRPT_H_

//系统头文件
#include "exception"

//////////////////////////////////////////////////////////////////////////

//灾难恢复器引擎
class CCrashRpt
{
	//函数定义
public:
	//构造函数
	CCrashRpt();
	//析构函数
	virtual ~CCrashRpt(void);

public:
	//启动命令
	static bool Execute(LPCTSTR lpFileNameEXE, LPCTSTR lpParameters = NULL);

private:
	//VC-MFC异常
	static LONG WINAPI GEHExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);
	//C++异常
	static void terminate_function();

private:
	//创建转储文件
	static void CreateMiniDump(EXCEPTION_POINTERS * pep = NULL);
};

//////////////////////////////////////////////////////////////////////////

#endif // _CRASHRPT_H_

