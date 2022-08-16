
#include <QtGui>
#include <QApplication>

// 无人机状态
#include "../include/map_widget/state_uav_widget.hpp"
// 离线地图界面 api js
#include "../include/map_widget/bd_map_widget.hpp"
// 无人机 视频监控
#include "../include/map_widget/rtsp_multi_widget.hpp"
// console 程序node控制终端
#include "../include/map_widget/console_widget.hpp"
// mission 任务终端
#include "../include/map_widget/mission_widget.hpp"
// tracker KCF 框选追踪
#include "../include/map_widget/tracker_widget.hpp"

// formation 编队
#include "../include/map_widget/formation_widget.hpp"

// test
// #include "../include/map_widget/myviz.h"

/*****************************************************************************
**                                                                        Main
*****************************************************************************/

void quit_handler(int sig);

QTabWidget * tabWidget_show;
QTabWidget * tabWidget_ctmi;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // 蜂群无人机监视
    tabWidget_show = new QTabWidget();
    tabWidget_show->setWindowTitle("蜂群无人机监视");

    tabWidget_show->setMinimumWidth(930);
    tabWidget_show->setMaximumWidth(930);
    tabWidget_show->setMinimumHeight(530);
    tabWidget_show->setMaximumHeight(530);
    tabWidget_show->resize(930,530);
    //tabWidget_show->setStyleSheet("QTabBar::tab { height: 100px; width: 100px; }");

    // 无人机状态
    map_widget::state_uav_widget qw_state_uav(argc,argv);
    tabWidget_show->addTab(&qw_state_uav, "飞机状态" );
    // 地图界面 GPS轨迹
    map_widget::bd_map_widget qw_bd_map(argc,argv);
    tabWidget_show->addTab(&qw_bd_map, "GPS轨迹" );
    // 编队控制
    map_widget::formation_widget formation_widget(argc,argv);
    tabWidget_show->addTab(&formation_widget, "ENU轨迹_编队阵型设置" );
    // 视频监控
    map_widget::rtsp_multi_widget qw_rtsp_multi(argc,argv);
    tabWidget_show->addTab(&qw_rtsp_multi, "视频监控" );
    // 框选追踪
    kcfTracker_widget kcf_widget1(argc,argv,1);
    tabWidget_show->addTab(&kcf_widget1, "框选追踪" );

    tabWidget_show->show();

    // 蜂群无人机设置
    tabWidget_ctmi = new QTabWidget();
    tabWidget_ctmi->setWindowTitle("蜂群无人机设置");

    tabWidget_ctmi->setMinimumWidth(700);
    tabWidget_ctmi->setMaximumWidth(700);
    tabWidget_ctmi->setMinimumHeight(650);
    // tabWidget_ctmi->setMaximumHeight(480);
    tabWidget_ctmi->resize(700,650);

    map_widget::console_widget qw_console(argc,argv);
    tabWidget_ctmi->addTab(&qw_console, "飞机ROS Node程序管理" );

    map_widget::mission_widget qw_mission(argc,argv);
    tabWidget_ctmi->addTab(&qw_mission, "任务流程设置" );

    tabWidget_ctmi->show();


    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    int result = app.exec();
    return result;

    /* // Get IP address
    QString m_qRosIp, m_qMasterIp;
    foreach (QHostAddress address, QNetworkInterface::allAddresses())
    {
      if (address.protocol() == QAbstractSocket::IPv4Protocol)
      {
        QString addre = address.toString();
        if (addre.split(".")[0] == "192")
        {
          m_qRosIp = addre;
          m_qMasterIp = "http://" + addre + ":11311";
        } else if (addre.split(".")[0] == "10")
        {
          m_qRosIp = addre;
          m_qMasterIp = "http://" + addre + ":11311";
        } else if (addre.split(".")[0] == "172")
        {
          m_qRosIp = addre;
          m_qMasterIp = "http://" + addre + ":11311";
        }
      }
    }
    */
}
