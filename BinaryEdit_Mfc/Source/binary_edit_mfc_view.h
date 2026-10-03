// binary_edit_mfc_view.h : ビュークラス

#ifndef BINARYEDIT_MFC_SOURCE_BINARY_EDIT_MFC_VIEW_H_
#define BINARYEDIT_MFC_SOURCE_BINARY_EDIT_MFC_VIEW_H_

class BinaryEditMfcView : public CView
{
protected:	// シリアル化からのみ作成する
	BinaryEditMfcView() = default;
	DECLARE_DYNCREATE(BinaryEditMfcView)

public:
	BinaryEditMfcDoc* GetDocument() const;

	virtual void OnDraw(CDC* dc) override;

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* info) override;

	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT flags, CPoint point);
	afx_msg void OnContextMenu(CWnd* wnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// デバッグ版は binary_edit_mfc_view.cc で型チェック付き
inline BinaryEditMfcDoc* BinaryEditMfcView::GetDocument() const
{
	return reinterpret_cast<BinaryEditMfcDoc*>(m_pDocument);
}
#endif

#endif  // BINARYEDIT_MFC_SOURCE_BINARY_EDIT_MFC_VIEW_H_
