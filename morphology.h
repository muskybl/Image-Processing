#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <QString>
#include <QVector>
#include <QtMath>
#include <QDebug>
#include "pointprocessing.h"
#include "spatialfiltering.h"

#pragma once
using namespace cv;
static const int object  = 0;
static const int background =  255;
static const QVector<int> structuringElement({0, 1, 0, 1, 1, 1, 0, 1, 0});
QVector<int> get3x3Mat(const Mat &, const int &, const int &);

bool isFit(const QVector<int> &, const QVector<int> &);
bool isHit(const QVector<int>&, const QVector<int>&);
bool isbinaryImage(const Mat &);
Mat erosion(const Mat &, const QVector<int> &);
Mat dilation(const Mat &, const QVector<int> &);
Mat openingAl(const Mat &, const QVector<int> &);
Mat closingAl(const Mat &, const QVector<int> &);
Mat boundaryExtraction(const Mat &, const QVector<int> &);



static const std::function<Mat (const Mat &, const QVector<int> &)> morphology[] = {erosion,
                                                                                    dilation,
                                                                                    openingAl,
                                                                                    closingAl,
                                                                                    boundaryExtraction
                                                                                   };


#endif // MORPHOLOGY_H
