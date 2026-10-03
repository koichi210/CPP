// 色空間：縦方向に赤（と補色の青）、横方向に緑を変化させた画像を glDrawPixels で描く

#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <vector>

namespace
{
	constexpr GLsizei kWidth = 500;
	constexpr GLsizei kHeight = 300;

	// RGB 各1バイトのピクセル列（左下から右上へ）
	std::vector<GLubyte> bits;

	void Disp(void) {
		glClear(GL_COLOR_BUFFER_BIT);
		glRasterPos2i(-1, -1);
		glDrawPixels(kWidth, kHeight, GL_RGB, GL_UNSIGNED_BYTE, bits.data());
		glFlush();
	}
}

int main(int argc, char** argv) {
	bits.reserve(3 * kWidth * kHeight);
	for (GLsizei i = 0; i < kHeight; i++) {
		const int r = (i * 0xFF) / kHeight;
		for (GLsizei j = 0; j < kWidth; j++) {
			bits.push_back(static_cast<GLubyte>(r));
			bits.push_back(static_cast<GLubyte>((j * 0xFF) / kWidth));
			bits.push_back(static_cast<GLubyte>(~r));
		}
	}

	glutInit(&argc, argv);
	glutInitWindowSize(kWidth, kHeight);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("色空間");
	glutDisplayFunc(Disp);

	glutMainLoop();

	return 0;
}
