
#include "../include/map_widget/mission_single_widget.hpp"

namespace map_widget {

mission_single_widget::mission_single_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mission_single_widget)
{
    ui->setupUi(this);

    // 设置
    flag_button_clicked = false;
}

mission_single_widget::~mission_single_widget()
{
    delete ui;
}

}
