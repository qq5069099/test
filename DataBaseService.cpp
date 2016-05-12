#include "StdAfx.h"
#include "Math.h"
#include "DataBaseService.h"
#include "DataBaseException.h"
//////////////////////////////////////////////////////////////////////////

//宏定义
_COM_SMARTPTR_TYPEDEF(IADORecordBinding, __uuidof(IADORecordBinding));

//效验结果宏
#define EfficacyResult(hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }

//////////////////////////////////////////////////////////////////////////

//数据库通知
struct NTY_DataBaseNotify
{
	WORD									wRequestID;							//请求标识
	DWORD									dwContextID;						//对象标识
};

#define NOTIFY_DATABASE_QUEUE_REQUEST		0x0001								//数据库通知
#define NOTIFY_DATABASE_QUEUE_RESULT		0x0002								//数据库通知

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBase::CDataBase() : m_dwResumeConnectCount(30L), m_dwResumeConnectTime(30L)
{
	//状态变量
	m_dwConnectCount = 0;
	m_dwConnectErrorTime = 0L;

	//创建对象
	m_DBCommand.CreateInstance(__uuidof(Command));
	m_DBRecordset.CreateInstance(__uuidof(Recordset));
	m_DBConnection.CreateInstance(__uuidof(Connection));

	//效验数据
	ASSERT(m_DBCommand != NULL);
	ASSERT(m_DBRecordset != NULL);
	ASSERT(m_DBConnection != NULL);
	if (m_DBCommand == NULL) throw TEXT("数据库命令对象创建失败");
	if (m_DBRecordset == NULL) throw TEXT("数据库记录集对象创建失败");
	if (m_DBConnection == NULL) throw TEXT("数据库连接对象创建失败");

	//设置变量
	m_DBCommand->CommandType = adCmdStoredProc;

	return;
}

//析构函数
CDataBase::~CDataBase()
{
	//关闭连接
	CloseConnection();

	//释放对象
	if (NULL != m_DBCommand) 
		m_DBCommand.Release();
	if (NULL != m_DBRecordset) 
		m_DBRecordset.Release();
	if (NULL != m_DBConnection) 
		m_DBConnection.Release();
}

//打开连接
bool __cdecl CDataBase::OpenConnection()
{
	try
	{
		//关闭连接
		CloseConnection();

		//连接数据库
		EfficacyResult(m_DBConnection->Open(_bstr_t(m_strConnect), L"", L"", adConnectUnspecified));
		m_DBConnection->CursorLocation = adUseClient;
		m_DBCommand->ActiveConnection = m_DBConnection;

		//设置变量
		m_dwConnectCount = 0L;
		m_dwConnectErrorTime = 0L;
		
		return TRUE;
	}
	catch (_com_error & e)
	{
		throw &CDataBaseException(SQLException_Connect, (LPCSTR)e.Description());
	}
}

//关闭记录
bool __cdecl CDataBase::CloseRecordset()
{
	if (IsRecordsetOpened()) EfficacyResult(m_DBRecordset->Close());

	return true;
}

//关闭连接
bool __cdecl CDataBase::CloseConnection()
{
	CloseRecordset();
	if ((m_DBConnection != NULL) && (m_DBConnection->GetState() != adStateClosed))
	{
		EfficacyResult(m_DBConnection->Close());
	}

	return true;
}

//重新连接
bool __cdecl CDataBase::TryConnectAgain(bool bFocusConnect, _com_error * pComError)
{
	try
	{
		//判断重连
		bool bReConnect = bFocusConnect;
		if (bReConnect == false)
		{
			DWORD dwNowTime = (DWORD)time(NULL);
			if ((m_dwConnectErrorTime + m_dwResumeConnectTime) > dwNowTime) bReConnect = true;
		}
		if ((bReConnect == false) && (m_dwConnectCount > m_dwResumeConnectCount)) bReConnect = true;

		//设置变量
		m_dwConnectCount++;
		m_dwConnectErrorTime = (DWORD)time(NULL);
		if (bReConnect == false)
		{
			if (pComError)
				throw &CDataBaseException(SQLException_Syntax, pComError->Description());
			return false;
		}

		//重新连接
		OpenConnection();
		return true;
	}
	catch (_com_error &e)
	{
		//重新连接错误
		throw &CDataBaseException(SQLException_Syntax, e.Description());
	}

	return false;
}

