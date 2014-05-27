// morphologicalOperations.cpp
// Computes the morphological operator upon an image
// **first draft** may contain bugs
#include <iostream>
#include <stdlib.h>
using namespace std;

// prototypes
int** imageDilation(int[][5], int[][3]);
int** imageErosion(int[][5], int[][3]);
int** imageOpening(int inIm[][5], int mask[][3]);
int** imageClosing(int inIm[][5], int mask[][3]);
int** gradDilationErosion(int inIm[][5], int mask[][3]);
int** gradClosingOpening(int inIm[][5], int mask[][3]);
int freeMemory(int**);

int main() {
   int **dilatada;   // dilated image
   int **erosionada; // eroded image
   int **apertura;   // opened image
   int **cerradura;  // closed image
   int **gradiente;  // gradient: dilated - eroded
   int **grad2;      // gradiente: closed - opened
   
   int image[5][5] = {{0, 0, 0, 0, 0}, 
                      {0, 1, 1, 1, 0}, 
                      {0, 1, 1, 1, 0}, 
                      {0, 1, 1, 1, 0}, 
                      {0, 0, 0, 0, 0}};

   int image2[5][5] = {{0, 1, 1, 1, 0}, 
                       {0, 1, 1, 1, 0}, 
                       {0, 1, 1, 1, 0}, 
                       {0, 1, 1, 1, 0}, 
                       {0, 0, 0, 0, 0}};

   int mascara[3][3] = {{1, 1, 1}, 
                        {1, 1, 1}, 
                        {1, 1, 1}};

   int mascara2[3][3] = {{0, 1, 0}, 
                         {1, 1, 1}, 
                         {0, 1, 0}};

   dilatada = imageDilation(image, mascara2);

   erosionada = imageErosion(image, mascara2);

   apertura = imageOpening(image, mascara2);

   cerradura = imageClosing(image, mascara2);

   gradiente = gradDilationErosion(image, mascara2);

   grad2 = gradClosingOpening(image, mascara2);

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
   //
   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << dilatada[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << erosionada[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << cerradura[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << apertura[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << gradiente[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for

   cout << endl;

   for (int a = 0; a <= 4; ++a) {
      for (int b = 0; b <= 4; ++b) {
         cout << grad2[a][b] << " ";
      }  // end for
      cout << endl;
   }  // end for
   
   //cout << "~1 " << ~1 << endl; 
   //cout << "!1 " << !1 << endl; 

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
int** imageDilation(int inIm[][5], int mask[][3]) {
   int** dIm;   // Dilated image

   dIm = (int**)calloc(5, sizeof(int*));

   for (int i = 0; i < 5; ++i)
      dIm[i] = (int*)calloc(5, sizeof(int));

   // Dilation algorithm
   for (size_t i = 0; i < 5 - 2; ++i) {
      for (size_t j = 0; j < 5 - 2; ++j) {
         if (inIm[i+1][j+1] & mask[1][1]) {
            for (size_t m = 0 + i; m < 1 + i - 0; ++m) {
               for (size_t n = 0 + j; n < 1 + j - 0; ++n) {
                  dIm[m+0][n+0] |= mask[0][0], dIm[m+0][n+1] |= mask[0][1], dIm[m+0][n+2] |= mask[0][2];
                  dIm[m+1][n+0] |= mask[1][0], dIm[m+1][n+1]  = mask[1][1], dIm[m+1][n+2] |= mask[1][2];
                  dIm[m+2][n+0] |= mask[2][0], dIm[m+2][n+1] |= mask[2][1], dIm[m+2][n+2] |= mask[2][2];
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for
   cout << "dilation algorithm" << endl;
   
   return dIm;
}  // end imageDilation function

// Erosion
int** imageErosion(int inIm[][5], int mask[][3]) {
   int** eIm;   // Eroded image

   eIm = (int**)calloc(5, sizeof(int*));

   for (int i = 0; i < 5; ++i)
      eIm[i] = (int*)calloc(5, sizeof(int));

   // Erosion algorithm
   for (size_t i = 0; i < 5 - 2; ++i) {
      for (size_t j = 0; j < 5 - 2; ++j) {   
         // TODO: find a way, if any, to make generic the decision mechanism considering any mask
         // Mask 1
         //if ((inIm[i+0][j+0] & mask[0][0]) & (inIm[i+0][j+1] & mask[0][1]) & (inIm[i+0][j+2] & mask[0][2]) &
         //    (inIm[i+1][j+0] & mask[1][0]) & (inIm[i+1][j+1] & mask[1][1]) & (inIm[i+1][j+2] & mask[1][2]) &
         //    (inIm[i+2][j+0] & mask[2][0]) & (inIm[i+2][j+1] & mask[2][1]) & (inIm[i+2][j+2] & mask[2][2])) {
         // Mask 2
         if (/*(inIm[i+0][j+0] & mask[0][0]) & */(inIm[i+0][j+1] & mask[0][1]) /* & (inIm[i+0][j+2] & mask[0][2])*/ &
             (inIm[i+1][j+0] & mask[1][0]) & (inIm[i+1][j+1] & mask[1][1]) & (inIm[i+1][j+2] & mask[1][2]) &
             /*(inIm[i+2][j+0] & mask[2][0]) & */(inIm[i+2][j+1] & mask[2][1]) /*& (inIm[i+2][j+2] & mask[2][2])*/) {
            for (size_t m = 0 + i; m < 1 + i - 0; ++m) {
               for (size_t n = 0 + j; n < 1 + j - 0; ++n) {
                  eIm[m+0][n+0] |= !mask[0][0], eIm[m+0][n+1] |= !mask[0][1], eIm[m+0][n+2] |= !mask[0][2];
                  eIm[m+1][n+0] |= !mask[1][0], eIm[m+1][n+1]  =  mask[1][1], eIm[m+1][n+2] |= !mask[1][2];
                  eIm[m+2][n+0] |= !mask[2][0], eIm[m+2][n+1] |= !mask[2][1], eIm[m+2][n+2] |= !mask[2][2];
               }  // end for
            }  // end for
         }  // end if
      }  // end for
   }  // end for
   cout << "erosion algorithm" << endl;
   
   return eIm;

}  // end imageErosion function

// Opening
int** imageOpening(int inIm[][5], int mask[][3]) {
   int **oIm;  // Opened image
   int **eIm;  // Eroded image
   int aux[5][5];

   oIm = (int**)calloc(5, sizeof(int*));

   for (int i = 0; i < 5; ++i)
      oIm[i] = (int*)calloc(5, sizeof(int));

   // algorithm: first erode then dilate
   eIm = imageErosion(inIm, mask);
   for (size_t i = 0; i < 5; ++i) {
      for (size_t j = 0; j < 5; ++j) {
         aux[i][j] = eIm[i][j];  // TODO: optimize
      }  // end for
   }  // end for
   oIm = imageDilation(aux, mask);  // TODO: fix memory leakage
   
   // release memory
   freeMemory(eIm);

   return oIm;
}  // end imageOpening function

// Closing
int** imageClosing(int inIm[][5], int mask[][3]) {
   int **cIm;  // Closed image
   int **dIm;  // Dilated image
   int aux[5][5];

   cIm = (int**)calloc(5, sizeof(int*));

   for (int i = 0; i < 5; ++i)
      cIm[i] = (int*)calloc(5, sizeof(int));

   // algorithm: first dilate then erode
   dIm = imageDilation(inIm, mask);
   for (size_t i = 0; i < 5; ++i) {
      for (size_t j = 0; j < 5; ++j) {
         aux[i][j] = dIm[i][j];  // TODO: optimize
      }  // end for
   }  // end for
   cIm = imageErosion(aux, mask);   // TODO: fix memory leakage

   // release memory
   freeMemory(dIm);

   return cIm;
}  // end imageClosing function

// Gradient: Dilation - Erosion
int** gradDilationErosion(int inIm[][5], int mask[][3]) {
   int **grad; // gradient
   int **dIm;  // Dilated image
   int **eIm;  // Eroded image

   grad = (int**)calloc(5, sizeof(int*));

   for (int i = 0; i < 5; ++i)
      grad[i] = (int*)calloc(5, sizeof(int));

   dIm = imageDilation(inIm, mask);
   eIm = imageErosion(inIm, mask);

   // algorith: Dilated - Eroded
   for (size_t i = 0; i < 5; ++i) {
      for (size_t j = 0; j < 5; ++j) {
         grad[i][j] = dIm[i][j] & !eIm[i][j]; 
      }  // end for
   }  // end for

   // release memory
   freeMemory(dIm);
   freeMemory(eIm);

   return grad;
}  // end gradDilationErosion function

// Gradient: Closing - Opening
int** gradClosingOpening(int inIm[][5], int mask[][3]) {
   int **grad; // gradient
   int **oIm;  // Opened image
   int **cIm;  // Closed image

   grad = (int**)calloc(5, sizeof(int*));

   for (int i = 0; i < 5; ++i)
      grad[i] = (int*)calloc(5, sizeof(int));

   oIm = imageOpening(inIm, mask);
   cIm = imageClosing(inIm, mask);

   // algorith: Closed - Opened
   for (size_t i = 0; i < 5; ++i) {
      for (size_t j = 0; j < 5; ++j) {
         grad[i][j] = cIm[i][j] & !oIm[i][j]; 
      }  // end for
   }  // end for

   // release memory
   freeMemory(oIm);
   freeMemory(cIm);

   return grad;
}  // end gradClosingOpening function

// release memory
int freeMemory(int** blockImage) {

   for (size_t i = 0; i < 5; ++i)
      free(blockImage[i]);
   free(blockImage);

   return 0;
}
