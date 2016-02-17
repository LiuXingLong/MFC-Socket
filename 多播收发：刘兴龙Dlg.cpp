
// �ಥ�շ���������Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "�ಥ�շ���������.h"
#include "�ಥ�շ���������Dlg.h"
#include "afxdialogex.h"

#include<winsock2.h>
#include<ws2tcpip.h>
#include<stdio.h>
#include<stdlib.h>
#pragma comment(lib,"Ws2_32.lib")
//#define MCASTADDR "233.0.0.1"  //����ʹ�õĶಥ���ַ
//#define MCASTPORT 5150  //�󶨵ı��ض˿�
#define BUFSIZE 1024    //�������ݻ����С

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C�ಥ�շ���������Dlg �Ի���

static int flag = 0;    //������Ϣ�߳��Ƿ������־
static int MCASTPORT;   //�շ��˿�
static int Setinfo = 0; //���ñ�־
static char *MCASTADDR; //�ಥIP��ַ
static IN_ADDR IpAddr;  //�ಥIP��ַ
static CString Username;//��������

static WSADATA wsd;
struct sockaddr_in local, remote;
static SOCKET sock, sockM;


C�ಥ�շ���������Dlg::C�ಥ�շ���������Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C�ಥ�շ���������Dlg::IDD, pParent)
	, str(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C�ಥ�շ���������Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, str);
}

BEGIN_MESSAGE_MAP(C�ಥ�շ���������Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &C�ಥ�շ���������Dlg::OnBnClickedOk)	
	ON_BN_CLICKED(IDC_RADIO1, &C�ಥ�շ���������Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &C�ಥ�շ���������Dlg::OnBnClickedRadio2)
	ON_EN_CHANGE(IDC_EDIT1, &C�ಥ�շ���������Dlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &C�ಥ�շ���������Dlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT4, &C�ಥ�շ���������Dlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &C�ಥ�շ���������Dlg::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_BUTTON1, &C�ಥ�շ���������Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C�ಥ�շ���������Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &C�ಥ�շ���������Dlg::OnBnClickedButton3)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, &C�ಥ�շ���������Dlg::OnIpnFieldchangedIpaddress)
	ON_EN_SETFOCUS(IDC_EDIT1, &C�ಥ�շ���������Dlg::OnEnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT4, &C�ಥ�շ���������Dlg::OnEnSetfocusEdit4)
	ON_EN_SETFOCUS(IDC_EDIT5, &C�ಥ�շ���������Dlg::OnEnSetfocusEdit5)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// C�ಥ�շ���������Dlg ��Ϣ�������

BOOL C�ಥ�շ���������Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C�ಥ�շ���������Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//����λͼ  
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);

		//ѡ��DC  
		CClientDC cdc(this);
		CDC comdc;
		comdc.CreateCompatibleDC(&cdc);
		comdc.SelectObject(&bitmap);

		//����BITMAP  
		BITMAP bit;
		bitmap.GetBitmap(&bit);

		//�ͻ�����  
		CRect rect;
		GetClientRect(rect);

		//�ÿͻ�����DC���������ɵ�BITMAP������ӦΪ���ڴ�С  
		cdc.StretchBlt(0, 0, rect.Width(), rect.Height(), &comdc, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);
		CDialogEx::OnPaint();
	}	
}
HBRUSH C�ಥ�շ���������Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	/*
	if (IDC_EDIT1 == pWnd->GetDlgCtrlID())
	{
	pDC->SetBkMode(RGB(255, 255, 255));
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	if (IDC_EDIT2 == pWnd->GetDlgCtrlID())
	{
	pDC->SetBkMode(RGB(255, 255, 255));
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	*/
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
/*
��������

CDC* pDC:һ��ָ�����,ָ���ͼ����(�豸����)
CWnd* pWnd:һ��ָ�����,ָ�򴰿ڵ�����
UINT nCtlColor: �ؼ������ͱ�� ���¿�ѡ:
CTLCOLOR_BTN Button control
CTLCOLOR_DLG Dialog box
CTLCOLOR_EDIT Edit control
CTLCOLOR_LISTBOX List-box control
CTLCOLOR_MSGBOX Message box
CTLCOLOR_SCROLLBAR Scroll-bar control

CWnd   *pWnd = GetDlgItem(IDC_EDIT1); // ȡ�ÿؼ���ָ��
CDC *pDC = pWnd->GetWindowDC();
OnCtlColor(pDC,pWnd, CTLCOLOR_EDIT);

*/



