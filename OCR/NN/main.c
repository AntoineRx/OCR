// Neural Network
// gcc  -Wall -Wextra -Werror -std=c99 -O1 -o main *.c -lm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct NeuralNetwork {
	double layers[3][4];
	double weights[3][4];
	double biases[3][4];
	double deltas[3][4];
	int layerNumber; // 3
	int layerLength[3]; // = {2, 4, 1};
	int maxLength; // = 2;
	double learningRate;
};

struct Dataset {
	double inputs[4][2]; // = 	{{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	
	double outputs[4][1]; // = 	{{0}, {1}, {1}, {0}};
	
	int datasetLength; // = 4;
};

double RandomFrom(double min, double max);
struct NeuralNetwork InitNetwork(struct NeuralNetwork NN);
struct NeuralNetwork Train(struct NeuralNetwork NN, struct Dataset dataset);
struct NeuralNetwork Run(struct NeuralNetwork NN, double input[]);
struct NeuralNetwork Input(struct NeuralNetwork NN, double input[]);
char Output(struct NeuralNetwork NN);
struct NeuralNetwork Backward(struct NeuralNetwork NN, double output[]);
struct NeuralNetwork UpdateWeights(struct NeuralNetwork NN);
struct NeuralNetwork Forward(struct NeuralNetwork NN, double (*activation)(double));
double Error(double output, double desired);
double Sigmoid(double x);
double TransfertDerivative(double output);
int Argmax(double output[], int outputLength);
int PrintXOR(struct NeuralNetwork NN);

int main ()
{
	srand (time ( NULL));
	
	struct NeuralNetwork NN;
	struct Dataset dataset;

	NN.layerLength[0] = 2;
	NN.layerLength[1] = 4;
	NN.layerLength[2] = 1;

	NN.layerNumber = 3;
	NN.maxLength = 4;
	
	NN.learningRate = 0.5; //0.01;
	
	double _inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	double _outputs[4][1] = {{0}, {1}, {1}, {0}};
	
	int i, j;
	for(j = 0; j < 4; j++)
	{
		for(i = 0; i < 2; i++)
		{
			dataset.inputs[j][i] = _inputs[j][i];
			dataset.outputs[j][0] = _outputs[j][0];
		}
	}
	dataset.datasetLength = 4;

	NN = InitNetwork(NN);
	NN = Train(NN, dataset);

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
struct NeuralNetwork InitNetwork(struct NeuralNetwork NN)
{
    int w, h;
    for(h = 0; h < NN.layerNumber; h++)
    {
        for(w = 0; w < NN.layerLength[h]; w++)
        {
            NN.weights[h][w] = RandomFrom(0, 1);
            NN.biases[h][w] = RandomFrom(0, 1);
        }
    }

    return NN;
}

// Train
struct NeuralNetwork Train(struct NeuralNetwork NN, struct Dataset dataset)
{
	int epoch = 1000, inputIndex, iteration;

	// Load from JSON

	for(iteration = 0; iteration < epoch; iteration++){
		printf("--- EPOCH: %i --- \n", iteration);
		for(inputIndex = 0; inputIndex < dataset.datasetLength; inputIndex++)
		{
			NN = Input(NN, dataset.inputs[inputIndex]);
			printf("INPUT: %f | %f\n", NN.layers[0][0], NN.layers[0][1]);
			
			NN = Forward(NN, Sigmoid);
			printf("OUTPUT: %f \n", NN.layers[2][0]);
			
			
			//PrintXOR(NN);
			NN = Backward(NN, dataset.outputs[inputIndex]);
			
			//PrintXOR(NN);
			//printf("weight= %f\n", NN.weights[1][0]);
			NN = UpdateWeights(NN);
			//PrintXOR(NN);
			//printf("weight= %f\n", NN.weights[1][0]);
		}
	}

    return NN;
}


// Run
struct NeuralNetwork Run(struct NeuralNetwork NN, double input[])
{
	struct NeuralNetwork NN_ = NN;
	NN_ = Input(NN_, input);
	NN_ = Forward(NN_, Sigmoid);

    return NN_;
}


// Input
struct NeuralNetwork Input(struct NeuralNetwork NN, double input[])
{
	struct NeuralNetwork NN_ = NN;
	int i, layerLength = NN_.layerLength[0];

	for(i = 0; i < layerLength; i++)
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
struct NeuralNetwork Backward(struct NeuralNetwork NN, double output[])
{
	int i, j, k;
	for(i = NN.layerNumber-1; i >= 0; i--)
	{
		//printf("______I: %i\n", i);
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
				//printf("***ERROR***: %f\n", error);
				errors[j] = error;
			}
		}
		else
		{
			for(j = 0; j < NN.layerLength[i]; j++)
			{
				errors[j] = output[j] - NN.layers[i][j];
				//printf("ERROR: %f\n", errors[0]);
			}
		}

		for(j = 0; j < NN.layerLength[i]; j++)
		{
			NN.deltas[i][j] = errors[j] * TransfertDerivative(NN.layers[i][j]);
			//printf("deltas[%i][%i] =  %f\n", i, j, NN.deltas[i][j]);
		}
	}
	return NN;
}
		
		

