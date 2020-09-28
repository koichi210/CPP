// nugetでインストール
//   nupengl：OpenGLのCG描画におけるコア機能のみ
//   glm    ：OpenGLの行列関連ライブラリ

// プロジェクトのリンカーに、追加の依存ファイルとして下記を設定
//  （OpenGL本体はwindowsOSに含まれている）
//    opengl32.lib


#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

void disp(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	// 描画位置
	glutInitWindowPosition(100, 50);

	// 描画サイズ
	glutInitWindowSize(200, 100);

	// ディスプレイモード（RGBやバッファ）
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	//glutCreateWindow(argv[0]);
	glutCreateWindow("Kitty on your lap");

	// ウィンドウの再描画が必要なときに呼ばれるコールバック
	glutDisplayFunc(disp);

	// Windowを塗り潰す色（RGBA）
	glClearColor(0, 0, 1, 0);

	// イベント処理ループに入る
	glutMainLoop();
	return 0;
}

