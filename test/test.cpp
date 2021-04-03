
#include <fstream>
#include "dendrogram.hpp"

int main(int argc, char** argv) {

    // Считываем данные в вектор точек
    std::vector<cv::Point2f> points;
    float num1, num2;
    
    std::string line;
    std::ifstream in ("data/coord.txt");
    if (in.is_open())
    {
        while (in >> num1) {
            in >> num2;
            points.push_back(cv::Point2f(num1, num2));
        }
    }
    in.close();
    
    // Считываем данные в вектор меток
    std::vector<std::string> labels;
    std::string label;
    in = std::ifstream("data/city_names.txt");
    if (in.is_open())
    {
        while (getline(in, label)) {
            labels.push_back(label);
        }
    }
    in.close();
        
    cv::Mat img  = get_dendrogram(points, labels);
    cv::imshow("Dendrogram", img);
    cv::waitKey(0);
    
    return 0;

}
