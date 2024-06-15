#include "Grid_Graph.hpp"
// struct Point
struct Point {
    int x;
    int y;
    int l;
    bool operator==(const Point& p2) const {
        return (x ==p2.x) && (y == p2.y) && (l == p2.l);
    }
    bool operator!=(const Point& p2) const {
        return !((x ==p2.x) && (y == p2.y) && (l == p2.l));
    }
    Point(const Point& other) : x(other.x), y(other.y), l(other.l) {}
};

struct Pair{
    Point p;
    Point q;
}

class Net {
    public:
        std::vector<Point> pins;
        std::vector<Pair> route;
        std::vector<Pair> metalzero;
        std::String name;
        Point cornerl;
        Point cornerh;
        Net(Grid_Graph G, std::vector<Point> pin_list, std::String Name, GAMER GAMER);
        void pattern_route(Grid_Graph G);
        void maze_route(Grid_Graph G, GAMER g, int alternations);
        float traverse_line(Grid_Graph G, Pair pair,int increment);
        float traverse_route(Grid_Graph G, int increment);
        void bounding_box(float k, int box_min_side);
}