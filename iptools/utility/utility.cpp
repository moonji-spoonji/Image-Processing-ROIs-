#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

std::string utility::intToString(int number)
{
   std::stringstream ss;	//create a stringstream
   ss << number;			//add number to the stream
   return ss.str();			//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}


/*---------------------------------------------------------------------------------------------------------------**
***										ADDGREY - FROM HW 0 BUT WITH ROIS										***
*---------------------------------------------------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int XL, int YL, int xLen, int yLen, int value)
{
	for (int i =XL; i < (XL + xLen); i++){
		for (int j = YL; j < (YL + yLen); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}// end inner for()
	}// end outer for()
}// end addGrey() function


/*---------------------------------------------------------------------------------------------------------------**
***										BINARIZE - FROM HW 0 BUT WITH ROIS										***
*---------------------------------------------------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int XL, int YL, int xLen, int yLen, int threshold)
{
	for (int i = XL; i < (XL + xLen); i++) {
		for (int j = YL; j < (YL + yLen); j++) {
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}// end inner for()
	}// end outer for()
}// end binarize() function

/*---------------------------------------------------------------------------------------------------------------**
***											SCALE - FROM HW 0 BUT WITH ROIS										***
*---------------------------------------------------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, int XL, int YL, int xLen, int yLen, float ratio)
{
	// replicate the size of the source image 
	image replica;
	replica.copyImage(src);
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	replica.resize(rows, cols);

	//scale the source down to the intended scale in the replica image
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);

			if (ratio == 2) {
				/* Directly copy the value */
				replica.setPixel(i, j, checkValue(src.getPixel(i2, j2)));
			}// end if()

			if (ratio == 0.5) {
				// averages values of the four px
				int value = src.getPixel(i2, j2) + src.getPixel(i2, j2 + 1) + src.getPixel(i2 + 1, j2) + src.getPixel(i2 + 1, j2 + 1);
				replica.setPixel(i, j, checkValue(value/4));
			}// end if()
		}// end inner for()
	}// end outer for()

	//copies over replica pixels into the target image
	for (int i = 0; i < (xLen); i++)
	{
		for (int j = 0; j < (yLen); j++)
		{	
			// original image gets new pixels put onto it (like a layer)
			int i2 = (int)floor((float)i/ratio) + XL;
			int j2 = (int)floor((float)j/ratio) + YL;

			// if user wants to double (zoom in on) an ROI
			if (ratio == 2) {
				// copies the values from the original image
				tgt.setPixel(i + XL, j + YL, checkValue(src.getPixel(i2, j2)));
			}// end if()

			// if user wants to shrink (zoom out on) an ROI
			if (ratio == 0.5) {
				// takes the average of the pixel values
				if(i + XL/2 >= rows || j + YL/2 >= cols) {
					tgt.setPixel(i + XL, j + YL, 0);
				}// end inner if()
				else 
					tgt.setPixel(i + XL, j + YL, replica.getPixel(i + XL/2, j + YL/2));
			}// end outer if()
		}//end inner for()
	}// end outer for()
}// end scale() function


/*---------------------------------------------------------------------------------------------------------------**
***									PART C: brightening away from the center of the AOI							***
*---------------------------------------------------------------------------------------------------------------**/
void utility::aoi(image &src, image &tgt, int XL, int YL, int xLen, int yLen, int value) 
{
	// x-coordinate of center of the AOI --> (only bc radial dispersion would be too difficult computationally for us at this point)
	double xCenter = (xLen / 2) + XL;

	for (int i = XL; i < (xLen + XL); i++)
	{
		for (int j = YL; j < (yLen + YL); j++)
		{
			// value of the center of the AOI is unchanged, but everything above and below it is brightened 
			if(abs(i - xCenter) < 21) {
				tgt.setPixel(i, j, ((checkValue(src.getPixel(i, j))) + (value * (abs(i - xCenter) / 20.0))));
			}// end if()
		}// end inner for()
	}// end outer for()
}// end aoi() function

/*---------------------------------------------------------------------------------------------------------------**
***										PART D: intensifying RGB colors											***
*---------------------------------------------------------------------------------------------------------------**/
void utility::color(image &src, image &tgt, int XL, int YL, int xLen, int yLen, int DR, int DG, int DB) 
{
	for (int i = XL; i < (xLen + XL); i++)
	{
		for (int j = YL; j < (yLen + YL); j++)
		{
			// intensifies each RGB value within the ROI
			tgt.setPixel(i, j, 0, checkValue(src.getPixel(i, j, 0) + DR)); 
			tgt.setPixel(i, j, 1, checkValue(src.getPixel(i, j, 1) + DG)); 
			tgt.setPixel(i, j, 2, checkValue(src.getPixel(i, j, 2) + DB));
		}// end inner for()
	}// end outer for()
}// end color() function

/*---------------------------------------------------------------------------------------------------------------**
***										PART E: adding color visualization										***
*---------------------------------------------------------------------------------------------------------------**/
void utility::visualize(image &src, image &tgt, int XL, int YL, int xLen, int yLen, int intensity, int threshold) 
{
	// variables for dimensions
	int width = xLen + XL;
	int height = yLen + YL;

	for (int i = XL; i < width; i++)
	{
		for (int j = YL; j < height; j++)
		{
			// variables
			int pixel = src.getPixel(i,j);
			int value = abs(intensity - pixel);

			// if the value is closer to the intensity, make it RED of the same intensity
			if (value < threshold) {
				tgt.setPixel(i, j, RED, checkValue(MAXRGB));
				tgt.setPixel(i, j, GREEN, checkValue(MINRGB));
				tgt.setPixel(i, j, BLUE, checkValue(MINRGB));
			}// end if()
			// if not, make it GREEEEEEEEEN
			else {
				tgt.setPixel(i, j, GREEN, checkValue(MAXRGB));
				tgt.setPixel(i, j, RED, checkValue(MINRGB));
				tgt.setPixel(i, j, BLUE, checkValue(MINRGB));
			}// end else()
		}// end inner for()
	}// end outer for()
}// end visualize() function