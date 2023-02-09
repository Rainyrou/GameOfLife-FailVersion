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

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	Color* newone=(Color*)malloc(sizeof(Color));
	if(!newone) return NULL;
	int count=0;
	if(row<0||row>=image->rows||col<0||col>=image->cols) return NULL;
	Color** cur=image->image;
	cur+=(image->cols*row+col);
	for(int i=row-1;i<=row+1;++i){
		for(int j=col-1;j<=col+1;++j){
			int x=i,y=j;
			if(x<0) x=image->rows-1;
			if(x==image->rows) x=0;
			if(y<0) y=image->cols-1;
			if(y==image->cols) y=0;
			if(!(x==row&&y==col)){
				Color* newpos=*(image->image+x*image->cols+y);
				if(newpos->R==255&&newpos->G==255&&newpos->B==255) ++count;
			}
		}
	}
	int rule1 = (rule & 0x1C) >> 2;
	int rule2 = (rule & 0x3E0) >> 5;
	if((*cur)->R==255&&(*cur)->G==255&&(*cur)->B==255){
		if(count >= rule1 && count <= rule2)  newone->R=newone->G=newone->B=255;
		else                                    newone->R=newone->G=newone->B=0;
	}
	else{
		if(count == (rule & 0x1F))    newone->R=newone->G=newone->B=255;
		else                          newone->R=newone->G=newone->B=0;
	}
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
	for(int j=0;j<newimage->cols;++j){
		for(int i=0;i<newimage->rows;++i){
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
