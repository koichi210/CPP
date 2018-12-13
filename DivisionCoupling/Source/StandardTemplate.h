/////////////////////////////////////////////////////////////////////////////
// 共通のモジュールヘッダ
/////////////////////////////////////////////////////////////////////////////

//特定のライブラリを使用する場合、呼び出し側でdefineが必要
#ifndef USE_IMM_H
#define USE_IMM_H	0	// imm.hの使用可否
#endif


#define SUBITEM_IDX		(0)
#define VK_NONE			(-1)
#define STR_MIN_SIZE	(10)
#define splitpath_s		_splitpath_s

//#define splitpath() _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
#define issjiskanji(c)	((0x81 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f) \
		|| (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))


// 構造体定義
typedef struct
{
	int		iCtrlId;			//コントロールID
	int		iStrId;				//文字列リソースID (Caption)
} TABLE;


// グローバル関数
void MyMergePath(CString *path, CString dir, CString file, CString ext);
void MySplitPath(CString csPath, CString *cspDrv, CString *cspDir, CString *cspFile, CString *cspExt);
void MyConnection(CString *csFile1, CString csFile2);
void AppendPath(CString *path, CString string);
void AppendExt(CString *path, CString string);
void ReplaceString(CString oname, CString *nname, LPSTR srch, LPSTR rep, BOOL bDiff);
BOOL Browse(HWND m_hWnd, CString title, CString *path);
BOOL SetDlgItemTextAll(HWND hDlg, const TABLE *pTbl, int iMax);
void han2zen(char *str);
void zen2han(char *str);


