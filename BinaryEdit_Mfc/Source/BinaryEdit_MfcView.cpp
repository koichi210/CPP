
// BinaryEdit_MfcView.cpp : CBinaryEdit_MfcView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�T���l�C���A����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
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
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBinaryEdit_MfcView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBinaryEdit_MfcView �R���X�g���N�V����/�f�X�g���N�V����

CBinaryEdit_MfcView::CBinaryEdit_MfcView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

CBinaryEdit_MfcView::~CBinaryEdit_MfcView()
{
}

BOOL CBinaryEdit_MfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// CBinaryEdit_MfcView �`��

void CBinaryEdit_MfcView::OnDraw(CDC* /*pDC*/)
{
	CBinaryEdit_MfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}


// CBinaryEdit_MfcView ���


void CBinaryEdit_MfcView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBinaryEdit_MfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ����̈������
	return DoPreparePrinting(pInfo);
}

void CBinaryEdit_MfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CBinaryEdit_MfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
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


// CBinaryEdit_MfcView �f�f

#ifdef _DEBUG
void CBinaryEdit_MfcView::AssertValid() const
{
	CView::AssertValid();
}

void CBinaryEdit_MfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBinaryEdit_MfcDoc* CBinaryEdit_MfcView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBinaryEdit_MfcDoc)));
	return (CBinaryEdit_MfcDoc*)m_pDocument;
}
#endif //_DEBUG


// CBinaryEdit_MfcView ���b�Z�[�W �n���h���[
