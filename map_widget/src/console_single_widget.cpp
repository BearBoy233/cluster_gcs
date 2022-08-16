
#include "../include/map_widget/console_single_widget.hpp"

namespace map_widget {

console_single_widget::console_single_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::console_single_widget)
{
    ui->setupUi(this);

    // 设置
    flag_button_clicked = false;
}

console_single_widget::~console_single_widget()
{
    delete ui;
}

}
