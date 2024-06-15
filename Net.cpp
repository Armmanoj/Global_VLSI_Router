#define BOX_SCALING 1.75
#define BOX_MIN_SIDE 8
#include "Net.hpp"

// Defining the constructor
Net::Net(Grid_Graph G, std::vector<Point> pin_list, std::String Name, GAMER GAMER){
    strcp(name,Name);
    route.reserve(4*pin_list.size);
    pins.reserve(pin_list.size);
    for (int k = 0; k<pin_list.size; k++){
        if (pin_list[k].l == -1){
            metalzero.push_back(pin_list[k]);
            pin_list[k].l = 0;
        }
        pins[k] = pin_list[k];
    } 
    bounding_box(BOX_SCALING,BOX_MIN_SIDE); // calculate the net's bounding box
    // set the vectors size after setting its initial memory usage
    route.maze_route(G, GAMER, 8);
    bounding_box(BOX_SCALING,BOX_MIN_SIDE); // calculate the net's bounding box
}

// Code to choose the best L shaped route for the net
void Net::pattern_route(Grid_Graph G){
    if (!((src.x == dst.x)||(src.y == dst.y))){
        route[0] = Point(src.x,dst.y,0);
        float weight1 = traverse_route(G,0);
        route[0] = Point(dst.x,src.y,0);
        float weight2 = traverse_route(G,0);
        route[0] = (weight2>weight1) ? Point(dst.x,src.y,0) : Point(src.x,dst.y,0);
    }
    traverse_route(G,1);  
}
void Net::maze_route(Grid_Graph G, GAMER g, int alternations){
    traverse_route(G,-1); // rip up
    route.clear();
    g.Init_GAMER(pins[0],cornerl,cornerh);
    for (int i=0; i<pins.size; i++){
        g.algo(G,cornerl,cornerh,alternations);
        back_trace(G,g); // labels routes as new source, appends to routes
    }
    traverse_route(G,1); // updates grid graph
}

void Net::back_trace(Grid_Graph G, GAMER g){
    //backtracing
    Point here = dst;
    Point was_here = dst;
    char dir;
    char diro = g.Sdir[here.l][G.N*here.y+here.x];     
    while (g.Sdist[was_here.l][G.N*was_here.y+was_here.x]!=0){
        dir = g.Sdir[here.l][G.M*here.x+here.y];
        if (dir != diro){
            route.push_back(Pair(here,was_here)); // push_back has amortized O(1) time complexity
            was_here = here;
        }
        diro = dir;
        switch(dir){
            case 'd':
                here.l--; break;
            case 'u':
                here.l++; break;
            case 'l':
                here.x-- break;
            case 'r':
                here.x++; break;
            case 'n':
                here.y++; break;
            case 's':
                here.y--; break;
            default: break;
        }
        g.Sdist[here.l][G.M*here.x+here.y] = 0;
        g.Sdir[here.l][G.M*here.x+here.y] = 'x';
    }
}

float Net::traverse_line(Grid_Graph G, Pair pair,int increment)
{
    /*
    if increment is +1, we are incrementing grid_graphs edge demand (routing the net) on traversing through that edge,
    for increment=-1, we are "ripping" the net by decrementing Grid_Graph demand, increment = 0 for just calculating cost of a route
    The assumption is that atleast 2 of x,y,l are same for Src and Dest
    */
    Point Src = pair.p;
    Point Dest = pair.q;
    float sum = 0; 
    int l = Src.l;
    if (Src.x == Dest.x) {
        // north-south line
        for (int y = std::min(Src.y,Dest.y)+1; y < std::max(Dest.y,Src.y); ++y) {
            G.G[l][Src.x*(G.M+1)+y] += increment;
            sum += G.weight(l,x,y);
        }
    }
    else if (Src.y == Dest.y){
        // east-west line
        for (int x = std::min(Src.x,Dest.x) + 1; x < std::max(Dest.x,Src.x); ++x) {
            G.G[l][Src.y*(G.N+1)+x] += increment;
            sum += G.weight(l,x,y);
        }
    }
    else{
        sum += G.v*std::abs(Src.l-Dest.l);
    }
    return sum;
}

float Net::traverse_route(Grid_Graph G, int increment)
{
    /*
    if increment is +1, we are incrementing grid_graphs edge demand (routing the net) on traversing through that edge,
    for increment=-1, we are "ripping" the net by decrementing Grid_Graph demand
    */
    float sum = 0;
    for (const auto& line : route){
        sum += traverse_line(G, line,increment);
    }
    return sum;
}

/*
GPT prompt- Let pins be a vector of "Point" elements, where point has an x,y. Define the bounding box of the pins as a box whose x
side length is k times the max distance between x values of points in pins and whose y side length is k times the max distance 
between y values of pins, centered at x = midpoint of the 2 furthest apart points along x direction and y= midpoint of the 2 
furthest apart points in y direction. Write code that calculates the lower and upper corners of bounding box
*/
void Net::bounding_box(float k, int box_min_side){

    double minX = std::numeric_limits<double>::max();
    double maxX = 0;
    double minY = std::numeric_limits<double>::max();
    double maxY = 0;

    // Find the min and max values for x and y
    for (const auto& p : pins) {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    // Calculate the midpoints
    double midX = (minX + maxX) / 2.0;
    double midY = (minY + maxY) / 2.0;

    // Calculate the side lengths of the bounding box
    double sideLengthX = std::max(k * (maxX - minX),(double)box_min_side);
    double sideLengthY = std::max(k * (maxY - minY),(double)box_min_side);

    // Calculate the lower and upper corners
    cornerl = Point((int)(midX - sideLengthX / 2.0),(int)(midY - sideLengthY / 2.0));
    cornerh = POint((int)(midX + sideLengthX / 2.0),(int)(midY + sideLengthY / 2.0));
}