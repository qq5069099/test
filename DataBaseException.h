#ifndef __DATABASEEXCEPTION_H__
#define __DATABASEEXCEPTION_H__

//SQL 异常类型
enum enSQLException
{
	SQLException_None				= 0,									//没有异常
	SQLException_Connect			= 1,									//连接错误
	SQLException_Syntax				= 2,									//语法错误
};

//ADO 异常类
class CDataBaseException
{
	//变量定义
protected:
	enSQLException					m_enErrorType;						//异常代号
	TCHAR							m_szDescribe[1024];						//异常描述

	//函数定义
public:
	//构造函数
	CDataBaseException(enSQLException enErrorType, LPCTSTR pszDescribe);
	//析构函数
	virtual ~CDataBaseException();

	//功能接口
public:
	//异常类型
	virtual enSQLException __cdecl GetErrorType()
	{
		return m_enErrorType;
	}

	//异常描述
	virtual LPCTSTR __cdecl GetErrorDescribe()
	{
		return m_szDescribe;
	}
};


#endif // __DATABASEEXCEPTION_H__
