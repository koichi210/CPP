#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

GLubyte* bits;
GLuint width, height;

void disp(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(-1, -1);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, bits);
	glFlush();
}

int main(int argc, char** argv) {
	unsigned int i, j, index = 0;
	width = 500;
	height = 300;
	bits = (GLubyte* )malloc(3 * (width * height));

	for (i = 0; i < height; i++) {
		int r = (i * 0xFF) / height;
		for (j = 0; j < width; j++) {
			bits[index++] = r;
			bits[index++] = ((j * 0xFF) / width);
			bits[index++] = ~r;
		}
	}

	glutInit(&argc, argv);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("色空間");
	glutDisplayFunc(disp);

	glutMainLoop();
	free(bits);

	return 0;
}