struct NeuralNetwork UpdateWeights(struct NeuralNetwork NN)
{
	int i, j, k, n, inputsLength;

	for(i = 0; i < NN.layerNumber; i++)
	{
			double inputs[NN.maxLength];
		
			if(i != 0)
			{			
				inputsLength = NN.layerLength[i - 1];
				for(n = 0; n < inputsLength; n++)
				{
					inputs[n] = NN.layers[i - 1][n];
					//printf("inputs[%i] = %lf\n", n, inputs[n]);
				}
			}else
			{
				inputsLength = NN.layerLength[i];
				for(n = 0; n < inputsLength; n++)
				{
					inputs[n] = NN.layers[i][n];
					//printf("inputs[%i] = %lf\n", n, inputs[n]);
				}
			}
		
			for(j = 0; j < NN.layerLength[i]; j++) 
			{
				for(k = 0; k < inputsLength; k++)
				{
					//printf("Weight___ = %lf\n", NN.deltas[i][j] * inputs[k]);
					NN.weights[i][j] += NN.learningRate * NN.deltas[i][j] * inputs[k];
				}
				//printf("Bias____ = %lf\n", NN.learningRate * NN.deltas[i][j]);
				NN.biases[i][j] += NN.learningRate * NN.deltas[i][j];
			}
	}
	return NN;
}

int PrintXOR(struct NeuralNetwork NN)
{
	
	printf("LAYERS________________\n");
	printf("| %f |", NN.layers[0][0]);
	printf(" %f |\n", NN.layers[0][1]);
	
	
	printf("| %f ", NN.layers[1][0]);
	printf("| %f ", NN.layers[1][1]);
	printf("| %f ", NN.layers[1][2]);
	printf("| %f |\n", NN.layers[1][3]);
	
	printf("| %f |\n", NN.layers[2][0]);
	printf("_____________\n");
	
	printf("WEIGHTS________________\n");
	printf("| %f |", NN.weights[0][0]);
	printf(" %f |\n", NN.weights[0][1]);
	
	
	printf("| %f ", NN.weights[1][0]);
	printf("| %f ", NN.weights[1][1]);
	printf("| %f ", NN.weights[1][2]);
	printf("| %f |\n", NN.weights[1][3]);
	
	printf("| %f |\n", NN.weights[2][0]);
	printf("_____________\n");
	
	printf("BIASES________________\n");
	printf("| %f |", NN.biases[0][0]);
	printf(" %f |\n", NN.biases[0][1]);
	
	
	printf("| %f ", NN.biases[1][0]);
	printf("| %f ", NN.biases[1][1]);
	printf("| %f ", NN.biases[1][2]);
	printf("| %f |\n", NN.biases[1][3]);
	
	printf("| %f |\n", NN.biases[2][0]);
	printf("_____________\n");
	
	printf("DELTAS________________\n");
	printf("| %f |", NN.deltas[0][0]);
	printf(" %f |\n", NN.deltas[0][1]);
	
	
	printf("| %f ", NN.deltas[1][0]);
	printf("| %f ", NN.deltas[1][1]);
	printf("| %f ", NN.deltas[1][2]);
	printf("| %f |\n", NN.deltas[1][3]);
	
	printf("| %f |\n", NN.deltas[2][0]);
	printf("_____________\n");
	return 0;
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

	double outputs_[outputLength];

	int i;
	for(i = 0; i < outputLength; i++)
	{
		outputs_[i] = (i == y_ ? 0.5 : (-0.5));
	}
	return outputs_[0];
}

//////////////

// Argmax
int Argmax(double output[], int outputLength)
{
	int y = 0, y_value = 0;
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
				y += (NN_.layers[i-1][k] * NN_.weights[i-1][k]);
			}
			y += NN_.biases[i][j];
			y = (*activation)(y);
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

