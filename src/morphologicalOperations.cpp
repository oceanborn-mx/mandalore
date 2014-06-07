// morphologicalOperations.cpp
// Computes the morphological operators upon a **binary** image
// **first draft** may contain bugs
#include <iostream>
#include <stdlib.h>
#include <QtGui/QImage> // Qt library, needed for jpg image handling
#include <QColor>
extern "C" {   // include this C library, needed for multithreading
   #include <pthread.h>
}
#define DEBUG

using namespace std;

// 2D array structure
typedef struct {
   size_t nRows;           // number of rows
   size_t nCols;           // number of columns
   unsigned int **pixel;   // image nRows x nCols
} Image2D;

// arguments' structure to pass them into a thread
typedef struct {
   Image2D *imagen1;    // image 1 - argument
   Image2D *imagen2;    // image 2 - argument
   Image2D *imFiltered; // image 3 - returned
} ArgImage2D;

// image constants
const int width  = 9;   // image width
const int height = 9;   // image height

// prototypes
Image2D* imageDilation(const Image2D*, const Image2D*);
QImage* imageDilation(const QImage*, const Image2D*);
Image2D* imageErosion(const Image2D*, const Image2D*);
Image2D* imageOpening(const Image2D*, const Image2D*);
Image2D* imageClosing(const Image2D*, const Image2D*);
Image2D* gradDilationErosion(const Image2D*, const Image2D*);
Image2D* gradClosingOpening(const Image2D*, const Image2D*);
Image2D* setMemoryAllocation(Image2D*, size_t, size_t);
int freeMemory(Image2D*);
int outputImage(const Image2D*);
void* wrapperImageDilation(void*);
void* wrapperImageErosion(void*);
void* wrapperImageOpening(void*);
void* wrapperImageClosing(void*);
void* wrapperGradDilationErosion(void*);
void* wrapperGradClosingOpening(void*);
QImage* imageBinarization(const QImage*);

int main() {
   Image2D *image1;     // input image
   Image2D *image2;     // input image
   Image2D *mascara1;   // mask (structuring element)
   Image2D *mascara2;   // mask (structuring element)

   pthread_t  threadDilation; // thread 1
   pthread_t  threadErosion;  // thread 2
   pthread_t  threadOpening;  // thread 3
   pthread_t  threadClosing;  // thread 4
   pthread_t  threadGDE;      // thread 5
   pthread_t  threadGCO;      // thread 6

   ArgImage2D argsDilation;      // argumets to pass into the thread
   ArgImage2D *argsDilationPtr;  // pointer to argsDilation
   ArgImage2D argsErosion;       // argumets to pass into the thread
   ArgImage2D argsOpening;       // argumets to pass into the thread
   ArgImage2D argsClosing;       // argumets to pass into the thread
   ArgImage2D argsGDE;           // argumets to pass into the thread
   ArgImage2D argsGCO;           // argumets to pass into the thread

#ifdef DEBUG
   cout << "## before setting test images and masks" << endl;
#endif

   // loading input jpg image
   QImage *img = new QImage("tux_fedora.jpg");

    if(img->isNull())
        cout << "Image is null" << endl;
    else
        cout << "Image loaded" << endl;

#ifdef DEBUG
   cout << "image width: " << img->width() << endl;
   cout << "image height: " << img->height() << endl;
   cout << "pixel " << hex << (img->pixel(110, 110) & 0x0000FFFF) << endl;
#endif
   
   // setting the matrices emulating a 2D image
   image1 = setMemoryAllocation(image1, width, height);
   int aux1[width][height] = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, 
                              {0, 0, 0, 0, 0, 0, 0, 0, 0}, 
                              {0, 0, 1, 1, 1, 1, 1, 0, 0}, 
                              {0, 0, 1, 1, 1, 1, 1, 0, 0},
                              {0, 0, 1, 1, 1, 1, 1, 0, 0},
                              {0, 0, 1, 1, 1, 1, 1, 0, 0},
                              {0, 0, 1, 1, 1, 1, 1, 0, 0}, 
                              {0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0}};
   for (size_t i = 0; i < width; ++i) {
      for (size_t j = 0; j < height; ++j) {
         image1->pixel[i][j] = aux1[i][j];
      }  // end for
   }  // end for