//连接信息
bool __cdecl CDataBase::SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//效验参数
	ASSERT(dwDBAddr != 0);
	ASSERT(szDBName != NULL);
	ASSERT(szUser != NULL);
	ASSERT(szPassword != NULL);

	BYTE a = (BYTE)((dwDBAddr >> 24) & 0xFF);
	BYTE b = (BYTE)((dwDBAddr >> 16) & 0xFF);
	BYTE c = (BYTE)((dwDBAddr >> 8) & 0xFF);
	BYTE d = (BYTE)(dwDBAddr & 0xFF);
	//构造连接字符串
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%d.%d.%d.%d,%ld;"),
		szPassword, szUser, szDBName, a, b, c, d, wPort);

	return true;
}

//设置信息
bool __cdecl CDataBase::SetConnectionInfo(LPCTSTR szDBConnectString)
{
	m_strConnect = szDBConnectString;
	return true;
}

//切换记录
VOID __cdecl CDataBase::NextRecordset()
{
	VARIANT lngRec;
	m_DBRecordset->NextRecordset(&lngRec);
	return;
}

//切换记录
VOID __cdecl CDataBase::LastRecordset()
{
	_RecordsetPtr temp_RecordsetPtrOld = m_DBRecordset;
	try
	{
		while (m_DBRecordset != NULL)
		{
			temp_RecordsetPtrOld = m_DBRecordset;

			VARIANT lngRec;
			m_DBRecordset = m_DBRecordset->NextRecordset(&lngRec);
		}
		m_DBRecordset = temp_RecordsetPtrOld;
	}
	catch (_com_error &e)
	{
		//重新连接错误
		m_DBRecordset = temp_RecordsetPtrOld;
		throw &CDataBaseException(SQLException_Syntax, e.Description());
	}
	catch (...)
	{
		m_DBRecordset = temp_RecordsetPtrOld;
	}
}

//设置信息
bool __cdecl CDataBase::SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//效验参数
	ASSERT(szDBAddr != NULL);
	ASSERT(szDBName != NULL);
	ASSERT(szUser != NULL);
	ASSERT(szPassword != NULL);
	//构造连接字符串
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		szPassword, szUser, szDBName, szDBAddr, wPort);

	return true;
}

//是否连接错误
bool __cdecl CDataBase::IsConnectError()
{
	//状态判断
	if (m_DBConnection == NULL) return true;
	if (m_DBConnection->GetState() == adStateClosed) return true;

	//参数判断
	long lErrorCount = m_DBConnection->Errors->Count;
	if (lErrorCount > 0L)
	{
		ErrorPtr pError = NULL;
		for (long i = 0; i < lErrorCount; i++)
		{
			pError = m_DBConnection->Errors->GetItem(i);
			if (pError->Number == 0x80004005) return true;
		}
	}

	return false;
}

//是否连接状态
bool __cdecl CDataBase::IsConnected()
{
	//状态判断
	if (m_DBConnection == NULL) return false;
	if (m_DBConnection->GetState() == adStateOpen) return true;

	return false;
}

//是否打开
bool __cdecl CDataBase::IsRecordsetOpened()
{
	if (m_DBRecordset == NULL) return false;
	if (m_DBRecordset->GetState() == adStateClosed) return false;
	return true;
}

//往下移动
void __cdecl CDataBase::MoveToNext()
{
	m_DBRecordset->MoveNext();
}

//往上移动
void __cdecl CDataBase::MovePrevious()
{
	m_DBRecordset->MovePrevious();
	if (m_DBRecordset->BOF)
		m_DBRecordset->MoveFirst();

	return;
}

//移到开头
void __cdecl CDataBase::MoveToFirst()
{
	m_DBRecordset->MoveFirst();

	return;
}

