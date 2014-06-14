// morphology.h
// class definition
#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

// Qt library for JPEG image manipulation
#include <QtGui/QImage>

// 2D array structure
typedef struct {
   size_t nRows;           // number of rows
   size_t nCols;           // number of columns
   unsigned int **pixel;   // image nRows x nCols
} Image2D;

class Morphology {
   public:
      // constructor
      Morphology(QImage, Image2D*);
      // destructor
      ~Morphology();

      // member functions
      // performs the binarization of an image
      void setBinaryImage();
      // returns the binary image
      QImage getBinaryImage() const;

      // performs the dilation of an image
      void setDilatedImage();
      // returns the dilated image
      QImage getDilatedImage() const;

      // performs the erosion of an image
      void setErodedImage();
      // returns the eroded image
      QImage getErodedImage() const;

      // performs the binarization algorithm
      int imageBinarization();
      // performs the dilation algorithm
      int imageDilation();
      // performs the erosion algorithm
      int imageErosion();

   private:
      Image2D *mask;          // mask
      QImage imageOriginal;   // original image
      QImage imageBinary;     // binary image
      QImage imageDilated;    // dilated image
      QImage imageEroded;     // eroded image
      QImage imageOpened;     // opened image
      QImage imageClosed;     // closed image
}; // end class Morphology

#endif
