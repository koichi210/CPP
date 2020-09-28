﻿// nugetでインストール
//   nupengl：OpenGLのCG描画におけるコア機能のみ
//   glm    ：OpenGLの行列関連ライブラリ

// プロジェクトのリンカーに、追加の依存ファイルとして下記を設定
//  （OpenGL本体はwindowsOSに含まれている）
//    opengl32.lib


#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

void dispMonocrhome(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glVertex2f(0, 0);
	glVertex2f(-1, 0.9);
	glVertex2f(1, 0.9);

	glVertex2f(0, 0);
	glVertex2f(-1, -0.9);
	glVertex2f(1, -0.9);

	glEnd();
	glFlush();
}

void dispColor(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glColor3ub(0xFF, 0, 0);
	glVertex2f(0, 0);
	glColor3f(0, 0, 1);
	glVertex2f(-1, 0.9);
	glVertex2f(1, 0.9);

	glColor3i(2147483647, 0, 0);
	glVertex2f(0, 0);
	glColor3b(0, 127, 0);
	glVertex2f(-1, -0.9);
	glVertex2f(1, -0.9);

	glEnd();
	glFlush();
}

void dispTriangle(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2f(-0.9, -0.9);
	glColor3f(0, 1, 0);
	glVertex2f(0, 0.9);
	glColor3f(0, 0, 1);
	glVertex2f(0.9, -0.9);
	glEnd();

	glFlush();
}

void timer(int value) {
	glRotatef(2, 0.5, 1, 0.25);
	glutPostRedisplay();
	glutTimerFunc(50, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutCreateWindow("図形描画");
	glutDisplayFunc(dispTriangle);
	glutTimerFunc(100, timer, 0);
	glutMainLoop();
	return 0;
}