#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>

class utility
{
    public:
        utility();
        virtual ~utility();
        static std::string intToString(int number);
        static int checkValue(int value);
        // functional modifications
        static void addGrey(image &src, image &tgt, int X1, int Y1, int xLen, int yLen, int value);
        static void binarize(image &src, image &tgt, int X1, int Y1, int xLen, int yLen, int threshold);
        static void scale(image &src, image &tgt, int X1, int Y1, int xLen, int yLen, float ratio);
        // hw 0
        static void brighten(image &src, image &tgt, int X1, int Y1, int threshold, int size);
        // added functionalities 
        static void aoi(image &src, image &tgt, int X1, int Y1, int xLen, int yLen, int value);
        static void color(image &src, image &tgt, int X1, int Y1, int xLen, int yLen, int DR, int DG, int DB);
        static void visualize(image &src, image &tgt, int X1, int Y1, int xLen, int yLen, int intensity, int threshold);
};

#endif