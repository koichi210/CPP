// SampleCaptAreaDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MotionCapture.h"
#include "SampleCaptAreaDlg.h"
#include "afxdialogex.h"


// SampleCaptAreaDlg ダイアログ

IMPLEMENT_DYNAMIC(SampleCaptAreaDlg, CDialogEx)

SampleCaptAreaDlg::SampleCaptAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SampleCaptAreaDlg::IDD, pParent)
{
	m_Preview.top = 0;
	m_Preview.left = 0;
	m_Preview.bottom = 0;
	m_Preview.right = 0;
	m_BitmapBpp = 24;
}

SampleCaptAreaDlg::SampleCaptAreaDlg(RECT rt, UINT BitmapBpp, CWnd* pParent /*=NULL*/)
	: CDialogEx(SampleCaptAreaDlg::IDD, pParent)
{
	m_Preview = rt;
	m_BitmapBpp = BitmapBpp;
}

SampleCaptAreaDlg::~SampleCaptAreaDlg()
{
}

void SampleCaptAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(SampleCaptAreaDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
// SampleCaptAreaDlg メッセージ ハンドラー

void SampleCaptAreaDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	PreView();
}

void SampleCaptAreaDlg::GetBitmapInfo()
{
	ZeroMemory(&m_bitmapInfo, sizeof(BITMAPINFO));
	m_bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bitmapInfo.bmiHeader.biWidth = m_Preview.right - m_Preview.left;
	m_bitmapInfo.bmiHeader.biHeight = m_Preview.bottom - m_Preview.top;
	m_bitmapInfo.bmiHeader.biPlanes = 1;
	m_bitmapInfo.bmiHeader.biBitCount = m_BitmapBpp;
	m_bitmapInfo.bmiHeader.biCompression = BI_RGB;	// TODO：BI_JPEGに設定できない

	if ( m_BitmapBpp != 0 )
	{
		m_bitmapInfo.bmiHeader.biSizeImage = m_bitmapInfo.bmiHeader.biHeight * ((3 * m_bitmapInfo.bmiHeader.biWidth + 3) / 4) * 4;
	}
	else
	{
		// This may be set to zero for BI_RGB bitmaps
		// If biCompression is BI_JPEG or BI_PNG, biSizeImage indicates the size of the JPEG or PNG image buffer, respectively.
		m_bitmapInfo.bmiHeader.biSizeImage = 0;
	}
}

void SampleCaptAreaDlg::ScreenCapture()
{
	HDC hMemDC = ::CreateCompatibleDC(NULL);
	LPVOID pvBits;

	GetBitmapInfo();
	HBITMAP hMemBitmap = ::CreateDIBSection(NULL, &m_bitmapInfo, DIB_RGB_COLORS, &pvBits, NULL, 0);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

	HDC dcScreen = ::CreateDC(_T("DISPLAY"), _T("DISPLAY"), _T("DISPLAY"), NULL);

	// キャプチャ
	::BitBlt(hMemDC, 0, 0, m_bitmapInfo.bmiHeader.biWidth, m_bitmapInfo.bmiHeader.biHeight, dcScreen, 0, 0, SRCCOPY);

	//SaveBitmapFile(hMemBitmap,"c:\\Bitmap.bmp");

	::SelectObject(hMemDC, hOldBitmap);
	::DeleteObject(hMemBitmap);
	::DeleteDC(hMemDC);
	::DeleteDC(dcScreen);
}

void SampleCaptAreaDlg::PreView()
{
	CBitmap Bmp;
	CString SamplePath = "c:\\Sample.bmp";

	// Bitmapロード 「実績あるコード」
	if ( 1 ) 
	{
		// ファイルの存在チェック
		if (!PathFileExists(SamplePath))
		{
			MessageBox("ファイルオープンに失敗しました。\n" + SamplePath);
			return;
		}

		// PictureBoxに画像ロード
		Bmp.Attach( (HBITMAP)::LoadImage(
			AfxGetInstanceHandle(),
			SamplePath,
			IMAGE_BITMAP,
			PICTURE_BOX_WIDTH,
			PICTURE_BOX_HEIGHT,
			LR_LOADFROMFILE) );

		CStatic* pictbox = (CStatic*)GetDlgItem(IDPC_SAMPLE);
		pictbox->SetBitmap(Bmp);

		Bmp.Detach();
	}

	// 保存を試みる
	//CImage img;
	//img.Attach((Bmp);      // bmpはCBitmap
	//img.Save(pszName, ImageFormatBMP); // こんな感じじゃダメかな？
}

