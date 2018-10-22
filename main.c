// Neural Network
// gcc  -Wall -Wextra -Werror -std=c99 -O1 -o main *.c -lm


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


struct Neuron {
	double output;
	int weightsLength;
	double bias;
	double delta;
	double inputs[2];
	double weights[2];
};

// 2, 2, 1
struct NeuralNetwork {
	struct Neuron layers[2][2];
	int layerNumber; // 2
	int inputsLength; // 2
	int layerLength[2]; // = { 2, 1};
	double learningRate;
};


struct Dataset {
	double inputs[4][2]; // = 	{{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	double outputs[4][1]; // = 	{{0}, {1}, {1}, {0}};
	int datasetLength; // = 2;
};

double RandomFrom(double min, double max);
struct NeuralNetwork InitNetwork(struct NeuralNetwork NN);
struct NeuralNetwork Train(struct NeuralNetwork NN, struct Dataset dataset);
struct NeuralNetwork Input(struct NeuralNetwork NN, double input[]);
struct NeuralNetwork Backward(struct NeuralNetwork NN, double output[]);
struct NeuralNetwork UpdateWeights(struct NeuralNetwork NN, int i, int j);
struct NeuralNetwork Forward(struct NeuralNetwork NN, double (*activation)(double));
double Sigmoid(double x);
double Relu(double x);
double SigmoidDerivative(double output);
double ReluDerivative(double x);
int PrintXOR(struct NeuralNetwork NN);
int PrintNeuron(struct Neuron N, int i, int j);
struct NeuralNetwork Train2(struct NeuralNetwork NN, struct Dataset dataset);

int main ()
{
	srand (time(NULL));
	
	struct NeuralNetwork NN;
	struct Dataset dataset;

	NN.layerLength[0] = 2;
	NN.layerLength[1] = 1;
	NN.inputsLength = 2;

	NN.layerNumber = 2;
	
	NN.learningRate = 0.1;
	
	double _inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	double _outputs[4][1] = {{0}, {1}, {1}, {0}};
	
	int i, j;
	for(j = 0; j < 4; j++)
	{
		for(i = 0; i < 2; i++)
		{
			dataset.inputs[j][i] = _inputs[j][i];
		}
		dataset.outputs[j][0] = _outputs[j][0];
	}
	dataset.datasetLength = 4;

	NN = InitNetwork(NN);
	//PrintXOR(NN);
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
    int i, j, k;
    for(i = 0; i < NN.layerNumber; i++)
    {
        for(j = 0; j < NN.layerLength[i]; j++)
        {
			NN.layers[i][j].weightsLength = i > 0 ? NN.layerLength[i-1] : NN.inputsLength;
			for(k = 0; k < NN.layers[i][j].weightsLength; k++)
			{
				NN.layers[i][j].weights[k] = RandomFrom(-0.5, 0.5);
			}
            NN.layers[i][j].bias = RandomFrom(0, 1);
        }
    }

    return NN;
}

// Train
struct NeuralNetwork Train(struct NeuralNetwork NN, struct Dataset dataset)
{
	int epoch = 10000, inputIndex, iteration;

	// Load from JSON

	for(iteration = 0; iteration < epoch; iteration++){
		printf("--- EPOCH: %i --- \n", iteration);
		for(inputIndex = 0; inputIndex < dataset.datasetLength; inputIndex++)
		{
			NN = Input(NN, dataset.inputs[inputIndex]);
			NN = Forward(NN, Relu);
			printf("INPUT: %f %f  OUTPUT: %f \n", NN.layers[0][0].inputs[0], NN.layers[0][0].inputs[1], NN.layers[1][0].output);
			NN = Backward(NN, dataset.outputs[inputIndex]);
		}
	}
	PrintXOR(NN);
    return NN;
}


// Input
struct NeuralNetwork Input(struct NeuralNetwork NN, double input[])
{
	int j, k;

