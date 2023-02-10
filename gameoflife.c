/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int circular(int x,int y){
	return (x+y)%y;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	int dx[8]={-1,-1,-1,0,0,1,1,1};
	int dy[8]={-1,0,1,-1,1,-1,0,1};
	Color* newone=(Color*)malloc(sizeof(Color));
	if(!newone) return NULL;
	int LRN=0,LGN=0,LBN=0;
	int Rlive=(*(image->image+image->cols*row+col))->R==255;
	int Glive=(*(image->image+image->cols*row+col))->G==255;
	int Blive=(*(image->image+image->cols*row+col))->B==255;
	for(int i=0;i<8;++i){
		int x=circular(row+dx[i],image->rows);
		int y=circular(row+dy[i],image->cols);
		if((*(image->image+image->cols*x+y))->R==255) LRN++;
		if((*(image->image+image->cols*x+y))->G==255) LGN++;
		if((*(image->image+image->cols*x+y))->B==255) LBN++;
	}
	int codeR=9*Rlive+LRN;
	int codeG=9*Glive+LGN;
	int codeB=9*Blive+LBN;
	newone->R=(rule&(1<<codeR))?255:0;
	newone->G=(rule&(1<<codeG))?255:0;
	newone->B=(rule&(1<<codeB))?255:0;
	return newone;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image* newimage=(Image*)malloc(sizeof(Image));
	if(!newimage) return NULL;
	newimage->rows=image->rows;
	newimage->cols=image->cols;
	newimage->image=(Color**)malloc(sizeof(Color*)*newimage->rows*newimage->cols);
	if(!newimage->image){
		free(newimage);
		return NULL;
	}
	Color** tint=newimage->image;
	for(int i=0;i<newimage->rows;++i){
		for(int j=0;j<newimage->cols;++j){
			*tint=evaluateOneCell(image,i,j,rule);
			++tint;
		}
	}
	return newimage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if(argc!=3) exit(-1);
	Image* image=readData(argv[1]);
	char* ptr=NULL;
	uint32_t rule=strtol(argv[2],&ptr,16);
	Image* newimage=life(image,rule);
	writeData(newimage);
	freeImage(image);
	freeImage(newimage);
	return 0;
}