//是否结束
bool __cdecl CDataBase::IsRecordsetEnd()
{
	//m_DBRecordset->State
	return (m_DBRecordset->EndOfFile == VARIANT_TRUE);

	return true;
}

//获取数目
long __cdecl CDataBase::GetRecordCount()
{
	if (m_DBRecordset == NULL) return 0;
	return m_DBRecordset->GetRecordCount();

	return 0;
}

//获取大小
long __cdecl CDataBase::GetActualSize(LPCTSTR pszParamName)
{
	ASSERT(pszParamName != NULL);
	return m_DBRecordset->Fields->Item[pszParamName]->ActualSize;

	return -1;
}

//绑定对象
VOID __cdecl CDataBase::BindToRecordset(CADORecordBinding * pBind)
{
	ASSERT(pBind != NULL);
	IADORecordBindingPtr pIBind(m_DBRecordset);
	pIBind->BindToRecordset(pBind);
}

//获取数据
VOID __cdecl CDataBase::GetRecordsetValue(LPCTSTR pszItem, _variant_t & DBVarValue)
{
	ASSERT(pszItem != NULL);
	
	DBVarValue = m_DBRecordset->Fields->GetItem(pszItem)->Value;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, BYTE & cbValue)
{
	cbValue = 0;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) cbValue = vtFld;
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue)
{
	ulValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) ulValue = vtFld;
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue)
{
	dbValue = 0.0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) dbValue = vtFld;
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, FLOAT & dbValue)
{
	dbValue = 0.0;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) dbValue = (float)vtFld;
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, LONG & lValue)
{
	lValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) lValue = vtFld;
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue)
{
	ulValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) ulValue = vtFld;
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, INT & nValue)
{
	nValue = 0;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) nValue = vtFld;
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue)
{
	llValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) llValue = vtFld.llVal;
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, char szBuffer[], UINT uSize)
{
	ZeroMemory(szBuffer, uSize);
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if (vtFld.vt == VT_BSTR)
	{
		lstrcpyA(szBuffer, (char*)_bstr_t(vtFld));
		szBuffer[uSize - 1] = 0;
		return true;
	}
	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, BSTR *bstrString)
{
	try
	{
		*bstrString = NULL;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if (vtFld.vt == VT_BSTR)
		{
			_bstr_t b(vtFld);
			*bstrString = b.copy(true);
			return true;
		}
		return false;
	}
	catch (_com_error &e)
	{
		*bstrString = NULL;
		throw &CDataBaseException(SQLException_Syntax, e.Description());
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, WORD & wValue)
{
	wValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) wValue = vtFld;
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time)
{
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	switch (vtFld.vt)
	{
	case VT_DATE:
		{
			COleDateTime TempTime(vtFld);
			Time = TempTime;
			break;
		}
	case VT_EMPTY:
	case VT_NULL:
		{
			Time.SetStatus(COleDateTime::null);
			break;
		}
	default:
		return false;
	}
	return true;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, bool &bValue)
{
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	switch (vtFld.vt)
	{
	case VT_BOOL:
		{
			bValue = (vtFld.boolVal == VARIANT_FALSE) ? false : true;
			break;
		}
	case VT_EMPTY:
	case VT_NULL:
		{
			bValue = false;
			break;
		}
	default:
		return false;
	}
	return true;
}


/////////////////////////////////
////取数据

INT CDataBase::GetValue_INT(LPCTSTR pszItem)
{
	INT nValue;
	GetFieldValue(pszItem, nValue);
	return nValue;
}

UINT CDataBase::GetValue_UINT(LPCTSTR pszItem)
{
	UINT ulValue;
	GetFieldValue(pszItem, ulValue);
	return ulValue;
}

DOUBLE CDataBase::GetValue_DOUBLE(LPCTSTR pszItem)
{
	DOUBLE dbValue;
	GetFieldValue(pszItem, dbValue);
	return dbValue;
}

BYTE CDataBase::GetValue_BYTE(LPCTSTR pszItem)
{
	BYTE bValue;
	GetFieldValue(pszItem, bValue);
	return bValue;
}

