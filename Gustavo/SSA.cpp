
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <random>
#include <iomanip>

/*
    argv[1] - time limit
    argv[2] - number of molecules
    argv[3] - number of reactions
    argv[4] - File name
*/

// SET MANUALLY: Initial quantities of molecules, Reactions probability equations, Reaction Results. 
int main(int argc, char* argv[]){
    
    // Variables 
    double time, total, leap, cap, prob_sum, max_time, print_leap, sum; 
    int n_molec, n_react, reaction;
    std::ofstream file; 

    max_time = 100;
    n_molec = 2;
    n_react = 2;
    file.open("data.dat");

    double x[n_molec];
    double prob[n_react]; 
    time = 0; 

    // SET MANUALLY: Initial quantities of each molecule 
    x[0] = 100; x[1] = 0; 

    while(time < max_time){

        // SET MANUALLY: Probability Distribution for each reaction 
        prob[0] = x[0];
        prob[1] = x[1];
        

        prob_sum = 0; for(int k = 0; k < n_react; k++) prob_sum += prob[k];
        total = 0;    for(int k = 0; k < n_molec; k++) total += x[k];

        std::random_device rd;
        std::mt19937 generator(rd());
        // Define Exponential and Uniform distributions used
        std::exponential_distribution<double> distr(prob_sum);
        std::uniform_real_distribution<> unif(0, 1);

        // Generate leap and random number in [0, 1]
        cap = unif(generator) * prob_sum;
        leap = distr(generator);

        // Define which reaction will take place 
        sum = 0; 
        for(int c=0; c < n_react; c++){ // * c < n_reactions
            sum += prob[c];
            if(sum >= cap){
                reaction = c;
                break;
            } 
        } 

        // SET MANUALLY: Reaction Results  
        if(reaction == 0){
            x[0]--;
            x[1]++;
    }   else if(reaction == 1){
           x[1]--;
            x[0]++;
    }

        // Export results to file 
        time += leap;
        #pragma omp critical
        file << std::setprecision(4) << std::fixed << time;
        for(int k = 0; k < n_molec; k++){
           file << " " << x[k];        
        }
        file << "\n";
    }
    file.close();
} 
