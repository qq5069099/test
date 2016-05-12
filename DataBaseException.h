#ifndef __DATABASEEXCEPTION_H__
#define __DATABASEEXCEPTION_H__

//SQL �쳣����
enum enSQLException
{
	SQLException_None				= 0,									//û���쳣
	SQLException_Connect			= 1,									//���Ӵ���
	SQLException_Syntax				= 2,									//�﷨����
};

//ADO �쳣��
class CDataBaseException
{
	//��������
protected:
	enSQLException					m_enErrorType;						//�쳣����
	TCHAR							m_szDescribe[1024];						//�쳣����

	//��������
public:
	//���캯��
	CDataBaseException(enSQLException enErrorType, LPCTSTR pszDescribe);
	//��������
	virtual ~CDataBaseException();

	//���ܽӿ�
public:
	//�쳣����
	virtual enSQLException __cdecl GetErrorType()
	{
		return m_enErrorType;
	}

	//�쳣����
	virtual LPCTSTR __cdecl GetErrorDescribe()
	{
		return m_szDescribe;
	}
};


#endif // __DATABASEEXCEPTION_H__
