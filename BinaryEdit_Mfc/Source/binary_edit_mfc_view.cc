// binary_edit_mfc_view.cc : ビュークラス

#include "stdafx.h"
#include "binary_edit_mfc.h"
#include "binary_edit_mfc_doc.h"
#include "binary_edit_mfc_view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(BinaryEditMfcView, CView)

BEGIN_MESSAGE_MAP(BinaryEditMfcView, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &BinaryEditMfcView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

void BinaryEditMfcView::OnDraw(CDC* /*dc*/)
{
	// 描画は未実装（ウィザード生成の雛形のまま）
	ASSERT_VALID(GetDocument());
}

void BinaryEditMfcView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL BinaryEditMfcView::OnPreparePrinting(CPrintInfo* info)
{
	return DoPreparePrinting(info);
}

void BinaryEditMfcView::OnRButtonUp(UINT /*flags*/, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void BinaryEditMfcView::OnContextMenu(CWnd* /*wnd*/, CPoint point)
{
	the_app.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

#ifdef _DEBUG
BinaryEditMfcDoc* BinaryEditMfcView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(BinaryEditMfcDoc)));
	return static_cast<BinaryEditMfcDoc*>(m_pDocument);
}
#endif
