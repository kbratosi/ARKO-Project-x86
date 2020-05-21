#include "f.hpp"
#include <iostream>
const int BPP = 4;

void f( unsigned char *pBuffer, int W, int H, int A, int B, int C, int D, int S ) {
	for( int i = 0; i < H; ++i ) {
		for( int j = 0; j < W; ++j ) {
			pBuffer[4*i*W + 4*j] = 255;
			pBuffer[4*i*W + 4*j + 1] = 255;
			pBuffer[4*i*W + 4*j + 2] = 255;
		}
	}
	for( int j = 0; j < W; ++j ) {
		pBuffer[4*W*(H/2) + 4*j] = 0;
		pBuffer[4*W*(H/2) + 4*j + 1] = 0;
		pBuffer[4*W*(H/2) + 4*j + 2] = 0;
		pBuffer[4*W*j + 4*(W/2)] = 0;
		pBuffer[4*W*j + 4*(W/2) + 1] = 0;
		pBuffer[4*W*j + 4*(W/2) + 2] = 0;
	}
	double x = 0;
	double y;
	int row, column, mem_x = -1, mem_y, delta_x2, delta_y2;
	//
	for ( int i = 0; i < W; ++i ) {
		x = 4 * (double) i / W - 2;
		y = A*x*x*x + B*x*x + C*x + D;
		if( y <= 4 && y >= -4 ) {
			row = static_cast<int>( ( (y + 4) / 8 ) * H );
			delta_x2 = (i - mem_x)*(i - mem_x);
			delta_y2 = (row - mem_y)*(row - mem_y);
			if( mem_x == -1 || (delta_x2 + delta_y2 > S) ) {
				column = i - 1;
				pBuffer[BPP * (W * row + column) + 0] = 255;
				pBuffer[BPP * (W * row + column) + 1] = 0;
				pBuffer[BPP * (W * row + column) + 2] = 0;
				pBuffer[BPP * (W * row + column) + 3] = 0;
				//
				mem_x = column;
				mem_y = row;
			}
		}
	}
}