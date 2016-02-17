
// 多播收发：刘兴龙Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "多播收发：刘兴龙.h"
#include "多播收发：刘兴龙Dlg.h"
#include "afxdialogex.h"

#include<winsock2.h>
#include<ws2tcpip.h>
#include<stdio.h>
#include<stdlib.h>
#pragma comment(lib,"Ws2_32.lib")
//#define MCASTADDR "233.0.0.1"  //本例使用的多播组地址
//#define MCASTPORT 5150  //绑定的本地端口
#define BUFSIZE 1024    //接收数据缓冲大小

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C多播收发：刘兴龙Dlg 对话框

static int flag = 0;    //接受消息线程是否结束标志
static int MCASTPORT;   //收发端口
static int Setinfo = 0; //设置标志
static char *MCASTADDR; //多播IP地址
static IN_ADDR IpAddr;  //多播IP地址
static CString Username;//发送人名

static WSADATA wsd;
struct sockaddr_in local, remote;
static SOCKET sock, sockM;


C多播收发：刘兴龙Dlg::C多播收发：刘兴龙Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C多播收发：刘兴龙Dlg::IDD, pParent)
	, str(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C多播收发：刘兴龙Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, str);
}

BEGIN_MESSAGE_MAP(C多播收发：刘兴龙Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &C多播收发：刘兴龙Dlg::OnBnClickedOk)	
	ON_BN_CLICKED(IDC_RADIO1, &C多播收发：刘兴龙Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &C多播收发：刘兴龙Dlg::OnBnClickedRadio2)
	ON_EN_CHANGE(IDC_EDIT1, &C多播收发：刘兴龙Dlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &C多播收发：刘兴龙Dlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT4, &C多播收发：刘兴龙Dlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &C多播收发：刘兴龙Dlg::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_BUTTON1, &C多播收发：刘兴龙Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C多播收发：刘兴龙Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &C多播收发：刘兴龙Dlg::OnBnClickedButton3)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, &C多播收发：刘兴龙Dlg::OnIpnFieldchangedIpaddress)
	ON_EN_SETFOCUS(IDC_EDIT1, &C多播收发：刘兴龙Dlg::OnEnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT4, &C多播收发：刘兴龙Dlg::OnEnSetfocusEdit4)
	ON_EN_SETFOCUS(IDC_EDIT5, &C多播收发：刘兴龙Dlg::OnEnSetfocusEdit5)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// C多播收发：刘兴龙Dlg 消息处理程序

BOOL C多播收发：刘兴龙Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C多播收发：刘兴龙Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//加载位图  
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);

		//选入DC  
		CClientDC cdc(this);
		CDC comdc;
		comdc.CreateCompatibleDC(&cdc);
		comdc.SelectObject(&bitmap);

		//生成BITMAP  
		BITMAP bit;
		bitmap.GetBitmap(&bit);

		//客户区域  
		CRect rect;
		GetClientRect(rect);

		//用客户区的DC绘制所生成的BITMAP，并适应为窗口大小  
		cdc.StretchBlt(0, 0, rect.Width(), rect.Height(), &comdc, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);
		CDialogEx::OnPaint();
	}	
}
HBRUSH C多播收发：刘兴龙Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
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
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
/*
还不会用

CDC* pDC:一个指针对象,指向绘图区域(设备环境)
CWnd* pWnd:一个指针对象,指向窗口的名柄
UINT nCtlColor: 控件的类型编号 如下可选:
CTLCOLOR_BTN Button control
CTLCOLOR_DLG Dialog box
CTLCOLOR_EDIT Edit control
CTLCOLOR_LISTBOX List-box control
CTLCOLOR_MSGBOX Message box
CTLCOLOR_SCROLLBAR Scroll-bar control

CWnd   *pWnd = GetDlgItem(IDC_EDIT1); // 取得控件的指针
CDC *pDC = pWnd->GetWindowDC();
OnCtlColor(pDC,pWnd, CTLCOLOR_EDIT);

*/



