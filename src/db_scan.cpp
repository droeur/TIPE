#include "db_scan.hpp"

using namespace std;

enum
{
     unclassified = -1,
     core_point = 1,
     border_point = 2,
     noise = -2,
     success = 0,
     failure = -3
};

int db_scan::run()
{
     int cluster_id = 1;
     for (const auto& m_point : m_points)
     {
         if (m_point.cluster_id == unclassified)
         {
             if (expand_cluster(m_point, cluster_id) != failure)
             {
                 cluster_id += 1;
             }
         }
     }

     return 0;
}

int db_scan::expand_cluster(const point& p, const int cluster_id)
{
     vector<int> cluster_seeds = calculate_cluster(p);

     if (cluster_seeds.size() < m_min_points_)
     {
         return failure;
     }
     int index = 0, index_core_point = 0;
     for (const int& cluster_seed : cluster_seeds)
     {
         m_points.at(cluster_seed).cluster_id = cluster_id;
         if (m_points.at(cluster_seed).q == p.q && m_points.at(cluster_seed).r == p.r)
         {
             index_core_point = index;
         }
         ++index;
     }
     cluster_seeds.erase(cluster_seeds.begin() + index_core_point);

     for (vector<int>::size_type i = 0, n = cluster_seeds.size(); i < n; ++i)
     {

         if (vector<int> cluster_neighbors = calculate_cluster(m_points.at(cluster_seeds[i])); cluster_neighbors.size() >= m_min_points_)
         {
             vector<int>::iterator iterator_neighbors;
             for (iterator_neighbors = cluster_neighbors.begin(); iterator_neighbors != cluster_neighbors.end(); ++iterator_neighbors)
             {
                 if (m_points.at(*iterator_neighbors).cluster_id == unclassified ||
                     m_points.at(*iterator_neighbors).cluster_id == noise)
                 {
                     if (m_points.at(*iterator_neighbors).cluster_id == unclassified)
                     {
                         cluster_seeds.push_back(*iterator_neighbors);
                         n = cluster_seeds.size();
                     }
                     m_points.at(*iterator_neighbors).cluster_id = cluster_id;
                 }
             }
         }
     }

     return success;
}

vector<int> db_scan::calculate_cluster(const point &point) const
{
     int index = 0;
     vector<int> cluster_index;
     for (auto& m_point : m_points)
     {
         if (calculate_distance(point, m_point) <= static_cast<double>(m_epsilon_))
         {
             cluster_index.push_back(index);
         }
         index++;
     }
     return cluster_index;
}

inline double db_scan::calculate_distance(const point& point_core, const point& point_target)
{
     return pow(static_cast<double>(point_core.q) - static_cast<double>(point_target.q), 2.0) + pow(static_cast<double>(point_core.r) - static_cast<double>(point_target.r), 2.0);
}