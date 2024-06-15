class GAMER{
    public:
        std::vector<std::vector<float>> Sdist; // for metal1
        std::vector<std::vector<char>> Sdir;
        GAMER(int M, int N, int L);
        void Init_GAMER(Point src, Point cornerl, Point cornerh);
        algo(Grid_Graph G,Point cornerl, Point cornerh, int alternations);
}