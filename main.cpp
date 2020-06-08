#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <GL/glut.h>
#include <iterator>
#include "f.hpp"

// Image buffer
const int BPP = 4;
unsigned char *template_background, *g_pBuffer;
int imageSize, W, H;

// Algorithm params
double g_A = 0,
	   g_B = 0,
	   g_C = 0,
	   g_D = 0,
	   g_S = 0.127;

void refresh() {
	memcpy( g_pBuffer, template_background, imageSize );
}

void openBackground(char* filename) {
	FILE *inputFile;
    inputFile = fopen( filename, "r" );
	if(!inputFile)
	{
		fprintf( stderr, "Cannot open file!\n" );
        return;
	}
    int BM, fileSize, offset, headerSize;
    fread(&BM,2,1,inputFile);
    fread(&fileSize,4,1,inputFile);
    fread(&offset,4,1,inputFile);
    fread(&offset,4,1,inputFile);
	fread(&headerSize,4,1,inputFile);
	fread(&W,4,1,inputFile);
	fread(&H,4,1,inputFile);
    fclose(inputFile);

	unsigned char header[offset];
	imageSize = fileSize - offset;

	template_background = new unsigned char[imageSize];
	g_pBuffer = new unsigned char[imageSize];
	inputFile = fopen( filename,"r" );
	if(!inputFile)
	{
		fprintf( stderr, "Cannot open file!\n" );
        return;
	}
	fread(&header,offset, 1,inputFile);
	fread(template_background, imageSize, 1, inputFile);
	fclose(inputFile);
	refresh();
}

void redraw() {
	refresh();
	f( g_pBuffer, W, H, g_A, g_B, g_C, g_D, g_S );
	glDrawPixels( W, H, GL_RGBA, GL_UNSIGNED_BYTE, g_pBuffer );
	glutSwapBuffers();
}

void displayCallback() {
	redraw();
	std::cout << "displayCallback()" << std::endl;
}

void setNewTitle() {
	std::stringstream ss;
	ss << "A = " << std::fixed << std::setprecision(3) << g_A 
	   << " B = " << g_B
	   << " C = " << g_C 
	   << " D = " << g_D 
	   << ", S = " << g_S;
	std::string params = ss.str();
	char title[params.size() + 1];
	strcpy( title, &params[0] );
	glutSetWindowTitle( title );
}

void keyboardCallback( unsigned char key, int x, int y ) {
	double s;
	if( 'a' == key ) {
		g_A -= 0.005;
	} else if ( 'A' == key ) {
		g_A += 0.005;
	} else if( 'b' == key ) {
		g_B -= 0.005;
	} else if ( 'B' == key ) {
		g_B += 0.005;
	} else if( 'c' == key ) {
		g_C -= 0.005;
	} else if ( 'C' == key ) {
		g_C += 0.005;
	} else if( 'd' == key ) {
		g_D -= 0.005;
	} else if ( 'D' == key ) {
		g_D += 0.005;
	}
	redraw();
	if( 's' == key ) {
		s = g_S - 0.005;
	} else if ( 'S' == key ) {
		s = g_S + 0.005;
	}
	if( ( s != g_S ) && ( s > 0.001 ) && ( s <= 0.252 ) ) {
		g_S = s;
		redraw();
		std::cout << "period(" << g_S << ")" << std::endl;
	}
	setNewTitle();
}

int main( int argc, char *argv[] ) {
	if( argc == 1) {
		std::cout << "Please specify background image file name. Shutting down..." << std::endl;
		return -1;
	}
	openBackground( argv[1] );
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_SINGLE );
	glutInitWindowSize( W, H );
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow("Polynomial graphs");
	setNewTitle();
	glutDisplayFunc( displayCallback );
	glutKeyboardFunc( keyboardCallback );
	glutMainLoop();
	return 0;
}
