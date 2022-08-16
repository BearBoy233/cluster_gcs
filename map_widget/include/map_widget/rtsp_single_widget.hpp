
#ifndef rtsp_single_widget_H
#define rtsp_single_widget_H

#include <QWidget>
#include <QDebug>

#include <QtGui>
#include <QMessageBox>
#include <iostream>

#include <QLineEdit>
#include <QDateTime>
#include <QUrl>

#include <QtWidgets/QWidget>
#include <QtWidgets/QtWidgets>

#include <QImage>
#include <QMutex>
#include <iostream>

#include "ui_rtsp_single_widget.h"
#include "rtsp_single_qnode.hpp"

namespace map_widget {

class rtsp_single_widget : public QWidget
{
    Q_OBJECT

public:
    explicit rtsp_single_widget(int argc, char** argv, QWidget *parent = nullptr);
    ~rtsp_single_widget();

    Ui::rtsp_single_widget *ui;

private:
    /*用来保护一个对象、数据结构、代码段、使得它们在同一一时刻，只有一个线程有访问权限*/
    mutable QMutex showImage_mutex_;
    //cv::Mat receiveImage;//保存接受到的图像
    //QImage ImageToQImage;//转换为QImage
    bool receiveImageFlag;//接受到图像为true 处理完设置为false

public Q_SLOTS:

    void deal_receiveImageSignal();

private:

    QString urls_rtsp;
    QStringList urls_ip;
    QStringList urls_topic;
    QStringList urls_rostopic;

    QString rtsp_Temp1 = QString("http://");
    QString rtsp_Temp2 = QString("/stream?topic=");

    rtsp_single_QNode rtsp_single_qnode;

    // init func
    void initConncetion();  // 初始化槽函数

    int state_1rtsp_0rostopic = 1;

private slots:

    void on_pushButton_start_clicked();
    void on_pushButton_pause_clicked();

    // 开关选择
    void on_radioButton_rtsp_clicked();
    void on_radioButton_rostopic_clicked();

    void on_comboBox_topic_currentIndexChanged(const QString &arg);
    void on_comboBox_ip_currentIndexChanged(const QString &arg);

};

}

#endif // rtsp_single_widget_H
