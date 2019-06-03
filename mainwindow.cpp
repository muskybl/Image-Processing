#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->hide();
    Mat im = imread("C:\\Users\\royal\\Pictures\\engine.JPG",IMREAD_GRAYSCALE);
    vtImage.push_back(im);
    firstImage = im;
    displayImage(im);
    displayHistogram(im);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pointProcessing_clicked()
{
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentIndex(0);
    ui->subStackedWidget1->hide();
}

void MainWindow::on_spatialFiltering_clicked()
{
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionOpen_triggered()
{

    QString imagePath = QFileDialog::getOpenFileName(this,
                                                     tr("Open File"),
                                                     "",
                                                tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
    if(imagePath.isNull())
    {
        ui->notificateInfo->setText("Open Failed");
        return;
    }

    displayImage(imagePath.toStdString());

    vtImage.clear();
    updateResult(imread(imagePath.toStdString(),IMREAD_GRAYSCALE));
    currentImage = 0;
    firstImage = vtImage.last();

}


void MainWindow::on_saveButton_clicked()
{
    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save File"),
                    "",
                    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                    );

    if(imagePath.isNull())
    {
        ui->notificateInfo->setText("Save Failed");
        return;
    }
    cv::Mat imageObject = vtImage.last();
    QImage im = convertMat2QImage(imageObject);
    im.save(imagePath);
    ui->notificateInfo->setText("Save Done !");
}

void MainWindow::on_forwardButton_clicked()
{

    if(currentImage != vtImage.size() - 1)
    {
        ++currentImage;
        displayImage(vtImage.at(currentImage));
        displayHistogram(vtImage.at(currentImage));
    }
}

void MainWindow::on_backButton_clicked()
{

    if(currentImage != 0)
    {
        --currentImage;
        displayImage(vtImage.at(currentImage));
        displayHistogram(vtImage.at(currentImage));

    }
}

void MainWindow::on_resetButton_clicked()
{
    vtImage.clear();
    currentImage = -1;
    updateResult(firstImage);
}

void MainWindow::on_actionSave_triggered()
{
    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save File"),
                    "",
                    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                    );

    if(imagePath.isNull())
    {
        ui->notificateInfo->setText("Save Failed");
        return;
    }
    cv::Mat imageObject = vtImage.last();
    QImage im = convertMat2QImage(imageObject);
    im.save(imagePath);
    ui->notificateInfo->setText("Save Done !");
}

void MainWindow::on_actionExit_triggered()
{

}
void MainWindow::on_applyButton_clicked()
{
    Mat im;
    if(ui->pointProcessing->isChecked())
    {
        int currentIndex = ui->pointProcessingCombobox->currentIndex();
        switch (currentIndex)
        {
            case 0:
                im = transformToNegativeImage(vtImage.last());
                break;

            case 1:
                im = thresHolding(vtImage.last(),ui->thresSlither->value());
                break;
            case 2:
                im = logarithmic(vtImage.last(), ui->logaValueSlither->value());
                break;
            case 3:

                im = powerlaw(vtImage.last(), ui->cPLSlider->value(),ui->grammaPLSlider->value()/10.0);
                break;
            case 4:
                if(!isValidBitPlane()) return;
                im = bitplane(vtImage.last(),ui->frombitInput->text().toInt(),
                                  ui->tobitInput->text().toInt());
                break;
            case 5:
                im = histogramEqualization(vtImage.last());
            break;
        }
    }
    if (ui->spatialFiltering->isChecked())
    {
        int al = ui->spatialAl->currentIndex();
        int edge = ui->missingEdge->currentIndex();
        std::function<uchar(QVector<uchar>)> li[] = {minNeigh,
                                                     maxNeigh,
                                                     medianNeigh,
                                                     averageNeigh,
                                                     weightedAverageNeigh,
                                                     laplacian,
                                                     sobel
                                                     };
//        std::function<Mat(Mat const &, std::function<uchar(QVector<uchar>)>)> sp[] = {};
        switch (edge)
        {
            case 0:
                im = omitMissingPixels(vtImage.last(),li[al]);
            break;
            case 1:
                im = padImage(vtImage.last(),li[al]);
            break;
            case 2:
                im = replicateBorderPixels(vtImage.last(),li[al]);
            break;
            case 3:
                im = truncateImage(vtImage.last(),li[al]);
            break;
            case 4:
                im = pixelsWrapAround(vtImage.last(),li[al]);
            break;
        }


    }
    if(ui->segmentation->isChecked())
    {
        int segAl = ui->segmentCombobox->currentIndex();
        switch (segAl)
        {
            case 0: im = detectedPoints(vtImage.last()); break;

            case 1:
            {
                int funLine = ui->lineCombobox->currentIndex();

                im = detectedLines[funLine](vtImage.last());
                break;
            }
            case 2:
            {
                int funEdge = ui->edgeCombobox->currentIndex();
                im = detectedEdges[funEdge](vtImage.last());
                break;
            }
            case 3:
            {
                int T = 0;
                im = globalThresholding(vtImage.last(), T);
                ui->thresholdValue->setText(QString::number(T));
                break;
            }


        }

    }
    if(ui->morphology->isChecked())
    {
        int t = 0;
        Mat temp;
        temp = globalThresholding(vtImage.last(), t);

        im = morphology[ui->morphologyCombobox->currentIndex()](temp,getDataTable());
    }

    updateResult(im);
}


