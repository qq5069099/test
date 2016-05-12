#ifndef _DATABASE_H_
#define _DATABASE_H_

//���ͷ�ļ�
#include "DataBaseException.h"

#include <ICrsint.h>
#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

//////////////////////////////////////////////////////////////////////////

//���ݿ����
class CDataBase
{
	//��Ϣ����
protected:
	CString									m_strConnect;								//�����ַ���

	//״̬����
protected:
	DWORD									m_dwConnectCount;							//���Դ���
	DWORD									m_dwConnectErrorTime;						//����ʱ��
	const DWORD								m_dwResumeConnectCount;						//�ָ�����
	const DWORD								m_dwResumeConnectTime;						//�ָ�ʱ��

	//�ں˱���
protected:
	_CommandPtr								m_DBCommand;								//�������
	_RecordsetPtr								m_DBRecordset;								//��¼������
	_ConnectionPtr							m_DBConnection;								//���ݿ����

	//�̴߳���
protected:
	CCriticalSection							m_CriticalSection;							//ͬ������

	//��������
public:
	//���캯��
	CDataBase();
	//��������
	virtual ~CDataBase();

	//����ӿ�
public:
	//������
	virtual bool __cdecl OpenConnection();
	//�رռ�¼
	virtual bool __cdecl CloseRecordset();
	//�ر�����
	virtual bool __cdecl CloseConnection();
	//��������
	virtual bool __cdecl TryConnectAgain(bool bFocusConnect, _com_error * pComError);
	//������Ϣ
	virtual bool __cdecl SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//������Ϣ
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//������Ϣ
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szDBConnectString);
	//�л���¼
	virtual void __cdecl NextRecordset();
	//�л���¼
	virtual void __cdecl LastRecordset();

	//״̬�ӿ�
public:
	//�Ƿ����Ӵ���
	virtual bool __cdecl IsConnectError();
	//�Ƿ�����״̬
	virtual bool __cdecl IsConnected();
	//�Ƿ��
	virtual bool __cdecl IsRecordsetOpened();

	//��¼���ӿ�
public:
	//�����ƶ�
	virtual void __cdecl MoveToNext();
	//�����ƶ�
	virtual void __cdecl MovePrevious();
	//�Ƶ���ͷ
	virtual void __cdecl MoveToFirst();
	//�Ƿ����
	virtual bool __cdecl IsRecordsetEnd();
	//��ȡ��Ŀ
	virtual long __cdecl GetRecordCount();
	//��ȡ��С
	virtual long __cdecl GetActualSize(LPCTSTR pszParamName);
	//�󶨶���
	virtual void __cdecl BindToRecordset(CADORecordBinding * pBind);
	//��ȡ����
	virtual void __cdecl GetRecordsetValue(LPCTSTR pszItem, _variant_t & DBVarValue);

	//�ֶνӿ�
public:
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BYTE & cbValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, WORD & wValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, INT & nValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, LONG & lValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, FLOAT & dbValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, char szBuffer[], UINT uSize);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BSTR *bstrString);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, bool & bValue);

	//��ȡ����
public:
	//��ȡ����
	virtual INT GetValue_INT(LPCTSTR pszItem);
	//��ȡ����
	virtual UINT GetValue_UINT(LPCTSTR pszItem);
	//��ȡ����
	virtual LONG GetValue_LONG(LPCTSTR pszItem);
	//��ȡ����
	virtual BYTE GetValue_BYTE(LPCTSTR pszItem);
	//��ȡ����
	virtual WORD GetValue_WORD(LPCTSTR pszItem);
	//��ȡ����
	virtual DWORD GetValue_DWORD(LPCTSTR pszItem);
	//��ȡ����
	virtual FLOAT GetValue_FLOAT(LPCTSTR pszItem);
	//��ȡ����
	virtual DOUBLE GetValue_DOUBLE(LPCTSTR pszItem);
	//��ȡ����
	virtual LONGLONG GetValue_LONGLONG(LPCTSTR pszItem);
	//��ȡ����
	virtual __time64_t GetValue_TIME(LPCTSTR pszItem);
	//��ȡ����
	virtual bool GetValue_BOOL(LPCTSTR pszItem);
	//��ȡ�ַ�
	virtual VOID GetValue_LPSTR(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount);
	//��ȡ�ַ�
	virtual VOID GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount);
	//��ȡ�ַ�
	virtual VOID GetValue_BSTR(LPCTSTR pszItem, BSTR* bstrString);

	//�������ӿ�
public:
	//�������
	virtual void __cdecl AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, _variant_t & DBVarValue);
	//���ò���
	virtual VOID ResetParameter();
	//��ò���
	virtual void __cdecl GetParameter(LPCTSTR pszParamName, _variant_t & DBVarValue);
	//��ȡ������ֵ
	virtual long __cdecl GetReturnValue();

	//�������
public:
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, INT nValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, UINT uValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, LONG lValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, LONGLONG lValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, BYTE cbValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, WORD wValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, DWORD dwValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, FLOAT fValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, DOUBLE dValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, char* pszString);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, BSTR pszString);
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, LPCTSTR pszString);

	//ִ�нӿ�
public:
	//�洢����
	virtual LONG __cdecl ExecuteProcess(LPCTSTR pszSPName, bool bRecordset);
	//ִ������
	virtual void __cdecl ExecuteSentence(LPCTSTR pszCommand, bool bRecordset);

	//����
public:
	//��ʼ����
	virtual void __cdecl BeginTrans();
	//�������
	virtual void __cdecl CommitTrans();
	//�ع�����
	virtual void __cdecl RollbackTrans();

	//�ڲ�����
private:
	//ɾ������
	void __cdecl ClearParameters();
};



#endif // _DATABASE_H_