	for(j = 0; j < NN.layerLength[0]; j++)
	{
		for(k = 0; k < NN.inputsLength; k++)
		{
			NN.layers[0][j].inputs[k] = input[k];
		}
	}

    return NN;
}


// BackPropagation
struct NeuralNetwork Backward(struct NeuralNetwork NN, double output[])
{
	int i, j, k;
	for(i = NN.layerNumber-1; i >= 0; i--)
	{
		if(i != NN.layerNumber-1)
		{
			for(j = 0; j < NN.layerLength[i]; j++)
			{
				double error = 0;
				for(k = 0; k < NN.layerLength[i+1]; k++)
				{
					error += NN.layers[i+1][k].delta * NN.layers[i+1][k].weights[j];
				}
				NN.layers[i][j].delta = ReluDerivative(NN.layers[i][j].output) * error;
				NN = UpdateWeights(NN, i, j);
			}
		}
		else
		{
			for(j = 0; j < NN.layerLength[i]; j++)
			{
				NN.layers[i][j].delta = ReluDerivative(NN.layers[i][j].output) * (output[0] - NN.layers[i][j].output);
				NN = UpdateWeights(NN, i, j);
			}
		}
	}
	return NN;
}
//ADELE C EST TROP LA MEILLEURE D ABORDP OH YEAAAH HI HA 

		

struct NeuralNetwork UpdateWeights(struct NeuralNetwork NN, int i, int j)
{
	int k;
	for(k = 0; k < NN.layers[i][j].weightsLength; k++) 
	{
		NN.layers[i][j].weights[k] += NN.learningRate * NN.layers[i][j].delta * NN.layers[i][j].inputs[k];
	}
	
	NN.layers[i][j].bias += NN.learningRate * NN.layers[i][j].delta;
	return NN;
}

int PrintXOR(struct NeuralNetwork NN)
{
	int i, j;
	
	printf("--------------------------\n");
	for(i = 0; i < NN.layerNumber; i++)
	{
		for(j = 0; j < NN.layerLength[i]; j++) 
		{
			PrintNeuron(NN.layers[i][j], i, j);
		}
	}
	
	printf("--------------------------\n");
	return 0;
}

int PrintNeuron(struct Neuron N, int i, int j)
{
	printf("\nNeuron[%i][%i] =\n", i, j);
	int k;
	for(k = 0; k < N.weightsLength; k++)
	{
		printf("inputs[%i] = %f\n", k, N.inputs[k]);
	}
	for(k = 0; k < N.weightsLength; k++)
	{
		printf("weights[%i] = %f\n", k, N.weights[k]);
	}
	printf("bias = %f\n", N.bias);
	printf("delta = %f\n", N.delta);
	printf("output = %f\n\n", N.output);
	return 0;
}





// Forward Propagation
struct NeuralNetwork Forward(struct NeuralNetwork NN, double (*activation)(double))
{
	int i, j, k;
	double y = 0;

	for(i = 0; i < NN.layerNumber; i++)
	{
		for(j = 0; j < NN.layerLength[i]; j++)
		{
			if(i != 0)
			{
				for(k = 0; k < NN.layers[i][j].weightsLength; k++)
				{
					NN.layers[i][j].inputs[k] = NN.layers[i-1][k].output;
				}
			}
			
			y = 0;
			for(k = 0; k < NN.layers[i][j].weightsLength; k++)
			{
				y += (NN.layers[i][j].inputs[k] * NN.layers[i][j].weights[k]);
			}
			y += NN.layers[i][j].bias;
			y = (*activation)(y);
			NN.layers[i][j].output = y;
		}
	}
    return NN;
}


// Sigmoid
double Sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

double SigmoidDerivative(double output)
{
	return output * (1.0 - output);
}

// Reludouble ReluDerivative(double x)
double Relu(double x)
{
	return x > 0 ? x : 0;
}

double ReluDerivative(double x)
{
	return x > 0 ? 1 : 0;
}

