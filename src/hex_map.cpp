#include <iostream>
#include <queue>
#include <algorithm>
#include <cassert>
#include "hex_map.hpp"

using namespace std;

class compare
{
public:
    bool operator()(hex_tile* a, hex_tile* b)
    {
        return a->cost_f > b->cost_f;
    }
};


std::vector<hex_tile*> map_class::path_a_star(hex_tile* start, hex_tile* end)
{
    std::vector<hex_tile*> path;
    priority_queue<hex_tile*, vector<hex_tile*>, compare> list_open;
    auto** list_closed = new bool*[size_y_];
    auto*** list_parents = new hex_tile**[size_y_];
    for (int i=0; i<size_x_; i++)
    {
        list_closed[i] = new bool[size_x_];
        list_parents[i] = new hex_tile*[size_x_];
        for (int j = 0; j < size_y_; j++)
        {
            list_closed[i][j] = false;
            list_parents[i][j] = nullptr;
        }
    }

    hex_tile* current_node = nullptr;
    hex_tile* children = nullptr;

    if (start == end)
    {
        path.push_back(start);
        return path;
    }

    start->cost_f = 0.0;
    start->cost_g = 0.0;
    start->cost_h = 0.0;
    start->parent_q = start->q();
    start->parent_r = start->r();
    start->parent_s = start->s();

    list_open.push(start);
    int i = 0;

    while (!list_open.empty() && i < size_x_ * size_y_)
    {
        current_node = list_open.top();
        list_closed[current_node->index_x()][current_node->index_y()] = true;
        list_parents[current_node->index_x()][current_node->index_y()] = current_node;
        list_open.pop();
        for (int dir = 0; dir < 6; dir++)
        {
            if (in_map(current_node->neighbor(dir).q(), current_node->neighbor(dir).r(), this) &&
                tile_get(current_node->neighbor(dir).q(), current_node->neighbor(dir).r())->passable() && !list_closed[
                    current_node->neighbor(dir).index_x()][current_node->neighbor(dir).index_y()])
            {
                if (current_node->neighbor(dir).q() == end->q() && current_node->neighbor(dir).r() == end->r())
                {
                    path.push_back(end);
                    path.push_back(tile_get(current_node->q(), current_node->r()));
                    while (current_node != start)
                    {
                        hex_tile* t = tile_get(current_node->parent_q, current_node->parent_r);
                        path.push_back(t);
                        current_node = list_parents[current_node->parent_x()][current_node->parent_y()];
                    }
                    reverse(path.begin(), path.end());
                    for (const auto t : path)
                    {
                        vector<int> a = {0};
                        if (t->q() > 100 || t->r() > 100)
                        {
                            a[3] = 10;
                            cout << "error";
                        }
                    }
                    return path;
                }
                children = tile_get(current_node->neighbor(dir).q(), current_node->neighbor(dir).r());
                children->cost_g = current_node->cost_g + 1.0f;
                children->cost_h = children->distance(end);
                children->cost_f = children->cost_h + children->cost_g;
                children->parent_q = current_node->q();
                children->parent_r = current_node->r();
                children->parent_s = current_node->s();
                list_open.push(children);
            }
        }
        list_closed[current_node->index_x()][current_node->index_y()] = true;
        i++;
    }
    return path;
}
