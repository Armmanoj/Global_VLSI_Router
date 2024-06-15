
Netlist::Netlist(Grid_Graph G, string netfile){
    std::ifstream file(netfile);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {};
    }
    std::string line;
    std:: string name;
    int x, y, l;

    std::vector<Point> pins;
    while (std::getline(file, line)){
        pins.clear();
        name = line;
        std::getline(file, line);
        std::getline(file, line);
         while (line[0]=='['){
            sscanf(line.c_str(), "[(%d, %d, %d)", &x, &y, &l); // Parse the first access point for simplicity
            pins.push_back(Point(x,y,l-1)); // actual routing starts on metal1 only
            std::getline(file, line);
         }
        nets.push_back(Net(G, pins, name)); // add the new net to netlist 
    }
}

void Netlist::mazer(Grid_Graph G, GAMER GAMER){
    for (int i = 0; i<nets.size; i++){
        nets[i].maze_route(G, GAMER,ALTERNATIONS); // ALTERNATIONS is max number of alternations in GAMER
    }
}

void Netlist::PR_output(string outfile){
    std::ofstream file(netfile);
    for (int i = 0; i<nets.size; i++){
        file << nets[i].name << std::endl; << "(\n";
        for (int j=0; j<nets[i].route.size; j++){
            Pair pair = nets[i].route[j];
            Point p = pair.p; Point q = pair.q;
            file << p.x << " " << p.y << " " << p.l+1 << " " << q.x << " " << q.y << " " << q.l+1 << std::endl;
            // adding +1 to all layer's as metal1 is layer 0 here, assumption is x y -1 x y l is stored in route an endpoint vertical line
        }
        for (int j=0; j<nets[i].metalzero.size; j++){
            Point p = nets[i].metalzero[j];
            file << p.x << " " << p.y << " " << 0 << " " <<  p.x << " " << p.y << " " << 1 << std::endl; 
        }
        file << ")\n";
    }
}