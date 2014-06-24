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
   setBinaryImage(imgIn);
   setDilatedImage(imgIn);
   setErodedImage(imgIn);
   setOpenedImage(imgIn);
   setClosedImage(imgIn);
   setGradDilEro(imgIn);
   setGradDilOri(imgIn);
}  // end Morphology constructor

// destructor
Morphology::~Morphology() {
   // empty
}  // end ~Morphology destructor

// set binary image
void Morphology::setBinaryImage(QImage imgIn) {
   imageBinary = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
}  // end function setBinaryImage

// return binary image
QImage Morphology::getBinaryImage() const {
   return imageBinary;
}  // end function getBinaryImage

// set dilated image
void Morphology::setDilatedImage(QImage imgIn) {
   imageDilated = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
}  // end function setDilatedImage

// return dilated image
QImage Morphology::getDilatedImage() const {
   return imageDilated;
}  // end function getDilatedImage

// set eroded image
void Morphology::setErodedImage(QImage imgIn) {
   imageEroded = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
}  // end function setErodedImage

// return eroded image
QImage Morphology::getErodedImage() const {
   return imageEroded;
}  // end function getErodedImage

// set opened image
void Morphology::setOpenedImage(QImage imgIn) {
   imageOpened = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
}  // end function setOpenedImage

// return opened image
QImage Morphology::getOpenedImage() const {
   return imageOpened;
}  // end function getOpenedImage

// set closed image
void Morphology::setClosedImage(QImage imgIn) {
   imageClosed = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
}  // end function setClosedImage

// return closed image
QImage Morphology::getClosedImage() const {
   return imageClosed;
}  // end function getClosedImage

// set gradient: dilation - erosion
void Morphology::setGradDilEro(QImage imgIn) {
   imageGDilEro = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
}  // end function setGradDilEro

// return gradient: dilation - erosion
QImage Morphology::getGradDilEro() const {
   return imageGDilEro;
}  // end function getGradDilEro

// set gradient: dilation - original (binarized)
void Morphology::setGradDilOri(QImage imgIn) {
   imageGDilOri = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
}  // end function setGradDilOri

// return gradient: dilation - original (binarized)
QImage Morphology::getGradDilOri() const {
   return imageGDilOri;
}  // end function getGradDilOri

