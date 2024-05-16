
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <random>
#include <iomanip>
#include <vector>   

/*
    argv[1] - time limit
    argv[2] - number of molecules
    argv[3] - number of reactions
    argv[4] - Tau
    argv[5] - File name 
*/
void tau_leaping(void){
    // Variables 
    int total, n_molec, n_react, cont; 
    double leap, time, max_time; 
    std::ofstream file; 
    cont = 0;
    max_time = 100;
    n_molec = 4;
    n_react = 3;
    leap = 0.1;
    file.open("data.dat");


    int react[n_react];   // Number of ocurrences of reach reaction over Tau interval
    double prob[n_molec]; // Probability vector for each reaction
    time = 0;
    int x[n_molec];       // Molecule vector

    // SET MANUALLY: Stanting quantity for each molecule 
    x[0] = 301; x[1] = 120; x[2] = 0; x[3] = 0; 
    while(time <= max_time){
        cont++; 
        prob[0] = 1.66e-3 * x[0] * x[1];
        prob[1] = 1e-4 * x[2];
        prob[2] = 0.1 * x[2];
        
        total = 0; 
        for(int k = 0; k < n_react; k++) total += x[k];
        std::random_device rd;
        std::mt19937 gen(rd());
        // Define distributions
        for(int k = 0; k<n_react; k++){
            std::poisson_distribution<int> distrP(prob[k] * leap);
            react[k] = distrP(gen);
        }
        std::uniform_real_distribution<double> d(0, 1);

        // SET MANUALLY: Reactions results 
        for(int i = 0; i < react[0]; i++){
            x[0]--; 
            x[1]--; 
            x[2]++;
    }   for(int i = 0; i < react[1]; i++){
            x[2]--;
            x[0]++; 
            x[1]++;
    }   for(int i = 0; i < react[2]; i++){
            x[2]--;
            x[1]++;
            x[3]++;
    }
        time += leap;
        std::vector<int> time_result;
        time_result.push_back(time);
        file << std::setprecision(4) << std::fixed << time;
        for(int k = 0; k < n_molec; k++){
           file << " " << x[k];        
        }
        file << "\n";
    }
    file.close();
} 

int main(void){ 
    tau_leaping();
} 
