/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <strings.h>
#include <string.h>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	// variables
	image src, tgt;		// original and target images
	FILE *fp;	
	char str[MAXLEN];
	char outfile[MAXLEN];
	char *pch, *qch;	// represent (x,y) coordinate of top left point
	char *xLen, *yLen, *readChar;	// represent how long the sides of the ROI are
	char *value, *intensity, *threshold, *scaleFactor;	// represent the additional values for addGray, binarize, etc.
	char *Red, *Green, *Blue;	// represent RGB values

	// if the block can't be opened
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}// end if

	// reads each ROI request
	while(fgets(str,MAXLEN,fp) != NULL) {
		// obtains the original and target images 
		pch = strtok(str, " ");
		src.read(pch);
		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		// copying original to the target image 
		tgt.copyImage(src);

		// getting the number of iterations needed
		pch = strtok(NULL, " ");
		int loop = atoi(pch);
		
		// defining map of the original (X, Y)
		int map[3][4] = {0};

		// ROI loop
		for(int i = 0; i < loop; i++) {
			int index = 0;
			//store x and y coordiantes
			pch = strtok(NULL, " ");
			
			// storing top left corner coordinates in the map to be used in the for loop below
			int X1 = atoi(pch);			// first x-coordinate
			map[i][index++] = X1;
			qch = strtok(NULL, " ");
			int Y1 = atoi(qch);			// first y-coordinate
			map[i][index++] = Y1;

			// storing lengths from each coordinate
			xLen = strtok(NULL, " ");
			int X2 = atoi(xLen) + X1;	// second x-coordinate
			map[i][index++] = X2;
			yLen = strtok(NULL, " ");
			int Y2 = atoi(yLen) + Y1;	// second y-coordinate
			map[i][index++] = Y2;

			int doesOverlap = 0;	// binary value to track whether any ROIs overlap

			// borrowed from utility.cpp formatting
			int rows = src.getNumberOfRows();
			int cols = src.getNumberOfColumns();

			// looks for any ROIs that may be overlapping another ROI
			for(int j = 0; j < i; j++) {
				if(X1 > rows || X2 > rows || Y1 > cols || Y2 > cols) {
					doesOverlap = 1;
				}// end if 

				if((map[j][0] <= X1 && X1 <= map[j][2]) || (map[j][0] <= X2 && X2 <= map[j][2])){
					if((map[j][1] <= Y1 && Y1 <= map[j][3]) || (map[j][1] <= Y2 && Y2 <= map[j][3])){
						doesOverlap = 1;
					}// end inner if()
				}// end outer if()
			}// end for() for overlap check

			// prints error message if there is an overlap
			if(doesOverlap) {
				printf("Unfortunately, an ROI is overlapping another. Please try again with different coordinate values!\n");
				break;
			}// end if()

			// reads each entry in parameters.txt
			readChar = strtok(NULL, " ");

			// ADD GRAY: BRIGHTENING THE GRAYSCALE IMAGE
			if (strncasecmp(readChar,"add",MAXLEN)==0) {
				intensity = strtok(NULL, " ");	// intensity
				utility::addGrey(src,tgt,atoi(pch),atoi(qch),atoi(xLen),atoi(yLen),atoi(intensity));
			}// end if()

			// BINARIZATION
			else if (strncasecmp(readChar,"binarize",MAXLEN)==0) {
				threshold = strtok(NULL, " ");	// threshold
				utility::binarize(src,tgt,atoi(pch),atoi(qch),atoi(xLen),atoi(yLen),atoi(threshold));
			}// end else if()

			// SCALES THE IMAGE BY SHRINKING TO HALF THE SIZE, OR DOUBLING ITS SIZE 
			else if (strncasecmp(readChar,"scale",MAXLEN)==0) {
				scaleFactor = strtok(NULL, " ");	// scale factor
				utility::scale(src,tgt,atoi(pch),atoi(qch),atoi(xLen),atoi(yLen),atof(scaleFactor));
			}// end else if()

/**************************************** NEW FUNCTIONALITIES BESIDES THE ROI FEATURE  ****************************************/	

			// AOI INSIDE ROI WILL BRIGHTEN FURTHER AWAY IT IS FROM THE CENTER OF THE ROI
			else if (strncasecmp(readChar,"aoi",MAXLEN)==0) {
				value = strtok(NULL, " ");
				utility::aoi(src,tgt,atoi(pch),atoi(qch),atoi(xLen),atoi(yLen),atoi(value));
			}// end else if()

			// BRIGHTENS COLORS/INTENSIFIES RGB VALUES OF PPM FILE
			else if (strncasecmp(readChar,"color",MAXLEN)==0) {
				Red = strtok(NULL, " "); 	// user-defined value to increase brightness of red 
				Green = strtok(NULL, " ");	// user-defined value to increase brightness of green 
				Blue = strtok(NULL, " ");	// user-defined value to increase brightness of blue 
				utility::color(src,tgt,atoi(pch),atoi(qch),atoi(xLen),atoi(yLen),atoi(Red),atoi(Green),atoi(Blue));
			}// end else if()

			// COLOR VISUALIZATION: EITHER RED OR GREEN IN THE ROI DEPENDING ON CLOSENESS OF INTENSITY TO THRESHOLD
			else if (strncasecmp(readChar,"visualize",MAXLEN)==0) {
				intensity = strtok(NULL, " ");
				threshold = strtok(NULL, " ");
				utility::visualize(src,tgt,atoi(pch),atoi(qch),atoi(xLen),atoi(yLen),atoi(intensity),atoi(threshold));
			}// end else if()

			// IF THE FUNCTION WRITTEN IN PARAMETERS.TXT IS NOT FOUND
			else {
				printf("No function: %s\n", readChar);
				continue;
			}// end else()

		}// end for() for the ROIs

		tgt.save(outfile);
	}// end while()

	fclose(fp);
	return 0;

}// end main() 