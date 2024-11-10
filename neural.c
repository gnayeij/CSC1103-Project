#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INPUT_NEURONS 9  // One for each board position
#define HIDDEN_NEURONS 18
#define OUTPUT_NEURONS 9  // Possible moves
#define LEARNING_RATE 0.1
#define EPOCHS 10000

// Neural Network Structure
typedef struct {
    double weights[HIDDEN_NEURONS][INPUT_NEURONS];
    double hidden_bias[HIDDEN_NEURONS];
    double output_weights[OUTPUT_NEURONS][HIDDEN_NEURONS];
    double output_bias[OUTPUT_NEURONS];
} NeuralNetwork;

// Activation function (ReLU)
double relu(double x) {
    return x > 0 ? x : 0;
}

// Derivative of ReLU
double relu_derivative(double x) {
    return x > 0 ? 1 : 0;
}

// Initialize neural network with random weights
void init_network(NeuralNetwork* nn) {
    srand(time(NULL));
    
    // Initialize hidden layer weights and biases
    for (int i = 0; i < HIDDEN_NEURONS; i++) {
        for (int j = 0; j < INPUT_NEURONS; j++) {
            nn->weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
        nn->hidden_bias[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
    
    // Initialize output layer weights and biases
    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        for (int j = 0; j < HIDDEN_NEURONS; j++) {
            nn->output_weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
        nn->output_bias[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
}

// Forward propagation
void forward_propagate(NeuralNetwork* nn, double* inputs, double* hidden_layer, double* outputs) {
    // Hidden layer
    for (int i = 0; i < HIDDEN_NEURONS; i++) {
        double sum = nn->hidden_bias[i];
        for (int j = 0; j < INPUT_NEURONS; j++) {
            sum += inputs[j] * nn->weights[i][j];
        }
        hidden_layer[i] = relu(sum);
    }
    
    // Output layer
    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        double sum = nn->output_bias[i];
        for (int j = 0; j < HIDDEN_NEURONS; j++) {
            sum += hidden_layer[j] * nn->output_weights[i][j];
        }
        outputs[i] = relu(sum);
    }
}

// Backpropagation
void backpropagate(NeuralNetwork* nn, double* inputs, double* hidden_layer, 
                   double* outputs, double* targets) {
    // Output layer errors
    double output_errors[OUTPUT_NEURONS];
    double hidden_errors[HIDDEN_NEURONS] = {0};
    
    // Calculate output layer errors
    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        output_errors[i] = (outputs[i] - targets[i]) * relu_derivative(outputs[i]);
        
        // Update output weights
        for (int j = 0; j < HIDDEN_NEURONS; j++) {
            nn->output_weights[i][j] -= LEARNING_RATE * output_errors[i] * hidden_layer[j];
        }
        nn->output_bias[i] -= LEARNING_RATE * output_errors[i];
    }
    
    // Calculate hidden layer errors
    for (int i = 0; i < HIDDEN_NEURONS; i++) {
        double error = 0;
        for (int j = 0; j < OUTPUT_NEURONS; j++) {
            error += output_errors[j] * nn->output_weights[j][i];
        }
        hidden_errors[i] = error * relu_derivative(hidden_layer[i]);
        
        // Update hidden weights
        for (int j = 0; j < INPUT_NEURONS; j++) {
            nn->weights[i][j] -= LEARNING_RATE * hidden_errors[i] * inputs[j];
        }
        nn->hidden_bias[i] -= LEARNING_RATE * hidden_errors[i];
    }
}

// Train the network
void train(NeuralNetwork* nn, double** training_inputs, double** training_outputs, 
          int training_size) {
    double hidden_layer[HIDDEN_NEURONS];
    double outputs[OUTPUT_NEURONS];
    
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0;
        
        for (int i = 0; i < training_size; i++) {
            // Forward propagation
            forward_propagate(nn, training_inputs[i], hidden_layer, outputs);
            
            // Calculate error
            for (int j = 0; j < OUTPUT_NEURONS; j++) {
                double error = outputs[j] - training_outputs[i][j];
                total_error += error * error;
            }
            
            // Backpropagation
            backpropagate(nn, training_inputs[i], hidden_layer, outputs, training_outputs[i]);
        }
        
        if (epoch % 1000 == 0) {
            printf("Epoch %d, Error: %f\n", epoch, total_error / training_size);
        }
    }
}

// Predict next move
int predict_move(NeuralNetwork* nn, double* board_state) {
    double hidden_layer[HIDDEN_NEURONS];
    double outputs[OUTPUT_NEURONS];
    
    forward_propagate(nn, board_state, hidden_layer, outputs);
    
    // Find the highest output (best move)
    int best_move = 0;
    double max_value = outputs[0];
    
    for (int i = 1; i < OUTPUT_NEURONS; i++) {
        if (outputs[i] > max_value && board_state[i] == 0) {  // Check if move is legal
            max_value = outputs[i];
            best_move = i;
        }
    }
    
    return best_move;
}

// Example usage
int main() {
    NeuralNetwork nn;
    init_network(&nn);
    
    // Example training data (you would need much more real data)
    double training_input1[9] = {0,0,0,0,1,0,0,0,0};  // Empty board with center taken
    double training_output1[9] = {0,0,1,0,0,0,0,0,0}; // Take top-right corner
    
    // Create training datasets
    double* training_inputs[] = {training_input1};
    double* training_outputs[] = {training_output1};
    
    // Train the network
    train(&nn, training_inputs, training_outputs, 1);
    
    // Example game state
    double current_board[9] = {0,0,0,0,1,0,0,0,0};
    
    // Get prediction
    int move = predict_move(&nn, current_board);
    printf("Recommended move: %d\n", move);
    
    return 0;
}