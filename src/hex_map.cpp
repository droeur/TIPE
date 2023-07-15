#include <iostream>
#include <queue>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <boost/log/trivial.hpp>
#include "hex_map.hpp"

using namespace std;

struct compare
{
    bool operator()(const hex_tile* a, const hex_tile* b) const
    {
        return a->cost_f > b->cost_f;
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

int hex_tile::parent_x() const
{
    return parent_q + (parent_r + (parent_r & 1)) / 2;
}

int hex_tile::parent_y() const
{
    return parent_r;
}

bool hex_tile::operator<(const hex_tile& a) const
{
    return this->cost_f < a.cost_f;
}

bool hex_tile::operator>(const hex_tile& a) const
{
    return this->cost_f > a.cost_f;
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

std::vector<hex_tile*> map_class::path_a_star(hex_tile* start, const hex_tile* end) const
{
    constexpr int a_weight = 5;
    constexpr float depth = 200.0f;
    const auto list_closed = new bool*[size_y_];
    for (int i = 0; i < size_y_; i++)
    {
        list_closed[i] = new bool[size_x_];
    }
    vector<hex_tile*> list_open;
    hex_tile* current_node = start;
    current_node->cost_g = 0;
    current_node->cost_h = static_cast<float>(current_node->distance(end)) * a_weight;
    current_node->cost_f = current_node->cost_g + current_node->cost_h;
    list_open.push_back(current_node);
    make_heap(list_open.begin(), list_open.end(), compare());
    int index = 0;
    while (!list_open.empty())
    {
        index++;
        current_node = list_open.front();
        list_open.erase(list_open.begin());
        list_closed[current_node->index_y()][current_node->index_x()] = false;
        const float cost_g = current_node->cost_g + 1.0f;
        if (!list_closed[end->index_y()][end->index_x()])
        {
            break;
        }
        if (cost_g > depth)
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
            if (!list_closed[children->index_y()][children->index_x()])
            {
                continue;
            }
            if (find(list_open.begin(), list_open.end(), children) == list_open.end())
            {
                children->cost_g = cost_g;
                children->cost_h = static_cast<float>(children->distance(end)) * a_weight;
                children->cost_f = children->cost_g + children->cost_h;
                list_open.push_back(children);
                push_heap(list_open.begin(), list_open.end(), compare());
                children->parent = current_node;
            }
            else if (children->cost_f > cost_g + children->cost_h)
            {
                children->cost_g = cost_g;
                children->cost_f = children->cost_g + children->cost_h;
                children->parent = current_node;
                make_heap(list_open.begin(), list_open.end(), compare());
            }
        }
    }
    vector<hex_tile*> path;
    if (!list_closed[current_node->index_y()][current_node->index_x()])
    {
        while (current_node != start && current_node != nullptr)
        {
            path.push_back(current_node);
            current_node = current_node->parent;
        }
        reverse(path.begin(), path.end());
    }
    for (int i = 0; i < size_y_; i++)
        delete[] list_closed[i];
    delete[] list_closed;
    return path;
}
/* std::vector<hex_tile*> map_class::path_a_star(hex_tile* start, hex_tile* end) const
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

    list_open.push(start);
    int i = 0;

    while (!list_open.empty() && i < size_x_ * size_y_*100)
    {
        hex_tile* current_node = list_open.top();
        list_closed[current_node->index_x()][current_node->index_y()] = true;
        list_parents[current_node->index_x()][current_node->index_y()] = current_node;
        list_open.pop();
        for (int dir = 0; dir < 6; dir++)
        {
            if (neighbor(current_node, dir) != nullptr && neighbor(current_node, dir)->passable() &&
                !list_closed[neighbor(current_node, dir)->index_x()][neighbor(current_node, dir)->index_y()])
            {
                if (neighbor(current_node, dir)->q() == end->q() && neighbor(current_node, dir)->r() == end->r())
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
                    path.erase(path.begin());
                    return path;
                }
                children = neighbor(current_node, dir);
                children->cost_g = current_node->cost_g + 1.0f;
                children->cost_h = children->distance(end);
                children->cost_f = children->cost_h + children->cost_g;
                children->parent_q = current_node->q();
                children->parent_r = current_node->r();
                list_open.push(children);
            }
        }
        list_closed[current_node->index_x()][current_node->index_y()] = true;
        i++;
    }
    return path;
}*/

hex_tile* map_class::tile_get(const int q, const int r) const
{
    if (!in_map(q,r,this))
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

bool map_class::in_map(const int q, const int r, const map_class* map)
{
    return q + (r + (r & 1)) / 2 >= 0 && r >= 0 && q + (r + (r & 1)) / 2 < map->size_x_get() && r < map->size_y_get();
}

bool map_class::passable(const int q, const int r) const
{
    if (!in_map(q, r, this))
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
