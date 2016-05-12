#include "StdAfx.h"
#include "Math.h"
#include "DataBaseService.h"
#include "DataBaseException.h"
//////////////////////////////////////////////////////////////////////////

//�궨��
_COM_SMARTPTR_TYPEDEF(IADORecordBinding, __uuidof(IADORecordBinding));

//Ч������
#define EfficacyResult(hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }

//////////////////////////////////////////////////////////////////////////

//���ݿ�֪ͨ
struct NTY_DataBaseNotify
{
	WORD									wRequestID;							//�����ʶ
	DWORD									dwContextID;						//�����ʶ
};

#define NOTIFY_DATABASE_QUEUE_REQUEST		0x0001								//���ݿ�֪ͨ
#define NOTIFY_DATABASE_QUEUE_RESULT		0x0002								//���ݿ�֪ͨ

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBase::CDataBase() : m_dwResumeConnectCount(30L), m_dwResumeConnectTime(30L)
{
	//״̬����
	m_dwConnectCount = 0;
	m_dwConnectErrorTime = 0L;

	//��������
	m_DBCommand.CreateInstance(__uuidof(Command));
	m_DBRecordset.CreateInstance(__uuidof(Recordset));
	m_DBConnection.CreateInstance(__uuidof(Connection));

	//Ч������
	ASSERT(m_DBCommand != NULL);
	ASSERT(m_DBRecordset != NULL);
	ASSERT(m_DBConnection != NULL);
	if (m_DBCommand == NULL) throw TEXT("���ݿ�������󴴽�ʧ��");
	if (m_DBRecordset == NULL) throw TEXT("���ݿ��¼�����󴴽�ʧ��");
	if (m_DBConnection == NULL) throw TEXT("���ݿ����Ӷ��󴴽�ʧ��");

	//���ñ���
	m_DBCommand->CommandType = adCmdStoredProc;

	return;
}

//��������
CDataBase::~CDataBase()
{
	//�ر�����
	CloseConnection();

	//�ͷŶ���
	if (NULL != m_DBCommand) 
		m_DBCommand.Release();
	if (NULL != m_DBRecordset) 
		m_DBRecordset.Release();
	if (NULL != m_DBConnection) 
		m_DBConnection.Release();
}

//������
bool __cdecl CDataBase::OpenConnection()
{
	try
	{
		//�ر�����
		CloseConnection();

		//�������ݿ�
		EfficacyResult(m_DBConnection->Open(_bstr_t(m_strConnect), L"", L"", adConnectUnspecified));
		m_DBConnection->CursorLocation = adUseClient;
		m_DBCommand->ActiveConnection = m_DBConnection;

		//���ñ���
		m_dwConnectCount = 0L;
		m_dwConnectErrorTime = 0L;
		
		return TRUE;
	}
	catch (_com_error & e)
	{
		throw &CDataBaseException(SQLException_Connect, (LPCSTR)e.Description());
	}
}

//�رռ�¼
bool __cdecl CDataBase::CloseRecordset()
{
	if (IsRecordsetOpened()) EfficacyResult(m_DBRecordset->Close());

	return true;
}

//�ر�����
bool __cdecl CDataBase::CloseConnection()
{
	CloseRecordset();
	if ((m_DBConnection != NULL) && (m_DBConnection->GetState() != adStateClosed))
	{
		EfficacyResult(m_DBConnection->Close());
	}

	return true;
}

//��������
bool __cdecl CDataBase::TryConnectAgain(bool bFocusConnect, _com_error * pComError)
{
	try
	{
		//�ж�����
		bool bReConnect = bFocusConnect;
		if (bReConnect == false)
		{
			DWORD dwNowTime = (DWORD)time(NULL);
			if ((m_dwConnectErrorTime + m_dwResumeConnectTime) > dwNowTime) bReConnect = true;
		}
		if ((bReConnect == false) && (m_dwConnectCount > m_dwResumeConnectCount)) bReConnect = true;

		//���ñ���
		m_dwConnectCount++;
		m_dwConnectErrorTime = (DWORD)time(NULL);
		if (bReConnect == false)
		{
			if (pComError)
				throw &CDataBaseException(SQLException_Syntax, pComError->Description());
			return false;
		}

		//��������
		OpenConnection();
		return true;
	}
	catch (_com_error &e)
	{
		//�������Ӵ���
		throw &CDataBaseException(SQLException_Syntax, e.Description());
	}

	return false;
}

//������Ϣ
bool __cdecl CDataBase::SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//Ч�����
	ASSERT(dwDBAddr != 0);
	ASSERT(szDBName != NULL);
	ASSERT(szUser != NULL);
	ASSERT(szPassword != NULL);

	BYTE a = (BYTE)((dwDBAddr >> 24) & 0xFF);
	BYTE b = (BYTE)((dwDBAddr >> 16) & 0xFF);
	BYTE c = (BYTE)((dwDBAddr >> 8) & 0xFF);
	BYTE d = (BYTE)(dwDBAddr & 0xFF);
	//���������ַ���
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%d.%d.%d.%d,%ld;"),
		szPassword, szUser, szDBName, a, b, c, d, wPort);

	return true;
}

