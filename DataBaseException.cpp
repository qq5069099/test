#include "StdAfx.h"

#include "DataBaseException.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseException::CDataBaseException(enSQLException enErrorType, LPCTSTR pszDescribe)
	: m_enErrorType(enErrorType)
{
	_snprintf_s(m_szDescribe, sizeof(m_szDescribe), _T("%s"), pszDescribe);
}

//析构函数
CDataBaseException::~CDataBaseException()
{
}
