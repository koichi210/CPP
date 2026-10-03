// binary_edit_mfc_view.h : ビュークラス

#pragma once

class CBinaryEdit_MfcView : public CView
{
protected:	// シリアル化からのみ作成する
	CBinaryEdit_MfcView() = default;
	DECLARE_DYNCREATE(CBinaryEdit_MfcView)

public:
	CBinaryEdit_MfcDoc* GetDocument() const;

	virtual void OnDraw(CDC* pDC) override;

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo) override;

	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// デバッグ版は binary_edit_mfc_view.cc で型チェック付き
inline CBinaryEdit_MfcDoc* CBinaryEdit_MfcView::GetDocument() const
{
	return reinterpret_cast<CBinaryEdit_MfcDoc*>(m_pDocument);
}
#endif