//������Ϣ
bool __cdecl CDataBase::SetConnectionInfo(LPCTSTR szDBConnectString)
{
	m_strConnect = szDBConnectString;
	return true;
}

//�л���¼
VOID __cdecl CDataBase::NextRecordset()
{
	VARIANT lngRec;
	m_DBRecordset->NextRecordset(&lngRec);
	return;
}

//�л���¼
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
		//�������Ӵ���
		m_DBRecordset = temp_RecordsetPtrOld;
		throw &CDataBaseException(SQLException_Syntax, e.Description());
	}
	catch (...)
	{
		m_DBRecordset = temp_RecordsetPtrOld;
	}
}

//������Ϣ
bool __cdecl CDataBase::SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//Ч�����
	ASSERT(szDBAddr != NULL);
	ASSERT(szDBName != NULL);
	ASSERT(szUser != NULL);
	ASSERT(szPassword != NULL);
	//���������ַ���
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		szPassword, szUser, szDBName, szDBAddr, wPort);

	return true;
}

//�Ƿ����Ӵ���
bool __cdecl CDataBase::IsConnectError()
{
	//״̬�ж�
	if (m_DBConnection == NULL) return true;
	if (m_DBConnection->GetState() == adStateClosed) return true;

	//�����ж�
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

//�Ƿ�����״̬
bool __cdecl CDataBase::IsConnected()
{
	//״̬�ж�
	if (m_DBConnection == NULL) return false;
	if (m_DBConnection->GetState() == adStateOpen) return true;

	return false;
}

//�Ƿ��
bool __cdecl CDataBase::IsRecordsetOpened()
{
	if (m_DBRecordset == NULL) return false;
	if (m_DBRecordset->GetState() == adStateClosed) return false;
	return true;
}

//�����ƶ�
void __cdecl CDataBase::MoveToNext()
{
	m_DBRecordset->MoveNext();
}

//�����ƶ�
void __cdecl CDataBase::MovePrevious()
{
	m_DBRecordset->MovePrevious();
	if (m_DBRecordset->BOF)
		m_DBRecordset->MoveFirst();

	return;
}

//�Ƶ���ͷ
void __cdecl CDataBase::MoveToFirst()
{
	m_DBRecordset->MoveFirst();

	return;
}

//�Ƿ����
bool __cdecl CDataBase::IsRecordsetEnd()
{
	//m_DBRecordset->State
	return (m_DBRecordset->EndOfFile == VARIANT_TRUE);

	return true;
}

//��ȡ��Ŀ
long __cdecl CDataBase::GetRecordCount()
{
	if (m_DBRecordset == NULL) return 0;
	return m_DBRecordset->GetRecordCount();

	return 0;
}

//��ȡ��С
long __cdecl CDataBase::GetActualSize(LPCTSTR pszParamName)
{
	ASSERT(pszParamName != NULL);
	return m_DBRecordset->Fields->Item[pszParamName]->ActualSize;

	return -1;
}

//�󶨶���
VOID __cdecl CDataBase::BindToRecordset(CADORecordBinding * pBind)
{
	ASSERT(pBind != NULL);
	IADORecordBindingPtr pIBind(m_DBRecordset);
	pIBind->BindToRecordset(pBind);
}

//��ȡ����
VOID __cdecl CDataBase::GetRecordsetValue(LPCTSTR pszItem, _variant_t & DBVarValue)
{
	ASSERT(pszItem != NULL);
	
	DBVarValue = m_DBRecordset->Fields->GetItem(pszItem)->Value;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, BYTE & cbValue)
{
	cbValue = 0;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) cbValue = vtFld;
	return true;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue)
{
	ulValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) ulValue = vtFld;
	return true;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue)
{
	dbValue = 0.0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) dbValue = vtFld;
	return true;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, FLOAT & dbValue)
{
	dbValue = 0.0;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) dbValue = (float)vtFld;
	return true;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, LONG & lValue)
{
	lValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) lValue = vtFld;
	return true;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue)
{
	ulValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) ulValue = vtFld;
	return true;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, INT & nValue)
{
	nValue = 0;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) nValue = vtFld;
	return true;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue)
{
	llValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) llValue = vtFld.llVal;
	return true;
}

//��ȡ����
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

//��ȡ����
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

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, WORD & wValue)
{
	wValue = 0L;
	_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
	if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) wValue = vtFld;
	return true;
}

//��ȡ����
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

//��ȡ����
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
////ȡ����

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

