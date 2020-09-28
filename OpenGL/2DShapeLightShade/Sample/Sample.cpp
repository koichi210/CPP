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

const GLfloat lightPos[] = { 3 , 0 , -2 , 0 };
const GLfloat lightCol[] = { 1 , 0 , 0 , 1 };

void dispPyramid(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glNormal3f(3, 0, -2);
	glVertex3f(0, -0.9, -2);
	glVertex3f(3, -0.9, -7);
	glVertex3f(0, 0.9, -2);

	glNormal3f(-3, 0, -2);
	glVertex3f(0, -0.9, -2);
	glVertex3f(-3, -0.9, -7);
	glVertex3f(0, 0.9, -2);
	glEnd();

	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(500, 300);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutCreateWindow("図形描画");
	glutDisplayFunc(dispPyramid);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(1, -1, -1, 1, 2, 10);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}