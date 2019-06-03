#ifndef POINTPROCESSING_H
#define POINTPROCESSING_H
#include <QMainWindow>
#include <QVector>
#include <QFileDialog>
#include <QImage>
#include <QImageReader>
#include <QDebug>
#include <QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#pragma once
using namespace cv;
Mat transformToNegativeImage(Mat const &);
Mat thresHolding(Mat const &,int const &);
Mat logarithmic(Mat const &, int);
Mat powerlaw(Mat const &, int, float const &);
Mat bitplane(Mat const &, int const &, int const &);
Mat histogramEqualization(Mat const &);
QVector<int> calculateHistogram(const cv::Mat &);

#endif // POINTPROCESSING_H