LONG CDataBase::GetValue_LONG(LPCTSTR pszItem)
{
	LONG lValue;
	GetFieldValue(pszItem, lValue);
	return lValue;
}

WORD CDataBase::GetValue_WORD(LPCTSTR pszItem)
{
	WORD wValue;
	GetFieldValue(pszItem, wValue);
	return wValue;
}

DWORD CDataBase::GetValue_DWORD(LPCTSTR pszItem)
{
	DWORD ulValue;
	GetFieldValue(pszItem, ulValue);
	return ulValue;
}

FLOAT CDataBase::GetValue_FLOAT(LPCTSTR pszItem)
{
	FLOAT ulValue;
	GetFieldValue(pszItem, ulValue);
	return ulValue;
}

LONGLONG CDataBase::GetValue_LONGLONG(LPCTSTR pszItem)
{
	__int64 llValue;
	GetFieldValue(pszItem, llValue);
	return llValue;
}

//获取数据
__time64_t CDataBase::GetValue_TIME(LPCTSTR pszItem)
{
	try
	{
		COleDateTime Time;
		GetFieldValue(pszItem, Time);
		if (Time.GetStatus() == COleDateTime::invalid) return 0;

		CTime time(Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond());
		return time.GetTime();
	}
	catch (...)
	{
	}

	return 0;
}

//获取数据
bool CDataBase::GetValue_BOOL(LPCTSTR pszItem)
{
	bool bValue;
	GetFieldValue(pszItem, bValue);
	return bValue;
}

//获取字符
VOID CDataBase::GetValue_LPSTR(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)
{
	GetFieldValue(pszItem, (char *)pszString, uMaxCount);
}

//获取字符
VOID CDataBase::GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)
{
	GetFieldValue(pszItem, (char *)pszString, uMaxCount);
}

//获取字符
VOID CDataBase::GetValue_BSTR(LPCTSTR pszItem, BSTR* bstrString)
{
	GetFieldValue(pszItem, bstrString);
}


//插入参数
void __cdecl CDataBase::AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, _variant_t & DBVarValue)
{
	ASSERT(pszName != NULL);
	_ParameterPtr Parameter = m_DBCommand->CreateParameter(pszName, Type, Direction, lSize, DBVarValue);
	m_DBCommand->Parameters->Append(Parameter);
}

//获取返回数值
long __cdecl CDataBase::GetReturnValue()
{
	_ParameterPtr Parameter;
	long lParameterCount = m_DBCommand->Parameters->Count;
	for (long i = 0; i < lParameterCount; i++)
	{
		Parameter = m_DBCommand->Parameters->Item[i];
		if (Parameter->Direction == adParamReturnValue)
			return Parameter->Value.lVal;
	}

	return -1;
}
//重置参数
VOID CDataBase::ResetParameter()
{
	ClearParameters();
	AddParameter(TEXT("RETURN_VALUE"), adInteger, adParamReturnValue, sizeof(INT), _variant_t((INT)-1));
}


//获得参数
void __cdecl CDataBase::GetParameter(LPCTSTR pszParamName, _variant_t & DBVarValue)
{
	//效验参数
	ASSERT(pszParamName != NULL);

	//获取参数
	DBVarValue.Clear();
	DBVarValue = m_DBCommand->Parameters->Item[pszParamName]->Value;
}


