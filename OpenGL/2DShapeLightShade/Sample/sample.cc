// nugetでインストール
//   nupengl：OpenGLのCG描画におけるコア機能のみ
//   glm    ：OpenGLの行列関連ライブラリ

// プロジェクトのリンカーに、追加の依存ファイルとして下記を設定
//  （OpenGL本体はwindowsOSに含まれている）
//    opengl32.lib


#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

const GLfloat kLightPos[] = { 3, 0, -2, 0 };
const GLfloat kLightCol[] = { 1, 0, 0, 1 };

void DispPyramid(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 2 枚の三角形の面を描く（GL_POLYGON だと 6 頂点が 1 枚のねじれた多角形になる）
	glBegin(GL_TRIANGLES);
	glNormal3f(3, 0, -2);
	glVertex3f(0, -0.9f, -2);
	glVertex3f(3, -0.9f, -7);
	glVertex3f(0, 0.9f, -2);

	glNormal3f(-3, 0, -2);
	glVertex3f(0, -0.9f, -2);
	glVertex3f(-3, -0.9f, -7);
	glVertex3f(0, 0.9f, -2);
	glEnd();

	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(500, 300);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);	// GL_DEPTH_TEST を使うので深度バッファも要求する

	glutCreateWindow("図形描画");
	glutDisplayFunc(DispPyramid);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(1, -1, -1, 1, 2, 10);

	glLightfv(GL_LIGHT0, GL_POSITION, kLightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, kLightCol);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}
