// morphologicalOperations.cpp
// Computes the morphological operator upon an image
// **first draft** may contain bugs
#include <iostream>
#include <stdlib.h>
using namespace std;

// 2D array structure
typedef struct {
   size_t nRows;    // number of rows
   size_t nCols;    // number of columns
   int **image;  // image nRows x nCols
} Image2D;

// prototypes
Image2D* imageDilation(Image2D*, Image2D*);
Image2D* imageErosion(Image2D*, Image2D*);
Image2D* imageOpening(Image2D*, Image2D*);
Image2D* imageClosing(Image2D*, Image2D*);
Image2D* gradDilationErosion(Image2D*, Image2D*);
Image2D* gradClosingOpening(Image2D*, Image2D*);
Image2D* setMemoryAllocation(Image2D*, size_t, size_t);
int freeMemory(Image2D*);

int main() {
   Image2D *dilatada;   // dilated image
   Image2D *erosionada; // eroded image
   Image2D *apertura;   // opened image
   Image2D *cerradura;  // closed image
   Image2D *gradiente;  // gradient: dilated - eroded
   Image2D *grad2;      // gradiente: closed - opened
   
   Image2D *image1;
   Image2D *image2;
   Image2D *mascara1;
   Image2D *mascara2;

   cout << "before setting test images and masks" << endl;

   image1 = setMemoryAllocation(image1, 5, 5);
   int aux1[5][5] = {{0, 0, 0, 0, 0}, 
                   {0, 1, 1, 1, 0}, 
                   {0, 1, 1, 1, 0}, 
                   {0, 1, 1, 1, 0}, 
                   {0, 0, 0, 0, 0}};
   for (size_t i = 0; i < 5; ++i) {
      for (size_t j = 0; j < 5; ++j) {
         image1->image[i][j] = aux1[i][j];
      }  // end for
   }  // end for
   
   cout << "after setting image1" << endl;
   
   image2 = setMemoryAllocation(image2, 5, 5);
   int aux2[5][5] = {{0, 1, 1, 1, 0},
                   {0, 1, 1, 1, 0}, 
                   {0, 1, 1, 1, 0}, 
                   {0, 1, 1, 1, 0}, 
                   {0, 0, 0, 0, 0}};
   for (size_t i = 0; i < 5; ++i) {
      for (size_t j = 0; j < 5; ++j) {
         image2->image[i][j] = aux2[i][j];
      }  // end for
   }  // end for

   mascara1 = setMemoryAllocation(mascara1, 3, 3);
   int aux3[3][3] = {{1, 1, 1},
                     {1, 1, 1}, 
                     {1, 1, 1}};
   for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
         mascara1->image[i][j] = aux3[i][j];
      }  // end for
   }  // end for

   mascara2 = setMemoryAllocation(mascara2, 3, 3);
   int aux4[3][3] = {{0, 1, 0}, 
                     {1, 1, 1}, 
                     {0, 1, 0}};
   for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
         mascara2->image[i][j] = aux4[i][j];
      }  // end for
   }  // end for

   cout << "before operations" << endl;

   dilatada = imageDilation(image1, mascara2);

   erosionada = imageErosion(image1, mascara2);

   apertura = imageOpening(image1, mascara2);

   cerradura = imageClosing(image1, mascara2);

   gradiente = gradDilationErosion(image1, mascara2);

   grad2 = gradClosingOpening(image1, mascara2);

   //for (size_t i = 0; i < 5 - 2; ++i) {
   //   for (size_t j = 0; j < 5 - 2; ++j) {
   //      for (size_t m = 0 + i; m < 3 + i; ++m) {    // -2 o -3? porque el elemento estructurante de 3x3
   //         for (size_t n = 0 + j; n < 3 + j; ++n) { // con origen en el centro al llegar al borde
   //            cout << "I[" << m << "][" << n << "] ";   // quedan 2 elementos fuera
   //            cout << dilatada[m][n] << " ";
   //            if ((n - j + 1) % 3 == 0)
   //               cout << endl;
   //         }  // end for
   //      }  // end for
   //      cout << endl;
   //   }  // end for
   //}  // end for
   
   cout << "displaying results:" << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << dilatada->image[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << erosionada->image[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << cerradura->image[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << apertura->image[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << gradiente->image[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << grad2->image[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for
   
   //cout << "~1 " << ~1 << endl; 
   cout << "!1 " << !1 << endl; 

   // release memory
   freeMemory(dilatada);
   freeMemory(erosionada);
   freeMemory(apertura);
   freeMemory(cerradura);
   freeMemory(gradiente);
   freeMemory(grad2);
}  // end main

//1111111111111SSS
//1111111111111SOS
//1111111111111SSS
//111111111111111
//111111111111111
//111111111111111


// Dilation
Image2D* imageDilation(Image2D* inIm, Image2D* mask) {
   Image2D* dIm;   // Dilated image

   cout << "dilation before memset" << endl;
   dIm = setMemoryAllocation(dIm, 5, 5);
   cout << "dilation after memset" << endl;

   //dIm = (Image2D*)calloc(5, sizeof(int*));

   //for (int i = 0; i < 5; ++i)
   //   dIm[i] = (int*)calloc(5, sizeof(int));

   // Dilation algorithm
   for (size_t i = 0; i < 5 - 2; ++i) {
      for (size_t j = 0; j < 5 - 2; ++j) {
         if (inIm->image[i+1][j+1] & mask->image[1][1]) {
            for (size_t m = 0 + i; m < 1 + i - 0; ++m) {
               for (size_t n = 0 + j; n < 1 + j - 0; ++n) {
                  dIm->image[m+0][n+0] |= mask->image[0][0], dIm->image[m+0][n+1] |= mask->image[0][1], dIm->image[m+0][n+2] |= mask->image[0][2];
                  dIm->image[m+1][n+0] |= mask->image[1][0], dIm->image[m+1][n+1]  = mask->image[1][1], dIm->image[m+1][n+2] |= mask->image[1][2];
                  dIm->image[m+2][n+0] |= mask->image[2][0], dIm->image[m+2][n+1] |= mask->image[2][1], dIm->image[m+2][n+2] |= mask->image[2][2];
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for
   cout << "dilation algorithm" << endl;
   
   return dIm;
}  // end imageDilation function

// Erosion
Image2D* imageErosion(Image2D* inIm, Image2D* mask) {
   Image2D* eIm;   // Eroded image

   eIm = setMemoryAllocation(eIm, 5, 5);

   //eIm = (Image2D*)calloc(5, sizeof(int*));

   //for (int i = 0; i < 5; ++i)
   //   eIm[i] = (int*)calloc(5, sizeof(int));

   // Erosion algorithm
   for (size_t i = 0; i < 5 - 2; ++i) {
      for (size_t j = 0; j < 5 - 2; ++j) {   
         // TODO: find a way, if any, to make generic the decision mechanism considering any mask->image
         // Mask 1
         //if ((inIm->image[i+0][j+0] & mask->image[0][0]) & (inIm->image[i+0][j+1] & mask->image[0][1]) & (inIm->image[i+0][j+2] & mask->image[0][2]) &
         //    (inIm->image[i+1][j+0] & mask->image[1][0]) & (inIm->image[i+1][j+1] & mask->image[1][1]) & (inIm->image[i+1][j+2] & mask->image[1][2]) &
         //    (inIm->image[i+2][j+0] & mask->image[2][0]) & (inIm->image[i+2][j+1] & mask->image[2][1]) & (inIm->image[i+2][j+2] & mask->image[2][2])) {
         // Mask 2
         if (/*(inIm->image[i+0][j+0] & mask->image[0][0]) & */(inIm->image[i+0][j+1] & mask->image[0][1]) /* & (inIm->image[i+0][j+2] & mask->image[0][2])*/ &
             (inIm->image[i+1][j+0] & mask->image[1][0]) & (inIm->image[i+1][j+1] & mask->image[1][1]) & (inIm->image[i+1][j+2] & mask->image[1][2]) &
             /*(inIm->image[i+2][j+0] & mask->image[2][0]) & */(inIm->image[i+2][j+1] & mask->image[2][1]) /*& (inIm->image[i+2][j+2] & mask->image[2][2])*/) {
            for (size_t m = 0 + i; m < 1 + i - 0; ++m) {
               for (size_t n = 0 + j; n < 1 + j - 0; ++n) {
                  eIm->image[m+0][n+0] |= !mask->image[0][0], eIm->image[m+0][n+1] |= !mask->image[0][1], eIm->image[m+0][n+2] |= !mask->image[0][2];
                  eIm->image[m+1][n+0] |= !mask->image[1][0], eIm->image[m+1][n+1]  =  mask->image[1][1], eIm->image[m+1][n+2] |= !mask->image[1][2];
                  eIm->image[m+2][n+0] |= !mask->image[2][0], eIm->image[m+2][n+1] |= !mask->image[2][1], eIm->image[m+2][n+2] |= !mask->image[2][2];
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for
   cout << "erosion algorithm" << endl;
   
   return eIm;

}  // end imageErosion function

// Opening
Image2D* imageOpening(Image2D* inIm, Image2D* mask) {
   Image2D*oIm;  // Opened image
   Image2D*eIm;  // Eroded image

   //setMemoryAllocation();
   //oIm = (Image2D*)calloc(5, sizeof(int*));

   //for (int i = 0; i < 5; ++i)
   //   oIm[i] = (int*)calloc(5, sizeof(int));

   // algorithm: first erode then dilate
   eIm = imageErosion(inIm, mask);
   oIm = imageDilation(eIm, mask);  // TODO: fix memory leakage
   
   // release memory
   freeMemory(eIm);

   return oIm;
}  // end imageOpening function

// Closing
Image2D* imageClosing(Image2D* inIm, Image2D* mask) {
   Image2D*cIm;  // Closed image
   Image2D*dIm;  // Dilated image

   //cIm = (Image2D*)calloc(5, sizeof(int*));

   //for (int i = 0; i < 5; ++i)
   //   cIm[i] = (int*)calloc(5, sizeof(int));

   // algorithm: first dilate then erode
   dIm = imageDilation(inIm, mask);
   cIm = imageErosion(dIm, mask);   // TODO: fix memory leakage

   // release memory
   freeMemory(dIm);

   return cIm;
}  // end imageClosing function

// Gradient: Dilation - Erosion
Image2D* gradDilationErosion(Image2D* inIm, Image2D* mask) {
   Image2D*grad; // gradient
   Image2D*dIm;  // Dilated image
   Image2D*eIm;  // Eroded image

   grad = setMemoryAllocation(grad, 5, 5);

   //grad = (Image2D*)calloc(5, sizeof(int*));

   //for (int i = 0; i < 5; ++i)
   //   grad[i] = (int*)calloc(5, sizeof(int));

   dIm = imageDilation(inIm, mask);
   eIm = imageErosion(inIm, mask);

   // algorith: Dilated - Eroded
   for (size_t i = 0; i < 5; ++i) {
      for (size_t j = 0; j < 5; ++j) {
         grad->image[i][j] = dIm->image[i][j] & !eIm->image[i][j]; 
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

   grad = setMemoryAllocation(grad, 5, 5);

   //grad = (Image2D*)calloc(5, sizeof(int*));

   //for (int i = 0; i < 5; ++i)
   //   grad[i] = (int*)calloc(5, sizeof(int));

   oIm = imageOpening(inIm, mask);
   cIm = imageClosing(inIm, mask);

   // algorith: Closed - Opened
   for (size_t i = 0; i < 5; ++i) {
      for (size_t j = 0; j < 5; ++j) {
         grad->image[i][j] = cIm->image[i][j] & !oIm->image[i][j]; 
      }  // end for
   }  // end for

   // release memory
   freeMemory(oIm);
   freeMemory(cIm);

   return grad;
}  // end gradClosingOpening function

// set dynamic memory allocation
Image2D* setMemoryAllocation(Image2D* blockImage, size_t rows, size_t cols) {

   blockImage = (Image2D*)calloc(1, sizeof(Image2D));

   // sizing array 2D
   blockImage->nRows = rows;
   blockImage->nCols = cols;

   // dynamic memory allocation
   blockImage->image = (int**)calloc(blockImage->nRows, sizeof(int*));

   for (size_t k = 0; k < blockImage->nRows; ++k) {
      blockImage->image[k] = (int*)calloc(blockImage->nCols, sizeof(int));
   }  // end for

   return blockImage;
}  // end setMemoryAllocation function

// release memory
int freeMemory(Image2D* blockImage) {

   for (size_t k = 0; k < 5; ++k)
      free(blockImage->image[k]);
   free(blockImage->image);
   free(blockImage);

   return 0;   // success
}  // end freeMemory function
