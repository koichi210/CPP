
// BinaryEdit_MfcView.h : CBinaryEdit_MfcView クラスのインターフェイス
//

#pragma once


class CBinaryEdit_MfcView : public CView
{
protected: // シリアル化からのみ作成します。
	CBinaryEdit_MfcView();
	DECLARE_DYNCREATE(CBinaryEdit_MfcView)

// 属性
public:
	CBinaryEdit_MfcDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CBinaryEdit_MfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // BinaryEdit_MfcView.cpp のデバッグ バージョン
inline CBinaryEdit_MfcDoc* CBinaryEdit_MfcView::GetDocument() const
   { return reinterpret_cast<CBinaryEdit_MfcDoc*>(m_pDocument); }
#endif

