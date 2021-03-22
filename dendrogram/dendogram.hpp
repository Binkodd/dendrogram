//
//  dendogram.hpp
//  Библиотека для построения дендрограмм
//
//  Created by Данил Шумков on 18.03.2021.
//  Copyright © 2021 Данил Шумков. All rights reserved.
//

#ifndef dendogram_hpp
#define dendogram_hpp

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>


// Вспомогательный класс, использующийся для построения дендрограмм. Содержит в себе информацию о кол-ве точек
// в кластере и центр кластера (для построения используется метод медиан - Median clustering).
class Cluster {
public:
    Cluster(const cv::Point2f p);   // Создает кластер из точки
    Cluster(const Cluster& lhs, const Cluster& rhs); // Объединияет 2 кластера и создает новый
    
    const float get_dist(); // Возвращает
    const float get_height(); // Возвращает height -
    
    // Возвращает кортеж с индексами самых ближайших кластеров
    static std::tuple<float, float> find_min_dist_indices(const std::vector<Cluster> &clusters);
    
    // Возвращает расстояние между двумя кластерами
    const float get_dist_between(const Cluster& rhs) const;
    
private:
    float dist;   // расстрояние между элементами кластера. x-координата кластера
    float height; // среднее значение кластера. y-координата кластера
    cv::Point2f center; // центр кластера
    int size;  // кол-во точек в кластере
};

// Компаратор для пар <точка, метка>. Метка - "название" точки
static bool sort_points_labels(const std::pair<cv::Point2f, std::string> & a, const std::pair<cv::Point2f, std::string> &b);

// Возвращает вектор точек, которые используются при постройке дендрограммы
static std::vector<cv::Point2f> dendrogram_points(const std::vector<std::pair<cv::Point2f, std::string>>& points);

// Рисует сетку, метки и другие обозначения для дендрограммы
// img - cv::Mat с дедрограммой, где рисуем сетку
// pts - изначальные входные данные - вектор пар <точка, метка>

//----__!!!!!! Поменять отступы на статические
// xdev - отступ по X-оси!!!!!!!!!!
// ydev - отступ по Y-оси
// height_mult, dist_mult
//-----!!!!!!!!!!!

// min_height - значение нормы точки, с наименьшей длиной радиус вектора из всех изначальных
static void draw_grid(cv::Mat & img, const std::vector<std::pair<cv::Point2f, std::string>> pts, int xdev, int ydev, float dist_mult, float height_mult, float max_dist, float min_height);

// основная функция построения дендрограммы
// points - вектор точек
// labels - вектор меток (названий) точек
// НАДО РАЗБИТЬ НА ФУНКЦИИ
void draw(const std::vector<cv::Point2f>& points, const std::vector<std::string>& labels);

#endif /* dendogram_hpp */
