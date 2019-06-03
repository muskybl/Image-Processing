#include "segment.h"

static const int pointMask[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};

static const int horizontalLineMask[] = {-1, -1, -1, 2, 2, 2, -1, -1, -1};
static const int verticalLineMask[] = {-1, 2, -1, -1, 2, -1, -1, 2, -1};
static const int positive45Mask[] = {-1, -1, 2, -1, 2, -1, 2, -1, -1};
static const int negative45Mask[] = {2, -1, -1, -1, 2, -1, -1, -1, 2};

static const int HorizontalEdgePrewitt[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
static const int VerticalEdgePrewitt[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

static const int robertsMask1[] = {0, 0, 0, 0, -1, 0, 0, 0, 1};
static const int robertsMask2[] = {0, 0, 0, 0, 0, -1, 0, 1, 0};

const double limitT = 0.5;

Mat detectedPoints(const Mat &im)
{
    Mat result = im.clone();
    for (int i = 1; i < im.rows - 1; i++)
    {
        for(int j = 1; j < im.cols - 1; j++)
        {
            result.at<uchar>(i, j) = mulVec(pointMask,take_mat(i, j, im));
        }
    }
    return result;
}

uchar mulVec(const int ar[], const QVector<uchar> &mat)
{
    int re = 0;
    for (int i = 0; i < mat.size(); i++)
    {
        re += ar[i]*mat[i];
    }
    return cv::saturate_cast<uchar>(re);
}

Mat detectedHorizontalLines(const Mat &im)
{
    Mat result = im.clone();
    for (int i = 1; i < im.rows - 1; i++)
    {
        for(int j = 1; j < im.cols - 1; j++)
        {
            result.at<uchar>(i, j) = mulVec(horizontalLineMask,take_mat(i, j, im));
        }
    }
    return result;
}

Mat detectedVerticalLines(const Mat &im)
{
    Mat result = im.clone();
    for (int i = 1; i < im.rows - 1; i++)
    {
        for(int j = 1; j < im.cols - 1; j++)
        {
            result.at<uchar>(i, j) = mulVec(verticalLineMask,take_mat(i, j, im));
        }
    }
    return result;
}

Mat detectedPositive45Lines(const Mat &im)
{
    Mat result = im.clone();
    for (int i = 1; i < im.rows - 1; i++)
    {
        for(int j = 1; j < im.cols - 1; j++)
        {
            result.at<uchar>(i, j) = mulVec(positive45Mask,take_mat(i, j, im));
        }
    }
    return result;
}

Mat detectedNegative45Lines(const Mat &im)
{
    Mat result = im.clone();
    for (int i = 1; i < im.rows - 1; i++)
    {
        for(int j = 1; j < im.cols - 1; j++)
        {
            result.at<uchar>(i, j) = mulVec(negative45Mask,take_mat(i, j, im));
        }
    }
    return result;
}

Mat detectedEdgePrewitt(const Mat &im)
{

    Mat result = im.clone();
    int tam;
    for (int i = 1; i < im.rows - 1; i++)
    {
        for(int j = 1; j < im.cols - 1; j++)
        {
            tam = cv::saturate_cast<int>(sqrt( pow(mulVec(HorizontalEdgePrewitt,take_mat(i, j, im)),2)
                      + pow(mulVec(VerticalEdgePrewitt,take_mat(i, j, im)), 2)));
            result.at<uchar>(i, j) = cv::saturate_cast<uchar>(tam);
        }
    }
    return result;
}

Mat detectedEdgeRoberts(const Mat &im)
{
    Mat result = im.clone();
    int tam;
    for (int i = 1; i < im.rows - 1; i++)
    {
        for(int j = 1; j < im.cols - 1; j++)
        {
            tam = mulVec(robertsMask1,take_mat(i, j, im)) +
                  mulVec(robertsMask2,take_mat(i, j, im));
            result.at<uchar>(i, j) = cv::saturate_cast<uchar>(tam);
        }
    }
    return result;
}

Mat detectedEdgeSobel(const Mat &im)
{
    Mat result = im.clone();

    for (int i = 1; i < im.rows - 1; i++)
    {
        for(int j = 1; j < im.cols - 1; j++)
        {
            result.at<uchar>(i, j) = sobel(take_mat(i, j, im));
        }
    }
    return result;
}

//Mat detectedEdgeLaplacianofGaussian(const Mat &)
//{

//}
QVector<double> findAveragePixels(const QVector<int> &his, int const &t)
{
    QVector<double> re;
    double g1, g2; g1 = g2 = 0;
    int sum1, sum2; sum1 = sum2 = 0;
    int totalFre1, totalFre2; totalFre1 = totalFre2 = 0;
    for (int i = t + 1; i < 256; i++)
    {
        sum1 += his[i]*i;
        totalFre1 += his[i];
    }
    g1 = (sum1*1.0)/totalFre1;
    for (int j = 0; j <= t; j++)
    {
        sum2 += his[j]*j;
        totalFre2 += his[j];
    }
    g2 = (sum2*1.0)/totalFre2;

    re.push_back(g1);
    re.push_back(g2);
    return re;
}

int findGlobalThresholding(const Mat &im)
{

    QVector<int> his = findHistogram(im);
    QVector<double> temp = findAveragePixels(his, 255); // first T
    double T1, T2, distance;
    T1 = temp.last();
    T2 = 0;
    distance = T1;

    while(true)
    {
        temp = findAveragePixels(his, cv::saturate_cast<int>(T1));
        T2 = 0.5*(temp.first() + temp.last());
        distance = abs(T1 - T2);
        if(distance <= limitT) break;
        T1 = T2;
    }
    return cv::saturate_cast<int>(T2);
}


QVector<int> findHistogram(const Mat &im)
{
    QVector<int> re(256,0);
    for(int i = 0; i < im.rows; ++i)
    {
        for (int j = 0; j < im.cols; ++j)
        {
            ++re[im.at<uchar>(i, j)];
        }
    }
    return re;
}
bool isbinaryImage(const Mat &im)
{

    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            if(im.at<uchar>(i, j) < 255 && im.at<uchar>(i, j) > 0) return false;
        }
    }
    return true;
}

Mat globalThresholding(Mat const &im, int &T)
{
    if(isbinaryImage(im) || im.rows == 0 || im.cols == 0) return im;
    Mat result = im.clone();
    T = findGlobalThresholding(im);
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            if(result.at<uchar>(i, j) > T) result.at<uchar>(i, j) = 255;
            else result.at<uchar>(i, j) = 0;
        }
    }
    return result;
}
