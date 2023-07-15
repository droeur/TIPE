#pragma once
#include <vector>
#include "units.hpp"

typedef struct
{
    unit_class* u;
    int q, r;
    int cluster_id;
} point;

class db_scan
{
    unsigned int m_point_size_;
    unsigned int m_min_points_;
    float m_epsilon_;

public:
    db_scan(const unsigned int min_pts, const float eps, const std::vector<point> &points)
    {
        m_min_points_ = min_pts;
        m_epsilon_ = eps;
        m_points = points;
        m_point_size_ = static_cast<unsigned int>(points.size());
    }
    ~db_scan() = default;

    int run();
    [[nodiscard]] std::vector<int> calculate_cluster(const point& point) const;
    int expand_cluster(const point& p, int cluster_id);
    static inline double calculate_distance(const point& point_core, const point& point_target);

    [[nodiscard]] unsigned int get_total_point_size() const { return m_point_size_; }
    [[nodiscard]] unsigned int get_minimum_cluster_size() const { return m_min_points_; }
    [[nodiscard]] float get_epsilon_size() const { return m_epsilon_; }
    
    std::vector<point> m_points;
};