#ifdef DEBUG
   cout << "## after setting image1" << endl;
#endif
   
   image2 = setMemoryAllocation(image2, width, height);
   int aux2[width][height] = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 1, 1, 1, 0, 0, 0}, 
                              {0, 0, 0, 1, 1, 1, 0, 0, 0}, 
                              {0, 1, 1, 1, 1, 1, 1, 1, 0}, 
                              {0, 1, 1, 1, 1, 1, 1, 1, 0},
                              {0, 1, 1, 1, 1, 1, 1, 1, 0},
                              {0, 0, 0, 1, 1, 1, 0, 0, 0},
                              {0, 0, 0, 1, 1, 1, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0}};
   for (size_t i = 0; i < width; ++i) {
      for (size_t j = 0; j < height; ++j) {
         image2->pixel[i][j] = aux2[i][j];
      }  // end for
   }  // end for

   mascara1 = setMemoryAllocation(mascara1, 3, 3);
   //unsigned int aux3[3][3] = {{0xFF000000, 0xFFFFFFFF, 0xFF000000},
   //                           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 
   //                           {0xFF000000, 0xFFFFFFFF, 0xFF000000}};
   unsigned int aux3[3][3] = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                              {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 
                              {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
   //unsigned int aux3[3][3] = {{0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF},
   //                           {0xFF000000, 0xFF000000, 0xFF000000}, 
   //                           {0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF}};
   for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
         mascara1->pixel[i][j] = aux3[i][j];
      }  // end for
   }  // end for

   mascara2 = setMemoryAllocation(mascara2, 3, 3);
   int aux4[3][3] = {{0, 1, 0}, 
                     {1, 1, 1}, 
                     {0, 1, 0}};
   for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
         mascara2->pixel[i][j] = aux4[i][j];
      }  // end for
   }  // end for

#ifdef DEBUG
   cout << "## setting the image arguments for multithreading" << endl;
#endif

   // setting the arguments for the threads
   argsDilationPtr = &argsDilation;
   argsDilationPtr->imagen1 = image1;
   argsDilationPtr->imagen2 = mascara2;

   argsErosion.imagen1 = image1;
   argsErosion.imagen2 = mascara2;

   argsOpening.imagen1 = image1;
   argsOpening.imagen2 = mascara2;

   argsClosing.imagen1 = image1;
   argsClosing.imagen2 = mascara2;

   argsGDE.imagen1 = image1;
   argsGDE.imagen2 = mascara2;

   argsGCO.imagen1 = image1;
   argsGCO.imagen2 = mascara2;

#ifdef DEBUG
   cout << "## before operations" << endl;
#endif

   // threads' status variables
   int status1;   // status of last system call
   int status2;   // status of last system call
   int status3;   // status of last system call
   int status4;   // status of last system call
   int status5;   // status of last system call
   int status6;   // status of last system call

   // Digital Signal Processing
   // throw a new thread
   status1 = pthread_create(&threadDilation, NULL, wrapperImageDilation, (void*)argsDilationPtr);
      if (status1 != 0) 
         cerr << "Error: the thread could not be launched" << endl;

   // throw a new thread
   status2 = pthread_create(&threadErosion, NULL, wrapperImageErosion, (void*)&argsErosion);
      if (status2 != 0) 
         cerr << "Error: the thread could not be launched" << endl;

   // throw a new thread
   status3 = pthread_create(&threadOpening, NULL, wrapperImageOpening, (void*)&argsOpening);
      if (status3 != 0) 
         cerr << "Error: the thread could not be launched" << endl;

   // throw a new thread
   status4 = pthread_create(&threadClosing, NULL, wrapperImageClosing, (void*)&argsClosing);
      if (status4 != 0) 
         cerr << "Error: the thread could not be launched" << endl;

   // throw a new thread
   status5 = pthread_create(&threadGDE, NULL, wrapperGradDilationErosion, (void*)&argsGDE);
      if (status5 != 0) 
         cerr << "Error: the thread could not be launched" << endl;

   // throw a new thread
   status6 = pthread_create(&threadGCO, NULL, wrapperGradClosingOpening, (void*)&argsGCO);
      if (status6 != 0) 
         cerr << "Error: the thread could not be launched" << endl;

   // testing with jpeg images
   QImage *imagenBin;   // binary jpg image
   QImage *imagenDil;   // dilated jpg image

   // digital signal processing
   imagenBin = imageBinarization(img);
   imagenDil = imageDilation(imagenBin, mascara1);

   // waiting for threads
   pthread_join(threadDilation, NULL);
   pthread_join(threadErosion, NULL);
   pthread_join(threadOpening, NULL);
   pthread_join(threadClosing, NULL);
   pthread_join(threadGDE, NULL);
   pthread_join(threadGCO, NULL);
  