//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, INT nValue)
{
	AddParameter(pszItem, adInteger, adParamInput, sizeof(INT), _variant_t(nValue));
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, UINT uValue)
{
	AddParameter(pszItem, adUnsignedInt, adParamInput, sizeof(UINT), _variant_t(uValue));
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, LONG lValue)
{
	AddParameter(pszItem, adInteger, adParamInput, sizeof(LONG), _variant_t(lValue));
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, LONGLONG lValue)
{
	AddParameter(pszItem, adBigInt, adParamInput, sizeof(__int64), _variant_t((__int64)lValue));
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, BYTE cbValue)
{
	AddParameter(pszItem, adUnsignedTinyInt, adParamInput, sizeof(BYTE), _variant_t(cbValue));
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, WORD wValue)
{
	AddParameter(pszItem, adUnsignedSmallInt, adParamInput, sizeof(WORD), _variant_t(wValue));
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, DWORD dwValue)
{
	AddParameter(pszItem, adUnsignedInt, adParamInput, sizeof(DWORD), _variant_t(dwValue));
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, FLOAT fValue)
{
	AddParameter(pszItem, adSingle, adParamInput, sizeof(FLOAT), _variant_t(fValue));
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, DOUBLE dValue)
{
	AddParameter(pszItem, adDouble, adParamInput, sizeof(DOUBLE), _variant_t(dValue));
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, char* pszString)
{
	if (strlen(pszString) != 0)
	{
		AddParameter(pszItem, adChar, adParamInput, lstrlenA(pszString), _variant_t(pszString));
	}
	else
	{
		AddParameter(pszItem, adChar, adParamInput, lstrlenA(pszString) + 1, _variant_t(pszString));
	}
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, BSTR pszString)
{
	_bstr_t b(pszString);
	char *pStr = (char*)b;
	AddParameter(pszItem, pStr);
}

//插入参数
VOID CDataBase::AddParameter(LPCTSTR pszItem, LPCTSTR pszString)
{
	CStringA lp(pszString);
	AddParameter(pszItem, lp.GetBuffer());
	lp.ReleaseBuffer();
}

//存储过程
LONG __cdecl CDataBase::ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)
{
	ASSERT(pszSPName != NULL);
	try
	{
		//关闭记录集
		CloseRecordset();

		m_DBCommand->CommandText = pszSPName;

		//执行命令
		if (bRecordset == true)
		{
			m_DBRecordset->PutRefSource(m_DBCommand);
			m_DBRecordset->CursorLocation = adUseClient;
//			EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand, vtMissing, adOpenForwardOnly, adLockReadOnly, adOptionUnspecified));
			EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand, vtMissing, adOpenStatic, adLockOptimistic, adOptionUnspecified));//adLockOptimistic乐观锁 adLockReadOnly,adExecuteRecord
//			LastRecordset();
		}
		else
		{
			m_DBConnection->CursorLocation = adUseClient;
			EfficacyResult(m_DBCommand->Execute(NULL, NULL, adExecuteNoRecords));
		}

		return GetReturnValue();
	}
	catch (_com_error &e)
	{
		if (IsConnectError() == true) TryConnectAgain(false, &e);
		else throw &CDataBaseException(SQLException_Syntax, e.Description());
	}

	return -1;
}

//执行命令
VOID __cdecl CDataBase::ExecuteSentence(LPCTSTR pszCommand, bool bRecordset)
{
	ASSERT(pszCommand != NULL);
	try
	{
		//关闭记录集
		CloseRecordset();
		m_DBConnection->CursorLocation = adUseClient;
		if (bRecordset)
		{
			m_DBRecordset = m_DBConnection->Execute(pszCommand, NULL, adCmdText);
		}
		else
			m_DBConnection->Execute(pszCommand, NULL, adCmdText);
	}
	catch (_com_error &e)
	{
		if (IsConnectError() == true) TryConnectAgain(false, &e);
		else throw &CDataBaseException(SQLException_Syntax, e.Description());
	}
}

//开始事务
void __cdecl CDataBase::BeginTrans()
{
	if (m_DBConnection)
		m_DBConnection->BeginTrans();
}

//完成事务
void __cdecl CDataBase::CommitTrans()
{
	if (m_DBConnection)
		m_DBConnection->CommitTrans();
}

//回滚事务
void __cdecl CDataBase::RollbackTrans()
{
	if (m_DBConnection)
		m_DBConnection->RollbackTrans();
}


//删除参数
void __cdecl CDataBase::ClearParameters()
{
	long lParameterCount = m_DBCommand->Parameters->Count;
	if (lParameterCount > 0L)
	{
		for (long i = lParameterCount; i > 0; i--)
		{
			m_DBCommand->Parameters->Delete(i - 1);
		}
	}
}

class CIniCom
{
public:
	CIniCom()
	{
		::CoInitialize(NULL);
	}
	~CIniCom()
	{
		::CoUninitialize();
	}
};

CIniCom IniComObj;