// BinaryEdit_MfcDoc.cpp : ドキュメントクラス

#include "stdafx.h"
#include "BinaryEdit_Mfc.h"
#include "BinaryEdit_MfcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBinaryEdit_MfcDoc, CDocument)

void CBinaryEdit_MfcDoc::Serialize(CArchive& /*ar*/)
{
	// バイナリの読み書きは未実装（ウィザード生成の雛形のまま）
}
