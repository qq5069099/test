
// SdupDlg.h : ͷ�ļ�
//

#pragma once

#include <list>
#include "DataBaseService.h"
#include "DlgAddItem.h"
#include "SdupListCtrl.h"
#include "GlobalDef.h"


using namespace std;


// CSdupDlg �Ի���
class CSdupDlg : public CDialogEx
{
	friend class CDlgAddItem;

private:
	CDataBase						m_DataBase;						//���ݿ����
	CSdupListCtrl					m_ListCtrl;
	CDateTimeCtrl					m_dtBegin;
	CDateTimeCtrl					m_dtEnd;
	CPrintDialog					m_PrintDialog;					//��ӡ�Ի���

	//��Ŀ����
public:
	tagKind							m_KindArray[32];				//��������
	int								m_nKindCount;					//��������

	tagSdupItem						m_SdupMonthTotal[12];			//�½�ͳ�� - ǰ���½�

	list<tagSdupItem*>				m_SdupItemList;					//��Ŀ����

	tagSdupItem						m_SdupItemTotal;				//��Ŀͳ�� - ���һ��
	tagSdupItem						m_SdupItemTotal2;				//��Ŀͳ�� - ���һ��+

	eLoadType						m_CurrentLoadType;				//��ǰ����

protected:
	//��������
	void LoadKind(eLoadType LoadType);
	//������Ŀ
	void LoadSdupItem(eLoadType LoadType);
	//��ȡ����
	tagKind * GetKind(int nKineID);
	//ͳ������+
	float Total2(int nPosKindID, int nTotalKindArray[], int nSize);
	//��������
	void CalcSdupItem();
	//ˢ�½���
	void FreshListCtrl(bool bResetColHead = false, int nSelIndex = -1);


private:
	void PrinterSdup(int nBeginIndex, int nEndIndex);			//��ӡ��Ŀ

// ����
public:
	CSdupDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ACCOUNT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//ȷ����ť
	afx_msg void OnBnClickedOk();
	//ȡ����ť
	afx_msg void OnBnClickedCancel();
	//�˳�����
	afx_msg void OnClose();
	//������С
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��ˮ��ѯ
	afx_msg void OnBnClickedButton2();
	//�����ѯ
	afx_msg void OnBnClickedButton6();
	//֧����ѯ
	afx_msg void OnBnClickedButton7();
	//����
	afx_msg void OnBnClickedButton4();
	//����
	afx_msg void OnBnClickedButton5();
	//����
	afx_msg void OnBnClickedButton1();
	//�޸�ѡ��
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	//����
	afx_msg void OnBnClickedButton8();
	//ȡ������
	int GetMonthDayCount(CTime time);
	//��ӡ
	afx_msg void OnBnClickedButton9();
	//�����ͷ
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//�����¼�
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
