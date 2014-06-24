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
      // default constructor
      Morphology();
      // overloaded constructor
      Morphology(QImage, Image2D*);
      // copy constructor
      //Morphology(Morphology &);
      // destructor
      ~Morphology();

      // member functions
      // set binary image
      void setBinaryImage(QImage);
      // return binary image
      QImage getBinaryImage() const;

      // set dilated image
      void setDilatedImage(QImage);
      // return dilated image
      QImage getDilatedImage() const;

      // set eroded image
      void setErodedImage(QImage);
      // return eroded image
      QImage getErodedImage() const;

      // set opened image
      void setOpenedImage(QImage);
      // return opened image
      QImage getOpenedImage() const;

      // set closed image
      void setClosedImage(QImage);
      // return closed image
      QImage getClosedImage() const;

      // set gradient: dilation - erosion
      void setGradDilEro(QImage);
      // return gradient: dilation - erosion
      QImage getGradDilEro() const;

      // set gradient: dilation - original (binarized)
      void setGradDilOri(QImage);
      // return gradient: dilation - original (binarized)
      QImage getGradDilOri() const;


      // dilation wrapper
      //void dilation();

      // perform binarization algorithm
      int imageBinarization(QImage);
      // perform dilation algorithm
      int imageDilation(QImage, int = 1);
      // perform erosion algorithm
      int imageErosion(QImage, int = 1);
      // perform opening algorithm
      int imageOpening(QImage);
      // perform closing algorithm
      int imageClosing(QImage);
      // perform gradient: dilation - erosion
      int imageGradDilEro(QImage);
      // perform gradient: dilation - original (binarized)
      int imageGradDilOri(QImage);

   private:
      Image2D *mask;          // mask
      QImage imageOriginal;   // original image
      QImage imageBinary;     // binary image
      QImage imageDilated;    // dilated image
      QImage imageEroded;     // eroded image
      QImage imageOpened;     // opened image
      QImage imageClosed;     // closed image
      QImage imageGDilEro;    // gradient: dilated - eroded
      QImage imageGDilOri;    // gradient: dilated - original (binarized)
}; // end class Morphology

#endif
