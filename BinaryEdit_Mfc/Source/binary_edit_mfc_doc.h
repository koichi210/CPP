// binary_edit_mfc_doc.h : ドキュメントクラス

#pragma once

class BinaryEditMfcDoc : public CDocument
{
protected:	// シリアル化からのみ作成する
	BinaryEditMfcDoc() = default;
	DECLARE_DYNCREATE(BinaryEditMfcDoc)

public:
	virtual void Serialize(CArchive& ar) override;
};
