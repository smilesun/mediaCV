#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cimgproc.h"
#include "cepsilonfilter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    ///*******user defined
    CImgProc * pstrategy;
    cv::Mat image;
    void salt(cv::Mat &,int);
    void convert_video(char*,char* );
    void sharpen(const cv::Mat &image, cv::Mat &result);//laplasian


};

#endif // MAINWINDOW_H
