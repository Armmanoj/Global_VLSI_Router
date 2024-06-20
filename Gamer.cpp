#include "Net.hpp"
#include "Netlist.hpp"
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
#include "Grid_Graph.hpp"
#include "Gamer.hpp"

GAMER::GAMER(int M, int N, int L){
    Sdist.resize(L,std::vector<float>(0));
    Sdir.resize(L,std::vector<char>(0));
    for (int i = 0; i < L; i++) {
        std::cout << "here";
        Sdist[i].resize(M * N, 0); // Fill with 0
        std::cout << Sdist[i].size();
        Sdir[i].resize(M * N, 'x'); // Fill with space character
    }
    std::cout << "size of sdist, sdir is " << Sdist.size() << "\n";
    printpath(2,5,5);
}

void GAMER::Init_GAMER(Grid_Graph& G, Point src, Point cornerl, Point cornerh, int first){
    if (first){
        for (int k = 0; k < Sdist.size(); k += 2){
            for (int i = cornerl.y; i<cornerh.y+1;i++){
                for (int j = cornerl.x; j<cornerh.x+1;j++){
                    Sdist[k][i*G.N+j] = std::numeric_limits<float>::max()*0.5;
                    Sdir[k][i*G.N+j] = 'x';
                }
            }
        }
        for (int k = 1; k < Sdist.size(); k += 2){
            for (int i = cornerl.x; i<cornerh.x+1;i++){
                for (int j = cornerl.y; j<cornerh.y+1;j++){
                    Sdist[k][i*G.M+j] = std::numeric_limits<float>::max()*0.5;
                    Sdir[k][i*G.M+j] = 'x';
                }
            }
        }
    }
    else{
        for (int k = 0; k < Sdist.size(); k += 2){
            for (int i = cornerl.y; i<cornerh.y+1;i++){
                for (int j = cornerl.x; j<cornerh.x+1;j++){
                    if (Sdist[k][i*G.N+j]){
                        Sdist[k][i*G.N+j] = std::numeric_limits<float>::max()*0.5;
                        Sdir[k][i*G.N+j] = 'x';
                    }
                }
            }
        }
        for (int k = 1; k < Sdist.size(); k += 2){
            for (int i = cornerl.x; i<cornerh.x+1;i++){
                for (int j = cornerl.y; j<cornerh.y+1;j++){
                    if (Sdist[k][i*G.M+j]){
                        Sdist[k][i*G.M+j] = std::numeric_limits<float>::max()*0.5;
                        Sdir[k][i*G.M+j] = 'x';
                    }
                }
            }
        }
    }
    std::cout << "now need to initialize the source " << std::endl;

    printpath(G.L,G.M,G.N);
    if (src.l%2==0){
        Sdist[src.l][src.y*G.N+src.x] = 0;
    }
    else{
        Sdist[src.l][src.x*G.M+src.y] = 0;
    }
    std::cout << "initialized src " << std::endl;
}

void GAMER::printpath(int L,int M,int N){
    // Printing out Sdist
    std::cout << "Sdist array:" << std::endl;
    for (int k = 0; k < L; k++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << Sdist[k][i * N + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // Printing out Sdir
    std::cout << "Sdir array:" << std::endl;
    for (int k = 0; k < L; k++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << Sdir[k][i * N + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

}

void GAMER::algo(Grid_Graph& G, Point cornerl, Point cornerh, int alternations){
    bool flag = 1; // to keep track of if the relaxation step has caused any change to the distances and routes or not
    int iter = 0;
    while (flag||iter<alternations){
        iter++;
        flag = 0;
        // Relaxing Sdir1,3... (rows)
        for (int l = 0; l < G.L; l += 2){
            for (int k = cornerl.y; k<cornerh.y+1; k++){
                // left to right
                for (int j = cornerl.x+1; j<cornerh.x+1; j++){
                    std::cout<<"weight is "<<G.weight(l,j,k);
                    if (Sdist[l][G.N*k+j] > Sdist[l][G.N*k+j-1]+G.weight(l,j,k)){
                        std::cout<<" j "<<j<<" ";
                        Sdist[l][G.N*k+j] = Sdist[l][G.N*k+j-1]+G.weight(l,j,k);
                        Sdir[l][G.N*k+j] = 'l';
                        flag = 1;
                    }
                }
                // right to left
                for (int j = cornerh.x-1; j>cornerl.x-1; j--){
                    if (Sdist[l][G.N*k+j] > Sdist[l][G.N*k+j+1]+G.weight(l,j,k)){
                        Sdist[l][G.N*k+j] = Sdist[l][G.N*k+j+1]+G.weight(l,j,k);
                        Sdir[l][G.N*k+j] = 'r';
                        flag = 1;
                    }
                }
            }
        }
        printpath(G.L,G.M,G.N);
        // Relaxing Sdir1,3... (cols)
        for (int l = 1; l < G.L;l += 2){
            for (int j = cornerl.x; j<cornerh.x+1; j++){
                // south to  north
                for (int k = cornerl.y+1; k<cornerh.y+1; k++){
                    if (Sdist[l][G.M*j+k] > Sdist[l][G.M*j+k-1]+G.weight(l,j,k)){
                        Sdist[l][G.M*j+k] = Sdist[l][G.M*j+k-1]+G.weight(l,j,k);
                        Sdir[l][G.M*j+k] = 's';
                        flag = 1;
                    }
                }
                // right to left
                for (int k = cornerh.y-1; k>cornerl.y-1; k--){
                    if (Sdist[l][G.M*j+k] > Sdist[l][G.M*j+k+1]+G.weight(l,j,k)){
                        Sdist[l][G.M*j+k] = Sdist[l][G.M*j+k+1]+G.weight(l,j,k);
                        Sdir[l][G.M*j+k] = 'n';
                        flag = 1;
                    }
                }
            }
        }
        printpath(G.L,G.M,G.N);
        // via sweep
        // down to up
        for (int l=0; l< G.L-1; l++){
            for (int k = cornerl.y; k<cornerh.y+1; k++){
                for (int j = cornerl.x; j<cornerh.x+1; j++){
                    if (Sdist[l][G.N*k+j] > Sdist[l+1][G.M*j+k]+G.v){
                        Sdist[l][G.N*k+j] = Sdist[l+1][G.M*j+k]+G.v;
                        Sdir[l][G.N*k+j] =  'u';
                        flag = 1;
                    }
                }
            }
        }
        // up to down
        for (int l=G.L-2; l>-1; l--){
            for (int k = cornerl.y; k<cornerh.y+1; k++){
                for (int j = cornerl.x; j<cornerh.x+1; j++){
                    if (Sdist[l][G.N*k+j] + G.v < Sdist[l+1][G.M*j+k]){
                        Sdist[l+1][G.M*j+k] = Sdist[l][G.N*k+j]+G.v;
                        Sdir[l+1][G.M*j+k] =  'd';
                        flag = 1;
                    }
                }
            }
        }
        printpath(G.L,G.M,G.N);
        
    }
}