GAMER::GAMER(int M, int N, int L){
    Sdist.resize(L);
    Sdir.resize(L);
    for (int i = 0; i < L; ++i) {
        Sdist[i].resize(M * N);
        Sdir[i].resize(M * N);
    }
}

void GAMER::Init_GAMER(Point src, Point cornerl, Point cornerh){
    for (int k = 0; k < G.L; k += 2){
        for (int i = cornerl.y; i<cornerh.y+1;i++){
            for (int j = cornerl.x; j<cornerh.x+1;j++){
                Sdist[0][i*G.N+j] = std::numeric_limits<int>::max();
                Sdir[0][i*G.N+j] = 'x';
            }
        }
    }
    for (int k = 1; k < G.L; k += 2){
        for (int i = cornerl.x; i<cornerh.x+1;i++){
            for (int j = cornerl.y; j<cornerh.y+1;j++){
                Sdist[1][i*G.M+j] = std::numeric_limits<int>::max();
                Sdir[1][i*G.M+j] = 'x';
            }
        }
    }
    Sdist[0][source.y*G.N+source.x] = 0;
}

void algo(Grid_Graph G, Point cornerl, Point cornerh, int alternations){
    bool flag = 1; // to keep track of if the relaxation step has caused any change to the distances and routes or not
    int iter = 0;
    while (flag||iter<alternations){
        iter++;
        flag = 0;
        // Relaxing Sdir1,3... (rows)
        for (int l = 0; k < G.L; l += 2){
            for (int k = cornerl.y; k<cornerh.y+1; k++){
                // left to right
                for (int j = cornerl.x+1; j<cornerh.x+1; j++){
                    if (Sdist[l][G.N*k+j] > Sdist[l][G.N*k+j-1]+G.weight(l,j,k)){
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
    
        // Relaxing Sdir1,3... (cols)
        for (int l = 1; l < G.L;l += 2){
            for (int j = cornerl.x; j<cornerh.x+1; j++){
                // south to  north
                for (int k = cornerl.y+1; k<cornerh.y+1; k++){
                    if (Sdist[l][G.M*j+k] > Sdist[l][G.M*j+k-1]+G.weight(l,j,k)){
                        Sdist[l][G.M*j+k] = Sdist[l][G.M*j+k-1]+G.weight(G.wire_cost,G.short_cost[k],G.G[k][(G.M+1)*j+k],G.C[k][(G.M+1)*j+k]);
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
        // via sweep
        // down to up
        for (int l=0; l< G.L-1; l++){
            for (int k = cornerl.y; k<cornerh.y+1; k++){
                for (int j = cornerl.x; j<cornerh.x+1; j++){
                    if (Sdist[l][G.N*k+j] > Sdist[l+1][G.M*j+k]+G.v){
                        Sdist[l][G.N*k+j] = Sdist2[l+1][G.M*j+k]+G.v;
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
        
    }
}