//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C�ಥ�շ���������Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
������Ϣ
*/
void C�ಥ�շ���������Dlg::OnBnClickedOk()
{
	if (Setinfo == 0){
		MessageBox(L"��δ���úã��������ã�");
	}else if (flag==0){
		MessageBox(L"��δ�����޷����ͣ�");
	}else{
		CString str1;
		GetDlgItem(IDC_EDIT2)->GetWindowText(str1);
		if (str1.GetLength() == 0){
			MessageBox(L"�������ݲ���Ϊ�գ�");
		}
		else{
			//MessageBox(str);
			str1 = Username + str1;
			int nLength = str1.GetLength();
			int nBytes = WideCharToMultiByte(CP_ACP, 0, str1, nLength, NULL, 0, NULL, NULL);

			char* sendbuf = new char[nBytes + 1];
			memset(sendbuf, 0, nLength + 1);

			WideCharToMultiByte(CP_OEMCP, 0, str1, nLength, sendbuf, nBytes, NULL, NULL);
			sendbuf[nBytes] = '\0';

			/*
			char* �� char[] ת CString
			CString str1(sendbuf);
			MessageBox(str1);
			*/

			//���Ͷಥ���ݣ����û��ڿ���̨���롰QUIT��ʱ�˳���
			//printf("SEND:");	
			if (sendto(sockM, sendbuf, strlen(sendbuf), 0, (struct sockaddr*)&remote, sizeof(remote)) == SOCKET_ERROR){
				//printf("sendto failed with:%d\n", WSAGetLastError());
				closesocket(sockM);
				closesocket(sock);
				WSACleanup();
				MessageBox(L"����ʧ�ܣ�");
			}else{
				str1 = L"";
				GetDlgItem(IDC_EDIT2)->SetWindowText(str1);
			}
			GetDlgItem(IDC_EDIT2)->SetFocus();   // ��������  GetDlgItem(IDC_EDIT2)->HideCaret();
		}
	}
}

/*
������Ϣ
*/
static UINT TestThreadFun(LPVOID pParam)
{
	C�ಥ�շ���������Dlg *pDlg = (C�ಥ�շ���������Dlg *)pParam;

	struct sockaddr_in  from;
	char recvbuf[BUFSIZE]; 
	int len = sizeof(struct sockaddr_in);
	int ret;
	//���նಥ���ݣ������յ�������Ϊ��QUIT��ʱ�˳���
	while (flag){
		if (((ret = recvfrom(sock, (char*)recvbuf, BUFSIZE, 0, (struct sockaddr*)&from, &len)) == SOCKET_ERROR)){
			//printf("recvfrom failed with:%d\n", WSAGetLastError());
			closesocket(sockM);
			closesocket(sock);
			WSACleanup();			
			if (flag){
				flag = 0;
				pDlg->MessageBox(L"���ܶಥ����ʧ�ܣ�");
			}
			return -1;
		}
		if (strcmp((char*)recvbuf, "QUIT") == 0) break;
		else{
			recvbuf[ret] = '\0';
			//printf("RECV:'%s'FROM<%s>\n", recvbuf, inet_ntoa(from.sin_addr));	
			pDlg->str += recvbuf;
			pDlg->str += L"\r\n";
			pDlg->UpdateData(FALSE);

			//AfxGetApp()->PumpMessage();
			//GetDlgItem(IDC_EDIT1)->SetWindowText(str);
			//pDlg->MessageBox(pDlg->str);
		}
	}
	return 0;
}