void MainWindow::updateResult(const Mat &im)
{
    vtImage.push_back(im);
    ++currentImage;
    displayImage(im);
    displayHistogram(im);
}

bool MainWindow::isValidBitPlane()
{
    int f,t;
    f = ui->frombitInput->text().toInt();
    t = ui->tobitInput->text().toInt();
    if(f > t)
    {
        ui->bitplaneNoti->setText("From bit cannot greater than to bit !");
        return false;
    }
    if(t < 1 || t >8 || f < 1 || t > 8)
    {
        ui->bitplaneNoti->setText("from bit and to bit must be in range [1,8] ");
        return false;
    }
    return true;
}

QImage MainWindow::convertMat2QImage(const cv::Mat &mat)
{
    return QImage(mat.data, mat.size().width, mat.size().height, mat.step1(), QImage::Format::Format_Grayscale8);
}

void MainWindow::displayImage(const String &imagePath)
{
    cv::Mat mat = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    displayImage(mat);
}

void MainWindow::displayImage(const Mat &mat)
{
    QLabel *lb = ui->label;
    QImage image = convertMat2QImage(mat);

    lb->setPixmap(QPixmap::fromImage(image));
    lb->setScaledContents(true);
    lb->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    ui->widthInfoLabel->setText(QString::number(image.width()));
    ui->heightInfoLabel->setText(QString::number(image.height()));
}

void MainWindow::displayHistogram(const Mat &im)
{
    QBarSet *bSet = new QBarSet("");
    int maxFr = 0;
    QVector<int> qVt = calculateHistogram(im);
    for(int FrValue : qVt)
    {
        maxFr = std::max(maxFr, FrValue);
        bSet->append(FrValue);
    }

    QBarSeries * series = new QBarSeries();

    series->append(bSet);
    QChart *chart = new QChart();
    chart->addSeries(series);

    chart->setTitle("Histogram");
    chart->setAnimationOptions(QChart::AnimationOption::NoAnimation);
    chart->setTheme(QChart::ChartTheme::ChartThemeDark);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 255);
    axisX->setLabelFormat(tr("%d"));
    axisX->setMinorTickCount(2);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxFr);
    axisY->setLabelFormat(tr("%d"));
    axisY->setMinorTickCount(2);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->hide();


    QGraphicsScene *scene = new QGraphicsScene();

    chart->setMinimumSize(QSize(560,460));
    scene->addItem(chart);
    ui->graphicsView->setScene(scene);
}


QVector<int> MainWindow::getDataTable()
{
    QVector<int> structEle;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            structEle.push_back(ui->structEleTable->item(i, j)->text().toInt());
        }
    }
    return structEle;
}


void MainWindow::on_pointProcessingCombobox_activated(int index)
{
    ui->subStackedWidget1->show();
    if(index == 0 || index == 5) ui->subStackedWidget1->hide();
    else ui->subStackedWidget1->setCurrentIndex(index - 1);

}
void MainWindow::on_thresSlither_valueChanged(int value)
{

    ui->thresValueLabel->setText(QString::number(value));
}
void MainWindow::on_logaValueSlither_valueChanged(int value)
{
    ui->logaritCValueLabel->setText(QString::number(value));
}

void MainWindow::on_cPLSlider_valueChanged(int value)
{
    ui->cPLLabel->setText(QString::number(value));
}

void MainWindow::on_grammaPLSlider_valueChanged(int value)
{
    ui->grammaPLLabel->setText(QString::number(value/10.0));
}

void MainWindow::on_segmentation_clicked()
{
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentIndex(2);
    ui->subSegmentStacked->setCurrentIndex(ui->segmentCombobox->currentIndex());
    ui->pointPlantext->setReadOnly(true);
    ui->linePlantext->setReadOnly(true);
    ui->edgePlantext->setReadOnly(true);

}

void MainWindow::on_segmentCombobox_currentIndexChanged(int index)
{
    ui->subSegmentStacked->setCurrentIndex(index);
    ui->linePlantext->setPlainText(lineExplain[ui->lineCombobox->currentIndex()]);
    ui->edgePlantext->setPlainText(edgeExplain[ui->edgeCombobox->currentIndex()]);
}



void MainWindow::on_lineCombobox_currentIndexChanged(int index)
{
    ui->linePlantext->setPlainText(lineExplain[index]);
}

void MainWindow::on_edgeCombobox_currentIndexChanged(int index)
{
    ui->edgePlantext->setPlainText(edgeExplain[index]);
}


void MainWindow::on_morphology_clicked()
{
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentIndex(3);
    ui->notificateInfo->setText("Object, background is black and white");

}



void MainWindow::on_structEleTable_cellChanged(int row, int column)
{
    bool ok;
    QString t = ui->structEleTable->item(row,column)->text();
    int value = t.toInt(&ok,10);
    if((value == 1 || value == 0 )&& ok) ui->structuringLabel->setText("");
    else
    {
        ui->structEleTable->setItem(row, column,new QTableWidgetItem(
                                        QString::number(structuringElement[row*3 + column])));

        ui->structuringLabel->setText("Value must be 1 or 0 !");
    }
}

void MainWindow::on_resetDefaultStructuringEle_clicked()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            ui->structEleTable->setItem(i, j, new QTableWidgetItem(
                                            QString::number(structuringElement[i*3 + j])));
        }
    }
}

