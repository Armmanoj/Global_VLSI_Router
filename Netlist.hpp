#define ALTERNATIONS 8

class Netlist {
public:
    std::vector<Net> nets;      // Vector of nets
    // Constructor
    Netlist(Grid_Graph G,string netfile);
    // Function to perform maze routing
    void mazer(Grid_Graph G);
    void PR_output(string outfile);
};



