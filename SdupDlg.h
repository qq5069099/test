
// SdupDlg.h : 头文件
//

#pragma once

#include <list>
#include "DataBaseService.h"
#include "DlgAddItem.h"
#include "SdupListCtrl.h"
#include "GlobalDef.h"


using namespace std;


// CSdupDlg 对话框
class CSdupDlg : public CDialogEx
{
	friend class CDlgAddItem;

private:
	CDataBase						m_DataBase;						//数据库对象
	CSdupListCtrl					m_ListCtrl;
	CDateTimeCtrl					m_dtBegin;
	CDateTimeCtrl					m_dtEnd;
	CPrintDialog					m_PrintDialog;					//打印对话框

	//帐目数据
public:
	tagKind							m_KindArray[32];				//类型数组
	int								m_nKindCount;					//类型数量

	tagSdupItem						m_SdupMonthTotal[12];			//月结统计 - 前行月结

	list<tagSdupItem*>				m_SdupItemList;					//帐目链表

	tagSdupItem						m_SdupItemTotal;				//帐目统计 - 最后一行
	tagSdupItem						m_SdupItemTotal2;				//帐目统计 - 最后一行+

	eLoadType						m_CurrentLoadType;				//当前加载

protected:
	//加载类型
	void LoadKind(eLoadType LoadType);
	//加载帐目
	void LoadSdupItem(eLoadType LoadType);
	//获取类型
	tagKind * GetKind(int nKineID);
	//统计数据+
	float Total2(int nPosKindID, int nTotalKindArray[], int nSize);
	//计算帐务
	void CalcSdupItem();
	//刷新界面
	void FreshListCtrl(bool bResetColHead = false, int nSelIndex = -1);


private:
	void PrinterSdup(int nBeginIndex, int nEndIndex);			//打印帐目

// 构造
public:
	CSdupDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ACCOUNT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//确定按钮
	afx_msg void OnBnClickedOk();
	//取消按钮
	afx_msg void OnBnClickedCancel();
	//退出程序
	afx_msg void OnClose();
	//调整大小
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//流水查询
	afx_msg void OnBnClickedButton2();
	//收入查询
	afx_msg void OnBnClickedButton6();
	//支出查询
	afx_msg void OnBnClickedButton7();
	//今年
	afx_msg void OnBnClickedButton4();
	//本月
	afx_msg void OnBnClickedButton5();
	//今天
	afx_msg void OnBnClickedButton1();
	//修改选择
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	//上月
	afx_msg void OnBnClickedButton8();
	//取月天数
	int GetMonthDayCount(CTime time);
	//打印
	afx_msg void OnBnClickedButton9();
	//点击列头
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//单击事件
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
