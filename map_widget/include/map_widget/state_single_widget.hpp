
#ifndef state_single_widget_H
#define state_single_widget_H

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

#include "ui_state_single_widget.h"

namespace map_widget {

class state_single_widget : public QWidget
{
    Q_OBJECT

public:
    explicit state_single_widget(QWidget *parent = nullptr);
    ~state_single_widget();

// private:
    Ui::state_single_widget *ui;

    void setLED(QLabel* label, int color, int size);

    std::string px4_mode_str[15] = {"NAN", "MANUAL", "ACRO", "ALTCTL", "POSCTL",
        "OFFBOARD", "STABILIZED", "RATTITUDE", "AUTO.MISSION", "AUTO.LOITER",
        "AUTO.RTL", "AUTO.LAND", "AUTO.RTGS", "AUTO.READY", "AUTO.TAKEOFF"};
    /*
        string MANUAL=MANUAL
        string ACRO=ACRO
        string ALTITUDE=ALTCTL
        string POSITION=POSCTL
        string OFFBOARD=OFFBOARD
        string STABILIZED=STABILIZED
        string RATTITUDE=RATTITUDE
        string MISSION=AUTO.MISSION
        string LOITER=AUTO.LOITER
        string RTL=AUTO.RTL
        string LAND=AUTO.LAND
        string RTGS=AUTO.RTGS
        string READY=AUTO.READY
        string TAKEOFF=AUTO.TAKEOFF
     */

    std::string px4_sys_state_str[9] = {"UNINIT", "BOOT", "CALIBRATING", "STANDBY", "ACTIVE",
        "CRITICAL", "EMERGENCY", "POWEROFF", "FLIGHT_TERMINATION"};
    /*
        UNINIT=0, // Uninitialized system, state is unknown.
        BOOT=1, // System is booting up.
        CALIBRATING=2, // System is calibrating and not flight-ready.
        STANDBY=3, // System is grounded and on standby. It can be launched any time.
        ACTIVE=4, // System is active and might be already airborne. Motors are engaged.
        CRITICAL=5, // System is in a non-normal flight mode. It can however still navigate.
        EMERGENCY=6, // System is in a non-normal flight mode. It lost control over parts or over the whole airframe. It is in mayday and going down.
        POWEROFF=7, // System just initialized its power-down sequence, will shut down now.
        FLIGHT_TERMINATION=8, // System is terminating itself.
      */

    std::string gps_fix_type_str[9] = {"NO_GPS", "NO_FIX", "2D_FIX", "3D_FIX", "DGPS",
        "RTK_FLOATR", "RTK_FIXEDR", "STATIC", "PPP"};
    /*
      uint8 NO_GPS=0
      uint8 NO_FIX=1
      uint8 2D_FIX=2
      uint8 3D_FIX=3
      uint8 DGPS=4
      uint8 RTK_FLOATR=5
      uint8 RTK_FIXEDR=6
      uint8 STATIC=7
      uint8 PPP=8
     * */

    // 无人机状态 changestate /uav_ctrl.cpp#enum FlightState
    // 需要同步修改 px4_ctrl.cpp & console_widget.hpp/cpp & state_single_widget.hpp
    std::string ctrl_state_type_str[9] = {"UNINIT",
                                          "IDLE",
                                          "TAKING_OFF",
                                          "LANDING",
                                          "POS_EXECUTION",
                                          "FORMATION_FLY",
                                          "WAITING_FOR_HOME_POSE",
                                          "TAKEOFF_FAIL",
                                          "LANDED"};
    // uav_ctrl.cpp#enum FlightState
    /*
  enum FlightState {
    UNINIT,                    // 未知状态
    IDLE,                     // 什么都不干 地面待机状态
    TAKING_OFF,               // 当前 loc 点起飞
    LANDING,                  // 当前 loc 点降落
    POS_EXECUTION,            // 位置控制
    FORMATION_FLY,            // 编队飞行
    WAITING_FOR_HOME_POSE,    // 此和下面的状态 无法通过地面站切换到
    TAKEOFF_FAIL,
    LANDED,
  }
     */

};

}

#endif // state_single_widget_H