//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C多播收发：刘兴龙Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
发送消息
*/
void C多播收发：刘兴龙Dlg::OnBnClickedOk()
{
	if (Setinfo == 0){
		MessageBox(L"还未设置好，请先设置！");
	}else if (flag==0){
		MessageBox(L"还未上线无法发送！");
	}else{
		CString str1;
		GetDlgItem(IDC_EDIT2)->GetWindowText(str1);
		if (str1.GetLength() == 0){
			MessageBox(L"发送内容不能为空！");
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
			char* 与 char[] 转 CString
			CString str1(sendbuf);
			MessageBox(str1);
			*/

			//发送多播数据，当用户在控制台输入“QUIT”时退出。
			//printf("SEND:");	
			if (sendto(sockM, sendbuf, strlen(sendbuf), 0, (struct sockaddr*)&remote, sizeof(remote)) == SOCKET_ERROR){
				//printf("sendto failed with:%d\n", WSAGetLastError());
				closesocket(sockM);
				closesocket(sock);
				WSACleanup();
				MessageBox(L"发送失败！");
			}else{
				str1 = L"";
				GetDlgItem(IDC_EDIT2)->SetWindowText(str1);
			}
			GetDlgItem(IDC_EDIT2)->SetFocus();   // 隐常焦点  GetDlgItem(IDC_EDIT2)->HideCaret();
		}
	}
}