void C�ಥ�շ���������Dlg::OnBnClickedRadio1()
{
	// ������Ϣ����
	if (Setinfo==0){
		MessageBox(L"��δ���úã��������ã�");
		CButton* radio = (CButton*)GetDlgItem(IDC_RADIO1);
		radio->SetCheck(0);
	}
	else{
		if (flag == 0){
			flag = 1;
			str = L"";
			UpdateData(FALSE);
			/* struct ip_mreq mcast;// Winsock1.0 */

			//��ʼ�� WinSock2.2
			if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0){
				//printf("WSAStartup() failed\n");
				flag = 0;
				MessageBox(L"Sock ��ʼ��ʧ�ܣ�");
				return;
			}
			/*
			����һ��SOCK_DGRAM���͵�SOCKET
			���У�WSA_FLAG_MULTIPOINT_C_LEAF ��ʾ IP �ಥ�ڿ�����������ڡ��޸������ͣ�
			WSA_FLAG_MULTIPOINT_D_LEAF ��ʾ IP �ಥ��������������ڡ��޸������йؿ�������
			��������йظ������Ķ�MSDN˵��
			*/
			if ((sock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF | WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET){
				//printf("socket failed with:%d\n", WSAGetLastError());
				WSACleanup();
				flag = 0;
				MessageBox(L"Socket ����ʧ�ܣ�");
				return;
			}
			//�� sock �󶨵�����ĳ�˿��ϡ�
			local.sin_family = AF_INET;
			local.sin_port = htons(MCASTPORT);
			local.sin_addr.s_addr = INADDR_ANY;

			if (bind(sock, (struct sockaddr*)&local, sizeof(local)) == SOCKET_ERROR){
				//printf("bind failed with:%d\n", WSAGetLastError());		
				closesocket(sock);
				WSACleanup();
				flag = 0;
				//	AfxEndThread(0);
				MessageBox(L"Sock �󶨱����˿�ʧ�ܣ�");
				return;
			}
			//����ಥ��
			remote.sin_family = AF_INET;
			remote.sin_port = htons(MCASTPORT);
			remote.sin_addr.s_addr = inet_addr(MCASTADDR);

			/* Winsock2.0 */
			if ((sockM = WSAJoinLeaf(sock, (SOCKADDR*)&remote, sizeof(remote), NULL, NULL, NULL, NULL, JL_BOTH)) == INVALID_SOCKET){
				//printf("WSAJoinLeaf() failed:%d\n", WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				flag = 0;
				MessageBox(L"���벥����ʧ�ܣ�");
				return;
			}
			MessageBox(L"���߳ɹ���");
			AfxBeginThread(TestThreadFun, this);
		}else{
			MessageBox(L"��������״̬�ˣ�");
		}
	}
}

