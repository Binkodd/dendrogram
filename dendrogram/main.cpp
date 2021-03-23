//Метод медиан - тот же центроидный метод, но центр объединенного кластера вычисляется как среднее всех объектов (Median clustering).
// https://intuit.ru/studies/courses/6/6/lecture/182?page=4

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include "dendogram.hpp"


using namespace cv;
using namespace std;

int main(int argc, char** argv) {

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
        
    Mat img  = get_dendrogram(points, labels);
    imshow("Out", img);
    waitKey(0);
    
    return 0;
    

}
