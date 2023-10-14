#pragma once

#include <assert.h>
#include <vector>
#include <float.h>
#include <string>

/**
 * Hex directions:
 *     3 2
 *    4 H 1
 *     5 6 
 */

class hex_tile
{
    int q_ = -1;
    int r_ = -1;
    int s_ = -1;
    bool passable_ = false;
    double x_graphic_;
    int x_index_;

public:
    hex_tile(const int q, const int r)
        : q_(q)
        , r_(r)
        , s_(-q - r)
        , x_graphic_(q + static_cast<double>(r) / 2)
        , x_index_(q + (r + (r & 1)) / 2)
    {}

    hex_tile(const int q, const int r, const int s)
        : q_(q)
        , r_(r)
        , s_(s)
        , x_graphic_(q + static_cast<double>(r) / 2)
        , x_index_(q + (r + (r & 1)) / 2)
    {}

    hex_tile(const int q, const int r, const int s, const bool passable)
        : q_(q)
        , r_(r)
        , s_(s)
        , passable_(passable)
        , x_graphic_(q + static_cast<double>(r) / 2)
        , x_index_(q + (r + (r & 1)) / 2)
    {}

    hex_tile(const int q, const int r, const bool passable)
        : q_(q)
        , r_(r)
        , s_(-q - r)
        , passable_(passable)
        , x_graphic_(q + static_cast<double>(r) / 2)
        , x_index_(q + (r + (r & 1)) / 2)
    {}

    bool operator==(const hex_tile &a) const;
    bool operator!=(const hex_tile &a) const;
    hex_tile operator+(const hex_tile &a) const;
    hex_tile operator-(const hex_tile &a) const;

    [[nodiscard]] bool passable() const;

    [[nodiscard]] double distance(const hex_tile& a) const;
    double distance(const hex_tile* a) const;

    // coordinates cube
    [[nodiscard]] int q() const;
    [[nodiscard]] int r() const;
    [[nodiscard]] int s() const;

    //coordinates "offset even-r"
    [[nodiscard]] int index_x() const;
    [[nodiscard]] int index_y() const;

    [[nodiscard]] double graphic_x() const;
    [[nodiscard]] double graphic_y() const;
    
    [[nodiscard]] bool is_valid() const;
};

class map_class
{
    int size_x_;
    int size_y_;
    std::vector<std::vector<hex_tile*>*>* tiles_map_;

public:
    explicit map_class(const std::string& file_name);

    std::vector<hex_tile*> path_a_star(hex_tile* start, const hex_tile* end) const;

    static bool in_map(int q, int r, const map_class& map);
    [[nodiscard]] hex_tile* tile_get(int q, int r) const;

    void row_add(std::vector<hex_tile*>* row) const;

    [[nodiscard]] std::vector<std::vector<hex_tile*>*>* tiles_map_get() const;
    [[nodiscard]] hex_tile* neighbor(int q, int r, int dir) const;

    [[nodiscard]] bool passable(int q, int r) const;

    [[nodiscard]] int size_x_get() const;
    [[nodiscard]] int size_y_get() const;

    hex_tile* neighbor(const hex_tile* tile, int dir) const;
};
