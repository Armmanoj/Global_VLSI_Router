#include "Net.hpp"
#include "Netlist.hpp"
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
#include "Grid_Graph.hpp"
#include "Gamer.hpp"

// Constructor
Grid_Graph::Grid_Graph(std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    // storing the short, wire and via costs
    std::string line;
    infile >> L >> M >> N;
    infile >> wire_cost >> v; // Nlayers, xSize ySize
    std::cout << "wire and via costs " << wire_cost << " " << v << std::endl;
    float shortcosts;
    for (int k = 0; k<L; k++){
        infile >> shortcosts;
        short_cost.push_back(shortcosts);
    }
    std::cout << "Short costs are " << std::endl; 
    for(int m=0; m<short_cost.size();m++){
        std::cout << short_cost[m] << "";
    }
    std::cout << "\n";
    // Creating the vector of demand, G
    for (int k = 0; k < L; ++k) {
        if (k % 2 == 0) {
            G.push_back(std::vector<int>((M + 1) * N, 0));
        }
        else {
            G.push_back(std::vector<int>((N + 1) * M, 0));
        }
    }
    // printing G
    std::cout << "G is- " << std::endl;
    std::cout << G.size() << std::endl;
    for (const auto& row : G) {
        // Iterate through each element in the row (inner vector)
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }

    // creating the vector of capacity C
    std::cout << "Now to create C, L,M,N is " << L << " " << M << " " << N << std::endl;
    for (int k = 0; k < L; ++k) {
        // read the rest of the first line
        while (std::getline(infile, line)) {
            if (line.find("metal") != std::string::npos) {
                std::cout << line << "\n";
                // Stop processing when "metal" is found
                break;
            }
        }
        if (k % 2 == 0) {
            C.push_back(std::vector<int>((M + 1) * N, 0));
            for (int i = 0; i < M; ++i) {
                for (int j = 1; j <= N; ++j) {
                    // j=0 has C=0 ie, edge of chip but this is not given in .cap file format
                    infile >> C[k][i*(N+1)+j];
                }
            }
        }
        else {
            C.push_back(std::vector<int>((N + 1) * M, 0));
            for (int j = 1; j <= M; ++j) {
                // j = 0 has C=0, but not there in file format, also, file format is the transpose of this format
                for (int i = 0; i < N; ++i){
                    infile >> C[k][i*(M+1)+j];
                }
            }
        }
    } 
    // printing C
    std::cout << "C is- " << std::endl;
    std::cout << C.size() << std::endl;
    for (const auto& row : C) {
        // Iterate through each element in the row (inner vector)
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
}

// calculate number of electrical shorts in the chip
int Grid_Graph::overflows() {
    int count = 0;
    for (int k = 0; k < L; ++k) {
        if (k % 2 == 0) {
            for (int i = 0; i < (N + 1) * M; i++) {
                if (G[k][i] > C[k][i]) {
                    count+=G[k][i]-C[k][i];
                }
            }
        }
        else {
            for (int i = 0; i < (M + 1) * N; i++) {
                if (G[k][i] > C[k][i]) {
                    count+=G[k][i]-C[k][i];
                }
            }
        }
    }
    return count;
}

float Grid_Graph::weight(int l, int x, int y){
    if (l%2==0){
        // the return values of this function can be memoised for further speedup
        if (!C[l][x*(M+1)+y]){
            return short_cost[l];
        }
        return wire_cost-short_cost[l] + 2*short_cost[l]/(1+std::exp(-float(G[l][x*(M+1)+y])/float(C[l][x*(M+1)+y])));
    }
    else{
        if (!C[l][x*(M+1)+y]){
            return short_cost[l];
        }
        return wire_cost-short_cost[l] + 2*short_cost[l]/(1+std::exp(-float(G[l][y*(N+1)+x])/float(C[l][y*(N+1)+x])));
    }
}