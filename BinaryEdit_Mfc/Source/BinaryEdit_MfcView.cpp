
// BinaryEdit_MfcView.cpp : CBinaryEdit_MfcView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、サムネイル、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "BinaryEdit_Mfc.h"
#endif

#include "BinaryEdit_MfcDoc.h"
#include "BinaryEdit_MfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBinaryEdit_MfcView

IMPLEMENT_DYNCREATE(CBinaryEdit_MfcView, CView)

BEGIN_MESSAGE_MAP(CBinaryEdit_MfcView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBinaryEdit_MfcView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBinaryEdit_MfcView コンストラクション/デストラクション

CBinaryEdit_MfcView::CBinaryEdit_MfcView()
{
	// TODO: 構築コードをここに追加します。

}

CBinaryEdit_MfcView::~CBinaryEdit_MfcView()
{
}

BOOL CBinaryEdit_MfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CBinaryEdit_MfcView 描画

void CBinaryEdit_MfcView::OnDraw(CDC* /*pDC*/)
{
	CBinaryEdit_MfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CBinaryEdit_MfcView 印刷


void CBinaryEdit_MfcView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBinaryEdit_MfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CBinaryEdit_MfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CBinaryEdit_MfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CBinaryEdit_MfcView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBinaryEdit_MfcView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBinaryEdit_MfcView 診断

#ifdef _DEBUG
void CBinaryEdit_MfcView::AssertValid() const
{
	CView::AssertValid();
}

void CBinaryEdit_MfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBinaryEdit_MfcDoc* CBinaryEdit_MfcView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBinaryEdit_MfcDoc)));
	return (CBinaryEdit_MfcDoc*)m_pDocument;
}
#endif //_DEBUG


// CBinaryEdit_MfcView メッセージ ハンドラー
