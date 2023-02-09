/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	if(row<0||row>=image->rows||col<0||col>=image->cols) return NULL;
	Color** cur=image->image;
	cur+=(image->cols*row+col);
	int mybit=(*cur)->B &1;
	Color* newone=(Color*)malloc(sizeof(Color));
	if(!newone) return NULL;
	newone->B=newone->G=newone->R=mybit*255;
	return newone;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
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
	for(int i=0;i<newimage->cols;++i){
		for(int j=0;j<newimage->rows;++j){
			*tint=evaluateOnePixel(image,i,j);
			++tint;
		}
	}
	return newimage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if(argc!=2) exit(-1);
	Image* image=readData(argv[1]);
	Image* newimage=steganography(image);
	writeData(newimage);
	freeImage(image);
	freeImage(newimage);
	return 0;
}
