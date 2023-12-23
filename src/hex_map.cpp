#include <iostream>
#include <queue>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <cmath>
#include <boost/log/trivial.hpp>
#include "hex_map.hpp"

using namespace std;

class compare
{
    double** weight_g_;
    double** weight_h_;

public:
    compare(double** weight_g, double** weight_h)
        : weight_g_(weight_g)
        , weight_h_(weight_h)
    {}
    bool operator()(const hex_tile* a, const hex_tile* b) const
    {
        return weight_g_[a->index_y()][a->index_x()] + weight_h_[a->index_y()][a->index_x()] >
               weight_g_[b->index_y()][b->index_x()] + weight_h_[b->index_y()][b->index_x()];
    }
};


bool hex_tile::operator==(const hex_tile &a) const
{
    return q_ == a.q_ && r_ == a.r_ && s_ == a.s_;
}

bool hex_tile::operator!=(const hex_tile &a) const
{
    return q_ != a.q_ || r_ != a.r_ || s_ != a.s_;
}

hex_tile hex_tile::operator+(const hex_tile &a) const
{
    const hex_tile b(q_ + a.q_, r_ + a.r_);
    return b;
}

hex_tile hex_tile::operator-(const hex_tile &a) const
{
    const hex_tile b(q_ - a.q_, r_ - a.r_);
    return b;
}

bool hex_tile::passable() const
{
    return passable_;
}

double hex_tile::distance(const hex_tile& a) const
{
    return sqrt(pow(this->graphic_x() - a.graphic_x(), 2) + pow(this->graphic_y() - a.graphic_y(), 2));
}

double hex_tile::distance(const hex_tile* a) const
{
    return sqrt(pow(this->graphic_x() - a->graphic_x(), 2) + pow(this->graphic_y() - a->graphic_y(), 2));
}

int hex_tile::q() const
{
    return q_;
}

int hex_tile::r() const
{
    return r_;
}

int hex_tile::s() const
{
    return s_;
}

int hex_tile::index_x() const
{
    return x_index_;
}

int hex_tile::index_y() const
{
    return r_;
}

double hex_tile::graphic_x() const
{
    return x_graphic_;
}

double hex_tile::graphic_y() const
{
    return r_;
}

bool hex_tile::is_valid() const
{
    return this->passable_;
}



map_class::map_class(const std::string& file_name)
{
    tiles_map_ = new std::vector<std::vector<hex_tile*>*>;
    std::fstream file{file_name};
    std::string line;
    int r = 0;
    int q = 0;
    size_x_ = 0;
    while (std::getline(file, line))
    {
        const auto row = new std::vector<hex_tile*>;
        for (const auto c :line)
        {
            const bool passable = c != '0';
            auto t = new hex_tile{q - (r + (r & 1)) / 2, r, passable};
            row->push_back(t);
            q++;
        }
        this->row_add(row);
        size_x_ = q;
        q = 0;
        r++;
    }
    size_y_ = r;
}

