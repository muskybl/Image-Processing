#include "pointprocessing.h"

Mat transformToNegativeImage(cv::Mat const &im)
{
    cv::Mat result = im.clone();
    for(int i = 0; i < im.rows; i++)
    {
        for(int j = 0; j < im.cols; j++)
        {
            result.at<uchar>(i, j) = 255 - result.at<uchar>(i, j);
        }
    }
    return result;
}
Mat thresHolding(cv::Mat const &im, int const &thres)
{
    cv::Mat result = im.clone();

    for(int i = 0; i < im.rows; i++)
    {
        for(int j = 0; j < im.cols; j++)
        {
            result.at<uchar>(i, j) =  result.at<uchar>(i, j) < thres ? 0 : 255;
        }
    }
    return result;
}
Mat logarithmic(Mat const &im, int c)
{
    Mat result = im.clone();
    c *= 255;
    for(int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            result.at<uchar>(i, j) = cv::saturate_cast<uchar>(c*log(1 + im.at<uchar>(i,j)/255.0));
        }
    }

    return result;
}
Mat powerlaw(Mat const &im, int c, float const &y)
{
    Mat result = im.clone();
    c *= 255;
    for(int i = 0; i < im.rows; i++)
    {
        for(int j = 0; j < im.cols; j++)
        {
            result.at<uchar>(i, j) = cv::saturate_cast<uchar>(c*pow(im.at<uchar>(i, j)/255.0,y));
        }
    }
    return result;
}

Mat bitplane(Mat const &im, int const &f, int const &t)
{
    Mat result = im.clone();
    int r,getbit, conlai;
    r = f - 1;
    conlai = 8 - (t - f + 1);
    getbit = 0xff >> conlai;
    for (int i = 0; i < im.rows; i++)
    {
        for(int j = 0; j < im.cols; j++)
        {
            result.at<uchar>(i, j) = cv::saturate_cast<uchar>(((im.at<uchar>(i,j) >> r) & getbit) << conlai);
//            qDebug() << result.at<uchar>(i, j);
        }
    }
    return  result;
}
QVector<int> calculateHistogram(const Mat &im)
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
Mat histogramEqualization(Mat const &im)
{
    Mat re = im.clone();
    QVector<int> his = calculateHistogram(im);
    QVector<int> sumofHis(256, 0);
    int sum = 0;
    for(int i = 0; i < 256; i++)
    {
        sum += his[i];
        sumofHis[i] = sum;
    }
    int dm = 256.0;
    int area = im.rows*im.cols;
    for(int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
             re.at<uchar>(i, j) = cv::saturate_cast<uchar>((dm*sumofHis[im.at<uchar>(i,j)])/area);
        }
    }
    return re;
}
