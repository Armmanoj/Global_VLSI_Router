#include "main.hpp"
#include "Grid_Graph.hpp"
#include "struct.hpp"

// Constructor
Grid_Graph::Grid_Graph(std::string filename,  float v_val) :  v(v_val) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }
    // storing the short, wire and via costs
    infile L >> M >> N >> wire_cost >> via_cost; // Nlayers, xSize ySize
    float shortcosts;
    for (int k = 0; k<L; k++){
        infile >> shortcosts;
        short_cost.push_back(shortcosts);
    }
    // Creating the vector of demand, G
    for (int k = 0; k < L; ++k) {
        if (k % 2 == 0) {
            G.push_back(std::vector<int>((M + 1) * N, 0));
        }
        else {
            G.push_back(std::vector<int>((N + 1) * M, 0));
        }
    }
    // creating the vector of capacity C
    std::string line;
    for (int k = 0; k < L; ++k) {
        std::getline(infile, line); // read the rest of the first line
        while (line.find("metal") == std::string::npos){
            std::getline(infile, line);
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
                for (int i = 0; i < N; ++i) {
                    infile >> C[k][i*(M+1)+j];
                }
            }
        }
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
        return wire_cost-short_cost + 2*overflow_cost/(1+std::exp(-float(G.G[l][Src.x*(G.M+1)+y])/float(G.C[l][Src.x*(G.M+1)+y])));
    }
    else{
        return wire_cost-short_cost + 2*overflow_cost/(1+std::exp(-float(G.G[l][Src.y*(G.N+1)+x])/float(G.C[l][Src.y*(G.N+1)+x])));
    }
}