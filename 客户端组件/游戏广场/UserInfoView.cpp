#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "UserInfoView.h"

//////////////////////////////////////////////////////////////////////////

//颜色定义
#define CR_TEXT						RGB(0,0,0)					//字体颜色
#define CR_BACKGROUP				RGB(255,232,160)					//背景颜色

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUserInfoView, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CUserInfoView::CUserInfoView() : CDialog(IDD_USER_INFO)
{
	//状态变量
	m_nTitleHeight=0;
	m_bShowInfo=false;
	m_bMouseOnFace=false;
	m_bCreateFlag=false;
	return;
}

//析构函数
CUserInfoView::~CUserInfoView()
{
}

//控件绑定
VOID CUserInfoView::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
   
}

//初始化函数
BOOL CUserInfoView::OnInitDialog()
{
	__super::OnInitDialog();

	//加载资源
	UpdateSkinResource();

	//获取大小
	CImageHandle HandleTitleM(&m_ImageTitleM);
	m_nTitleHeight=m_ImageTitleM.GetHeight();

	//移动窗口
	SetWindowPos(NULL,0,0,0,m_nTitleHeight,SWP_NOMOVE|SWP_NOZORDER);

	//设置变量
	m_bCreateFlag=true;
    HINSTANCE hResInstance=AfxGetInstanceHandle();
	CRect rcCreate(0,0,0,0);

	
	return TRUE;
}

//标题高度
UINT CUserInfoView::GetTitleHeight()
{
	return m_nTitleHeight;
}

//更新信息
VOID CUserInfoView::UpdateUserInfo()
{
	//更新界面
	Invalidate();

	return;
}

//显示信息
VOID CUserInfoView::ShowUserInfo(bool bShowInfo)
{
	//状态判断
	if (m_bShowInfo==bShowInfo) return;

	//设置变量
	m_bShowInfo=bShowInfo;

	//更新界面
	Invalidate();

	return;
}

//更新资源
bool CUserInfoView::UpdateSkinResource()
{
	//获取资源
	tagControlViewImage & ControlViewImage=g_GlobalUnits.m_ControlViewImage;
	HINSTANCE hInstance=g_GlobalUnits.m_PlatformResourceModule->GetResInstance();

	m_ImageTitleL.SetLoadInfo(ControlViewImage.uUserInfoL,hInstance);
	m_ImageTitleM.SetLoadInfo(ControlViewImage.uUserInfoM,hInstance);
	m_ImageTitleR.SetLoadInfo(ControlViewImage.uUserInfoR,hInstance);

	HINSTANCE hInstance1=AfxGetInstanceHandle();

	HINSTANCE hMainInstance=AfxGetInstanceHandle();
	m_ImageMale.SetLoadInfo(IDB_FACE_MALE,hMainInstance);
	m_ImageFemale.SetLoadInfo(IDB_FACE_FEMALE,hMainInstance);
	
	m_ImageMan.SetLoadInfo(IDB_FACE_MAN,hInstance1);
	m_ImageWoMan.SetLoadInfo(IDB_FACE_WOMAN,hInstance1);
	
	return true;
}
//绘画数字
void CUserInfoView::DrawNumberString(CDC * pDC, __int64 iNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//加载资源
	CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	//CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/10,m_ImageScoreNumber.GetHeight());

	//if ( bMeScore ) 
//		SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//计算数目
	int iNumberCount=0;
	__int64 iNumberTemp=iNumber;
	do
	{
		iNumberCount++;
		iNumberTemp/=10;
	} while (iNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos+iNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//绘画桌号
	for (LONG i=0;i<iNumberCount;i++)
	{
		//绘画号码
		int iCellNumber=(int)(iNumber%10);
	
		
			m_ImageScoreNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				iCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
	

		//设置变量
		iNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	}

	return;
}

//绘画背景
BOOL CUserInfoView::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

    int nWidth = rcClient.Width();
	int nHeight = rcClient.Height();
	//建立缓冲
	//用户信息位置修改
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),24);

	//创建 DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(pDC);
	BufferDC.SelectObject(ImageBuffer);

	//加载资源
	CImageHandle HandleTitleL(&m_ImageTitleL);
	CImageHandle HandleTitleM(&m_ImageTitleM);
	CImageHandle HandleTitleR(&m_ImageTitleR);
	CImageHandle HandleBombEffect(&m_ImageScore);
	CImageHandle HandleBombMan(&m_ImageMan);
	CImageHandle HandleBombWoMan(&m_ImageWoMan);

	HDWP hDwp=BeginDeferWindowPos(0);
	EndDeferWindowPos(hDwp);
	//绘画背景
	m_ImageTitleL.BitBlt(BufferDC,0,0);
    
	//填充中间
	for (INT nXPos=m_ImageTitleL.GetWidth();nXPos<rcClient.Width();nXPos+=m_ImageTitleM.GetWidth())
	{
		m_ImageTitleM.BitBlt(BufferDC,nXPos,0);
	}
    //绘画背景
	m_ImageTitleR.BitBlt(BufferDC,rcClient.Width()-m_ImageTitleR.GetWidth(),0);
	 
	//设置 DC
	BufferDC.SetTextColor(CR_TEXT);
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());

	////用户信息
	if (m_bShowInfo==true)
	{
		//变量定义
		//tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
		tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();

		m_rcFace = CRect(27-4,42-4,27+48+4,42+48+4);
		CBrush BackBrush(RGB(255,255,255));
		CPen LinePen(PS_SOLID,1,CSkinWndObject::m_SkinAttribute.m_crInsideBorder);
		BufferDC.SelectObject(&LinePen);
		BufferDC.SelectObject(&BackBrush);
		CBrush BackBrush2(RGB(0,255,255));

		//绘画上框
		if(m_bMouseOnFace)
		{
			BufferDC.SelectObject(&BackBrush2);
			BufferDC.RoundRect(27-3,42-3,27+48+3,42+48+3,10,10);
		}
		else
			BufferDC.RoundRect(27-2,42-2,27+48+2,42+48+2,10,10);

		//用户头像
		g_GlobalUnits.m_UserFaceRes->DrawNormalFace(&BufferDC,27,42,GlobalUserInfo.wFaceID,GlobalUserInfo.dwUserID,GlobalUserInfo.dwCustomFaceVer,48,48);

			//构造位置
		TCHAR szBuffer[255]=TEXT("");
		CRect rcAccounts(115-21,27+5+7,rcClient.Width()-10,17+25+5+7);//昵称
		CRect rcGameID(rcAccounts.left,rcAccounts.bottom+2,rcClient.Width()-10,rcAccounts.bottom+22);//ID号
		CRect rcEX(rcAccounts.left,rcAccounts.bottom+2+22,rcClient.Width()-10,rcAccounts.bottom+22*2);//ID号
		CRect rcLV(115-21,66-35+11,rcClient.Width()-10,94+11);//ID号
		//CRect rcUnderGame(70,44,rcClient.Width()-20,59);//性别
		//BufferDC.FillRect(rcAccounts, &BackBrush);
		//BufferDC.FillRect(rcGameID, &BackBrush);
		//BufferDC.FillRect(rcEX, &BackBrush);

		//用户帐号

		CFont GlodFont;
		GlodFont.CreateFont(-14,0,0,0,500,0,0,0,134,3,2,1,2,TEXT("宋体"));
		CFont * pOldFont=BufferDC.SelectObject(&GlodFont);
		BufferDC.SetTextColor(RGB(0,87,233));
		//用户昵称
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s\t\t[ID:%d]"),GlobalUserInfo.szAccounts, GlobalUserInfo.dwGameID);
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcAccounts,DT_LEFT|DT_WORD_ELLIPSIS|DT_SINGLELINE);

		//GameID
