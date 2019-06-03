#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "pointprocessing.h"
#include "spatialfiltering.h"
#include "segment.h"
#include "morphology.h"
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
using namespace QtCharts;
namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pointProcessing_clicked();

    void on_spatialFiltering_clicked();

    void on_actionOpen_triggered();

    void on_saveButton_clicked();

    void on_forwardButton_clicked();

    void on_backButton_clicked();

    void on_resetButton_clicked();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_applyButton_clicked();

    void on_thresSlither_valueChanged(int value);

    void on_pointProcessingCombobox_activated(int index);

    void on_logaValueSlither_valueChanged(int value);

    void on_cPLSlider_valueChanged(int value);

    void on_grammaPLSlider_valueChanged(int value);

    void on_segmentation_clicked();

    void on_segmentCombobox_currentIndexChanged(int index);


    void on_lineCombobox_currentIndexChanged(int index);

    void on_edgeCombobox_currentIndexChanged(int index);


    void on_morphology_clicked();

    void on_structEleTable_cellChanged(int row, int column);

    void on_resetDefaultStructuringEle_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat firstImage;
    QVector<cv::Mat> vtImage;
    int currentImage = 0;

    void updateResult(const cv::Mat &);
    void initialElements();
    bool isValidBitPlane();
    QImage convertMat2QImage(const cv::Mat &);

    void saveImage();
    void displayImage(const cv::String &);
    void displayImage(const cv::Mat &);
    void displayHistogram(const cv::Mat &);
    QVector<int> getDataTable();
//    QImage *stackImage = new QImage[20];

};


#endif // MAINWINDOW_H