void SampleCaptArea()
{
	HDC hdc = GetDC(NULL); // get the desktop device context
	HDC hDest = CreateCompatibleDC(hdc); // create a device context to use yourself

	// get the height and width of the screen
	int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);

	// create a bitmap
	HBITMAP hbDesktop = CreateCompatibleBitmap( hdc, width, height);

	// use the previously created device context with the bitmap
	SelectObject(hDest, hbDesktop);

	// copy from the desktop device context to the bitmap device context
	// call this once per 'frame'
	BitBlt(hDest, 0,0, width, height, hdc, 0, 0, SRCCOPY);

	// after the recording is done, release the desktop context you got..
	ReleaseDC(NULL, hdc);

	// ..and delete the context you created
	DeleteDC(hDest);
}

BOOL SampleCaptAreaDlg::WriteBitmap(LPTSTR lpszFileName, int nWidth, int nHeight, LPVOID lpBits)
{
	HANDLE           hFile;
	DWORD            dwResult;
	DWORD            dwSizeImage;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;

	hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	dwSizeImage = nHeight * ((3 * nWidth + 3) / 4) * 4;

	ZeroMemory(&bmfHeader, sizeof(BITMAPFILEHEADER));
	bmfHeader.bfType    = *(LPWORD)"BM";
	bmfHeader.bfSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwSizeImage;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwResult, NULL);

	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth       = nWidth;
	bmiHeader.biHeight      = nHeight;
	bmiHeader.biPlanes      = 1;
	bmiHeader.biBitCount    = 24;
	bmiHeader.biSizeImage   = dwSizeImage;
	bmiHeader.biCompression = BI_RGB;
	
	WriteFile(hFile, &bmiHeader, sizeof(BITMAPINFOHEADER), &dwResult, NULL);

	WriteFile(hFile, lpBits, dwSizeImage, &dwResult, NULL);

	CloseHandle(hFile);
	
	return TRUE;
}

BOOL SaveBitmapFile(HBITMAP bmp,LPSTR filename)
{
    //// BITMAP情報を取得する
    //BITMAP bi;
    //ZeroMemory(&bi,sizeof(bi));
    //if(GetObject(bmp,sizeof(bi),&bi) == 0)return FALSE;

    //// ファイルサイズ計算
    //LONG filesize = bi.bmWidthBytes * bi.bmHeight + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    //LONG bpp = 0;
    //switch(bi.bmBitsPixel){
    //case 2:
    //    filesize += (sizeof(RGBQUAD) * 2);
    //    bpp = 2;
    //    break;
    //case 4:
    //    filesize += (sizeof(RGBQUAD) * 16);
    //    bpp = 16;
    //    break;
    //case 8:
    //    filesize += (sizeof(RGBQUAD) * 256);
    //    bpp = 256;
    //    break;
    //}

    //// ファイルオープン
    //HFILE fp = _lcreat(filename,0);
    //if(fp == HFILE_ERROR)return FALSE;

    //// BITMAPFILEHEADERヘッダー出力
    //BITMAPFILEHEADER fh;
    //ZeroMemory(&fh,sizeof(fh));
    //memcpy(&fh.bfType,"BM",2);
    //fh.bfSize = filesize;
    //fh.bfReserved1 = 0;
    //fh.bfReserved2 = 0;
    //fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bpp;
    //_lwrite(fp,(LPSTR)&fh,sizeof(fh));

    //// BITMAPINFOHEADERヘッダー出力
    //LPBITMAPINFO pbi = (LPBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bpp);
    //ZeroMemory(pbi,sizeof(BITMAPINFOHEADER));
    //pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    //pbi->bmiHeader.biWidth = bi.bmWidth;
    //pbi->bmiHeader.biHeight = bi.bmHeight;
    //pbi->bmiHeader.biPlanes = 1;
    //pbi->bmiHeader.biBitCount = bi.bmBitsPixel;
    //pbi->bmiHeader.biCompression = BI_RGB;
    //if(bpp != 0){
    //    GetDIBColorTable(bmp,0,bpp,pbi->bmiColors);
    //}
    //_lwrite(fp,(LPSTR)pbi,sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bpp);

    //// 画像データを得る
    //LPBYTE bits = (LPBYTE)malloc(bi.bmWidthBytes * bi.bmHeight);
    //HDC dc = GetDC(0);
    //HDC mdc = CreateCompatibleDC(dc);
    //ReleaseDC(0,dc);
    //HBITMAP old = SelectObject(mdc,bmp);
    //int l = GetDIBits(mdc,bmp,0,bi.bmHeight,bits,pbi,DIB_RGB_COLORS);
    //_lwrite(fp,(LPSTR)bits,bi.bmWidthBytes * bi.bmHeight);

    //// 作業領域を開放
    //free(pbi);
    //free(bits);
    //_lclose(fp);

    return TRUE;
}
