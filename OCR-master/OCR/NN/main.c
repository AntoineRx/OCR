// Neural Network

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct NeuralNetwork {
	double layers[3][2];
	double weights[3][2];
	double biases[3][2];
	double deltas[3][2];
	int layerNumber; // 3
	int layerLength[3]; // = {2, 2, 1};
	int maxLength; // = 2;
};

struct Dataset {
	int inputs[4][2]; // = 	{{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	
	int outputs[4][1]; // = 	{{0}, {1}, {1}, {0}};
	
	int datasetLength; // = 2;
};

double RandomFrom(double min, double max);
struct NeuralNetwork InitNetwork(int layerNumber, int layerLength[]);
struct NeuralNetwork Train(struct NeuralNetwork NN, struct Dataset dataset);
struct NeuralNetwork Run(struct NeuralNetwork NN, int input[]);
struct NeuralNetwork Input(struct NeuralNetwork NN, int input[]);
char Output(struct NeuralNetwork NN);
struct NeuralNetwork Backward(struct NeuralNetwork NN);
struct NeuralNetwork UpdateWeights(struct NeuralNetwork NN, double learningRate);
struct NeuralNetwork Forward(struct NeuralNetwork NN, double (*activation)(double));
double Error(double output, double desired);
double Sigmoid(double x);
double TransfertDerivative(double output);
int Argmax(double output[], int outputLength);

int main ()
{
	printf("First Try !");
	return 0;
}

double RandomFrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}


// (3, [256, 128, 26])
// 26: [a-z]

// Init Network
struct NeuralNetwork InitNetwork(int layerNumber, int layerLength[])
{
	int maxLength = 0, n;
	for(n = 0; n < layerNumber; n++)
	{
		if(maxLength < layerLength[n])
		{
			maxLength = layerLength[n];
		}
	}

    //double layers[layerNumber][maxLength];
    //double weights[layerNumber-1][maxLength];
    //double biases[layerNumber-1][maxLength];
	
	struct NeuralNetwork NN;
	

	
    int w, h;
    for(h = 0; h < NN.layerNumber; h++)
    {
        for(w = 0; w < NN.layerNumber; w++)
        {
            NN.weights[h][w] = RandomFrom(-0.5, 0.5);
            NN.biases[h][w] = RandomFrom(-0.5, 0.5);
        }
    }

    //NeuralNetwork NN; = { layers, weights, biases, layerNumber, layerLength}

    return NN;
}

// Train
struct NeuralNetwork Train(struct NeuralNetwork NN, struct Dataset dataset)
{
	int epoch = 100, inputIndex, iteration;
	double learningRate = 0.01;

	// Load from JSON

	struct NeuralNetwork NN_ = NN;
	for(iteration = 0; iteration < epoch; iteration++){
		for(inputIndex = 0; inputIndex < dataset.datasetLength; inputIndex++)
		{
			NN_ = Input(NN_, dataset.inputs[inputIndex]);
			NN_ = Forward(NN_, Sigmoid);
			NN_ = Backward(NN_);
			NN_ = UpdateWeights(NN_, learningRate);
		}
	}

    return NN_;
}


// Run
struct NeuralNetwork Run(struct NeuralNetwork NN, int input[])
{
	struct NeuralNetwork NN_ = NN;
	NN_ = Input(NN_, input);
	NN_ = Forward(NN_, Sigmoid);

    return NN_;
}


// Input
struct NeuralNetwork Input(struct NeuralNetwork NN, int input[])
{
	struct NeuralNetwork NN_ = NN;
	int i, layerLength = NN_.layerLength[0];

	for(i = 1; i < layerLength; i++)
	{
		NN_.layers[0][i] = input[i];
	}

    return NN_;
}

// Output
char Output(struct NeuralNetwork NN)
{
	char y_ = Argmax(NN.layers[NN.layerNumber-1], NN.layerLength[NN.layerNumber-1]);
    return y_;
}

// BackPropagation
struct NeuralNetwork Backward(struct NeuralNetwork NN)
{
	int i, j, k;

	for(i = NN.layerNumber-1; i >= 0; i--)
	{
		double errors[NN.layerLength[NN.layerNumber]];
		if(i != NN.layerNumber-1)
		{
			for(j = 0; j < NN.layerLength[i]; j++)
			{
				double error = 0;
				for(k = 0; k < NN.layerLength[i+1]; k++)
				{
					error += NN.weights[i][j] * NN.deltas[i+1][k];
				}
				errors[j] = error;
			}
		}
		else // if beginning backprop (on ouput)
		{
			for(j = 0; j < NN.layerLength[i]; j++)
			{
				errors[j] = NN.expected[j] - NN.layers[i][j];
			}
		}

		for(j = 0; j < NN.layerLength[i]; j++)
		{
			deltas[i][j] = errors[j] * TransfertDerivative(NN.layers[i][j]);
		}
	}
}

struct NeuralNetwork UpdateWeights(struct NeuralNetwork NN, double learningRate)
{
	int i, j, inputsLength;
	double[] inputs;

	for(i = 0; i < layerNumber; i++)
	{
		for(j = 0; j < layerLength[NN.layerNumber]; j++)
		{
			if(i == 0){
				inputs = layers[0][j];
				inputsLength = 1;
			}else{
				inputs = layers[i - 1];
				inputsLength = layerLength[i - 1];
			}

			for(k = 0; k < inputsLength; k++)
			{
				NN.weights[i][j] += NN.learningRate * NN.deltas[i][j] * inputs[k];
			}
			NN.biases[i][j] += NN.learningRate * NN.deltas[i][j];
		}
	}

	return NN;
}


double Error(double output, double desired)
{
	return (1/2) * pow((desired - output), 2);
}

double TransfertDerivative(double output)
{
	return output * (1.0 - output);
}

int DesiredOutputs(char y, int outputLength)
{
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
int Argmax(double output[], int outputLength)
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
struct NeuralNetwork Forward(struct NeuralNetwork NN, double (*activation)(double))
{
	struct NeuralNetwork NN_ = NN;
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
			y += NN_.biases[i][j]
			y = (*activation)(y)
	struct		NN_.layers[i][j] = y;
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

