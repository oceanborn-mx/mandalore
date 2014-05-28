// morphologicalOperations.cpp
// Computes the morphological operator upon an pixel
// **first draft** may contain bugs
#include <iostream>
#include <stdlib.h>
using namespace std;

// 2D array structure
typedef struct {
   size_t nRows;  // number of rows
   size_t nCols;  // number of columns
   int **pixel;   // image nRows x nCols
} Image2D;

// image constants
const int width  = 7;   // image width
const int height = 7;   // image height

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

int main() {
   Image2D *dilatada;   // dilated pixel
   Image2D *erosionada; // eroded pixel
   Image2D *apertura;   // opened pixel
   Image2D *cerradura;  // closed pixel
   Image2D *gradiente;  // gradient: dilated - eroded
   Image2D *grad2;      // gradiente: closed - opened
   
   Image2D *image1;     // input image
   Image2D *image2;     // input image
   Image2D *mascara1;   // mask (structuring element)
   Image2D *mascara2;   // mask (structuring element)

   cout << "*debug* before setting test images and masks" << endl;

   image1 = setMemoryAllocation(image1, width, height);
   int aux1[width][height] = {{0, 0, 0, 0, 0, 0, 0}, 
                     {0, 0, 0, 0, 0, 0, 0}, 
                     {0, 0, 1, 1, 1, 1, 0}, 
                     {0, 0, 1, 1, 1, 1, 0},
                     {0, 0, 1, 1, 1, 1, 0},
                     {0, 0, 1, 1, 1, 1, 0},
                     {0, 0, 0, 0, 0, 0, 0}};
   for (size_t i = 0; i < width; ++i) {
      for (size_t j = 0; j < height; ++j) {
         image1->pixel[i][j] = aux1[i][j];
      }  // end for
   }  // end for
   
   cout << "*debug* after setting image1" << endl;
   
   image2 = setMemoryAllocation(image2, width, height);
   int aux2[width][height] = {{0, 1, 1, 1, 0, 0, 0},
                     {0, 1, 1, 1, 0, 0, 0}, 
                     {0, 1, 1, 1, 0, 0, 0}, 
                     {0, 1, 1, 1, 0, 0, 0}, 
                     {0, 0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0, 0}};
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

   cout << "*debug* before operations" << endl;

   // Digital Signal Processing
   dilatada = imageDilation(image1, mascara2);

   erosionada = imageErosion(image1, mascara2);

   apertura = imageOpening(image1, mascara2);

   cerradura = imageClosing(image1, mascara2);

   gradiente = gradDilationErosion(image1, mascara2);

   grad2 = gradClosingOpening(image1, mascara2);
   
   cout << "*debug* displaying results:" << endl;

   // output results
   outputImage(dilatada);
   cout << endl;

   outputImage(erosionada);
   cout << endl;

   outputImage(cerradura);
   cout << endl;

   outputImage(apertura);
   cout << endl;

   outputImage(gradiente);
   cout << endl;

   outputImage(grad2);
   
   // release memory
   freeMemory(dilatada);
   freeMemory(erosionada);
   freeMemory(apertura);
   freeMemory(cerradura);
   freeMemory(gradiente);
   freeMemory(grad2);
   freeMemory(image1);
   freeMemory(image2);
   freeMemory(mascara1);
   freeMemory(mascara2);

   cout << "*debug* end main" << endl;
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
   cout << "*debug* dilation algorithm" << endl;
   
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
         // TODO: find a way, if any, to make generic the decision mechanism considering any mask->pixel
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
   cout << "*debug* erosion algorithm" << endl;
   
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
   grad = setMemoryAllocation(grad, 7, 7);

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
   for (size_t k = 0; k < width; ++k)
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
