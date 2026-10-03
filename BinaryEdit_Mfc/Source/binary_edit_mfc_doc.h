// binary_edit_mfc_doc.h : ドキュメントクラス

#ifndef BINARYEDIT_MFC_SOURCE_BINARY_EDIT_MFC_DOC_H_
#define BINARYEDIT_MFC_SOURCE_BINARY_EDIT_MFC_DOC_H_

class BinaryEditMfcDoc : public CDocument
{
protected:	// シリアル化からのみ作成する
	BinaryEditMfcDoc() = default;
	DECLARE_DYNCREATE(BinaryEditMfcDoc)

public:
	virtual void Serialize(CArchive& ar) override;
};

#endif  // BINARYEDIT_MFC_SOURCE_BINARY_EDIT_MFC_DOC_H_
