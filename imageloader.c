/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE* fp=fopen(filename,"r");
	if(!fp) return NULL;
	char format[100];int scale;
	Image* line=(Image*) malloc(sizeof(Image));
	if(!line){
		fclose(fp);
		return NULL;
	}
	fscanf(fp,"%s%u%u%d",format,&line->cols,&line->rows,&scale);
	line->image=(Color**)malloc(sizeof(Color*)*line->rows*line->cols);
	if(!line->image){
		fclose(fp);
		free(line);
		return NULL;
	}
	for(int i=0;i<line->rows*line->cols;++i){
	line->image[i]=(Color*)malloc(sizeof(Color));
	if(!line->image[i]){
		fclose(fp);
		for(int j=0;j<i;++j) free(line->image[j]);
		free(line->image);
		free(line);
		return NULL;
	}
	fscanf(fp,"%hhu%hhu%hhu",&line->image[i]->R,&line->image[i]->G,&line->image[i]->B);
	}
	fclose(fp);
	return line;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("%s\n%d %d\n255\n","P3",image->cols,image->rows);
	Color** tint=image->image;
	for(int i=0;i<image->rows;++i){
		for(int j=0;j<image->cols-1;++j){
			printf("%3hhu %3hhu %3hhu   ",(*tint)->R,(*tint)->G,(*tint)->B);
			tint++;
		}
		printf("%3hhu %3hhu %3hhu\n",(*tint)->R,(*tint)->G,(*tint)->B);
		tint++;
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for(int i=0;i<image->rows*image->cols;i++) free(image->image[i]);
	free(image->image);
	free(image);
}