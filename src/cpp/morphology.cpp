// morphology.cpp
// member class functions definitions
#include <iostream>
#include <stdlib.h>
// Qt libraries for JPEG image manipulation
#include <QtGui/QImage>
#include <QColor>
// Morphology class definition
#include "morphology.h"

#define DEBUG

using namespace std;

// constructor
Morphology::Morphology(QImage imgIn, Image2D *mask) {
   imageOriginal = imgIn;
   this->mask    = mask;
   imageBinary   = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
   imageDilated  = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
   imageEroded   = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
}  // end Morphology constructor

// destructor
Morphology::~Morphology() {
   // empty
}  // end ~Morphology destructor

// performs the binarization of an image
void Morphology::setBinaryImage() {
   imageBinarization();
}  // end function setBinaryImage

// returns the binary image
QImage Morphology::getBinaryImage() const {
   return imageBinary;
}  // end function getBinaryImage

// performs the dilation of an image
void Morphology::setDilatedImage() {
   imageDilation();
}  // end function setDilatedImage

// returns the dilated image
QImage Morphology::getDilatedImage() const {
   return imageDilated;
}  // end function getDilatedImage

// performs the erosion of an image
void Morphology::setErodedImage() {
   imageErosion();
}  // end function setErodedImage

// returns the eroded image
QImage Morphology::getErodedImage() const {
   return imageEroded;
}  // end function getErodedImage

// performs the binarization algorithm
int Morphology::imageBinarization() {
   QRgb value; // rgb value

   // threshold
   for (int x = 0; x < imageBinary.width(); ++x) {
      for (int y = 0; y < imageBinary.height(); ++y) {
         QColor currentPixel(imageOriginal.pixel(x, y));  // current pixel
         // color filter TODO: remove the hard-code filter
         if (currentPixel.blue() > 127) { // threshold value to umbralize 
                                          // TODO: remove the hard-code
            value = qRgb(255, 255, 255);  // white
            imageBinary.setPixel(x, y, value);
         }  // end if
         else {
            value = qRgb(0, 0, 0);        // black
            imageBinary.setPixel(x, y, value);
         }  // end else
      }  // end for
   }  // end for

#ifdef DEBUG
   imageBinary.save("../../tests/binary.jpg", "jpg");
#endif

   return 0;   // success
}  // end function imageBinarization

