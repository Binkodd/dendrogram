#include "dendrogram.hpp"

int main(int argc, char** argv) {

    std::vector<std::string> labels{
        "Ufa",
        "Volgograd",
        "Nizhnij",
        "Novosibirsk",
        "Omsk",
        "Perm",
        "Rostov" };

    std::vector<double> coord {
        54.7349439, 55.9578468,
            48.7070042, 44.5170339,
            56.3240627, 44.0053912,
            55.0281910, 82.9211489,
            54.9848566, 73.3674517,
            58.0102583, 56.2342034,
            47.2224566, 39.7188030
    };

    std::vector<cv::Point2f> points;
    for (size_t i = 0; i < coord.size(); i+=2) {
        points.push_back(cv::Point2f(coord[i], coord[i + 1]));
    }
    
        
   cv::Mat img  = get_dendrogram(points, labels);
   cv::imshow("Dendrogram", img);
   cv::waitKey(0);
    
    return 0;

}