// perform binarization algorithm
int Morphology::imageBinarization(QImage imgIn) {
   QRgb value; // rgb value

   // threshold
   for (int x = 0; x < imageBinary.width(); ++x) {
      for (int y = 0; y < imageBinary.height(); ++y) {
         QColor currentPixel(imgIn.pixel(x, y));   // current pixel
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

// dilation wrapper
//void Morphology::dilation() {
//   
//   imageBinarization();
//   imgBin = getBinaryImage();
//
//   imageDilation(imgBin);
//}  // end function dilation

// perform dilation algorithm
int Morphology::imageDilation(QImage imgIn, int level) {
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
   for (int i = 0; i < imageDilated.width() - mask->nRows + 1; ++i) {
      for (int j = 0; j < imageDilated.height() - mask->nCols + 1; ++j) {
         // verify if the center of the mask is contained in the 'original' image
         if (imgIn.pixel(i + 1, j + 1) == mask->pixel[1][1]) {
            for (int m = i; m < 1 + i; ++m) {
               for (int n = j; n < 1 + j; ++n) {
                  // applying the mask
                  val00 = imgIn.pixel(m + 0, n + 0) | mask->pixel[0][0];
                  val01 = imgIn.pixel(m + 0, n + 1) | mask->pixel[0][1];
                  val02 = imgIn.pixel(m + 0, n + 2) | mask->pixel[0][2];

                  val10 = imgIn.pixel(m + 1, n + 0) | mask->pixel[1][0];
                  val11 = mask->pixel[1][1]; // center of the mask
                  val12 = imgIn.pixel(m + 1, n + 2) | mask->pixel[1][2];

                  val20 = imgIn.pixel(m + 2, n + 0) | mask->pixel[2][0];
                  val21 = imgIn.pixel(m + 2, n + 1) | mask->pixel[2][1];
                  val22 = imgIn.pixel(m + 2, n + 2) | mask->pixel[2][2];

                  // drawing the dilated image
                  imageDilated.setPixel(m + 0, n + 0, val00);
                  imageDilated.setPixel(m + 0, n + 1, val01);
                  imageDilated.setPixel(m + 0, n + 2, val02);

                  imageDilated.setPixel(m + 1, n + 0, val10);
                  imageDilated.setPixel(m + 1, n + 1, val11);
                  imageDilated.setPixel(m + 1, n + 2, val12);
                  
                  imageDilated.setPixel(m + 2, n + 0, val20);
                  imageDilated.setPixel(m + 2, n + 1, val21);
                  imageDilated.setPixel(m + 2, n + 2, val22);
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for

#ifdef DEBUG
   // write the image into to a file disk
   if (level == 1) { // write the image (default)
    imageDilated.save("../../tests/dilated.jpg", "jpg");
   }  // end if
   else  // the function was called from an upper level process
      ;// do nothing
   cout << "## level: " << level << endl;
   cout << "## ending: dilation" << endl;
#endif

   return 0;   // success
}  // end function imageDilation

// perform erosion algorithm
int Morphology::imageErosion(QImage imgIn, int level) {
   QRgb val00; // rgb value 
   QRgb val01; // rgb value
   QRgb val02; // rgb value
   QRgb val10; // rgb value
   QRgb val11; // rgb value
   QRgb val12; // rgb value
   QRgb val20; // rgb value
   QRgb val21; // rgb value
   QRgb val22; // rgb value
   
   // erosion algorithm
   for (int i = 0; i < imageEroded.width() - mask->nRows + 1; ++i) {
      for (int j = 0; j < imageEroded.height() - mask->nCols + 1; ++j) {
         // TODO: find a way, if any, to make generic the decision mechanism considering any mask
         // mask 1
         if ((imgIn.pixel(i + 0, j + 0) == mask->pixel[0][0]) && 
             (imgIn.pixel(i + 0, j + 1) == mask->pixel[0][1]) && 
             (imgIn.pixel(i + 0, j + 2) == mask->pixel[0][2]) &&

             (imgIn.pixel(i + 1, j + 0) == mask->pixel[1][0]) && 
             (imgIn.pixel(i + 1, j + 1) == mask->pixel[1][1]) && // center of the mask 
             (imgIn.pixel(i + 1, j + 2) == mask->pixel[1][2]) &&

             (imgIn.pixel(i + 2, j + 0) == mask->pixel[2][0]) && 
             (imgIn.pixel(i + 2, j + 1) == mask->pixel[2][1]) && 
             (imgIn.pixel(i + 2, j + 2) == mask->pixel[2][2])) {
         // mask 2
         //if (/*(imgIn.pixel(i + 0, j + 0) == mask->pixel[0][0]) && */ 
         //    (imgIn.pixel(i + 0, j + 1) == mask->pixel[0][1]) && 
         //  /*(imgIn.pixel(i + 0, j + 2) == mask->pixel[0][2]) && */

         //    (imgIn.pixel(i + 1, j + 0) == mask->pixel[1][0]) && 
         //    (imgIn.pixel(i + 1, j + 1) == mask->pixel[1][1]) && // center of the mask 
         //    (imgIn.pixel(i + 1, j + 2) == mask->pixel[1][2]) &&

         //  /*(imgIn.pixel(i + 2, j + 0) == mask->pixel[2][0]) && */
         //    (imgIn.pixel(i + 2, j + 1) == mask->pixel[2][1]) /* && 
         //    (imgIn.pixel(i + 2, j + 2) == mask->pixel[2][2])*/) {
            for (int m = i; m < 1 + i; ++m) {
               for (int n = j; n < 1 + j; ++n) {
                  // applying the mask
                  val00 = imgIn.pixel(m + 0, n + 0) | !mask->pixel[0][0];
                  val01 = imgIn.pixel(m + 0, n + 1) | !mask->pixel[0][1];
                  val02 = imgIn.pixel(m + 0, n + 2) | !mask->pixel[0][2];

                  val10 = imgIn.pixel(m + 1, n + 0) | !mask->pixel[1][0];
                  val11 = mask->pixel[1][1]; // center of the mask
                  val12 = imgIn.pixel(m + 1, n + 2) | !mask->pixel[1][2];

                  val20 = imgIn.pixel(m + 2, n + 0) | !mask->pixel[2][0];
                  val21 = imgIn.pixel(m + 2, n + 1) | !mask->pixel[2][1];
                  val22 = imgIn.pixel(m + 2, n + 2) | !mask->pixel[2][2];

                  // drawing the eroded image
                  imageEroded.setPixel(m + 0, n + 0, val00);
                  imageEroded.setPixel(m + 0, n + 1, val01);
                  imageEroded.setPixel(m + 0, n + 2, val02);

                  imageEroded.setPixel(m + 1, n + 0, val10);
                  imageEroded.setPixel(m + 1, n + 1, val11);
                  imageEroded.setPixel(m + 1, n + 2, val12);
                  
                  imageEroded.setPixel(m + 2, n + 0, val20);
                  imageEroded.setPixel(m + 2, n + 1, val21);
                  imageEroded.setPixel(m + 2, n + 2, val22);
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for

#ifdef DEBUG
   // write the image into to a file disk
   if (level == 1) { // write the image (default)
    imageEroded.save("../../tests/eroded.jpg", "jpg");
   }  // end if
   else  // the function was called from an upper level process
      ;// do nothing
   cout << "## level: " << level << endl;
   cout << "## ending: erosion" << endl;
#endif

   return 0;   // success
}  // end function imageErosion

// perform opening algorithm
int Morphology::imageOpening(QImage imgIn) {
   // eroded image
   QImage imgEro = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);

   // algorithm: first erode then dilate
   imageErosion(imgIn, 2);
   imgEro = getErodedImage();
   imageDilation(imgEro, 2);
   imageOpened = getDilatedImage();

   // write the image into a file disk
   imageOpened.save("../../tests/opened.jpg", "jpg");

   return 0;   // success
}  // end function imageOpening

// perform closing algorithm
int Morphology::imageClosing(QImage imgIn) {
   // dilated image
   QImage imgDil = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);

   // algorithm: first dilate then erode
   imageDilation(imgIn, 2);
   imgDil = getDilatedImage();
   imageErosion(imgDil, 2);
   imageClosed = getErodedImage();

   // write the image into a file disk
   imageClosed.save("../../tests/closed.jpg", "jpg");

   return 0;   // success
}  // end function imageClosing

// perform gradient: dilation - erosion 
int Morphology::imageGradDilEro(QImage imgIn) {
   // dilated image
   QImage imgDil = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
   // eroded image
   QImage imgEro = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);

   QRgb val;   // rgb value

   // getting the dilated and eroded images
   imageDilation(imgIn, 3);
   imgDil = getDilatedImage();
   imageErosion(imgIn, 3);
   imgEro = getErodedImage();

   // algorithm: dilated - eroded
   for (int i = 0; i < imageGDilEro.width(); ++i) {
      for (int j = 0; j < imageGDilEro.height(); ++j) {
         val = imgDil.pixel(i, j) & ~imgEro.pixel(i, j);
         //val = imageDilated.pixel(i, j) & ~imageEroded.pixel(i, j);
         imageGDilEro.setPixel(i, j, val); 
      }  // end for
   }  // end for

   // write the image into to a file disk
   imageGDilEro.save("../../tests/grad_d-e.jpg", "jpg");

   return 0;   // success
}  // end function imageGradDilEro

// perform gradient: dilation - original (binarized)
int Morphology::imageGradDilOri(QImage imgIn) {
   // dilated image
   QImage imgDil = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
   // eroded image
   //QImage imgEro = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);
   // binary image
   QImage imgBin = QImage(imgIn.width(), imgIn.height(), QImage::Format_RGB32);

   QRgb val;   // rgb value

   // getting the dilated and eroded images
   imageDilation(imgIn, 3);
   imgDil = getDilatedImage();
   //imageErosion(imgIn, 3);
   //imgEro = getErodedImage();
   imgBin = getBinaryImage();

   // algorithm: dilated - eroded
   for (int i = 0; i < imageGDilOri.width(); ++i) {
      for (int j = 0; j < imageGDilOri.height(); ++j) {
         val = imgDil.pixel(i, j) & ~imgBin.pixel(i, j);
         //val = imageDilated.pixel(i, j) & ~imageEroded.pixel(i, j);
         imageGDilOri.setPixel(i, j, val); 
      }  // end for
   }  // end for

   // write the image into to a file disk
   imageGDilOri.save("../../tests/grad_d-o.jpg", "jpg");

   return 0;   // success
}  // end function imageGradDilEro
