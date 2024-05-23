/*
    Numerical simulation of the systems presented in chapter 7 of Stochastic Modelling for Systems Biology by Wilkinson 
    ("Dimerisation kinetics" and "Michaelis-Menten kinetics") using SSA.
    
    argv[1] - maximum simulation time

    Compilation command: 
    g++ SSA_mm.cpp -o SSA_mm -std=c++11
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <string>
#include <cassert>

void SSA(double& tfinal, std::vector<std::vector<double>>& V, std::vector<double>& X, std::vector<double>& c);

int main(int argc, char* argv[])
{
    //---- Michaelis-Menten enzyme kinetics
    //Initial quantity of molecules
    std::vector<double> X = {301.0, 120.0, 0.0, 0.0};          
    
    // Stoichiometric matrix
    std::vector<std::vector<double>> V = {
        {{-1,  1,  0},
        {-1,  1,  1},
        { 1, -1, -1},
        { 0,  0,  1}}
    };

    // Reaction parameters
    std::vector<double> c = {1.66*pow(10, -3), pow(10, -4), 0.1};

    // Maximum simulation time
    double tfinal = std::stod(argv[1]);

    SSA(tfinal, V, X, c);

    return 0;
}

void SSA(double& tfinal, std::vector<std::vector<double>>& V, std::vector<double>& X, std::vector<double>& c)
{
    int j = 0, cumsumSize, XSize;
    double asum, rand1, rand2, tau, t=0;
    std::vector<double> a, cumsum;

    // Gerador de números aleatórios Mersenne Twister
    std::mt19937_64 mt(time(nullptr));
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    // Para exportar os resultados para arquivos:
    std::ofstream write_output1("x.dat", std::ios::app);
    std::ofstream write_output2("y.dat", std::ios::app);
    assert(write_output1.is_open());   
    assert(write_output2.is_open());

    while(t < tfinal)
    {   
        //---- Passo 1: determinar qual reação vai ocorrer (reação j)
        // Função de propensidade, um item para cada reação possível
        a = {c[0]*X[0]*X[1], c[1]*X[2], c[2]*X[2]}; 

        // Soma dos itens de a
        asum = 0.0;
        for (double item : a) asum += item;

        // Soma acumulativa dos valores de a divididor por asum
        cumsum = {a[0]/asum, (a[0]+a[1])/asum, (a[0]+a[1]+a[2])/asum};

        // Geração do primeiro número aleatório
        rand1 = dis(mt);

        // Determinar o *índice* do menor dos itens de cumsum que são maiores que esse número aleatório
        cumsumSize = cumsum.size();
        for (size_t i = 0; i < cumsumSize; i++)
        {
            if (cumsum[i] > rand1){j = i; break;}
        }

        //---- Passo 2: determinar quando a reação j irá ocorrer (tau)
        // Geração do segundo número aleatório
        rand2 = dis(mt);

        // O número aleatório rand2 passa por transformação inversa para que tau siga uma distribuição exponencial
        tau = log(1/rand2)/asum;
    
        //---- Passo 3: atualizar vetor de estados após ocorrer a reação j, isto é, X += coluna j da matriz estequiométrica V
        XSize = X.size();
        for (size_t i = 0; i < XSize; ++i) 
        {
            write_output1 << X[i] << " \n";
            X[i] += V[i][j];
        }
        write_output1 << "\n";

        write_output2 << t << " \n";

        //---- Passo 4: atualizar o tempo
        t += tau;
    }

    write_output1.close();
    write_output2.close();
}
