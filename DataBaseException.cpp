#include "StdAfx.h"

#include "DataBaseException.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseException::CDataBaseException(enSQLException enErrorType, LPCTSTR pszDescribe)
	: m_enErrorType(enErrorType)
{
	_snprintf_s(m_szDescribe, sizeof(m_szDescribe), _T("%s"), pszDescribe);
}

//��������
CDataBaseException::~CDataBaseException()
{
}