//��ȡ����
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

//��ȡ����
bool CDataBase::GetValue_BOOL(LPCTSTR pszItem)
{
	bool bValue;
	GetFieldValue(pszItem, bValue);
	return bValue;
}

//��ȡ�ַ�
VOID CDataBase::GetValue_LPSTR(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)
{
	GetFieldValue(pszItem, (char *)pszString, uMaxCount);
}

//��ȡ�ַ�
VOID CDataBase::GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)
{
	GetFieldValue(pszItem, (char *)pszString, uMaxCount);
}

//��ȡ�ַ�
VOID CDataBase::GetValue_BSTR(LPCTSTR pszItem, BSTR* bstrString)
{
	GetFieldValue(pszItem, bstrString);
}


//�������
void __cdecl CDataBase::AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, _variant_t & DBVarValue)
{
	ASSERT(pszName != NULL);
	_ParameterPtr Parameter = m_DBCommand->CreateParameter(pszName, Type, Direction, lSize, DBVarValue);
	m_DBCommand->Parameters->Append(Parameter);
}

//��ȡ������ֵ
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
//���ò���
VOID CDataBase::ResetParameter()
{
	ClearParameters();
	AddParameter(TEXT("RETURN_VALUE"), adInteger, adParamReturnValue, sizeof(INT), _variant_t((INT)-1));
}


//��ò���
void __cdecl CDataBase::GetParameter(LPCTSTR pszParamName, _variant_t & DBVarValue)
{
	//Ч�����
	ASSERT(pszParamName != NULL);

	//��ȡ����
	DBVarValue.Clear();
	DBVarValue = m_DBCommand->Parameters->Item[pszParamName]->Value;
}


//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, INT nValue)
{
	AddParameter(pszItem, adInteger, adParamInput, sizeof(INT), _variant_t(nValue));
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, UINT uValue)
{
	AddParameter(pszItem, adUnsignedInt, adParamInput, sizeof(UINT), _variant_t(uValue));
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, LONG lValue)
{
	AddParameter(pszItem, adInteger, adParamInput, sizeof(LONG), _variant_t(lValue));
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, LONGLONG lValue)
{
	AddParameter(pszItem, adBigInt, adParamInput, sizeof(__int64), _variant_t((__int64)lValue));
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, BYTE cbValue)
{
	AddParameter(pszItem, adUnsignedTinyInt, adParamInput, sizeof(BYTE), _variant_t(cbValue));
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, WORD wValue)
{
	AddParameter(pszItem, adUnsignedSmallInt, adParamInput, sizeof(WORD), _variant_t(wValue));
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, DWORD dwValue)
{
	AddParameter(pszItem, adUnsignedInt, adParamInput, sizeof(DWORD), _variant_t(dwValue));
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, FLOAT fValue)
{
	AddParameter(pszItem, adSingle, adParamInput, sizeof(FLOAT), _variant_t(fValue));
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, DOUBLE dValue)
{
	AddParameter(pszItem, adDouble, adParamInput, sizeof(DOUBLE), _variant_t(dValue));
}

//�������
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

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, BSTR pszString)
{
	_bstr_t b(pszString);
	char *pStr = (char*)b;
	AddParameter(pszItem, pStr);
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszItem, LPCTSTR pszString)
{
	CStringA lp(pszString);
	AddParameter(pszItem, lp.GetBuffer());
	lp.ReleaseBuffer();
}

//�洢����
LONG __cdecl CDataBase::ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)
{
	ASSERT(pszSPName != NULL);
	try
	{
		//�رռ�¼��
		CloseRecordset();

		m_DBCommand->CommandText = pszSPName;

		//ִ������
		if (bRecordset == true)
		{
			m_DBRecordset->PutRefSource(m_DBCommand);
			m_DBRecordset->CursorLocation = adUseClient;
//			EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand, vtMissing, adOpenForwardOnly, adLockReadOnly, adOptionUnspecified));
			EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand, vtMissing, adOpenStatic, adLockOptimistic, adOptionUnspecified));//adLockOptimistic�ֹ��� adLockReadOnly,adExecuteRecord
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

//ִ������
VOID __cdecl CDataBase::ExecuteSentence(LPCTSTR pszCommand, bool bRecordset)
{
	ASSERT(pszCommand != NULL);
	try
	{
		//�رռ�¼��
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

//��ʼ����
void __cdecl CDataBase::BeginTrans()
{
	if (m_DBConnection)
		m_DBConnection->BeginTrans();
}

//�������
void __cdecl CDataBase::CommitTrans()
{
	if (m_DBConnection)
		m_DBConnection->CommitTrans();
}

//�ع�����
void __cdecl CDataBase::RollbackTrans()
{
	if (m_DBConnection)
		m_DBConnection->RollbackTrans();
}


//ɾ������
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