/*
接受消息
*/
static UINT TestThreadFun(LPVOID pParam)
{
	C多播收发：刘兴龙Dlg *pDlg = (C多播收发：刘兴龙Dlg *)pParam;

	struct sockaddr_in  from;
	char recvbuf[BUFSIZE]; 
	int len = sizeof(struct sockaddr_in);
	int ret;
	//接收多播数据，当接收到的数据为“QUIT”时退出。
	while (flag){
		if (((ret = recvfrom(sock, (char*)recvbuf, BUFSIZE, 0, (struct sockaddr*)&from, &len)) == SOCKET_ERROR)){
			//printf("recvfrom failed with:%d\n", WSAGetLastError());
			closesocket(sockM);
			closesocket(sock);
			WSACleanup();			
			if (flag){
				flag = 0;
				pDlg->MessageBox(L"接受多播数据失败！");
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

void C多播收发：刘兴龙Dlg::OnBnClickedRadio1()
{
	// 接收消息上线
	if (Setinfo==0){
		MessageBox(L"还未设置好，请先设置！");
		CButton* radio = (CButton*)GetDlgItem(IDC_RADIO1);
		radio->SetCheck(0);
	}
	else{
		if (flag == 0){
			flag = 1;
			str = L"";
			UpdateData(FALSE);
			/* struct ip_mreq mcast;// Winsock1.0 */

			//初始化 WinSock2.2
			if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0){
				//printf("WSAStartup() failed\n");
				flag = 0;
				MessageBox(L"Sock 初始化失败！");
				return;
			}
			/*
			创建一个SOCK_DGRAM类型的SOCKET
			其中，WSA_FLAG_MULTIPOINT_C_LEAF 表示 IP 多播在控制面层上属于“无根”类型；
			WSA_FLAG_MULTIPOINT_D_LEAF 表示 IP 多播在数据面层上属于“无根”，有关控制面层和
			数据面层有关概念请阅读MSDN说明
			*/
			if ((sock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF | WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET){
				//printf("socket failed with:%d\n", WSAGetLastError());
				WSACleanup();
				flag = 0;
				MessageBox(L"Socket 创建失败！");
				return;
			}
			//将 sock 绑定到本机某端口上。
			local.sin_family = AF_INET;
			local.sin_port = htons(MCASTPORT);
			local.sin_addr.s_addr = INADDR_ANY;

			if (bind(sock, (struct sockaddr*)&local, sizeof(local)) == SOCKET_ERROR){
				//printf("bind failed with:%d\n", WSAGetLastError());		
				closesocket(sock);
				WSACleanup();
				flag = 0;
				//	AfxEndThread(0);
				MessageBox(L"Sock 绑定本机端口失败！");
				return;
			}
			//加入多播组
			remote.sin_family = AF_INET;
			remote.sin_port = htons(MCASTPORT);
			remote.sin_addr.s_addr = inet_addr(MCASTADDR);

			/* Winsock2.0 */
			if ((sockM = WSAJoinLeaf(sock, (SOCKADDR*)&remote, sizeof(remote), NULL, NULL, NULL, NULL, JL_BOTH)) == INVALID_SOCKET){
				//printf("WSAJoinLeaf() failed:%d\n", WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				flag = 0;
				MessageBox(L"加入播数组失败！");
				return;
			}
			MessageBox(L"上线成功！");
			AfxBeginThread(TestThreadFun, this);
		}else{
			MessageBox(L"以是上线状态了！");
		}
	}
}

void C多播收发：刘兴龙Dlg::OnBnClickedRadio2()
{
	// 接收消息下线
	if (Setinfo == 0){
		MessageBox(L"还未设置好，请先设置！");
		CButton* radio = (CButton*)GetDlgItem(IDC_RADIO2);
		radio->SetCheck(0);
	}else{
		if (flag == 1){
			flag = 0;
			str = L"";
			closesocket(sockM);
			closesocket(sock);
			WSACleanup();
			MessageBox(L"下线成功！");
		}else{
			MessageBox(L"以是下线状态了！");
		}
	}	
}
void C多播收发：刘兴龙Dlg::OnIpnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult)
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
void C多播收发：刘兴龙Dlg::OnBnClickedButton1()
{
	// 清空消息记录
	str = L"";
	UpdateData(FALSE);
}
void C多播收发：刘兴龙Dlg::OnBnClickedButton2()
{
	// 设置信息    5001-65535    225.0.0.0-239.255.255.255  ip: MCASTADDR     MCASTPORT;   //接收端口  MCASTPORT1;  //发送端口   Username

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
		MessageBox(L"多播IP地址有误！IP地址范围：225.0.0.1-239.255.255.255");
		return;
	}
	if (!(MCASTPORT >= 5001 && MCASTPORT <= 65535)){
		MessageBox(L"接收端口有误！端口范围：5001-65535");
		return;
	}
	if (Username.GetLength() == 0){
		MessageBox(L"未设置发送人名，请设置发送人名。");
		return;
	}
	/*
	设置控件不可用
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(false);
	GetDlgItem(IDC_EDIT4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT5)->EnableWindow(false);

	//RedrawWindow(); //刷新界面
	//GetDlgItem(IDC_IPADDRESS)->PostMessage(WM_KILLFOCUS, 0, 0); //失去焦点
	*/
	Setinfo = 1;
	//MessageBox(szIp);
}
void C多播收发：刘兴龙Dlg::OnBnClickedButton3()
{
	// 修改设置信息
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
		MessageBox(L"请先下线，然后进行修改！");
	}	
}

void C多播收发：刘兴龙Dlg::OnEnSetfocusEdit1()
{
	GetDlgItem(IDC_EDIT2)->SetFocus(); 
}

/*
如果设置好后无法获取焦点
*/
void C多播收发：刘兴龙Dlg::OnEnSetfocusEdit4()
{
	if (Setinfo){
		GetDlgItem(IDC_EDIT2)->SetFocus();
	}
}

void C多播收发：刘兴龙Dlg::OnEnSetfocusEdit5()
{
	if (Setinfo){
		GetDlgItem(IDC_EDIT2)->SetFocus();
	}
}



void C多播收发：刘兴龙Dlg::OnEnChangeEdit1()
{

}

void C多播收发：刘兴龙Dlg::OnEnChangeEdit2()
{

}

void C多播收发：刘兴龙Dlg::OnEnChangeEdit4()
{

}

void C多播收发：刘兴龙Dlg::OnEnChangeEdit5()
{

}