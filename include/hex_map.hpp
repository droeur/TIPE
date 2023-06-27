#pragma once

#include <assert.h>
#include <vector>
#include <rapidcsv.h>
#include <float.h>

enum
{
    size_q = 64,
    size_r = 64
};

#define MAX_A_STAR_DEPTH size_q * size_r

/**
 * Hex directions:
 *     3 2
 *    4 H 1
 *     5 6 
 */

class hex_tile
{
public:
    hex_tile(const int q, const int r)
        : q_(q), r_(r), s_(-q - r), x_graphic_(q + static_cast<double>(r) / 2)
    {

    }

    hex_tile(const int q, const int r, const int s)
        : q_(q), r_(r), s_(s), x_graphic_(q + static_cast<double>(r) / 2)
    {

    }

    hex_tile(const int q, const int r, const int s, const bool passable)
        : q_(q), r_(r), s_(s), passable_(passable), x_graphic_(q + static_cast<double>(r) / 2)
    {

    }

    hex_tile(const int q, const int r, const bool passable)
        : q_(q), r_(r), s_(-q - r), passable_(passable), x_graphic_(q + static_cast<double>(r) / 2)
    {

    }

    bool operator==(const hex_tile a) const 
    {
        return q_ == a.q_ && r_ == a.r_ && s_ == a.s_;
    }

    bool operator!=(const hex_tile a) const 
    {
        return q_ != a.q_ || r_ != a.r_ || s_ != a.s_;
    }

    hex_tile operator+(const hex_tile a) const 
    {
        const hex_tile b(q_ + a.q_, r_ + a.r_);
        return b;
    }

    hex_tile operator-(const hex_tile a) const 
    {
        const hex_tile b(q_ - a.q_, r_ - a.r_);
        return b;
    }

    [[nodiscard]] bool passable() const 
    {
        return passable_;
    }

    [[nodiscard]] float distance(const hex_tile& a) const 
    {
        const hex_tile temp = *this - a;
        return static_cast<float>(abs(temp.q_) + abs(temp.r_) + abs(temp.s_)) / 2.0f;
    }

    float distance(const hex_tile* a) const 
    {
        const hex_tile temp = *this - *a;
        return static_cast<float>(abs(temp.q_) + abs(temp.r_) + abs(temp.s_)) / 2.0f;
    }

    static hex_tile direction(const int dir)
    {
        const hex_tile hex_tile_directions[6] = {
            hex_tile(1, 0), hex_tile(1, -1), hex_tile(0, -1),
            hex_tile(-1, 0), hex_tile(-1, 1), hex_tile(0, 1)
        };
        return hex_tile_directions[dir];
    }

    [[nodiscard]] hex_tile neighbor(const int dir) const 
    {
        return *this + direction(dir);
    }

    // coordinates cube
    [[nodiscard]] int q() const 
    {
        return q_;
    }

    [[nodiscard]] int r() const 
    {
        return r_;
    }

    [[nodiscard]] int s() const 
    {
        return s_;
    }

    //coordinates "offset even-r"
    [[nodiscard]] int index_x() const 
    {
        return r_;
    }

    [[nodiscard]] int index_y() const 
    {
        return q_ + (r_ + (r_ & 1)) / 2;
    }

    [[nodiscard]] double graphic_x() const 
    {
        return x_graphic_;
    }

    [[nodiscard]] double graphic_y() const 
    {
        return r_;
    }

    int parent_q = -1;
    int parent_r = -1;
    int parent_s = -1;
    float cost_g = FLT_MAX; //distance from start
    float cost_h = FLT_MAX; //heuristic
    float cost_f = FLT_MAX; //g+h

    int parent_x() const 
    {
        return parent_r;
    }

    int parent_y() const 
    {
        return parent_q + (parent_r + (parent_r & 1)) / 2;
    }

    bool operator<(const hex_tile& a) const 
    {
        return this->cost_f < a.cost_f;
    }

    bool operator>(const hex_tile& a) const 
    {
        return this->cost_f > a.cost_f;
    }

    bool is_valid() const 
    {
        return this->passable_;
    }

protected:
    int q_ = -1;
    int r_ = -1;
    int s_ = -1;
    bool passable_ = false;
    double x_graphic_;
};

class coordinates
{
public:
    coordinates(const int q, const int r)
        : q(q), r(r)
    {
    }
    int q;
    int r;

    bool operator==(const coordinates* a) const
    {
        return a->q == this->q && a->r == this->r;
    }
};

class map_class
{
public:
    explicit map_class(const std::string& file_name)
    {
        const rapidcsv::Document map_doc(file_name, rapidcsv::LabelParams(-1, -1));
        for (int q = 0; static_cast<unsigned int>(q) < map_doc.GetColumnCount(); q++)
        {
            std::vector<hex_tile> column;
            for (int r = 0; static_cast<unsigned int>(r) < map_doc.GetColumn<int>(q).size(); r++)
            {
                const bool passable = map_doc.GetColumn<int>(q).at(r) != 0;
                hex_tile t{q - (r + (r & 1)) / 2, r, passable};
                column.push_back(t);
            }
            this->add_column(column);
        }
    }
    std::vector<hex_tile*> path_a_star(hex_tile* start, hex_tile* end);

    hex_tile* get_tile(const int q, const int r)
    {
        return &tiles_map_[q + (r + (r & 1)) / 2][r];
    }

    void add_column(const std::vector<hex_tile>& col)
    {
        tiles_map_.push_back(col);
    }

    std::vector<std::vector<hex_tile>> tiles_map_get()
    {
        return tiles_map_;
    }

    static bool in_map(const int q, const int r)
    {
        return q + (r + (r & 1)) / 2 >= 0 && r >= 0 && q + (r + (r & 1)) / 2 < size_q && r < size_r;
    }

    [[nodiscard]] bool passable(const int q, const int r) const 
    {
        if (!in_map(q, r))
            return false;
        return tiles_map_[q + (r + (r & 1)) / 2][r].passable();
    }

private:
    std::vector<std::vector<hex_tile>> tiles_map_;
};
