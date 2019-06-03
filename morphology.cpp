#include "morphology.h"



QVector<int> get3x3Mat(const Mat &im, const int &i, const int &j)
{
    int r,c; r = im.rows; c = im.cols;
    QVector<int> re;
    for (int dir = 0; dir < 9; dir++)
    {
       int new_row = i + dr[dir];
       int new_col = j + dc[dir];
       if(isInside(new_row, new_col, r, c)) re.push_back(im.at<uchar>(new_row, new_col));
       else re.push_back(-1);

    }
    return  re;
}
bool isFit(const QVector<int> &structEle, const QVector<int> &src)
{
    for (int i = 0; i < 9; i++)
    {
        // if la Oj ma anh khong phai la white(white la Obj == 1)
        if((structEle[i] == 1) && (src[i] != object)) return false;
    }
    return true;
}

bool isHit(const QVector<int> &structElement, const QVector<int> &src)
{
    for (int i = 0; i < src.size(); i++)
    {
        if(structElement[i] == 1 && src[i] == object) return true;
    }
    return false;
}

Mat erosion(const Mat &im, const QVector<int> &structElement)
{

    Mat re = im.clone();
    for (int i = 0; i < re.rows; i++)
    {
        for (int j = 0; j < re.cols; j++)
        {
            if(isFit(structElement, get3x3Mat(im, i, j))) re.at<uchar>(i, j) = object;
            else re.at<uchar>(i, j) = background;
        }
    }
    return re;
}


Mat dilation(const Mat &im, const QVector<int> &structElement)
{
    Mat re = im.clone();
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            if(isHit(structElement, get3x3Mat(im, i, j))) re.at<uchar>(i, j) = object;
            else re.at<uchar>(i, j) = background;
        }
    }
    return re;
}

Mat openingAl(const Mat &im, const QVector<int> &structElement)
{
    Mat opening = erosion(im, structElement);
    opening = dilation(opening, structElement);
    return opening;
}

Mat closingAl(const Mat &im, const QVector<int> &structElement)
{
    Mat closing = dilation(im, structElement);
    closing = erosion(closing, structElement);
    return closing;
}

Mat boundaryExtraction(const Mat &im, const QVector<int> &structElement)
{
    QVector<int> boundar({1, 1, 1, 1, 1, 1, 1, 1, 1});
    Mat ero = erosion(im, boundar).clone();
    // because I already set Object is black so i have to change the theor;
    return ero - im;
}