#ifdef DEBUG
   cout << "## displaying results:" << endl;

   // output results
   outputImage(argsDilationPtr->imFiltered);
   cout << endl;

   outputImage(argsErosion.imFiltered);
   cout << endl;

   outputImage(argsOpening.imFiltered);
   cout << endl;

   outputImage(argsClosing.imFiltered);
   cout << endl;

   outputImage(argsGDE.imFiltered);
   cout << endl;

   outputImage(argsGCO.imFiltered);
   cout << endl;
#endif

   // release memory
   freeMemory(argsDilationPtr->imFiltered);
   freeMemory(argsErosion.imFiltered);  
   freeMemory(argsOpening.imFiltered);
   freeMemory(argsClosing.imFiltered);
   freeMemory(argsGDE.imFiltered);
   freeMemory(argsGCO.imFiltered);
   freeMemory(image1);
   freeMemory(image2);
   freeMemory(mascara1);
   freeMemory(mascara2);

   delete imagenBin;
   delete imagenDil;
   delete img;

   // zero pointers after free to avoid reuse
   argsDilationPtr->imFiltered = NULL;
   argsErosion.imFiltered = NULL;
   argsOpening.imFiltered = NULL;
   argsClosing.imFiltered = NULL;
   argsGDE.imFiltered = NULL;
   argsGCO.imFiltered = NULL;
   image1 = NULL;
   image2 = NULL;
   mascara1 = NULL;
   mascara2 = NULL;
   imagenBin = NULL;
   imagenDil = NULL;
   img = NULL;
 
#ifdef DEBUG
   cout << "## end main" << endl;
#endif
}  // end main

// M -> stands for the 'mask' running through the image
// ---------->
//111111111111MMM    |
//111111111111MMM    |
//111111111111MMM    |
//111111111111111    v
//111111111111111
//111111111111111