/////////////////////////////////////////////////////////////////////////////
// CMyStaticImage class
class CMyStaticImage : public CStatic
{
// Attributes
public:
	int m_idBitmap;

// Implementation
	CMyStaticImage();
	virtual ~CMyStaticImage();
	void SetBitmapId(int id);

protected:
	//{{AFX_MSG(CMyStaticImage)
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CMyRistrictedSBCS class
class CMyRistrictedSBCS : public CEdit
{
// Attributes
public:
	DWORD	m_dwCharKind;
			enum
			{
				F_DEFAULT      = 0, //SBCS 文字全般
				F_DIGIT        = 1, //10進数(0 ～ 9)
				F_DIGIT_SIGN   = 2, //10進数(0 ～ 9)と負号'-'
				F_DECIMAL      = 3, //10進数(0 ～ 9)と小数点'.'
				F_DECIMAL_SIGN = 4, //10進数(0 ～ 9)と小数点'.'と負号'-'
				F_ASCII        = 5, //0x00 ～ 0x7F
			};
	char	*m_pszExceptions;
	BOOL	m_bDisableCopyAndPaste;

// Implementation
public:
	CMyRistrictedSBCS();
	virtual ~CMyRistrictedSBCS();
	void SetCharacterKind(DWORD dwRange);
	void SetExceptionCharacters(char *pszExceptions);
	void DisableCopyAndPaste(BOOL bDisableCopyAndPaste);

protected:
	//{{AFX_MSG(CMyRistrictedSBCS)
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CMyEdit
#define WM_MYEDIT_KILLFOCUS (WM_USER + 121)

class CMyEdit : public CEdit
{
	DECLARE_DYNAMIC(CMyEdit)

// Attributes
public:
	CWnd	* m_Parent;
	BOOL	m_State;
	CString	m_Value;
	CPoint	m_Pos;
	UINT	m_PossibleKind;
	int		m_MaxLength;
	enum
	{
		STATE_NG = 0,
		STATE_OK = 1,
	};
	enum
	{
		USESTRING_ANY         = 0xFF,
		USESTRING_DIGIT       = 0x01,
		USESTRING_ALPHA       = 0x02,
		USESTRING_DIGIT_ALPHA = (USESTRING_DIGIT | USESTRING_ALPHA),
	};

// Implementation
public:
	CMyEdit();
	CMyEdit(CWnd *parent, CPoint position);
	virtual ~CMyEdit();
	BOOL Create(DWORD nKind, UINT m_MaxLength, LPCTSTR str, const RECT &rect, CWnd *pParentWnd);

protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CMyList
#define WM_MYLIST_KILLFOCUS (WM_USER + 120)

class CMyList : public CListBox
{
// Attributes
public:
	int		m_Index;
	CWnd	*m_Parent;

// Implementation
public:
	CMyList();
	CMyList(CWnd * parent);
	virtual ~CMyList();
	BOOL Create(CString *strarray, DWORD num, const RECT &rect, CWnd *pParentWnd);

protected:
	//{{AFX_MSG(CMyList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl
class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

// Attributes
protected:
	CPoint		m_Pos;				// カレントポジション
	CMyList		*m_MyList;			// ポップアップリスト
	CString		*m_ListElements;	// リストの要素
	DWORD		m_NumElements;		// リストの要素数
	DWORD		m_EditKind;			// エディットボックスで使用可能な文字
	UINT		m_MaxLength;		// 入力文字最大数
	int			m_SelItem;			// 選択された列
	int			m_SelSubItem;		// 選択された行
	int			m_CurItem;			// カレント列
	int			m_CurSubItem;		// カレント行
	CMyEdit		*m_MyEdit;			// エディットコントロール
	BOOL		m_SideHeader;		// サイドヘッダ有無
	COLORREF	m_LineColor;		// 仕切りの色

// Implementation
public:
	CMyListCtrl();
	virtual ~CMyListCtrl();
	DWORD SetElements(CString *strarray, DWORD num);
	void SetEditPossible(DWORD nKind);
	void CenterJustifyHeader();
	void UseSideHeader(BOOL state = TRUE);
	COLORREF SetLineColor(COLORREF rgb = 0);
	void SetMaxLength(int nMaxLength);

// Implementation
protected:
	virtual BOOL UseInEditKey(UINT nChar);		// 上書き用（エディット状態にするためのキー入力を選別）
	virtual void CreatePopup(LONG col, CRect rect);	// 上書き用（生成するポップアップを切り分ける）
	void CreatePopupEditBox(CRect &rect);
	void CreatePopupListBox(CRect &rect);
	void CellEdit(LV_HITTESTINFO hittest);

	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual LRESULT OnMyEditKillFocus(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMyListKillFocus(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CIconComboBox class
#define MAX_ICON_NUM	10

typedef struct
{
	UINT		idText;					// リストに表示する文字列
	DWORD		value;					// アイテムの値
	DWORD		dwConstraint;			// アイテムの禁則条件
	UINT		idIcon[MAX_ICON_NUM];	// アイコンのID
	int			idxItem;				// テーブルを前詰めしたとき、Windowsから教えてもらうidxウソになるので。。
} ICONCOMBOBOXITEM;

class CIconComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CIconComboBox)

// Attributes
public:
	SIZE				m_sizeIcon;		//icon size
	int					m_iHeight;		//item height
	ICONCOMBOBOXITEM	*m_pItemList;
	UINT				m_uItemList;
	UINT				m_idxIcon;

// Implementation
public:
	CIconComboBox();
	virtual ~CIconComboBox();
	void SetItemList(ICONCOMBOBOXITEM *pItemList, UINT uItemList, UINT idxIcon = 0, DWORD dwConstraint = 0);

protected:
	//{{AFX_MSG(CMyRistrictedSBCS)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CMyEvent class
class CMyEvent
{
// Implementation
public:
	CMyEvent::CMyEvent();
	void MouseMove(CPoint pt);
	void MouseLButtonDown();
	void MouseLButtonUp();
	void MouseLButtonClick();
	void MouseRButtonDown();
	void MouseRButtonUp();
	void MouseRButtonClick();
	void KeyAction(WORD VirtualKey, BOOL bHold = FALSE);
	void FunctionKeyAction(BYTE VirtualKey, BOOL bHold = FALSE);
	void SendInputKey(WORD VirtualKey, BOOL bHold = FALSE);
};


/////////////////////////////////////////////////////////////////////////////
// CCustomCtrl class
class CCustomCtrl : public CListCtrl
{
// Attributes
protected:
	int m_nMaxColumn;
	int m_nMaxRow;

// Implementation
public :
	CCustomCtrl::CCustomCtrl();
	void SetLabel(int nIdx, int nWidth, CString name);
	void SetMaxColumnNum(int num);
	void SetMaxRowNum(int num);
	void SelectIdx(int nIdx);
	void FillRect(int nIdx, CString name);
	void EnableSelectRowAll();
};

/////////////////////////////////////////////////////////////////////////////
// CStdioFile class
//#define NO_ERROR		(0)		// winerror.hで定義済み
#define FILE1_OPEN_ERROR (-1)
#define FILE2_OPEN_ERROR (-2)

class CMyCompareFile
{
// Attributes
public:
	int m_err;
protected:
	CString m_file1;		// 呼び出し側から渡ってきたファイル名1
	CString m_file2;		// 呼び出し側から渡ってきたファイル名2
	CStdioFile m_cfile1;	// ファイルポインタ1
	CStdioFile m_cfile2;	// ファイルポインタ2

// Implementation
public:
	CMyCompareFile::CMyCompareFile();
	CMyCompareFile::CMyCompareFile(CString m_file1, CString m_file2);
	virtual ~CMyCompareFile();
	BOOL CompareBinary();
	BOOL CompareBinary(CString strFile1, CString strFile2);
protected:
	BOOL Open();
	void Close();
	BOOL IsSameFile();
};