// performs the dilation algorithm
int Morphology::imageDilation() {
   QRgb val00; // rgb value 
   QRgb val01; // rgb value
   QRgb val02; // rgb value
   QRgb val10; // rgb value
   QRgb val11; // rgb value
   QRgb val12; // rgb value
   QRgb val20; // rgb value
   QRgb val21; // rgb value
   QRgb val22; // rgb value
   
   // dilation algorithm
   for (int i = 0; i < imageOriginal.width() - mask->nRows + 1; ++i) {
      for (int j = 0; j < imageOriginal.height() - mask->nCols + 1; ++j) {
         // verify if the center of the mask is contained in the 'original' image
         if (imageBinary.pixel(i+1, j+1) == mask->pixel[1][1]) {
            for (int m = 0 + i; m < 1 + i; ++m) {
               for (int n = 0 + j; n < 1 + j; ++n) {
                  // applying the mask
                  val00 = imageBinary.pixel(m+0, n+0) | mask->pixel[0][0];
                  val01 = imageBinary.pixel(m+0, n+1) | mask->pixel[0][1];
                  val02 = imageBinary.pixel(m+0, n+2) | mask->pixel[0][2];

                  val10 = imageBinary.pixel(m+1, n+0) | mask->pixel[1][0];
                  val11 = mask->pixel[1][1]; // center of the mask
                  val12 = imageBinary.pixel(m+1, n+2) | mask->pixel[1][2];

                  val20 = imageBinary.pixel(m+2, n+0) | mask->pixel[2][0];
                  val21 = imageBinary.pixel(m+2, n+1) | mask->pixel[2][1];
                  val22 = imageBinary.pixel(m+2, n+2) | mask->pixel[2][2];

                  // drawing the dilated image
                  imageDilated.setPixel(m+0, n+0, val00);
                  imageDilated.setPixel(m+0, n+1, val01);
                  imageDilated.setPixel(m+0, n+2, val02);

                  imageDilated.setPixel(m+1, n+0, val10);
                  imageDilated.setPixel(m+1, n+1, val11);
                  imageDilated.setPixel(m+1, n+2, val12);
                  
                  imageDilated.setPixel(m+2, n+0, val20);
                  imageDilated.setPixel(m+2, n+1, val21);
                  imageDilated.setPixel(m+2, n+2, val22);
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for

#ifdef DEBUG
   // write the image into to a file disk
//   if (level == 1) { // write the image (default)
      imageDilated.save("../../tests/dilated.jpg", "jpg");
//   }  // end if
//   else  // the function was called from an upper level process
//      ;// do nothing
//   cout << "## level: " << level << endl;
//   cout << "## ending: dilation" << endl;
#endif

   return 0;   // success
}  // end function imageDilation

// performs the erosion algorithm
int Morphology::imageErosion() {
   QRgb val00; // rgb value 
   QRgb val01; // rgb value
   QRgb val02; // rgb value
   QRgb val10; // rgb value
   QRgb val11; // rgb value
   QRgb val12; // rgb value
   QRgb val20; // rgb value
   QRgb val21; // rgb value
   QRgb val22; // rgb value
   
   // dilation algorithm
   for (int i = 0; i < imageOriginal.width() - mask->nRows + 1; ++i) {
      for (int j = 0; j < imageOriginal.height() - mask->nCols + 1; ++j) {
         // TODO: find a way, if any, to make generic the decision mechanism considering any mask
         // mask 1
         if ((imageBinary.pixel(i+0,j+0) == mask->pixel[0][0]) && 
             (imageBinary.pixel(i+0,j+1) == mask->pixel[0][1]) && 
             (imageBinary.pixel(i+0,j+2) == mask->pixel[0][2]) &&

             (imageBinary.pixel(i+1,j+0) == mask->pixel[1][0]) && 
             (imageBinary.pixel(i+1,j+1) == mask->pixel[1][1]) && // center of the mask 
             (imageBinary.pixel(i+1,j+2) == mask->pixel[1][2]) &&

             (imageBinary.pixel(i+2,j+0) == mask->pixel[2][0]) && 
             (imageBinary.pixel(i+2,j+1) == mask->pixel[2][1]) && 
             (imageBinary.pixel(i+2,j+2) == mask->pixel[2][2])) {
         // mask 2
         //if (/*(imageBinary.pixel(i+0,j+0) == mask->pixel[0][0]) && */ 
         //    (imageBinary.pixel(i+0,j+1) == mask->pixel[0][1]) && 
         //    /*(imageBinary.pixel(i+0,j+2) == mask->pixel[0][2]) && */

         //    (imageBinary.pixel(i+1,j+0) == mask->pixel[1][0]) && 
         //    (imageBinary.pixel(i+1,j+1) == mask->pixel[1][1]) && // center of the mask 
         //    (imageBinary.pixel(i+1,j+2) == mask->pixel[1][2]) &&

         //    /*(imageBinary.pixel(i+2,j+0) == mask->pixel[2][0]) && */
         //    (imageBinary.pixel(i+2,j+1) == mask->pixel[2][1]) /* && 
         //    (imageBinary.pixel(i+2,j+2) == mask->pixel[2][2])*/) {

            for (int m = 0 + i; m < 1 + i; ++m) {
               for (int n = 0 + j; n < 1 + j; ++n) {
                  // applying the mask
                  val00 = imageBinary.pixel(m+0, n+0) | !mask->pixel[0][0];
                  val01 = imageBinary.pixel(m+0, n+1) | !mask->pixel[0][1];
                  val02 = imageBinary.pixel(m+0, n+2) | !mask->pixel[0][2];

                  val10 = imageBinary.pixel(m+1, n+0) | !mask->pixel[1][0];
                  val11 = mask->pixel[1][1]; // center of the mask
                  val12 = imageBinary.pixel(m+1, n+2) | !mask->pixel[1][2];

                  val20 = imageBinary.pixel(m+2, n+0) | !mask->pixel[2][0];
                  val21 = imageBinary.pixel(m+2, n+1) | !mask->pixel[2][1];
                  val22 = imageBinary.pixel(m+2, n+2) | !mask->pixel[2][2];

                  // drawing the dilated image
                  imageEroded.setPixel(m+0, n+0, val00);
                  imageEroded.setPixel(m+0, n+1, val01);
                  imageEroded.setPixel(m+0, n+2, val02);

                  imageEroded.setPixel(m+1, n+0, val10);
                  imageEroded.setPixel(m+1, n+1, val11);
                  imageEroded.setPixel(m+1, n+2, val12);
                  
                  imageEroded.setPixel(m+2, n+0, val20);
                  imageEroded.setPixel(m+2, n+1, val21);
                  imageEroded.setPixel(m+2, n+2, val22);
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for

#ifdef DEBUG
   // write the image into to a file disk
//   if (level == 1) { // write the image (default)
      imageEroded.save("../../tests/eroded.jpg", "jpg");
//   }  // end if
//   else  // the function was called from an upper level process
//      ;// do nothing
//   cout << "## level: " << level << endl;
//   cout << "## ending: dilation" << endl;
#endif

   return 0;   // success
}  // end function imageErosion
