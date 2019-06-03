#ifndef SEGMENT_H
#define SEGMENT_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <QString>
#include <QVector>
#include <QtMath>
#include <QDebug>
#include "spatialfiltering.h"
#pragma once
using namespace cv;

Mat detectedPoints(Mat const &);

Mat detectedHorizontalLines(Mat const &);
Mat detectedVerticalLines(Mat const &);
Mat detectedPositive45Lines(Mat const &);
Mat detectedNegative45Lines(Mat const &);

Mat detectedEdgePrewitt(Mat const &);
Mat detectedEdgeRoberts(Mat const &);
Mat detectedEdgeSobel(Mat const &);
//Mat detectedEdgeLaplacianofGaussian(Mat const &);

QVector<int> findHistogram(Mat const &);
QVector<double> findAveragePixels(QVector<int> const &, int const &);

bool isbinaryImage(const Mat &);
int findGlobalThresholding(Mat const &);
Mat globalThresholding(Mat const &, int &);
uchar mulVec(const int [],const QVector<uchar> &);


static const std::function<Mat (Mat const &)> detectedLines[] = {
                                                detectedHorizontalLines,
                                                detectedVerticalLines,
                                                detectedPositive45Lines,
                                                detectedNegative45Lines
                                                };
static const std::function<Mat (Mat const &)> detectedEdges[] = {
                                                                 detectedEdgePrewitt,
                                                                 detectedEdgeRoberts,
                                                                 detectedEdgeSobel
                                                                };

static const QString pointMat(       "     -1     -1     -1\n"
                                     "     -1      8     -1\n"
                                     "     -1     -1     -1");
static const QString horizontalMat(  "     -1     -1     -1\n"
                                     "      2      2     2\n"
                                     "     -1     -1     -1");
static const QString verticalMat(    "     -1     2     -1\n"
                                     "     -1     2     -1\n"
                                     "     -1     2     -1");
static const QString positive45Mat(  "     -1     -1     2\n"
                                     "     -1      2    -1\n"
                                     "      2     -1    -1");
static const QString negagtive45Mat( "     2    -1    -1\n"
                                     "    -1     2    -1\n"
                                     "    -1    -1     2");
static const QString lineExplain[] = {horizontalMat, verticalMat, positive45Mat, negagtive45Mat};



static const QString edgePrewittMat("     -1     -1     -1   |     -1     0     -1\n"
                                    "      0      0      0   |     -1     0     -1\n"
                                    "     -1     -1     -1   |     -1     0     -1");

static const QString robertsMat( "     0     0     0   |     0     0     0\n"
                                 "     0    -1     0   |     0     0    -1\n"
                                 "     0     0     1   |     0     1     0");

static const QString sobelMat("     -1     -2     -1   |     -1     0     1\n"
                              "      0      0      0   |     -2     0     2\n"
                              "      1      2      1   |     -1     0     1");

static const QString edgeExplain[] = {edgePrewittMat, robertsMat, sobelMat};

#endif // SEGMENT_H
