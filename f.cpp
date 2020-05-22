#include "f.hpp"
#include <iostream>
#include <cmath>

const int BPP = 4;

void f( unsigned char *pBuffer, int W, int H, float A, float B, float C, float D, float S ) {
	bool out;
	int id_x = 0, id_y, dx = -1;
	float x = -4, y, step;
	step = 8.0/W;
	std::cout << "step: " << step << std::endl;
	while( dx < 0 && !out ) {
		y = A*x*x*x + B*x*x + C*x + D;
		std::cout << "y = " << y << std::endl;
		if( y > 4 || y < -4 ) {
			++id_x;
			x += step;
		}
		else {
			dx = 0;
			id_y = (int)( (y + 4)/8 * H );
			pBuffer[BPP * (W * id_y + id_x)] = 255;
			pBuffer[BPP * (W * id_y + id_x) + 1] = 0;
			pBuffer[BPP * (W * id_y + id_x) + 2] = 0;
			pBuffer[BPP * (W * id_y + id_x) + 3] = 0;
		}
		if ( x >= 4)
			out = true;
	}
	while( !out ) {
		dx = round( sqrt( S / (pow( (3*A*x*x + 2*B*x + C), 2 ) + 1) ) );
		std::cout << "dx = " << dx << std::endl;
		if( 0 == dx )
			dx = 1;

		id_x += dx;
		x += dx*step;
		if( x >= 4 )
			out = true;
		else {
			y = A*x*x*x + B*x*x + C*x + D;
			std::cout << "x = " << x << ", y = " << y << std::endl;
			if( y <= 4 && y >= -4 ) {
				id_y = (int)( (y + 4)/8 * H );
				pBuffer[BPP * (W * id_y + id_x)] = 255;
				pBuffer[BPP * (W * id_y + id_x) + 1] = 0;
				pBuffer[BPP * (W * id_y + id_x) + 2] = 0;
				pBuffer[BPP * (W * id_y + id_x) + 3] = 0;
			}
		}
	}
}