#include "f.hpp"
#include <iostream>
#include <cmath>
const int BPP = 4;
void f( unsigned char *pBuffer, int W, int H, float A, float B, float C, float D, float S ) {
	int id_x, id_y;
	float dx, x = -2, y;
	y = A*x*x*x + B*x*x + C*x + D;
	if( y <= 4 && y >= -4 ) {
		id_y = (int)( (y + 4)/8 * H );	// FISTP
		pBuffer[BPP * (W * id_y)] = 255;
		pBuffer[BPP * (W * id_y) + 1] = 0;
		pBuffer[BPP * (W * id_y) + 2] = 0;
	}
	while( true ) {
		dx = S / sqrt( (pow( (3*A*x*x + 2*B*x + C), 2 ) + 1) );		// FSTP
		x += dx;
		if( x >= 2 ) return;
		y = A*x*x*x + B*x*x + C*x + D;
		if( y <= 4 && y >= -4 ) {
			id_x = (int)( (x + 2)/4 * W );
			id_y = (int)( (y + 4)/8 * H );	// FISTP
			// std::cout << dx << "   " << x << "   " << id_x << "   " << y << std::endl;
			pBuffer[BPP * (W * id_y + id_x)] = 255;
			pBuffer[BPP * (W * id_y + id_x) + 1] = 0;
			pBuffer[BPP * (W * id_y + id_x) + 2] = 0;
		}
	}
}