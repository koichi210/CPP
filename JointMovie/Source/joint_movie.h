// JointMovie.h : アプリケーションクラス

#ifndef JOINTMOVIE_SOURCE_JOINT_MOVIE_H_
#define JOINTMOVIE_SOURCE_JOINT_MOVIE_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class JointMovieApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // JOINTMOVIE_SOURCE_JOINT_MOVIE_H_
