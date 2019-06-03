#include "spatialfiltering.h"
#include <QDebug>
//static const int dr[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
//static const int dc[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
static const double weighted[] = {1.0/16, 2.0/16, 1.0/16, 2.0/16, 4.0/16, 2.0/16, 1.0/16, 2.0/16, 1.0/16};
static const int laplacianFilter[] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
static const int sobelhorizon[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
static const int sobelverical[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

bool isInside(int i, int j, int r, int c)
{
    return 0 <= i && i < r
        && 0 <= j && j < c;
}

QVector<uchar> take_mat(int i, int j, Mat im)
{
    int r,c; r = im.rows; c = im.cols;
    QVector<uchar> re;
    for (int dir = 0; dir < 9; dir++)
    {
       int new_row = i + dr[dir];
       int new_col = j + dc[dir];
       if(isInside(new_row, new_col, r, c)) re.push_back(im.at<uchar>(new_row, new_col));

    }

    return  re;
}
uchar minNeigh(QVector<uchar> vt)
{
    uchar m = *std::min_element(vt.constBegin(),vt.constEnd());
    return m;
}

uchar maxNeigh(QVector<uchar> vt)
{
    return *std::max_element(vt.constBegin(), vt.constEnd());
}

uchar medianNeigh(QVector<uchar> vt)
{
    std::sort(vt.begin(),vt.end());
    return vt[vt.size()/2];
}

uchar averageNeigh(QVector<uchar> vt)
{
    uchar re = 0;
    int si = vt.size();
    for(uchar x: vt)
    {
        re += x * (1.0/si);
    }
    return re;
}

uchar weightedAverageNeigh(QVector<uchar> vt)
{
    double re = 0;
    for (int i = 0; i < vt.size(); i++)
    {
        re += vt[i]*weighted[i];
    }
    return cv::saturate_cast<uchar>(re);
}
Mat omitMissingPixels(const Mat &im, std::function<uchar (QVector<uchar>)> func)
{
    Mat result = im.clone();

    for(int i = 1; i < im.rows - 1; i++)
    {
        for(int j = 1; j < im.cols - 1; j++)
        {
            result.at<uchar>(i, j) = func(take_mat(i, j, im));
        }
    }
    return result;
}
Mat padImage(const Mat &im, std::function<uchar (QVector<uchar>)> func)
{
    Mat tam;
    copyMakeBorder(im,tam,1,1,1,1,BORDER_CONSTANT,255);
    Mat re = tam.clone();
    for (int i = 1; i < tam.rows - 1; i++)
    {
        for (int j = 1; j < tam.cols - 1; j++)
        {
            re.at<uchar>(i, j) = func(take_mat(i, j, tam));
        }
    }
    return re;
}

Mat replicateBorderPixels(const Mat &im, std::function<uchar (QVector<uchar>)> func)
{
    Mat tam;
    copyMakeBorder(im,tam,1,1,1,1,BORDER_REPLICATE);
    Mat re = tam.clone();
    for (int i = 1; i < tam.rows - 1; i++)
    {
        for (int j = 1; j < tam.cols - 1; j++)
        {
            re.at<uchar>(i, j) = func(take_mat(i, j, tam));
        }
    }
    return re;
}

Mat truncateImage(const Mat &im, std::function<uchar (QVector<uchar>)> func)
{
    Mat re = im.clone();
    Rect myRoi(1, 1, re.cols - 2, re.rows -2);
    for (int i = 1; i < im.rows - 1; i++)
    {
        for (int j = 1; j < im.cols - 1; j++)
        {
            re.at<uchar>(i, j) = func(take_mat(i, j, im));
        }
    }
    re = re(myRoi);
    return re;
}

Mat pixelsWrapAround(const Mat &im, std::function<uchar (QVector<uchar>)> func)
{
    Mat re = im.clone();
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            re.at<uchar>(i, j) = func(take_mat(i, j, im));
        }
    }
    return re;
}

uchar laplacian(QVector<uchar> vt)
{
    int re = 0;

    for (int i = 0; i < vt.size(); i++)
    {
        re += vt[i]*laplacianFilter[i];
    }
    if(re < 0) re = 0;
    if(re > 255) re = 255;
    return (uchar)re;
}

uchar sobel(QVector<uchar> vt)
{
    int hor = 0;
    int ver = 0;
    for (int i = 0; i < vt.size(); i++)
    {
        hor += vt[i]*sobelhorizon[i];
        ver += vt[i]*sobelverical[i];
    }
    double re = sqrt(pow(hor,2)+pow(ver,2));
//    hor += ver;

    return cv::saturate_cast<uchar>(re);
//    return (uchar)sqrt(pow(hor,2) + pow(ver,2));

}
