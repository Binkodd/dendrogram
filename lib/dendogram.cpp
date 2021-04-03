//
//  dendogram.cpp
//  dendrogram
//
//  Created by Данил Шумков on 18.03.2021.
//  Copyright © 2021 Данил Шумков. All rights reserved.
//

#include "dendogram.hpp"

Cluster::Cluster(const cv::Point2f p, float h){
    center = p;
    height = h;
    dist = 0.0;
    size = 1;
}

Cluster::Cluster(const Cluster& lhs, const Cluster& rhs){
    float whole_size = float((lhs.size + rhs.size));
    center = lhs.center * float(lhs.size) / (whole_size) +
            rhs.center * float(rhs.size) / (whole_size);
    
    height = (lhs.height + rhs.height) / 2;
    dist = lhs.get_dist_between(rhs);
    size = whole_size;
}

const float Cluster::get_dist() {
    return this->dist;
}
const float Cluster::get_height() {
    return this->height;
}
const cv::Point2f Cluster::get_center() const{
    return this->center;
}

std::pair<float, float> find_min_dist_indices(const std::vector<Cluster> &clusters) {
    size_t ind1 = 0, ind2 = 1;
    float mindist = cv::norm(clusters[1].get_center() - clusters[0].get_center());
    for (size_t i = 0; i < clusters.size(); i++) {
        for (size_t j = i + 1; j < clusters.size(); j++) {
            if (clusters[j].get_dist_between(clusters[i]) < mindist) {
                mindist = cv::norm(clusters[j].get_center() - clusters[i].get_center());
                ind1 = i; ind2 = j;
            }
        }
    }
    return std::tuple<float, float>(ind1, ind2);
}
const float Cluster::get_dist_between(const Cluster& rhs) const {
    return norm(this->center - rhs.center);
}


bool sort_points_labels(const std::pair<cv::Point2f, std::string> & a, const std::pair<cv::Point2f, std::string> &b)
{
    return norm(a.first) < norm(b.first);
}

std::vector<cv::Point2f> dendrogram_points(const std::vector<std::pair<cv::Point2f, std::string>>& points, int window_size) {
    std::vector<Cluster> clusters;
    
    for (size_t i = 0; i < points.size(); i+=1) {
        clusters.push_back(Cluster(points[i].first, i * window_size / points.size()));
    }
    
  
    
    std::vector<cv::Point2f> ptd;
    
    while (clusters.size() >= 2) {
        std::pair<float, float> inds = find_min_dist_indices(clusters);
        
        ptd.push_back(
                                 cv::Point2f(clusters[std::get<0>(inds)].get_dist(), clusters[std::get<0>(inds)].get_height())
                      );
        ptd.push_back(
                                 cv::Point2f(clusters[std::get<0>(inds)].get_dist_between(clusters[std::get<1>(inds)]), clusters[std::get<0>(inds)].get_height())
                      );
        
        ptd.push_back(
                                 cv::Point2f(clusters[std::get<1>(inds)].get_dist(), clusters[std::get<1>(inds)].get_height())
                      );
        ptd.push_back(
                                 cv::Point2f(clusters[std::get<1>(inds)].get_dist_between(clusters[std::get<0>(inds)]), clusters[std::get<1>(inds)].get_height())
                      );
        
        if (clusters.size() != 1) {
            ptd.push_back(
                                     cv::Point2f(clusters[std::get<0>(inds)].get_dist_between(clusters[std::get<1>(inds)]), clusters[std::get<0>(inds)].get_height())
                          );
            ptd.push_back(
                                     cv::Point2f(clusters[std::get<1>(inds)].get_dist_between(clusters[std::get<0>(inds)]), clusters[std::get<1>(inds)].get_height())
                          );
            
        }
        
        clusters[std::get<1>(inds)] = Cluster(clusters[std::get<0>(inds)], clusters[std::get<1>(inds)]);
        clusters.erase(clusters.begin() + std::get<0>(inds));
    }
    
    return ptd;
    
}

void draw_grid(cv::Mat & img, const std::vector<std::pair<cv::Point2f, std::string>> pts, int xdev, int ydev, float dist_mult, float height_mult, float max_dist, float min_height, int window_size){
    //! Рисуем оси
    cv::line(img,
         cv::Point2f(xdev, 0),
         cv::Point2f(xdev, window_size),
         0, 2
         );
    
    cv::line(img,
         cv::Point2f(0, ydev),
         cv::Point2f(window_size, ydev),
         0, 2
         );
    
    cv::putText(img,
            "Distance between clusters",
            cv::Point2f((window_size/2 + xdev)/2, ydev/2), cv::FONT_HERSHEY_SIMPLEX, (float) window_size/850, 0, (float) window_size/850*2
            );
    
    //! Рисуем обозначения на X-оси
    for (size_t i = max_dist / 5; i < max_dist + 1; i += max_dist / 5) {
        cv::putText(img,
                std::to_string(i),
                cv::Point2f(i * dist_mult + xdev - std::to_string(i).length() * 9 * window_size/1100, ydev * 0.9), cv::FONT_HERSHEY_SIMPLEX, (float) window_size/1100, 0, (float) window_size/1100*2
                );
        cv::line(img,
             cv::Point2f(i * dist_mult + xdev, ydev * 0.95),
             cv::Point2f(i * dist_mult + xdev, ydev * 1.05),
             0, 2
             );
        
    }
    std::string label;
    
    //! Рисуем обозначения на Y-оси
    for (size_t i = 0; i < pts.size(); i+=1) {
        label = pts[i].second;
        if (label.size() > 5) {
            label = label.substr(0, 5);
            label += "...";
        }
        
        cv::putText(img, label,
                    cv::Point2f(5, window_size * i / pts.size() * 0.8 + 2 * ydev) , cv::FONT_HERSHEY_SIMPLEX, (float) window_size/1400, 0, 1
        );
    }
    
    
    
    
}

cv::Mat get_dendrogram(const std::vector<cv::Point2f>& points, const std::vector<std::string>& i_labels, int window_size) {
    cv::Mat img(window_size, window_size, CV_8UC1, cv::Scalar(255, 255, 255));
    
    
    std::vector<std::string> labels = i_labels;
    
    if (points.size() != labels.size()) {
        labels.resize(0);
        for (size_t i = 0; i < points.size(); i+=1) {
            labels.push_back(std::to_string(i + 1));
        }
    }
    
    std::vector<std::pair<cv::Point2f, std::string>> pts;
    for (size_t i = 0; i < points.size(); i++) {
        pts.push_back({points[i], labels[i]});
    }
    
    sort(pts.begin(), pts.end(), sort_points_labels);
       
    std::vector<cv::Point2f> ptd = dendrogram_points(pts, window_size);
    
    
    float min_height = norm(pts[0].first);
    
    float xdev = window_size/10; float ydev = window_size/10;
    float height_mult = (window_size - 3 * ydev) / (norm(pts[pts.size() - 1].first) - min_height);
    float dist_mult = (window_size - 2 * xdev) / ptd[ptd.size() - 1].x;
    
    
    draw_grid(img, pts, xdev, ydev, dist_mult, height_mult, ptd[ptd.size() - 1].x, min_height, window_size);
    
    
    for (size_t i = 0; i < ptd.size(); i+= 2) {
        cv::line(img,
            cv::Point2f(ptd[i].x * dist_mult + xdev,
                        ptd[i].y * 0.8 + 2 * ydev),
                            
                
            cv::Point2f(ptd[i+1].x * dist_mult + xdev,
                        ptd[i+1].y * 0.8 + 2 * ydev),
            0, 1.5
        );
    }
    return img;
}

