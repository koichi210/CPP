// binary_edit_mfc_view.cc : ビュークラス

#include "stdafx.h"
#include "binary_edit_mfc.h"
#include "binary_edit_mfc_doc.h"
#include "binary_edit_mfc_view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBinaryEdit_MfcView, CView)

BEGIN_MESSAGE_MAP(CBinaryEdit_MfcView, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBinaryEdit_MfcView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

void CBinaryEdit_MfcView::OnDraw(CDC* /*pDC*/)
{
	// 描画は未実装（ウィザード生成の雛形のまま）
	ASSERT_VALID(GetDocument());
}

void CBinaryEdit_MfcView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CBinaryEdit_MfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CBinaryEdit_MfcView::OnRButtonUp(UINT /*nFlags*/, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBinaryEdit_MfcView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

#ifdef _DEBUG
CBinaryEdit_MfcDoc* CBinaryEdit_MfcView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBinaryEdit_MfcDoc)));
	return static_cast<CBinaryEdit_MfcDoc*>(m_pDocument);
}
#endif
