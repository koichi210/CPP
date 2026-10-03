// binary_edit_mfc_doc.h : ドキュメントクラス

#pragma once

class CBinaryEdit_MfcDoc : public CDocument
{
protected:	// シリアル化からのみ作成する
	CBinaryEdit_MfcDoc() = default;
	DECLARE_DYNCREATE(CBinaryEdit_MfcDoc)

public:
	virtual void Serialize(CArchive& ar) override;
};