// 		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("用户ID: %d"),GlobalUserInfo.dwGameID);
// 		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcGameID,DT_VCENTER|DT_WORD_ELLIPSIS|DT_SINGLELINE);
		BufferDC.SelectObject(pOldFont);
 	    //经验
		if(lstrlen(GlobalUserInfo.szUnderWrite) < 1)
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("点击编辑"));
		else
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),GlobalUserInfo.szUnderWrite);

  		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcEX,DT_VCENTER|DT_WORD_ELLIPSIS|DT_SINGLELINE);

		//BufferDC.Draw3dRect(rcGameID, RGB(255,0,0), RGB(255,255,0));
//  
 		//会员信息
//  		LPCTSTR pszMemberOrder[]={TEXT("非会员"),TEXT("红钻会员"),TEXT("蓝钻会员"),TEXT("黄钻会员"),TEXT("紫钻会员")};
//  		if (GlobalUserInfo.cbMember<CountArray(pszMemberOrder))
//  		{
//  			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),pszMemberOrder[GlobalUserInfo.cbMember]);
//  			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcLV,DT_VCENTER|DT_WORD_ELLIPSIS|DT_SINGLELINE);
//  		}
		
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	ImageBuffer.Destroy();


	return TRUE;
 }

 void CUserInfoView::OnLButtonDown( UINT nFlags, CPoint Point )
 {
	 __super::OnLButtonDown(nFlags,Point);

	 //模拟按标题
	// if ((m_bMaxShow==false)&&(Point.y<=m_ImageInfoFrame.nTBorder-FRAME_EXALTATION))
	 {
		 ::PostMessage(AfxGetMainWnd()->m_hWnd, WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	 }

	 if(m_bMouseOnFace==true)
	 {
		CGameFrame *pGameFrame=(CGameFrame *)AfxGetMainWnd();
		pGameFrame->WebBrowse("http://www.7x78.com/MemberModifyFace.asp",true);
	 }

	 return;
 }

 BOOL CUserInfoView::OnSetCursor( CWnd * pWnd, UINT nHitTest, UINT uMessage )
 {
	 //获取光标
	 CPoint MousePoint;
	 GetCursorPos(&MousePoint);
	 ScreenToClient(&MousePoint);

	 //变量定义
	 bool bMouseEvent=false;
	 bool bRedrawWindow=false;

	 //盘旋判断
	 if ((m_bMouseOnFace==false)&&(m_rcFace.PtInRect(MousePoint)==TRUE))
	 {
		 //设置变量
		 bMouseEvent=true;
		 bRedrawWindow=true;



		 //设置变量
		 m_bMouseOnFace=true;
	 }
	 //离开判断
	 if ((m_bMouseOnFace==true)&&(m_rcFace.PtInRect(MousePoint)==FALSE))
	 {
		 //设置变量
		 bRedrawWindow=true;

		 //设置变量
		 m_bMouseOnFace=false;
	 }

	 //更新界面
	 if (bRedrawWindow==true)
	 {
		 RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	 }

	 //设置光标
	 if (m_bMouseOnFace==true)
	 {
		 SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		 return true;
	 }

	 return __super::OnSetCursor(pWnd,nHitTest,uMessage);
 }


 
 