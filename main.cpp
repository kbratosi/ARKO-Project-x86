#include <iostream>
#include <fstream>
#include <GL/glut.h> 	// sudo apt-get -y install freeglut3-dev
#include "f.hpp"

// Image buffer
const int H = 511, W = 511, BPP = 4;
unsigned char *g_pBuffer = 0;

// Algorithm params
int g_period = W / 5;

char *openPNGFile( char* fileName ) {
	std::fstream png;
	png.open( fileName, std::fstream::binary )
}

void redraw() {
	f( g_pBuffer, W, H, 1, 1, 1, -2, g_period );
	glDrawPixels( W, H, GL_RGBA, GL_UNSIGNED_BYTE, g_pBuffer );
	glutSwapBuffers();
}

void displayCallback() {
	redraw();
	std::cout << "displayCallback()" << std::endl;
}

void keyboardCallback( unsigned char key, int x, int y ) {
	int period;
	if( 'p' == key ) {
		period = g_period - 10;
	} else if ( 'P' == key ) {
		period = g_period + 10;
	}
	if( ( period != g_period ) && ( period > 0 ) && ( period <= ( W/2 ) ) ) {
		g_period = period;
	
		redraw();

		std::cout << "period(" << g_period << ")" << std::endl;
	}
}

int main( int argc, char *argv[] ) {
	g_pBuffer = new unsigned char[H * W * BPP];
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_SINGLE );
	glutInitWindowSize( W, H );
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( "Example x86 64 " );
	glutDisplayFunc( displayCallback );
	glutKeyboardFunc( keyboardCallback );
	glutMainLoop();
	return 0;
}