void C�ಥ�շ���������Dlg::OnBnClickedRadio2()
{
	// ������Ϣ����
	if (Setinfo == 0){
		MessageBox(L"��δ���úã��������ã�");
		CButton* radio = (CButton*)GetDlgItem(IDC_RADIO2);
		radio->SetCheck(0);
	}else{
		if (flag == 1){
			flag = 0;
			str = L"";
			closesocket(sockM);
			closesocket(sock);
			WSACleanup();
			MessageBox(L"���߳ɹ���");
		}else{
			MessageBox(L"��������״̬�ˣ�");
		}
	}	
}
void C�ಥ�շ���������Dlg::OnIpnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult)
{	
	if (Setinfo){
		GetDlgItem(IDC_EDIT2)->SetFocus();
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS))->SetAddress(IpAddr.S_un.S_un_b.s_b1, IpAddr.S_un.S_un_b.s_b2, IpAddr.S_un.S_un_b.s_b3, IpAddr.S_un.S_un_b.s_b4);
	}else{
		LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
		switch (pIPAddr->iField)
		{
		case 0:
			IpAddr.S_un.S_un_b.s_b1 = pIPAddr->iValue;
			break;
		case 1:
			IpAddr.S_un.S_un_b.s_b2 = pIPAddr->iValue;
			break;
		case 2:
			IpAddr.S_un.S_un_b.s_b3 = pIPAddr->iValue;
			break;
		case 3:
			IpAddr.S_un.S_un_b.s_b4 = pIPAddr->iValue;
			break;
		default:
			break;
		}
		*pResult = 0;	
	}	
}
void C�ಥ�շ���������Dlg::OnBnClickedButton1()
{
	// �����Ϣ��¼
	str = L"";
	UpdateData(FALSE);
}
void C�ಥ�շ���������Dlg::OnBnClickedButton2()
{
	// ������Ϣ    5001-65535    225.0.0.0-239.255.255.255  ip: MCASTADDR     MCASTPORT;   //���ն˿�  MCASTPORT1;  //���Ͷ˿�   Username

	CString szIp, Port;
	szIp.Format(TEXT("%d.%d.%d.%d"),IpAddr.S_un.S_un_b.s_b1,IpAddr.S_un.S_un_b.s_b2,IpAddr.S_un.S_un_b.s_b3,IpAddr.S_un.S_un_b.s_b4);	
	int nLength = szIp.GetLength();
	int nBytes = WideCharToMultiByte(CP_ACP, 0, szIp, nLength, NULL, 0, NULL, NULL);
	MCASTADDR = new char[nBytes + 1];
	memset(MCASTADDR, 0, nLength + 1);
	WideCharToMultiByte(CP_OEMCP, 0, szIp, nLength, MCASTADDR, nBytes, NULL, NULL);
	MCASTADDR[nBytes] = '\0'; //MessageBox((LPCTSTR)MCASTADDR);

	GetDlgItem(IDC_EDIT4)->GetWindowText(Port);
	GetDlgItem(IDC_EDIT5)->GetWindowText(Username);
	Username += L":";
	MCASTPORT=_ttoi(Port);
	if (!(IpAddr.S_un.S_un_b.s_b1 >= 225 && IpAddr.S_un.S_un_b.s_b1 <= 239 && IpAddr.S_un.S_un_b.s_b2 >= 0 && IpAddr.S_un.S_un_b.s_b2 <= 255 && IpAddr.S_un.S_un_b.s_b3 >= 0 && IpAddr.S_un.S_un_b.s_b3 <= 225 && IpAddr.S_un.S_un_b.s_b4 >= 1 && IpAddr.S_un.S_un_b.s_b4<225)){
		MessageBox(L"�ಥIP��ַ����IP��ַ��Χ��225.0.0.1-239.255.255.255");
		return;
	}
	if (!(MCASTPORT >= 5001 && MCASTPORT <= 65535)){
		MessageBox(L"���ն˿����󣡶˿ڷ�Χ��5001-65535");
		return;
	}
	if (Username.GetLength() == 0){
		MessageBox(L"δ���÷��������������÷���������");
		return;
	}
	/*
	���ÿؼ�������
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(false);
	GetDlgItem(IDC_EDIT4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT5)->EnableWindow(false);

	//RedrawWindow(); //ˢ�½���
	//GetDlgItem(IDC_IPADDRESS)->PostMessage(WM_KILLFOCUS, 0, 0); //ʧȥ����
	*/
	Setinfo = 1;
	//MessageBox(szIp);
}
void C�ಥ�շ���������Dlg::OnBnClickedButton3()
{
	// �޸�������Ϣ
	if (flag == 0){
		/*
		GetDlgItem(IDC_IPADDRESS)->EnableWindow();
		GetDlgItem(IDC_EDIT4)->EnableWindow();
		GetDlgItem(IDC_EDIT5)->EnableWindow();		
		*/
		CButton* radio1 = (CButton*)GetDlgItem(IDC_RADIO1);
		CButton* radio2 = (CButton*)GetDlgItem(IDC_RADIO2);
		radio1->SetCheck(0);
		radio2->SetCheck(0);
		flag = 0;
		Setinfo = 0;
	}else{
		MessageBox(L"�������ߣ�Ȼ������޸ģ�");
	}	
}

void C�ಥ�շ���������Dlg::OnEnSetfocusEdit1()
{
	GetDlgItem(IDC_EDIT2)->SetFocus(); 
}

/*
������úú��޷���ȡ����
*/
void C�ಥ�շ���������Dlg::OnEnSetfocusEdit4()
{
	if (Setinfo){
		GetDlgItem(IDC_EDIT2)->SetFocus();
	}
}

void C�ಥ�շ���������Dlg::OnEnSetfocusEdit5()
{
	if (Setinfo){
		GetDlgItem(IDC_EDIT2)->SetFocus();
	}
}



void C�ಥ�շ���������Dlg::OnEnChangeEdit1()
{

}

void C�ಥ�շ���������Dlg::OnEnChangeEdit2()
{

}

void C�ಥ�շ���������Dlg::OnEnChangeEdit4()
{

}

void C�ಥ�շ���������Dlg::OnEnChangeEdit5()
{

}