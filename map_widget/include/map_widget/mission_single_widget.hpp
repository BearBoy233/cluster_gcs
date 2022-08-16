
#ifndef mission_single_widget_H
#define mission_single_widget_H

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
#include <string>

#include "ui_mission_single_widget.h"
#include <mavcomm_msgs/Console.h>

namespace map_widget {

class mission_single_widget : public QWidget
{
    Q_OBJECT

public:
    explicit mission_single_widget(QWidget *parent = nullptr);
    ~mission_single_widget();

// private:
    Ui::mission_single_widget *ui;
    bool flag_button_clicked;

private slots:
    // void on_pushButton_clicked();

public:
    mavcomm_msgs::Console rosmsg;
    int  led_state;
};

}

#endif // mission_single_widget_H
