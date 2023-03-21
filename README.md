


*** FUNCTIONS ***

1. Add intensity: add
Increase the intensity within an ROI for a gray-level image.

2. Binarization: binarize
Binarize the pixels within an ROI with the threshold.

3. Scaling: scale
Reduce or expand the height and width with two scale factors within an ROI.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.

4. Area of Interest (AOI): air
Create the desired AOI inside the desired ROI
Increase brightness/intensity of pixels further away from the AOI

5. Brightening Colors: color
Increase intensity of RGB values with desired constants

6. Color Visualization: visualize
Choose a threshold and intensity to which the pixels are measured against
Change to red if pixel values are below the threshold
Change to green if pixel values are above the threshold



*** PARAMETERS FILE ***

There are for parameters:
1. The input file name;
2. The output file name;
3. The number of ROIs; 

NOTE: repeat the following for each number of ROIs 

4. The x-coordinate of the top left pixel for the ROI 		(XL);
5. The y-coordinate of the top left pixel for the ROI		(YL);
6. The x-coordinate of the bottom right pixel for the ROI 	(XR);
7. The y-coordinate of the bottom right pixel for the ROI	(YR);
8. The name of the filter. Use "add", "binarize", "scale", "aoi", "color", and "visualize" for your filters;
9. The value for adding intensity (for "add" and "visualize"), the threshold value (for "binarize"), the scaling factor (for "scale"), the constant to increase the brightness of RED (for "color");
10. The threshold value (for visualize), the constant to increase the brightness of GREEN (for "color");
11. The constant to increase the brightness of BLUE (for "color");


*** Run the program: ./iptool parameters.txt
