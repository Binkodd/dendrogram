//Метод медиан - тот же центроидный метод, но центр объединенного кластера вычисляется как среднее всех объектов (Median clustering).
// https://intuit.ru/studies/courses/6/6/lecture/182?page=4

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>



using namespace cv;
using namespace std;


class Cluster {
public:
    Cluster(const Point2f p){
        center = p;
        height = sqrt(pow(p.x, 2) + pow(p.y, 2));
        dist = 0.0;
        size = 1;
    }
    
    Cluster(const Cluster& lhs, const Cluster& rhs){
        float whole_size = float((lhs.size + rhs.size));
        center = lhs.center * float(lhs.size) / (whole_size) +
                rhs.center * float(rhs.size) / (whole_size);
        
        height = (lhs.height + rhs.height) / 2;
        dist = lhs.get_dist(rhs);
        size = whole_size;
    }
    
    
    
    static void draw(const vector<Point2f>& points, const vector<string>& labels) {
        Mat img(1024, 1024, CV_8UC1, Scalar(255, 255, 255));
        
        vector<pair<Point2f, string>> pts;
        for (int i = 0; i < points.size(); i++) {
            pts.push_back({points[i], labels[i]});
        }
        sort(pts.begin(), pts.end(), Cluster::sort_points_labels);
 
        
        vector<Point2f> ptd = dendrogram_points(pts);
        
        float min_height = norm(pts[0].first);
        
        
        float xdev = 100; float ydev = 100;
        float height_mult = (1024 - 3 * ydev) / (norm(pts[pts.size() - 1].first) - min_height);
        float dist_mult = (1024 - 2 * xdev) / ptd[ptd.size() - 1].x;
        
        
        draw_grid(img, pts, xdev, ydev , dist_mult, height_mult, ptd[ptd.size() - 1].x, min_height);
        
        for (int i = 0; i < ptd.size(); i+= 2) {
            line(img,
                 Point2f(ptd[i].x * dist_mult + xdev,
                         (ptd[i].y - min_height) * height_mult + 2 * ydev),
                 
                 Point2f(ptd[i+1].x * dist_mult + xdev,
                         (ptd[i+1].y - min_height) *  height_mult + 2 * ydev),
                 
                 (0,0,0), 1.5
                 );
            //cout << ptd[i] << endl;
            
           
        }
        imshow("Output", img);
        waitKey(0);
        
    }
private:
    float height, dist; // dist, height
    Point2f center;
    int size;
    
    
    static bool sort_points_labels(const pair<Point2f, string> & a, const pair<Point2f, string> &b)
    {
        return norm(a.first) < norm(b.first);
    }
    
    const float get_dist(const Cluster& rhs) const {
        return norm(this->center - rhs.center);
    }
    
    static tuple<float, float> find_min_dist_indices(const vector<Cluster> &clusters) {
        int ind1 = 0, ind2 = 1;
        float mindist = norm(clusters[1].center - clusters[0].center);
        for (int i = 0; i < clusters.size(); i++) {
            for (int j = i + 1; j < clusters.size(); j++) {
                if (clusters[j].get_dist(clusters[i]) < mindist) {
                    mindist = norm(clusters[j].center - clusters[i].center);
                    ind1 = i; ind2 = j;
                }
            }
        }
        return tuple<float, float>(ind1, ind2);
    }
    
    
    static vector<Point2f> dendrogram_points(const vector<pair<Point2f, string>>& points) {
        vector<Cluster> clusters;
        for (pair<Point2f, string> point : points) {
            clusters.push_back(Cluster(point.first));
        }
        
        vector<Point2f> ptd;
        
        while (clusters.size() >= 2) {
            tuple<float, float> inds = Cluster::find_min_dist_indices(clusters);
            
            ptd.push_back(
                                     Point2f(clusters[get<0>(inds)].dist, clusters[get<0>(inds)].height)
                          );
            ptd.push_back(
                                     Point2f(clusters[get<0>(inds)].get_dist(clusters[get<1>(inds)]), clusters[get<0>(inds)].height)
                          );
            
            ptd.push_back(
                                     Point2f(clusters[get<1>(inds)].dist, clusters[get<1>(inds)].height)
                          );
            ptd.push_back(
                                     Point2f(clusters[get<1>(inds)].get_dist(clusters[get<0>(inds)]), clusters[get<1>(inds)].height)
                          );
            
            if (clusters.size() != 1) {
                ptd.push_back(
                                         Point2f(clusters[get<0>(inds)].get_dist(clusters[get<1>(inds)]), clusters[get<0>(inds)].height)
                              );
                ptd.push_back(
                                         Point2f(clusters[get<1>(inds)].get_dist(clusters[get<0>(inds)]), clusters[get<1>(inds)].height)
                              );
                
            }
            
            clusters[get<1>(inds)] = Cluster(clusters[get<0>(inds)], clusters[get<1>(inds)]);
            clusters.erase(clusters.begin() + get<0>(inds));
        }
        
        return ptd;
        
    }
    
    static void draw_grid(Mat & img, const vector<pair<Point2f, string>> pts, int xdev, int ydev, float dist_mult, float height_mult, float max_dist, float min_height)
    {
        
        line(img,
             Point2f(xdev, 0),
             Point2f(xdev, 1024),
             (0,0,0), 2
             );
        
        line(img,
             Point2f(0, ydev),
             Point2f(1024, ydev),
             (0,0,0), 2
             );
        
        putText(img,
                "Distance between clusters",
                Point2f((512 + xdev)/2, ydev/2), FONT_HERSHEY_SIMPLEX, 1.2, (0,0,0), 2
                );
        
        for (int i = max_dist / 5; i < max_dist + 1; i += max_dist / 5) {
            putText(img,
                    to_string(i),
                    Point2f(i * dist_mult + xdev - to_string(i).length() * 8, ydev * 0.9), FONT_HERSHEY_SIMPLEX, 0.8, (0,0,0), 2
                    );
            line(img,
                 Point2f(i * dist_mult + xdev, ydev * 0.95),
                 Point2f(i * dist_mult + xdev, ydev * 1.05),
                 (0,0,0), 2
                 );
            
        }
        string label;
        for (pair<Point2f, string> point : pts) {
            label = point.second;
            if (label.size() > 5) {
                label = label.substr(0, 5);
                label += "...";
            }
            
            putText(img, label,
                    Point2f(xdev - label.length()*8 - 30, (norm(point.first) - min_height) * height_mult + 2 * ydev + 10), FONT_HERSHEY_SIMPLEX, 0.7, (0,0,0), 1
                    );
            cout << Point2f(xdev - label.length()*8 - 20, (norm(point.first) - min_height) * height_mult + 2 * ydev + 10);
            
        }
        
    }
    
};
int main(int argc, char** argv) {
    
    int mult = 70;
    
    // Считывание с файла
    vector<Point2f> points;
    float num1, num2;
    
    string line;
    ifstream in ("/Users/danil/Documents/Study/ImageProcessing/dendrogram/dendrogram/points.txt");
    if (in.is_open())
    {
        while (in >> num1) {
            in >> num2;
            points.push_back(Point2f(num1, num2));
        }
    }
    in.close();
    
    vector<string> labels;
    string label;

    in = ifstream("/Users/danil/Documents/Study/ImageProcessing/dendrogram/dendrogram/labels.txt");
    if (in.is_open())
    {
        while (getline(in, label)) {
            labels.push_back(label);
        }
    }
    in.close();
        
    Cluster::draw(points, labels);
    
    return 0;
    

}
