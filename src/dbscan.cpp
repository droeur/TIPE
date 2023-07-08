#include "dbscan.hpp"

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

int dbscan::run()
{
     int cluster_id = 1;
     vector<point>::iterator iter;
     for (iter = m_points.begin(); iter != m_points.end(); ++iter)
     {
         if (iter->cluster_id == unclassified)
         {
             if (expand_cluster(*iter, cluster_id) != failure)
             {
                 cluster_id += 1;
             }
         }
     }

     return 0;
}

int dbscan::expand_cluster(const point p, const int cluster_id)
{
     vector<int> cluster_seeds = calculate_cluster(p);

     if (cluster_seeds.size() < m_min_points_)
     {
         return failure;
     }
     int index = 0, index_core_point = 0;
     vector<int>::iterator iter_seeds;
     for (iter_seeds = cluster_seeds.begin(); iter_seeds != cluster_seeds.end(); ++iter_seeds)
     {
         m_points.at(*iter_seeds).cluster_id = cluster_id;
         if (m_points.at(*iter_seeds).q == p.q && m_points.at(*iter_seeds).r == p.r)
         {
             index_core_point = index;
         }
         ++index;
     }
     cluster_seeds.erase(cluster_seeds.begin() + index_core_point);

     for (vector<int>::size_type i = 0, n = cluster_seeds.size(); i < n; ++i)
     {
         vector<int> cluster_neighors = calculate_cluster(m_points.at(cluster_seeds[i]));

         if (cluster_neighors.size() >= m_min_points_)
         {
             vector<int>::iterator iter_neighors;
             for (iter_neighors = cluster_neighors.begin(); iter_neighors != cluster_neighors.end(); ++iter_neighors)
             {
                 if (m_points.at(*iter_neighors).cluster_id == unclassified ||
                     m_points.at(*iter_neighors).cluster_id == noise)
                 {
                     if (m_points.at(*iter_neighors).cluster_id == unclassified)
                     {
                         cluster_seeds.push_back(*iter_neighors);
                         n = cluster_seeds.size();
                     }
                     m_points.at(*iter_neighors).cluster_id = cluster_id;
                 }
             }
         }
     }

     return success;
}

vector<int> dbscan::calculate_cluster(point p)
{
     int index = 0;
     vector<point>::iterator iter;
     vector<int> cluster_index;
     for (iter = m_points.begin(); iter != m_points.end(); ++iter)
     {
         if (calculate_distance(p, *iter) <= static_cast<double>(m_epsilon_))
         {
             cluster_index.push_back(index);
         }
         index++;
     }
     return cluster_index;
}

inline double dbscan::calculate_distance(const point& point_core, const point& point_target)
{
     return pow(point_core.q - point_target.q, 2) + pow(point_core.r - point_target.r, 2);
}