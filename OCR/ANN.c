// Artificial Neural Neutwork

#include <stdio.h>

struct NetworkData {
    long[][] network;
    long[][] weight;
    long[][] bias;
}

    long[][] MultiplyMatrix(long[][] m1, long[][] m2, int lenRow, int lenColumn)
    {
      int i, j k, sum = 0;
      long m[lenRow][lenColumn];

        for (i = 0; i < lenRow; i++) {
          for (j = 0; j < lenColumn; j++) {
            for (k = 0; k < lenRow; k++) {
              sum = sum + first[i][k]*second[k][j];
            }

            m[i][j] = sum;
            sum = 0;
          }
        }

      return m;
    }



// Init Network
struct NetworkData InitNetwork(int layerNumber, int layerLenght)
{
    long network[layerNumber][layerLenght];
    long weight[layerNumber][layerLenght];
    long bias[layerNumber][layerLenght];

    int w, h;
    for(h = 0; h < layerNumber; h++)
    {
        for(w = 0; w < layerNumber; w++)
        {
            weight[h][w] = 0;
            bias[h][w] = 0;
        }
    }

    NetworkData NN = { network, weight, bias}

    return NN;
}

// Train

// Run

// Propagation

// Relu
    long Relu(long x)
    {
        return (x > 0 ? x : 0)
    }

// Softmax

