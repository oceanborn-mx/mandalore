// morphoTest.cpp
//
#include <iostream>
#include <stdlib.h>
#include <QtGui/QImage> // Qt library, needed for jpg image handling
#include <QColor>
extern "C" {   // include this C library, needed for multithreading
   #include <pthread.h>
}
#include "morphology.h"
#define DEBUG

using namespace std;

// prototypes
Image2D* setMemoryAllocation(Image2D*, size_t, size_t);
int freeMemory(Image2D*);

int main() {
   // loading input jpg image
   QImage inImage = QImage("../../tests/tux.jpg");

    if(inImage.isNull())
        cout << "## image is null" << endl;
    else
        cout << "## image loaded" << endl;

#ifdef DEBUG
   cout << "## image width: " << inImage.width() << endl;
   cout << "## image height: " << inImage.height() << endl;
   cout << "## pixel(110, 110): " << hex << (inImage.pixel(110, 110) & 0x0000FFFF) << endl;
#endif

   // mask
   Image2D *mask; // mask (structuring element)

   mask = setMemoryAllocation(mask, 3, 3);
   //unsigned int aux3[3][3] = {{0xFF000000, 0xFFFFFFFF, 0xFF000000},
   //                           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 
   //                           {0xFF000000, 0xFFFFFFFF, 0xFF000000}};
   unsigned int aux3[3][3] = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                              {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 
                              {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
   //unsigned int aux3[3][3] = {{0xFF000000, 0xFF000000, 0xFF000000},
   //                           {0xFF000000, 0xFF000000, 0xFF000000}, 
   //                           {0xFF000000, 0xFF000000, 0xFF000000}};
   for (size_t i = 0; i < mask->nRows; ++i) {
      for (size_t j = 0; j < mask->nCols; ++j) {
         mask->pixel[i][j] = aux3[i][j];
      }  // end for
   }  // end for

   // morphological transformations
   Morphology morphoImage = Morphology(inImage, mask);

   // binary image
   QImage imageBin = QImage(inImage.width(), inImage.height(), QImage::Format_RGB32);
   
   morphoImage.imageBinarization(inImage);
   imageBin = morphoImage.getBinaryImage();

   morphoImage.imageGradDilEro(imageBin);
   morphoImage.imageGradDilOri(imageBin);
   morphoImage.imageDilation(imageBin);
   morphoImage.imageErosion(imageBin);
   morphoImage.imageOpening(imageBin);
   morphoImage.imageClosing(imageBin);

   // release memory
   freeMemory(mask);
   
   cout << "## end of main" << endl;
}  // end main

// set dynamic memory allocation
Image2D* setMemoryAllocation(Image2D* blockImage, size_t rows, size_t cols) {
   
   // dynamic memory allocation
   blockImage = (Image2D*)calloc(1, sizeof(Image2D));

   // sizing array 2D
   blockImage->nRows = rows;
   blockImage->nCols = cols;

   // dynamic memory allocation
   blockImage->pixel = (unsigned int**)calloc(blockImage->nRows, sizeof(unsigned int*));

   for (size_t k = 0; k < blockImage->nRows; ++k) {
      blockImage->pixel[k] = (unsigned int*)calloc(blockImage->nCols, sizeof(unsigned int));
   }  // end for

   return blockImage;
}  // end function setMemoryAllocation

// release memory
int freeMemory(Image2D* blockImage) {

   // freeing memory allocated
   for (size_t k = 0; k < blockImage->nRows; ++k)
      free(blockImage->pixel[k]);
   free(blockImage->pixel);
   free(blockImage);

   return 0;   // success
}  // end function freeMemory
