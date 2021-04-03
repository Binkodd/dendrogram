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
     \brief Конструктор, создающий кластер из одной точки
     \param p Исходная точки
     \param height Аргумент, отвечающий за нахождение кластера по Y-оси
     */
    Cluster(const cv::Point2f p, float height);
    /*!
     \brief Конструктор, объединяющий два кластера в один
     */
    Cluster(const Cluster& lhs, const Cluster& rhs); // Объединияет 2 кластера и создает новый

    const float get_dist(); /// \brief Возвращает внутрикласстерное расстояние по X-оси
    const float get_height(); /// \brief Возвращает нахождение кластера по Y-оси
    const cv::Point2f get_center() const; /// \brief Возвращает центр кластера в виде точки
    
    
    /*!
     * @brief Возвращает расстояние между двумя кластерами
     * @param rhs До какого кластера находится расстояние
     * @return Расстояние между кластерами
    */
    const float get_dist_between(const Cluster& rhs) const;
    
private:
    float dist;   /// расстрояние между элементами кластера. x-координата кластера
    float height; /// среднее значение кластера. y-координата кластера
    cv::Point2f center; /// центр кластера
    int size;  /// кол-во точек в кластере
};

/*!
 * \brief Находит в векторе индексы двух ближайших кластеров
 *
 * \param clusters Вектор кластеров
 * \return Пара индексов двух ближайших кластеров
*/
std::pair<float, float> find_min_dist_indices(const std::vector<Cluster> &clusters);


/*!
 * \brief Компаратор для пар <точка, метка>
 * \param a Первая пара <точка, метка>
 * \param b Вторая пара <точка, метка>
 * \return Булево значение a < b
*/
bool sort_points_labels(const std::pair<cv::Point2f, std::string> & a, const std::pair<cv::Point2f, std::string> &b);

/*!
 Из исходных данных получает точки для построения дендрограммы
 \param points Вектор пар<точка, метка> - отсортированные исходные данные
 \param window_size Размер изображения
 \return Вектор точек для построения дендрограммы. Точки n и n+1 образуют отрезок, где n - четное натуральное число
 */
std::vector<cv::Point2f> dendrogram_points(const std::vector<std::pair<cv::Point2f, std::string>>& points, int window_size);
/*!
 Рисует оси, метки и другие вспомогательные обозначения для дендрограммы
 \param[in,out] img cv::Mat на котором рисуются все обозначения
 \param[in] pts Отсортированные изначальные входные данные, требуются для построения меток
 \param[in] xdev Отступ по X-оси
 \param[in] ydev Отступ по Y-оси
 \param[in] dist_mult Мультипликатор по X-оси
 \param[in] height_mult Мультипликатор по Y-оси
 \param[in] min_height Значение нормы минимальной точки из изначальных. Используется в кач-ве точки отсчета
 \param[in] window_size Размер изображения
 */
void draw_grid(cv::Mat & img, const std::vector<std::pair<cv::Point2f, std::string>> pts, int xdev, int ydev, float dist_mult, float height_mult, float max_dist, float min_height, int window_size);

/*!
 Основная функция построения дендрограммы
 \param points Вектор точек
 \param labels Вектор меток (обозначений) для точек. Необязательный параметр. Если он отсутствует или размер вектора не совпадает с размером вектора точек, то используется нумерация 1, 2, 3 ...
 \param window_size Размер изображения с дендрограммой. Необязательный параметр, по умолчанию = 768
 \return cv::Mat с постороенное дендрограммой и всеми обозначениями
 */
cv::Mat get_dendrogram(const std::vector<cv::Point2f>& points, const std::vector<std::string>& labels= {}, int window_size = 768);


#endif /* dendogram_hpp */
