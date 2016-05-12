#ifndef _DATABASE_H_
#define _DATABASE_H_

//组件头文件
#include "DataBaseException.h"

#include <ICrsint.h>
#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

//////////////////////////////////////////////////////////////////////////

//数据库对象
class CDataBase
{
	//信息变量
protected:
	CString									m_strConnect;								//连接字符串

	//状态变量
protected:
	DWORD									m_dwConnectCount;							//重试次数
	DWORD									m_dwConnectErrorTime;						//错误时间
	const DWORD								m_dwResumeConnectCount;						//恢复次数
	const DWORD								m_dwResumeConnectTime;						//恢复时间

	//内核变量
protected:
	_CommandPtr								m_DBCommand;								//命令对象
	_RecordsetPtr								m_DBRecordset;								//记录集对象
	_ConnectionPtr							m_DBConnection;								//数据库对象

	//线程处理
protected:
	CCriticalSection							m_CriticalSection;							//同步锁定

	//函数定义
public:
	//构造函数
	CDataBase();
	//析构函数
	virtual ~CDataBase();

	//管理接口
public:
	//打开连接
	virtual bool __cdecl OpenConnection();
	//关闭记录
	virtual bool __cdecl CloseRecordset();
	//关闭连接
	virtual bool __cdecl CloseConnection();
	//重新连接
	virtual bool __cdecl TryConnectAgain(bool bFocusConnect, _com_error * pComError);
	//连接信息
	virtual bool __cdecl SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//设置信息
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//设置信息
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szDBConnectString);
	//切换记录
	virtual void __cdecl NextRecordset();
	//切换记录
	virtual void __cdecl LastRecordset();

	//状态接口
public:
	//是否连接错误
	virtual bool __cdecl IsConnectError();
	//是否连接状态
	virtual bool __cdecl IsConnected();
	//是否打开
	virtual bool __cdecl IsRecordsetOpened();

	//记录集接口
public:
	//往下移动
	virtual void __cdecl MoveToNext();
	//往上移动
	virtual void __cdecl MovePrevious();
	//移到开头
	virtual void __cdecl MoveToFirst();
	//是否结束
	virtual bool __cdecl IsRecordsetEnd();
	//获取数目
	virtual long __cdecl GetRecordCount();
	//获取大小
	virtual long __cdecl GetActualSize(LPCTSTR pszParamName);
	//绑定对象
	virtual void __cdecl BindToRecordset(CADORecordBinding * pBind);
	//获取数据
	virtual void __cdecl GetRecordsetValue(LPCTSTR pszItem, _variant_t & DBVarValue);

	//字段接口
public:
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BYTE & cbValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, WORD & wValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, INT & nValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, LONG & lValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, FLOAT & dbValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, char szBuffer[], UINT uSize);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BSTR *bstrString);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, bool & bValue);

	//获取数据
public:
	//获取数据
	virtual INT GetValue_INT(LPCTSTR pszItem);
	//获取数据
	virtual UINT GetValue_UINT(LPCTSTR pszItem);
	//获取数据
	virtual LONG GetValue_LONG(LPCTSTR pszItem);
	//获取数据
	virtual BYTE GetValue_BYTE(LPCTSTR pszItem);
	//获取数据
	virtual WORD GetValue_WORD(LPCTSTR pszItem);
	//获取数据
	virtual DWORD GetValue_DWORD(LPCTSTR pszItem);
	//获取数据
	virtual FLOAT GetValue_FLOAT(LPCTSTR pszItem);
	//获取数据
	virtual DOUBLE GetValue_DOUBLE(LPCTSTR pszItem);
	//获取数据
	virtual LONGLONG GetValue_LONGLONG(LPCTSTR pszItem);
	//获取数据
	virtual __time64_t GetValue_TIME(LPCTSTR pszItem);
	//获取数据
	virtual bool GetValue_BOOL(LPCTSTR pszItem);
	//获取字符
	virtual VOID GetValue_LPSTR(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount);
	//获取字符
	virtual VOID GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount);
	//获取字符
	virtual VOID GetValue_BSTR(LPCTSTR pszItem, BSTR* bstrString);

	//命令对象接口
public:
	//插入参数
	virtual void __cdecl AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, _variant_t & DBVarValue);
	//重置参数
	virtual VOID ResetParameter();
	//获得参数
	virtual void __cdecl GetParameter(LPCTSTR pszParamName, _variant_t & DBVarValue);
	//获取返回数值
	virtual long __cdecl GetReturnValue();

	//插入参数
public:
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, INT nValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, UINT uValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, LONG lValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, LONGLONG lValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, BYTE cbValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, WORD wValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, DWORD dwValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, FLOAT fValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, DOUBLE dValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, char* pszString);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, BSTR pszString);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, LPCTSTR pszString);

	//执行接口
public:
	//存储过程
	virtual LONG __cdecl ExecuteProcess(LPCTSTR pszSPName, bool bRecordset);
	//执行命令
	virtual void __cdecl ExecuteSentence(LPCTSTR pszCommand, bool bRecordset);

	//事务
public:
	//开始事务
	virtual void __cdecl BeginTrans();
	//完成事务
	virtual void __cdecl CommitTrans();
	//回滚事务
	virtual void __cdecl RollbackTrans();

	//内部函数
private:
	//删除参数
	void __cdecl ClearParameters();
};



#endif // _DATABASE_H_