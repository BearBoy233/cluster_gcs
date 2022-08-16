
#include "../include/map_widget/state_single_widget.hpp"

namespace map_widget {

state_single_widget::state_single_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::state_single_widget)
{
    ui->setupUi(this);

    // 设置 LED 状态灯
    setLED(ui->label_connect, 1, 11);         // 1红未连接 2绿已连接 color 0:grey 1:red 2:green 3:yellow
    setLED(ui->label_armed, 0, 11);           // 3黄未解锁 2绿已解锁 color 0:grey 1:red 2:green 3:yellow
    setLED(ui->label_manual_input, 0, 11);    // 0灰无遥控 2绿有遥控 color 0:grey 1:red 2:green 3:yellow

    // 设置
    ui->label_px4_mode->setText( QString::fromStdString(px4_mode_str[0]) );
    ui->label_px4_sys_state->setText( QString::fromStdString(px4_sys_state_str[0]) );
    ui->label_gps_fix_type->setText( QString::fromStdString(gps_fix_type_str[0]) );
    ui->label_gps_satellites_visible->setText("0");
    ui->label_machine_state->setText( QString::fromStdString(ctrl_state_type_str[0]) );
    ui->lineEdit_battery->setReadOnly(1);
}

state_single_widget::~state_single_widget()
{
    delete ui;
}

void state_single_widget::setLED(QLabel* label, int color, int size)
{
    // 将label中的文字清空
    label->setText("");
    // 先设置矩形大小
    // 如果ui界面设置的label大小比最小宽度和高度小，矩形将被设置为最小宽度和最小高度；
    // 如果ui界面设置的label大小比最小宽度和高度大，矩形将被设置为最大宽度和最大高度；
    QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
    QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
    QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
    QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
    // 再设置边界形状及边框
    QString border_radius = QString("border-radius: %1px;").arg(size/2);    // 边框是圆角，半径为size/2
    QString border = QString("border:1px solid black;");                    // 边框为1px黑色
    // 最后设置背景颜色
    QString background = "background-color:";
    switch (color) {
    case 0:   // 灰色
        background += "rgb(190,190,190)";
        break;
    case 1:   // 红色
        background += "rgb(200,0,0)";
        break;
    case 2:   // 绿色
        background += "rgb(0,200,0)";
        break;
    case 3:   // 黄色
        background += "rgb(200,200,0)";
        break;
    default:
        break;
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    label->setStyleSheet(SheetStyle);
}

}
