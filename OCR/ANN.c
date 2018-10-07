// Artificial Neural Neutwork

#include <stdio.h>
#include <math.h>

struct NeuralNetwork {
    double[][] layers;
    double[][] weights;
    double[][] bias;
	int layerNumber;
	int[] layerLength[];
}

double[][] MultiplyMatrix(double[][] m1, double[][] m2, int lenRow, int lenColumn)
{
  int i, j k, sum = 0;
  double m[lenRow][lenColumn];

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

// (3, [256, 128, 26])
// 26: [a-z]
 
// Init Network
struct NeuralNetwork InitNetwork(int layerNumber, int[] layerLength)
{
	int maxLength = 0, n;
	for(n = 0; n < layerNumber; n++)
	{
		if(maxLength < layerLength[n])
		{
			maxLength = layerLength[n];
		}
	}
	
    double layers[layerNumber][maxLength];
    double weightss[layerNumber-1][maxLength];
    double bias[layerNumber-1][maxLength];
	
    int w, h;
    for(h = 0; h < layerNumber; h++)
    {
        for(w = 0; w < layerNumber; w++)
        {
            weights[h][w] = 0;
            bias[h][w] = 0;
        }
    }

    NeuralNetwork NN = { layers, weights, bias, layerNumber, layerLength}

    return NN;
}

// Train

// Run
struct NeuralNetwork Run(struct NN, int[] input)
{
	struct NN_ = NN;
	NN_ = Input(NN_, input);
	NN_ = Forward(NN_, Sigmoid);
	
    return NN_;
}


// Input
struct NeuralNetwork Input(struct NN, int[] input)
{
	struct NN_ = NN;
	int i, layerLength = NN_.layerLength[0];
	
	for(i = 1; i < layerLength; i++)
	{
		NN_.layers[0][i] = input[i];
	}
	
    return NN_;
}

// Output
char Output(struct NN)
{
	char y_ = argmax(NN.layers[NN.layerNumber-1], NN.layerLength[NN.layerNumber-1]);
    return y_;
}

// BackPropagation


//////////////

struct NeuralNetwork Updateweightss(network, row, l_rate):
	for i in range(len(network)):
		inputs = row[:-1]
		if i != 0:
			inputs = [neuron['output'] for neuron in network[i - 1]]
		for neuron in network[i]:
			for j in range(len(inputs)):
				neuron['weightss'][j] += l_rate * neuron['delta'] * inputs[j]
			neuron['weightss'][-1] += l_rate * neuron['delta']


double Error(double output, double desired):
	return (1/2) * pow((desired - output), 2);
	
int DesiredOutputs(char y, int outputLength){
	int y_ = y;
	y_ -= 97;
	
	double[outputLength] outputs_;
	
	int i;
	for(i = 0; i < outputLength; i++)
	{
		outputs_[i] = (i == y_ ? 0.5 : (-0.5))
	}
	return outputs_;
}

//////////////

// Argmax
int Argmax(int[] output, outputLength)
{
	int y, y_value;
	int i;
	
	for(i = 0; i < outputLength; i++){
		if(y_value < output[i]){
			y_value = output[i];
			y = i;
		}
	}
	y += 97;
	
	return y;
}

// Forward Propagation
struct NeuralNetwork Forward(struct NN, double (*activation)(double))
{
	struct NN_ = NN;
	int i, j, k;
	double y = 0;
	
	for(i = 1; i < NN_.layerNumber; i++)
	{
		for(j = 0; j < NN_.layerLength[i]; j++)
		{
			y = 0;
			for(k = 0; k < NN_.layerLength[i-1]; k++)
			{
				y += (NN_.layers[i-1][k] * NN_.weights[i][k])
			}
			y += NN_.bias[i][j]
			y = (*activation)(y)
			NN_.layers[i][j] = y;
		}
	}
	
    return NN_;
}


// Relu
double Relu(double x)
{
	return (x > 0 ? x : 0);
}

// Sigmoid
double Sigmoid(double x)
{
	return 1/(1+exp(-x));
}

// Softmax

