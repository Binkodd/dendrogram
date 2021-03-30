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

/*!
 \brief Вспомогательный класс, использующийся для построения дендрограмм.
 Содержит в себе информацию о кол-ве точек в кластере и центр кластера (для построения используется метод медиан - Median clustering).
 */
class Cluster {
public:
    /*!
     Конструктор, создающий кластер из одной точки
     \param p Исходная точки
     \param height Аргумент, отвечающий за нахождение кластера по Y-оси
     */
    Cluster(const cv::Point2f p, float height);
    /*!
     Конструктор, объединяющий два кластера в один
     */
    Cluster(const Cluster& lhs, const Cluster& rhs); // Объединияет 2 кластера и создает новый
    
    const float get_dist(); // Возвращает внутрикласстерное расстояние по X-оси
    const float get_height(); // Возвращает нахождение кластера по Y-оси
    const cv::Point2f get_center() const; //
    
    // Возвращает кортеж с индексами самых ближайших кластеров в векторе
    
    
    // Возвращает расстояние между двумя кластерами
    const float get_dist_between(const Cluster& rhs) const;
    
private:
    float dist;   // расстрояние между элементами кластера. x-координата кластера
    float height; // среднее значение кластера. y-координата кластера
    cv::Point2f center; // центр кластера
    int size;  // кол-во точек в кластере
};

std::tuple<float, float> find_min_dist_indices(const std::vector<Cluster> &clusters);

// Компаратор для пар <точка, метка>. Метка - "название" точки
bool sort_points_labels(const std::pair<cv::Point2f, std::string> & a, const std::pair<cv::Point2f, std::string> &b);

/*!
 Из исходных данных получает точки для построения дендрограммы
 \param points Вектор пар<точка, метка> - отсортированные исходные данные
 \return Вектор точек для построения дендрограммы. Точки n и n+1 образуют отрезок, где n - четное натуральное число
 */
std::vector<cv::Point2f> dendrogram_points(const std::vector<std::pair<cv::Point2f, std::string>>& points);

/*!
 Рисует оси, метки и другие вспомогательные обозначения для дендрограммы
 \param[in,out] img cv::Mat на котором рисуются все обозначения
 \param[in] pts Отсортированные изначальные входные данные, требуются для построения меток
 \param[in] xdev Отступ по X-оси
 \param[in] ydev Отступ по Y-оси
 \param[in] dist_mult Мультипликатор по X-оси
 \param[in] height_mult Мультипликатор по Y-оси
 \param[in] min_height Значение нормы минимальной точки из изначальных. Используется в кач-ве точки отсчета
 */
void draw_grid(cv::Mat & img, const std::vector<std::pair<cv::Point2f, std::string>> pts, int xdev, int ydev, float dist_mult, float height_mult, float max_dist, float min_height);

/*!
 Основная функция построения дендрограммы
 \param points Вектор точек
 \param labels Вектор меток (обозначений) для точек. Необязательный параметр. Если он отсутствует или размер вектора не совпадает с размером вектора точек, то используется нумерация 1, 2, 3 ...
 \return cv::Mat с постороенное дендрограммой и всеми обозначениями
 */

cv::Mat get_dendrogram(const std::vector<cv::Point2f>& points, const std::vector<std::string>& labels= {});


#endif /* dendogram_hpp */
