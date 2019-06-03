#ifndef SPATIALFILTERING_H
#define SPATIALFILTERING_H
#include <QVector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>

#pragma once
using namespace cv;


static const int dr[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
static const int dc[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

Mat omitMissingPixels(const Mat &, std::function<uchar(QVector<uchar>)> func);
Mat padImage(const Mat &, std::function<uchar(QVector<uchar>)> func);
Mat replicateBorderPixels(const Mat &, std::function<uchar(QVector<uchar>)> func);
Mat truncateImage(const Mat &, std::function<uchar(QVector<uchar>)> func);
Mat pixelsWrapAround(const Mat &, std::function<uchar(QVector<uchar>)> func);

uchar minNeigh(QVector<uchar> vt);
uchar maxNeigh(QVector<uchar>);
uchar medianNeigh(QVector<uchar>);
uchar averageNeigh(QVector<uchar>);
uchar weightedAverageNeigh(QVector<uchar>);
uchar laplacian(QVector<uchar>);
uchar sobel(QVector<uchar>);

bool isInside(int, int, int, int);
QVector<uchar> take_mat(int, int, Mat);


#endif // SPATIALFILTERING_H
