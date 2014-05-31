// morphologicalOperations.cpp
// Computes the morphological operator upon an image
// **first draft** may contain bugs
#include <iostream>
#include <stdlib.h>
extern "C" {   // include this C library, needed for multithreading
   #include <pthread.h>
}
#define DEBUG

using namespace std;

// 2D array structure
typedef struct {
   size_t nRows;  // number of rows
   size_t nCols;  // number of columns
   int **pixel;   // image nRows x nCols
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
Image2D* imageDilation(Image2D*, Image2D*);
Image2D* imageErosion(Image2D*, Image2D*);
Image2D* imageOpening(Image2D*, Image2D*);
Image2D* imageClosing(Image2D*, Image2D*);
Image2D* gradDilationErosion(Image2D*, Image2D*);
Image2D* gradClosingOpening(Image2D*, Image2D*);
Image2D* setMemoryAllocation(Image2D*, size_t, size_t);
int freeMemory(Image2D*);
int outputImage(Image2D* inImage);
void* wrapperImageDilation(void*);
void* wrapperImageErosion(void*);
void* wrapperImageOpening(void*);
void* wrapperImageClosing(void*);
void* wrapperGradDilationErosion(void*);
void* wrapperGradClosingOpening(void*);

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
   cout << "*debug* before setting test images and masks" << endl;
#endif

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
   cout << "*debug* after setting image1" << endl;
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
   int aux3[3][3] = {{1, 1, 1},
                     {1, 1, 1}, 
                     {1, 1, 1}};
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
   cout << "*debug* setting the image arguments for multithread" << endl;
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
   cout << "*debug* before operations" << endl;
#endif

   // Digital Signal Processing
   // throw a new thread
   int status1 = pthread_create(&threadDilation, NULL, wrapperImageDilation, (void*)argsDilationPtr);
      if (status1 != 0) 
         cout << "Error: the thread could not be launched" << endl;

   // throw a new thread
   int status2 = pthread_create(&threadErosion, NULL, wrapperImageErosion, (void*)&argsErosion);
      if (status2 != 0) 
         cout << "Error: the thread could not be launched" << endl;

   // throw a new thread
   int status3 = pthread_create(&threadOpening, NULL, wrapperImageOpening, (void*)&argsOpening);
      if (status3 != 0) 
         cout << "Error: the thread could not be launched" << endl;

   // throw a new thread
   int status4 = pthread_create(&threadClosing, NULL, wrapperImageClosing, (void*)&argsClosing);
      if (status4 != 0) 
         cout << "Error: the thread could not be launched" << endl;

   // throw a new thread
   int status5 = pthread_create(&threadGDE, NULL, wrapperGradDilationErosion, (void*)&argsGDE);
      if (status5 != 0) 
         cout << "Error: the thread could not be launched" << endl;

   // throw a new thread
   int status6 = pthread_create(&threadGCO, NULL, wrapperGradClosingOpening, (void*)&argsGCO);
      if (status6 != 0) 
         cout << "Error: the thread could not be launched" << endl;

   // waiting for threads
   pthread_join(threadDilation, NULL);
   pthread_join(threadErosion, NULL);
   pthread_join(threadOpening, NULL);
   pthread_join(threadClosing, NULL);
   pthread_join(threadGDE, NULL);
   pthread_join(threadGCO, NULL);
  
#ifdef DEBUG
   cout << "*debug* displaying results:" << endl;
#endif

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
   
#ifdef DEBUG
   cout << "*debug* end main" << endl;
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
Image2D* imageDilation(Image2D* inIm, Image2D* mask) {
   Image2D* dIm;   // Dilated image

   // dynamic memory allocation
   dIm = setMemoryAllocation(dIm, width, height);

   // Dilation algorithm
   for (size_t i = 0; i < width - 2; ++i) {
      for (size_t j = 0; j < height - 2; ++j) {
         if (inIm->pixel[i+1][j+1] & mask->pixel[1][1]) {
            for (size_t m = 0 + i; m < 1 + i; ++m) {
               for (size_t n = 0 + j; n < 1 + j; ++n) {
                  dIm->pixel[m+0][n+0] |= mask->pixel[0][0], dIm->pixel[m+0][n+1] |= mask->pixel[0][1], dIm->pixel[m+0][n+2] |= mask->pixel[0][2];
                  dIm->pixel[m+1][n+0] |= mask->pixel[1][0], dIm->pixel[m+1][n+1]  = mask->pixel[1][1], dIm->pixel[m+1][n+2] |= mask->pixel[1][2];
                  dIm->pixel[m+2][n+0] |= mask->pixel[2][0], dIm->pixel[m+2][n+1] |= mask->pixel[2][1], dIm->pixel[m+2][n+2] |= mask->pixel[2][2];
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for

#ifdef DEBUG
   cout << "*debug* dilation algorithm" << endl;
#endif
   
   return dIm;
}  // end imageDilation function

// Erosion
Image2D* imageErosion(Image2D* inIm, Image2D* mask) {
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
                  eIm->pixel[m+0][n+0] |= !mask->pixel[0][0], eIm->pixel[m+0][n+1] |= !mask->pixel[0][1], eIm->pixel[m+0][n+2] |= !mask->pixel[0][2];
                  eIm->pixel[m+1][n+0] |= !mask->pixel[1][0], eIm->pixel[m+1][n+1]  =  mask->pixel[1][1], eIm->pixel[m+1][n+2] |= !mask->pixel[1][2];
                  eIm->pixel[m+2][n+0] |= !mask->pixel[2][0], eIm->pixel[m+2][n+1] |= !mask->pixel[2][1], eIm->pixel[m+2][n+2] |= !mask->pixel[2][2];
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for

#ifdef DEBUG
   cout << "*debug* erosion algorithm" << endl;
#endif
   
   return eIm;
}  // end imageErosion function

// Opening
Image2D* imageOpening(Image2D* inIm, Image2D* mask) {
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
Image2D* imageClosing(Image2D* inIm, Image2D* mask) {
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
Image2D* gradDilationErosion(Image2D* inIm, Image2D* mask) {
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
Image2D* gradClosingOpening(Image2D* inIm, Image2D* mask) {
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
   blockImage->pixel = (int**)calloc(blockImage->nRows, sizeof(int*));

   for (size_t k = 0; k < blockImage->nRows; ++k) {
      blockImage->pixel[k] = (int*)calloc(blockImage->nCols, sizeof(int));
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

int outputImage(Image2D* inImage) {
   
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
   cout << "*debug* dilation algorithm in thread" << endl;
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
   cout << "*debug* erosion algorithm in thread" << endl;
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
   cout << "*debug* opening algorithm in thread" << endl;
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
   cout << "*debug* closing algorithm in thread" << endl;
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
   cout << "*debug* grad: d - e algorithm in thread" << endl;
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
   cout << "*debug* grad: c - o algorithm in thread" << endl;
#endif

   // finish the thread
   pthread_exit(NULL);

   return 0;   // success
}  // end wrapperImageDilation function