std::vector<hex_tile*> map_class::path_a_star(hex_tile* start, hex_tile* end) const
{
    constexpr int a_weight = 5;
    const auto list_closed = new bool*[size_y_];
    const auto weight_g = new double*[size_y_];
    const auto weight_h = new double*[size_y_];
    const auto parent = new hex_tile**[size_y_];
    for (int i = 0; i < size_y_; i++)
    {
        list_closed[i] = new bool[size_x_]();
        weight_g[i] = new double[size_x_]();
        weight_h[i] = new double[size_x_]();
        parent[i] = new hex_tile*[size_x_]();
    }
    vector<hex_tile*> list_open;
    hex_tile* current_node = start;
    weight_g[current_node->index_y()][current_node->index_x()] = 0;
    weight_h[current_node->index_y()][current_node->index_x()] = current_node->distance(end) * a_weight;
    list_open.push_back(current_node);
    make_heap(list_open.begin(), list_open.end(), compare(weight_g, weight_h));
    int index = 0;
    while (!list_open.empty())
    {
        index++;
        current_node = list_open.front();
        list_open.erase(list_open.begin());
        list_closed[current_node->index_y()][current_node->index_x()] = true;
        const double cost_g = weight_g[current_node->index_y()][current_node->index_x()] + 1.0;
        if (list_closed[end->index_y()][end->index_x()])
        {
            break;
        }
        if (constexpr double depth = 200.0; cost_g > depth)
        {
            break;
        }
        for (int dir = 0; dir < 6; dir++)
        {
            hex_tile* children = neighbor(current_node->q(), current_node->r(), dir);
            if (children == nullptr)
            {
                continue;
            }
            if (!children->passable())
            {
                continue;
            }
            if (list_closed[children->index_y()][children->index_x()])
            {
                continue;
            }
            if (find(list_open.begin(), list_open.end(), children) == list_open.end())
            {
                weight_g[children->index_y()][children->index_x()] = cost_g;
                weight_h[children->index_y()][children->index_x()] = (children->distance(end)) * a_weight;
                list_open.push_back(children);
                push_heap(list_open.begin(), list_open.end(), compare(compare(weight_g, weight_h)));
                parent[children->index_y()][children->index_x()] = current_node;
            }
            else if (weight_g[children->index_y()][children->index_x()] +
                         weight_h[children->index_y()][children->index_x()] >
                     cost_g + weight_h[children->index_y()][children->index_x()])
            {
                weight_g[children->index_y()][children->index_x()] = cost_g;
                parent[children->index_y()][children->index_x()] = current_node;
                make_heap(list_open.begin(), list_open.end(), compare(compare(weight_g, weight_h)));
            }
        }
    }
    vector<hex_tile*> path;
    /*cout << index << ' ' <<  start->index_x() << " " << start->index_y() << "/" << end->index_x() << " " << end->index_y() << endl;
    for(int y = 0; y<size_y_; y++)
    {
        for(int x = 0; x<size_x_; x++)
        {
            cout << (list_closed[y][x]);
        }
        cout << endl;
    }*/
    if (list_closed[current_node->index_y()][current_node->index_x()])
    {
        while (current_node != start && current_node != nullptr)
        {
            path.push_back(current_node);
            current_node = parent[current_node->index_y()][current_node->index_x()];
        }
        reverse(path.begin(), path.end());
    }
    for (int i = 0; i < size_y_; i++)
    {
        delete[] list_closed[i];
        delete[] weight_g[i];
        delete[] weight_h[i];
        delete[] parent[i];
    }
    delete[] list_closed;
    delete[] weight_g;
    delete[] weight_h;
    delete[] parent;
    return path;
}

hex_tile* map_class::tile_get(const int q, const int r) const
{
    if (!in_map(q,r,*this))
        return nullptr;
    return (*(*tiles_map_)[r])[q + (r + (r & 1)) / 2];
}

void map_class::row_add(std::vector<hex_tile*>* row) const
{
    tiles_map_->push_back(row);
}

std::vector<std::vector<hex_tile*>*>* map_class::tiles_map_get() const
{
    return tiles_map_;
}

hex_tile* map_class::neighbor(int q, int r, int dir) const
{
    int n_q = q;
    int n_r = r;
    if (dir == 0)
    {
        n_q++;
    }
    else if (dir == 1)
    {
        n_q++;
        n_r--;
    }
    else if (dir == 2)
    {
        n_r--;
    }
    else if (dir == 3)
    {
        n_q--;
    }
    else if (dir == 4)
    {
        n_q--;
        n_r++;
    }
    else if (dir == 5)
    {
        n_r++;
    }
    else
        return nullptr;
    return tile_get(n_q, n_r);
}

bool map_class::in_map(const int q, const int r, const map_class& map)
{
    return q + (r + (r & 1)) / 2 >= 0 && r >= 0 && q + (r + (r & 1)) / 2 < map.size_x_get() && r < map.size_y_get();
}

bool map_class::passable(const int q, const int r) const
{
    if (!in_map(q, r, *this))
        return false;
    return tile_get(q,r)->passable();
}

int map_class::size_x_get() const
{
    return size_x_;
}

int map_class::size_y_get() const
{
    return size_y_;
}

hex_tile* map_class::neighbor(const hex_tile* tile, const int dir) const
{
    int x_shift = 0;
    int y_shift = 0;
    if (dir == 0)
    {
        x_shift = 1;
    }
    else if (dir == 1)
    {
        x_shift = -1;
        y_shift = -1;
    }
    else if (dir == 2)
    {
        y_shift = -1;
    }
    else if (dir == 3)
    {
        x_shift = -1;
    }
    else if (dir == 4)
    {
        x_shift = 1;
        y_shift = 1;
    }
    else if (dir == 5)
    {
        y_shift = 1;
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "unknown direction " << dir;
    }
    if (!(tile->index_x() + x_shift >= 0 && tile->index_y() + y_shift >= 0 && tile->index_x() + x_shift < size_x_ &&
          tile->index_y() + y_shift < size_y_))
        return nullptr;
    return (*(*tiles_map_)[tile->index_y() + y_shift])[tile->index_x() + x_shift];
}