// Dilation
Image2D* imageDilation(const Image2D * const inIm, const Image2D * const mask) {
   Image2D* dIm;   // Dilated image

   // dynamic memory allocation
   dIm = setMemoryAllocation(dIm, width, height);

   // Dilation algorithm
   for (size_t i = 0; i < width - 2; ++i) {
      for (size_t j = 0; j < height - 2; ++j) {
         if (inIm->pixel[i+1][j+1] & mask->pixel[1][1]) {
            for (size_t m = 0 + i; m < 1 + i; ++m) {
               for (size_t n = 0 + j; n < 1 + j; ++n) {
                  dIm->pixel[m+0][n+0] |= mask->pixel[0][0]; 
                  dIm->pixel[m+0][n+1] |= mask->pixel[0][1]; 
                  dIm->pixel[m+0][n+2] |= mask->pixel[0][2];

                  dIm->pixel[m+1][n+0] |= mask->pixel[1][0];
                  dIm->pixel[m+1][n+1]  = mask->pixel[1][1];   // center of the mask
                  dIm->pixel[m+1][n+2] |= mask->pixel[1][2];

                  dIm->pixel[m+2][n+0] |= mask->pixel[2][0];
                  dIm->pixel[m+2][n+1] |= mask->pixel[2][1];
                  dIm->pixel[m+2][n+2] |= mask->pixel[2][2];
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for

#ifdef DEBUG
   cout << "## dilation algorithm" << endl;
#endif
   
   return dIm;
}  // end imageDilation function

// overloaded function imageDilation
QImage* imageDilation(const QImage * const inIm, const Image2D * const mask) {
   // getting the size
   // the binary image has the same size like the imput image
   int ancho = inIm->width();
   int alto = inIm->height();

#ifdef DEBUG
   cout << "## starting: overloaded dilation" << endl;
   cout << "## input image width: " << dec << ancho << endl;
   cout << "## input image height: " << dec << alto << endl;
#endif

   // binary image
   QImage *dIm = new QImage(ancho, alto, QImage::Format_RGB32);
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
   for (int i = 0; i < inIm->width() - 2; ++i) {
      for (int j = 0; j < inIm->height() - 2; ++j) {
         if (inIm->pixel(i+1, j+1) == mask->pixel[1][1]) {
            for (int m = 0 + i; m < 1 + i; ++m) {
               for (int n = 0 + j; n < 1 + j; ++n) {
                  // applying the mask
                  val00 = inIm->pixel(m+0, n+0) | mask->pixel[0][0];
                  val01 = inIm->pixel(m+0, n+1) | mask->pixel[0][1];
                  val02 = inIm->pixel(m+0, n+2) | mask->pixel[0][2];

                  val10 = inIm->pixel(m+1, n+0) | mask->pixel[1][0];
                  val11 = mask->pixel[1][1]; // center of the mask
                  val12 = inIm->pixel(m+1, n+2) | mask->pixel[1][2];

                  val20 = inIm->pixel(m+2, n+0) | mask->pixel[2][0];
                  val21 = inIm->pixel(m+2, n+1) | mask->pixel[2][1];
                  val22 = inIm->pixel(m+2, n+2) | mask->pixel[2][2];

                  // drawing the dilated image
                  dIm->setPixel(m+0, n+0, val00);
                  dIm->setPixel(m+0, n+1, val01);
                  dIm->setPixel(m+0, n+2, val02);

                  dIm->setPixel(m+1, n+0, val10);
                  dIm->setPixel(m+1, n+1, val11);
                  dIm->setPixel(m+1, n+2, val12);
                  
                  dIm->setPixel(m+2, n+0, val20);
                  dIm->setPixel(m+2, n+1, val21);
                  dIm->setPixel(m+2, n+2, val22);
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for

#ifdef DEBUG
   // write the image into to a file disk
   dIm->save("dilatada.jpg", "jpg");

   cout << "## ending: overloaded dilation" << endl;
#endif

   return dIm;
}  // end function template imageDilation

// Erosion
Image2D* imageErosion(const Image2D * const inIm, const Image2D * const mask) {
   Image2D* eIm;   // Eroded image

   // dynamic memory allocation
   eIm = setMemoryAllocation(eIm, width, height);

   // Erosion algorithm
   for (size_t i = 0; i < width - 2; ++i) {
      for (size_t j = 0; j < height - 2; ++j) {   
         // TODO: find a way, if any, to make generic the decision mechanism considering any mask
         // Mask 1
         //if ((inIm->pixel[i+0][j+0] & mask->pixel[0][0]) & (inIm->pixel[i+0][j+1] & mask->pixel[0][1]) & (inIm->pixel[i+0][j+2] & mask->pixel[0][2]) &
         //    (inIm->pixel[i+1][j+0] & mask->pixel[1][0]) & (inIm->pixel[i+1][j+1] & mask->pixel[1][1]) & (inIm->pixel[i+1][j+2] & mask->pixel[1][2]) &
         //    (inIm->pixel[i+2][j+0] & mask->pixel[2][0]) & (inIm->pixel[i+2][j+1] & mask->pixel[2][1]) & (inIm->pixel[i+2][j+2] & mask->pixel[2][2])) {
         // Mask 2
         if (/*(inIm->pixel[i+0][j+0] & mask->pixel[0][0]) & */(inIm->pixel[i+0][j+1] & mask->pixel[0][1]) /* & (inIm->pixel[i+0][j+2] & mask->pixel[0][2])*/ &
             (inIm->pixel[i+1][j+0] & mask->pixel[1][0]) & (inIm->pixel[i+1][j+1] & mask->pixel[1][1]) & (inIm->pixel[i+1][j+2] & mask->pixel[1][2]) &
             /*(inIm->pixel[i+2][j+0] & mask->pixel[2][0]) & */(inIm->pixel[i+2][j+1] & mask->pixel[2][1]) /*& (inIm->pixel[i+2][j+2] & mask->pixel[2][2])*/) {
            for (size_t m = 0 + i; m < 1 + i; ++m) {
               for (size_t n = 0 + j; n < 1 + j; ++n) {
                  eIm->pixel[m+0][n+0] |= !mask->pixel[0][0];
                  eIm->pixel[m+0][n+1] |= !mask->pixel[0][1];
                  eIm->pixel[m+0][n+2] |= !mask->pixel[0][2];

                  eIm->pixel[m+1][n+0] |= !mask->pixel[1][0];
                  eIm->pixel[m+1][n+1]  =  mask->pixel[1][1];  // center of the mask
                  eIm->pixel[m+1][n+2] |= !mask->pixel[1][2];

                  eIm->pixel[m+2][n+0] |= !mask->pixel[2][0];
                  eIm->pixel[m+2][n+1] |= !mask->pixel[2][1];
                  eIm->pixel[m+2][n+2] |= !mask->pixel[2][2];
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for

#ifdef DEBUG
   cout << "## erosion algorithm" << endl;
#endif
   
   return eIm;
}  // end imageErosion function

// Opening
Image2D* imageOpening(const Image2D * const inIm, const Image2D * const mask) {
   Image2D *oIm;  // Opened image
   Image2D *eIm;  // Eroded image

   // algorithm: first erode then dilate
   eIm = imageErosion(inIm, mask);
   oIm = imageDilation(eIm, mask);
   
   // release memory
   freeMemory(eIm);

   return oIm;
}  // end imageOpening function

// Closing
Image2D* imageClosing(const Image2D * const inIm, const Image2D * const mask) {
   Image2D *cIm;  // Closed image
   Image2D *dIm;  // Dilated image

   // algorithm: first dilate then erode
   dIm = imageDilation(inIm, mask);
   cIm = imageErosion(dIm, mask);

   // release memory
   freeMemory(dIm);

   return cIm;
}  // end imageClosing function

// Gradient: Dilation - Erosion
Image2D* gradDilationErosion(const Image2D * const inIm, const Image2D * const mask) {
   Image2D *grad; // gradient
   Image2D *dIm;  // Dilated image
   Image2D *eIm;  // Eroded image

   // dynamic memory allocation
   grad = setMemoryAllocation(grad, width, height);

   dIm = imageDilation(inIm, mask);
   eIm = imageErosion(inIm, mask);

   // algorith: Dilated - Eroded
   for (size_t i = 0; i < width; ++i) {
      for (size_t j = 0; j < height; ++j) {
         grad->pixel[i][j] = dIm->pixel[i][j] & !eIm->pixel[i][j]; 
      }  // end for
   }  // end for

   // release memory
   freeMemory(dIm);
   freeMemory(eIm);

   return grad;
}  // end gradDilationErosion function

// Gradient: Closing - Opening
Image2D* gradClosingOpening(const Image2D * const inIm, const Image2D * const mask) {
   Image2D*grad; // gradient
   Image2D*oIm;  // Opened image
   Image2D*cIm;  // Closed image

   // dynamic memory allocation
   grad = setMemoryAllocation(grad, width, height);

   oIm = imageOpening(inIm, mask);
   cIm = imageClosing(inIm, mask);

   // algorith: Closed - Opened
   for (size_t i = 0; i < width; ++i) {
      for (size_t j = 0; j < height; ++j) {
         grad->pixel[i][j] = cIm->pixel[i][j] & !oIm->pixel[i][j]; 
      }  // end for
   }  // end for

   // release memory
   freeMemory(oIm);
   freeMemory(cIm);

   return grad;
}  // end gradClosingOpening function

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
}  // end setMemoryAllocation function

// release memory
int freeMemory(Image2D* blockImage) {

   // freeing memory allocated
   for (size_t k = 0; k < blockImage->nRows; ++k)
      free(blockImage->pixel[k]);
   free(blockImage->pixel);
   free(blockImage);

   return 0;   // success
}  // end freeMemory function

int outputImage(const Image2D * const inImage) {
   
   for (size_t i = 0; i < width; ++i) {
      for (size_t j = 0; j < height; ++j) {
         cout << inImage->pixel[i][j] << " ";
      }  // end for
      cout << endl;
   }  // end for

   return 0;   // success
}  // end outputImage function

void* wrapperImageDilation(void* arg) {
   ArgImage2D *args = (ArgImage2D*)arg;   // arguments

   // Digital Signal Processing
   args->imFiltered = imageDilation(args->imagen1, args->imagen2);

#ifdef DEBUG
   cout << "## dilation algorithm in thread" << endl;
#endif

   // finish the thread
   pthread_exit(NULL);

   return 0;   // success
}  // end wrapperImageDilation function

void* wrapperImageErosion(void* arg) {
   ArgImage2D *args = (ArgImage2D*)arg;   // arguments

   // Digital Signal Processing
   args->imFiltered = imageErosion(args->imagen1, args->imagen2);

#ifdef DEBUG
   cout << "## erosion algorithm in thread" << endl;
#endif

   // finish the thread
   pthread_exit(NULL);

   return 0;   // success
}  // end wrapperImageDilation function

void* wrapperImageOpening(void* arg) {
   ArgImage2D *args = (ArgImage2D*)arg;   // arguments

   // Digital Signal Processing
   args->imFiltered = imageOpening(args->imagen1, args->imagen2);

#ifdef DEBUG
   cout << "## opening algorithm in thread" << endl;
#endif

   // finish the thread
   pthread_exit(NULL);

   return 0;   // success
}  // end wrapperImageDilation function

void* wrapperImageClosing(void* arg) {
   ArgImage2D *args = (ArgImage2D*)arg;   // arguments

   // Digital Signal Processing
   args->imFiltered = imageClosing(args->imagen1, args->imagen2);

#ifdef DEBUG
   cout << "## closing algorithm in thread" << endl;
#endif

   // finish the thread
   pthread_exit(NULL);

   return 0;   // success
}  // end wrapperImageDilation function

void* wrapperGradDilationErosion(void* arg) {
   ArgImage2D *args = (ArgImage2D*)arg;   // arguments

   // Digital Signal Processing
   args->imFiltered = gradDilationErosion(args->imagen1, args->imagen2);

#ifdef DEBUG
   cout << "## grad: d - e algorithm in thread" << endl;
#endif

   // finish the thread
   pthread_exit(NULL);

   return 0;   // success
}  // end wrapperImageDilation function

void* wrapperGradClosingOpening(void* arg) {
   ArgImage2D *args = (ArgImage2D*)arg;   // arguments

   // Digital Signal Processing
   args->imFiltered = gradClosingOpening(args->imagen1, args->imagen2);

#ifdef DEBUG
   cout << "## grad: c - o algorithm in thread" << endl;
#endif

   // finish the thread
   pthread_exit(NULL);

   return 0;   // success
}  // end wrapperImageDilation function

// binarization process
QImage* imageBinarization(const QImage * const imgIn) {
   // getting the size
   // the binary image has the same size like the imput image
   int ancho = imgIn->width();
   int alto = imgIn->height();

   // Binary image
   QImage *imBin = new QImage(ancho, alto, QImage::Format_RGB32);
   QRgb value; // rgb value

   // threshold
   for (int x = 0; x < imBin->width(); ++x) {
      for (int y = 0; y < imBin->height(); ++y) {
         QColor currentPixel(imgIn->pixel(x, y));  // current pixel
         // color filter TODO: remove the hard-code filter
         if (currentPixel.blue() > 127) { // threshold value to umbralize TODO: remove the hard-code
            value = qRgb(255, 255, 255);  // white
            imBin->setPixel(x, y, value);
         }  // end if
         else {
            value = qRgb(0, 0, 0);        // black
            imBin->setPixel(x, y, value);
         }  // end else
      }  // end for
   }  // end for

#ifdef DEBUG
   imBin->save("binary.jpg", "jpg");
#endif

   return imBin;
}  // end imageBinarization function
