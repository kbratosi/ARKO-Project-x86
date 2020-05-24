#include <iostream>
#include <fstream>
#include <GL/glut.h> 	// sudo apt-get -y install freeglut3-dev
#include "f.hpp"

// Image buffer
const int H = 512, W = 256, BPP = 4;
unsigned char *g_pBuffer;

// Algorithm params
float g_period = 0.127;

void refresh() {
	for( int i = 0; i < H; ++i ) {
		for( int j = 0; j < W; ++j ) {
			g_pBuffer[4*i*W + 4*j] = 255;
			g_pBuffer[4*i*W + 4*j + 1] = 255;
			g_pBuffer[4*i*W + 4*j + 2] = 255;
		}
	}
	for( int j = 0; j < W; ++j ) {
		g_pBuffer[4*W*(H/2) + 4*j] = 0;
		g_pBuffer[4*W*(H/2) + 4*j + 1] = 0;
		g_pBuffer[4*W*(H/2) + 4*j + 2] = 0;
	}
	for( int j = 0; j < H; ++j ) {
		g_pBuffer[4*W*j + 4*(W/2)] = 0;
		g_pBuffer[4*W*j + 4*(W/2) + 1] = 0;
		g_pBuffer[4*W*j + 4*(W/2) + 2] = 0;
	}
}

void redraw() {
	refresh();
	std::cout << "f(-2) = " << f( g_pBuffer, W, H, 0, 0, 0, 2, g_period ) << std::endl;
	glDrawPixels( W, H, GL_RGBA, GL_UNSIGNED_BYTE, g_pBuffer );
	glutSwapBuffers();
}

void displayCallback() {
	redraw();
	std::cout << "displayCallback()" << std::endl;
}

void keyboardCallback( unsigned char key, int x, int y ) {
	float period;
	if( 'p' == key ) {
		period = g_period - 0.005;
	} else if ( 'P' == key ) {
		period = g_period + 0.005;
	}
	if( ( period != g_period ) && ( period > 0.001 ) && ( period <= 0.252 ) ) {
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
