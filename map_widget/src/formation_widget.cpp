
#include "../include/map_widget/formation_widget.hpp"

#define NNN 11

// 编队控制 高度z 不在xy是否相同考虑中

namespace map_widget {

using namespace Qt;
// , formation_qnode(argc,argv)

// -------------------------------------------------------------------------
//                                                formation_widget 初始化声明
// -------------------------------------------------------------------------
formation_widget::formation_widget(int argc, char** argv, QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::formation_widget)
  , formation_qnode(argc,argv)
{
  ui->setupUi(this);

  // Init 初始化
  metaEnum = QMetaEnum::fromType<Cenum::Priority>();  // 用于 枚举<==>QString 转换

  ui->tableWidget->setRowCount(NNN-1);

  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    flag_show_track_first_time[temp_i] = true;

    ot_offset_uav_state[temp_i] = Cenum::unset;
    ot_offset_leader[temp_i] = false;
    ot_offset_xy[temp_i][0] = 0.0;
    ot_offset_xy[temp_i][1] = 0.0;
    ot_offset_xy[temp_i][2] = 0.0;
    ot_offset_xy[temp_i][3] = 0.0;

    for (int i=0; i<7; i++)
    { // 注意 ui->tableWidget->setRowCount(NNN-1);
      ui->tableWidget->setItem(temp_i-1, i, new QTableWidgetItem( ) );
      // 设置为不可编辑
      ui->tableWidget->item(temp_i-1, i)->setFlags(Qt::ItemIsEnabled); //
    }
  }

  // 航迹清除 all
  ui->showtrack_pushButton_all_clean->setEnabled(0);
  // 航迹清除 单架
  ui->showtrack_pushButton_select_clean->setEnabled(0);
  // 队形设置
  ui->formation_pushButton_check_data->setEnabled(0);
  ui->formation_pushButton_set_single->setEnabled(0);
  ui->formation_pushButton_set_all->setEnabled(0);

  //--------------------------------------------------------------------------
  initConncetion(); //初始化槽函数

  formation_qnode.init();
}

formation_widget::~formation_widget()
{
  delete ui;
}

// #########################################################################
//                                                                  Qt slots
// #########################################################################

// 初始化信号槽函数
void formation_widget::initConncetion()
{
   // print
   connect(this, SIGNAL( print_flag() ), this, SLOT( update() ) );
   // formation_QNode
   // Unused
   connect( &formation_qnode, SIGNAL( power( ) ), this, SLOT( slot_power( ) ) );
   // Qt UI UAV ID label update
   connect( &formation_qnode, SIGNAL( power_pose( ) ), this, SLOT( slot_power_pose( ) ) );
   // Qt UI UAV ID label update
   connect( &formation_qnode, SIGNAL( power_set_data( ) ), this, SLOT( slot_power_set_data( ) ) );
}

// -------------------------------------------------------------------------
//                                                   ROS solt SIGNAL connect
// -------------------------------------------------------------------------
// QT GUI
void formation_widget::slot_power( )
{

}

// QT GUI
void formation_widget::slot_power_set_data( )
{
  // 无人机的反馈 与本地 对比
  for (int i=1; i<NNN; i++)
  {
    if ( formation_qnode.flag_data_set_local_pos_enu[i] == true )
    {
      if ( formation_qnode.data_set_local_pos_enu[i].x == ot_offset_xy[i][0] &&
           formation_qnode.data_set_local_pos_enu[i].y == ot_offset_xy[i][1] &&
           formation_qnode.data_set_local_pos_enu[i].z == ot_offset_xy[i][2]) // yaw 先不管了
      {  ot_offset_uav_state[i] = Cenum::Set;
         formation_qnode.flag_save_path_data[i] = 2; // 对应无人机轨迹 清零
      }
      formation_qnode.flag_data_set_local_pos_enu[i] = false;
    }
  }

  // tabwidget 状态部分 更新
  int temp_k = 0; // ui tableWidget count
  for (int temp_j = 1; temp_j < NNN; temp_j++)
  {
    if ( flag_show_track_first_time[temp_j] == false )
    {
      ui->tableWidget->item(temp_k, 1)->setText( metaEnum.valueToKey( ot_offset_uav_state[temp_j] ) );
      temp_k = temp_k + 1;
    }
  }

  // 统计 flag
  int temp_num = 0;
  for (int i=1; i<NNN; i++)
  {
    if ( flag_show_track_first_time[i]==false && ot_offset_uav_state[i] == Cenum::Set)
    {
      temp_num++;
    }
  }

  if ( temp_num == curent_uav_num)
  {
    flag_set_all_formation_state = true;
    ui->lineEdit_set_formation_state->setText( "已设置全部无人机" );
  }
  else
  {
    flag_set_all_formation_state = false;
    ui->lineEdit_set_formation_state->setText( "部分无人机 未设置 !" );
  }

}

// Qt UI UAV ID label update
void formation_widget::slot_power_pose( )
{
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    if (formation_qnode.DataS_drone_enu_update_flag[temp_i])
    { // 有飞机位置信息更新
      // 首次有位置 -> ui 界面开启按钮
      if ( flag_show_track_first_time[temp_i])
      {
        flag_show_track_first_time[temp_i] = false;   // 置 0

        // Qt Ui pushButton Enable
        // rviz track
        ui->showtrack_pushButton_all_clean->setEnabled(1);
        ui->showtrack_pushButton_select_clean->setEnabled(1);
        // formation
        ui->formation_pushButton_check_data->setEnabled(1);
        // ui->formation_pushButton_set_single->setEnabled(1);
        // ui->formation_pushButton_set_all->setEnabled(1);

        // Qt Ui comboBox adding UAV No
        int temp_k = 0; // ui tableWidget count
        ui->comboBox_select_clean_no->clear();
        ui->comboBox_select_formation_single->clear();
        for (int temp_j = 1; temp_j < NNN; temp_j++)
        {
          if ( flag_show_track_first_time[temp_j] == false )
          {
            ui->comboBox_select_clean_no->addItem( QString("%1").arg(temp_j) );
            ui->comboBox_select_formation_single->addItem( QString("%1").arg(temp_j) );

            ui->tableWidget->item(temp_k, 0)->setText( QString::number(temp_j) );
            ui->tableWidget->item(temp_k, 1)->setText( metaEnum.valueToKey( ot_offset_uav_state[temp_j] ) );
            ui->tableWidget->item(temp_k, 2)->setText( QString::number( ot_offset_leader[temp_j] ) );
            ui->tableWidget->item(temp_k, 3)->setText( QString::number( ot_offset_xy[temp_j][0] ) );
            ui->tableWidget->item(temp_k, 4)->setText( QString::number( ot_offset_xy[temp_j][1] ) );
            ui->tableWidget->item(temp_k, 5)->setText( QString::number( ot_offset_xy[temp_j][2] ) );
            ui->tableWidget->item(temp_k, 6)->setText( QString::number( ot_offset_xy[temp_j][3] ) );

            for (int i=2; i<7; i++)
            { // 设置为可编辑
              ui->tableWidget->item(temp_k, i)->setFlags( (Qt::ItemFlag)63 );
            }

            temp_k = temp_k + 1;
          }
        }
        curent_uav_num = temp_k; // 当前存储了多少UAV no
      } // end if ( flag_show_track_first_time[temp_i])
    }
  }
}

// #########################################################################
//                                                       Qt on_XXX_clicked()
// #########################################################################
void formation_widget::on_pushButton_rviz_clicked()
{
  std::string temp_string;
  temp_string = "gnome-terminal --tab -e 'bash -c \"";
  temp_string = temp_string + "roslaunch map_widget rviz.launch";
  temp_string = temp_string + "; exec bash\"'";
  std::cout << "exec = " << temp_string << std::endl;
  system(temp_string.c_str());
}

// -------------------------------------------------------------------------
//                                                       多/单架无人机 轨迹清除
// -------------------------------------------------------------------------
void formation_widget::on_showtrack_pushButton_all_clean_clicked()
{
  for (int i=1; i<NNN; i++)
  {
    formation_qnode.flag_save_path_data[i] = 2;
  }
}

void formation_widget::on_showtrack_pushButton_select_clean_clicked()
{
  int i;
  i = ui->comboBox_select_clean_no->currentText().toInt();
  formation_qnode.flag_save_path_data[i] = 2;
}

// -------------------------------------------------------------------------
//                                                 多/单架无人机 编队阵型偏差设置
// -------------------------------------------------------------------------
void formation_widget::on_formation_pushButton_check_data_clicked()
{
  // 数据校验
  int currentColumn, no;
  QString current_Text;
  bool flag_test_print = 0; // 测试用 输出flag

  // 先判断文档的格式
  // Leader flag
  currentColumn = 2;
  if (flag_test_print)  // widget cout
  { qDebug() << "check flag ";
  }

  for (int i=0; i<curent_uav_num; i++)
  {    
    current_Text = ui->tableWidget->item(i, currentColumn)->text();
    if( current_Text == QString("0") || current_Text == QString("1") )  // 只能是 1/0
    { // 赋值
      no = ui->tableWidget->item(i, 0)->text().toInt(); //获取当前单元格的 No
      ot_offset_leader[no] = current_Text.toInt();
      ot_offset_uav_state[no] = Cenum::unset; //状态置于未设置
      ui->tableWidget->item(i, 1)->setText( metaEnum.valueToKey( ot_offset_uav_state[no] ) );

      if (flag_test_print)   // widget cout
      { qDebug() << "Set No=" << no << ", ot_offset_leader[no] " << ot_offset_leader[no];
      }
    }
    else
    {
      QMessageBox::information(this, "Check Data !", ( QString("检查第%1行，第%2列数据.\n 1设置为领机，有且仅能有1架").arg(i+1).arg(currentColumn+1) ) );
      return;
    }
  }

  // x 偏差
  currentColumn = 3;
  if (flag_test_print)  // widget cout
  { qDebug() << "check x ";
  }

  for (int i=0; i<curent_uav_num; i++)
  { // 判断 x/y 是否是小数
    if ( !ui->tableWidget->item(i, currentColumn)->text().contains(QRegExp("^(\\-|\\+)?\\d+(\\.\\d+)?$")) )
    {
      QMessageBox::information(this, "Check Data !", QString("检查第%1行，第%2列数据.\n 请确保输入正确的数字/小数.").arg(i+1).arg(currentColumn+1) );
      return;
    }
    else
    { // 赋值
      no = ui->tableWidget->item(i, 0)->text().toInt(); //获取当前单元格的 No
      ot_offset_xy[no][0] = ui->tableWidget->item(i, currentColumn)->text().toFloat();
      ot_offset_uav_state[no] = Cenum::unset; //状态置于未设置
      ui->tableWidget->item(i, 1)->setText( metaEnum.valueToKey( ot_offset_uav_state[no] ) );

      if (flag_test_print)   // widget cout
      { qDebug() << "Set No=" << no << ", ot_offset_xy[no][0] = " << ot_offset_xy[no][0];
      }
    }
  }

  // y 偏差
  currentColumn = 4;
  if (flag_test_print)  // widget cout
  { qDebug() << "check y ";
  }

  for (int i=0; i<curent_uav_num; i++)
  { // 判断 x/y 是否是小数
    if ( !ui->tableWidget->item(i, currentColumn)->text().contains(QRegExp("^(\\-|\\+)?\\d+(\\.\\d+)?$")) )
    {
      QMessageBox::information(this, "Check Data !", QString("检查第%1行，第%2列数据.\n 请确保输入正确的数字/小数.").arg(i+1).arg(currentColumn+1) );
      return;
    }
    else
    { // 赋值
      no = ui->tableWidget->item(i, 0)->text().toInt(); //获取当前单元格的 No
      ot_offset_xy[no][1] = ui->tableWidget->item(i, currentColumn)->text().toFloat();
      ot_offset_uav_state[no] = Cenum::unset; //状态置于未设置
      ui->tableWidget->item(i, 1)->setText( metaEnum.valueToKey( ot_offset_uav_state[no] ) );

      if (flag_test_print)   // widget cout
      { qDebug() << "Set No=" << no << ", ot_offset_xy[no][1] = " << ot_offset_xy[no][1] ;
      }
    }
  }

  // z 偏差
  currentColumn = 5;
  if (flag_test_print)  // widget cout
  { qDebug() << "check y ";
  }

  for (int i=0; i<curent_uav_num; i++)
  { // 判断 x/y 是否是小数
    if ( !ui->tableWidget->item(i, currentColumn)->text().contains(QRegExp("^(\\-|\\+)?\\d+(\\.\\d+)?$")) )
    {
      QMessageBox::information(this, "Check Data !", QString("检查第%1行，第%2列数据.\n 请确保输入正确的数字/小数.").arg(i+1).arg(currentColumn+1) );
      return;
    }
    else
    { // 赋值
      no = ui->tableWidget->item(i, 0)->text().toInt(); //获取当前单元格的 No
      ot_offset_xy[no][2] = ui->tableWidget->item(i, currentColumn)->text().toFloat();
      ot_offset_uav_state[no] = Cenum::unset; //状态置于未设置
      ui->tableWidget->item(i, 1)->setText( metaEnum.valueToKey( ot_offset_uav_state[no] ) );

      if (flag_test_print)   // widget cout
      { qDebug() << "Set No=" << no << ", ot_offset_xy[no][2] = " << ot_offset_xy[no][2] ;
      }
    }
  }

  // yaw 偏差
  currentColumn = 6;
  if (flag_test_print)  // widget cout
  { qDebug() << "check y ";
  }

  for (int i=0; i<curent_uav_num; i++)
  { // 判断 x/y 是否是小数
    if ( !ui->tableWidget->item(i, currentColumn)->text().contains(QRegExp("^(\\-|\\+)?\\d+(\\.\\d+)?$")) )
    {
      QMessageBox::information(this, "Check Data !", QString("检查第%1行，第%2列数据.\n 请确保输入正确的数字/小数.").arg(i+1).arg(currentColumn+1) );
      return;
    }
    else
    { // 赋值
      no = ui->tableWidget->item(i, 0)->text().toInt(); //获取当前单元格的 No
      ot_offset_xy[no][3] = ui->tableWidget->item(i, currentColumn)->text().toFloat();
      ot_offset_uav_state[no] = Cenum::unset; //状态置于未设置
      ui->tableWidget->item(i, 1)->setText( metaEnum.valueToKey( ot_offset_uav_state[no] ) );

      if (flag_test_print)   // widget cout
      { qDebug() << "Set No=" << no << ", ot_offset_xy[no][3] = " << ot_offset_xy[no][3] ;
      }
    }
  }

  // 数据存储没问题后 校验
  bool flag_check = true;

  int leader_no = 0;
  // 只有一个 leader
  for (int i = 1; i < NNN; i++)
  {
    if ( flag_show_track_first_time[i] == false && ot_offset_leader[i] == true)
    {
      if (leader_no == 0)
      { leader_no = i;
      }
      else
      {
        flag_check = false;
        QMessageBox::information(this, "Check Data !", QString("存在不止一个领机: %1和%2.").arg(leader_no).arg(i) );
        return;
      }
    }
  }
  if (leader_no==0)
  {
    flag_check = false;
    QMessageBox::information(this, "Check Data !", QString("请设置一个领机!") );
    return;
  }

  // 每一架无人机的xy偏差不能一样
  for (int i = 1; i < NNN; i++)
  { for (int j = i+1; j < NNN; j++)
    {
      if ( flag_show_track_first_time[i] == false && flag_show_track_first_time[j] == false)
      {
        if (ot_offset_xy[i][0] == ot_offset_xy[j][0] &&
            ot_offset_xy[i][1] == ot_offset_xy[j][1])
        {
          flag_check = false;
          QMessageBox::information(this, "Check Data !", QString("无人机%1和%2的编队偏差相同了！").arg(i).arg(j) );
          return;
        }
      }
    }
  }

  if (flag_check) //通过校验
  {
    ui->formation_pushButton_set_single->setEnabled(1);
    ui->formation_pushButton_set_all->setEnabled(1);

    // 队形预览
    // update() 会自动调用 paintEvent(new QPaintEvent);
    emit print_flag();
  }
}

void formation_widget::on_formation_pushButton_set_all_clicked()
{
  // publish
  for (int i=1; i<NNN ;i++)
  {
    if ( flag_show_track_first_time[i]==false )
    {
      formation_qnode.msg_pub_set_local_pos_enu.compid = i;
      formation_qnode.msg_pub_set_local_pos_enu.x = ot_offset_xy[i][0];
      formation_qnode.msg_pub_set_local_pos_enu.y = ot_offset_xy[i][1];
      formation_qnode.msg_pub_set_local_pos_enu.flag = 2; //
      formation_qnode.msg_pub_set_local_pos_enu.z = ot_offset_xy[i][2];
      formation_qnode.msg_pub_set_local_pos_enu.yaw = ot_offset_xy[i][3]/180.0*3.1415926;

      formation_qnode.cb_pub_set_local_pos_enu();

      ot_offset_uav_state[i] = Cenum::send;
    }
  }

  // tabwidget 状态部分 更新
  int temp_k = 0; // ui tableWidget count
  for (int temp_j = 1; temp_j < NNN; temp_j++)
  {
    if ( flag_show_track_first_time[temp_j] == false )
    {
      ui->tableWidget->item(temp_k, 1)->setText( metaEnum.valueToKey( ot_offset_uav_state[temp_j] ) );
      temp_k = temp_k + 1;
    }
  }

}

void formation_widget::on_formation_pushButton_set_single_clicked()
{
  // publish
  int i;
  i = ui->comboBox_select_formation_single->currentText().toInt();

  formation_qnode.msg_pub_set_local_pos_enu.compid = i;
  formation_qnode.msg_pub_set_local_pos_enu.x = ot_offset_xy[i][0];
  formation_qnode.msg_pub_set_local_pos_enu.y = ot_offset_xy[i][1];
  formation_qnode.msg_pub_set_local_pos_enu.flag = 2; //
  formation_qnode.msg_pub_set_local_pos_enu.z = ot_offset_xy[i][2];
  formation_qnode.msg_pub_set_local_pos_enu.yaw = ot_offset_xy[i][3]/180.0*3.1415926;

  formation_qnode.cb_pub_set_local_pos_enu();

  ot_offset_uav_state[i] = Cenum::send;  // Cenum::Priority 1-unsend 2-send 3-Set
  i = ui->comboBox_select_formation_single->currentIndex();
  ui->tableWidget->item(i, 1)->setText( metaEnum.valueToKey( Cenum::send ) );

}


void formation_widget::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
  QTableWidgetItem* item = ui->tableWidget->item(currentRow,currentColumn); //获取当前单元格的 Item
  if ( item->text() == NULL || item==NULL )
  {  return;
  }
  // 修改了就需要 check !
  if (currentColumn >1)
  {
    ui->formation_pushButton_set_single->setEnabled(0);
    ui->formation_pushButton_set_all->setEnabled(0);
  }
}


void formation_widget::paintEvent(QPaintEvent *event)
{
  // widget_print
  // if( painter.begin(ui->graphicsView_print_formation) )
  if( painter.begin( this ) )
  {
    // qDebug() << "check flag ";
    painter.setRenderHint(QPainter::Antialiasing,true);//设置反锯齿
    pen.setWidth(2);
    pen.setColor(Qt::gray);     // pen.setColor(QColor(0,0,0));
    painter.setPen(pen);        //设置笔

    float point_start_x = 590;  //起始边框
    float point_start_y = 110;
    float outframe_width = 240; // 边框长度

    float max_width = 210;      // 最大边框
    float icon_diameter = 20;   // 图标 圆直径
    QRectF rectangle;
    int startAngle = 0 * 16;
    int spanAngle = 360 * 16;
    float font_size = 12;       // 字体高度

    painter.drawRect(point_start_x, point_start_y, outframe_width, outframe_width);    // formaiton 外框

    font.setFamily("黑体");
    font.setPointSize(font_size);
    font.setItalic(false);//斜体
    painter.setFont(font);//设置字体
    painter.drawText( point_start_x+outframe_width/2, point_start_y+font_size, "+Y" );
    painter.drawText( point_start_x+outframe_width/2, point_start_y+outframe_width-font_size/2, "-Y" );
    painter.drawText( point_start_x, point_start_y+outframe_width/2, "-X" );
    painter.drawText( point_start_x+outframe_width-font_size*1.4, point_start_y+outframe_width/2, "+X" );

    point_start_x = point_start_x + font_size*1.5;
    point_start_y = point_start_y + font_size*1.5;
    max_width = max_width - icon_diameter;
    pen.setColor(Qt::blue);
    painter.setPen(pen);    //设置笔的大小

    /*
    painter.drawRect(point_start_x, point_start_y, max_width, max_width);    // formaiton 外框
    rectangle.setRect( point_start_x, point_start_y, icon_diameter, icon_diameter);
    painter.drawArc( rectangle, startAngle, spanAngle);
    painter.drawText( rectangle, Qt::AlignHCenter, "10" );
    rectangle.setRect( point_start_x+max_width, point_start_y+max_width, icon_diameter, icon_diameter);
    painter.drawArc( rectangle, startAngle, spanAngle);
    painter.drawText( rectangle, Qt::AlignHCenter, "10" );
    */

    // 绘制各个点
    if ( curent_uav_num > 0 )
    { // 获取 data
      int count = 0;
      for (int i=1; i<NNN; i++)
      {
        if (flag_show_track_first_time[i]==false)
        {
          print_offset_noxy[count][0] = i;
          print_offset_noxy[count][1] = ot_offset_xy[i][0];
          print_offset_noxy[count][2] = ot_offset_xy[i][1];
          print_offset_noxy[count][3] = ot_offset_xy[i][2];
          print_offset_noxy[count][4] = ot_offset_xy[i][3]/180.0*3.1415926;
          count = count+1;
        }
      }
      // 获得最大最小 x&y
      float max_x, max_y, min_x, min_y;
      max_x = print_offset_noxy[0][1];
      min_x = print_offset_noxy[0][1];
      max_y = print_offset_noxy[0][2];
      min_y = print_offset_noxy[0][2];
      for (int i=1; i<curent_uav_num; i++)
      {
        if ( max_x < print_offset_noxy[i][1]) max_x=print_offset_noxy[i][1];
        if ( min_x > print_offset_noxy[i][1]) min_x=print_offset_noxy[i][1];
        if ( max_y < print_offset_noxy[i][2]) max_y=print_offset_noxy[i][2];
        if ( min_y > print_offset_noxy[i][2]) min_y=print_offset_noxy[i][2];
      }
      // 缩放尺度
      float scale_x, scale_y;
      scale_x = (max_x-min_x)/max_width;
      scale_y = (max_y-min_y)/max_width;
      // 获得最终的 xy 在 qt ui 中的坐标
      if (scale_x==0)
      {
        for (int i=0; i<curent_uav_num; i++)
        { // 所有x点一致
          print_offset_noxy[i][1] = point_start_x + max_width/2;
        }
      }
      else
      {
        for (int i=0; i<curent_uav_num; i++)
        {
          print_offset_noxy[i][1] = point_start_x + (print_offset_noxy[i][1] - min_x)/scale_x;
        }
      }

      if (scale_y==0)
      {
        for (int i=0; i<curent_uav_num; i++)
        { // 所有y点一致
          print_offset_noxy[i][2] = point_start_y + max_width/2;
        }
      }
      else
      {
        for (int i=0; i<curent_uav_num; i++)
        {
          print_offset_noxy[i][2] = point_start_y + (max_y - print_offset_noxy[i][2])/scale_y;
        }
      }
      // 偏移处理

      // 绘制
      for (int i=0; i<curent_uav_num; i++)
      {
        rectangle.setRect( print_offset_noxy[i][1], print_offset_noxy[i][2], icon_diameter, icon_diameter);
        painter.drawArc( rectangle, startAngle, spanAngle);

        painter.drawLine(
              print_offset_noxy[i][1] + icon_diameter/2,
              print_offset_noxy[i][2] + icon_diameter/2,
              print_offset_noxy[i][1] + icon_diameter/2 + icon_diameter/2.0 * cos(print_offset_noxy[i][4]),
              print_offset_noxy[i][2] + icon_diameter/2 + icon_diameter/2.0 * -sin(print_offset_noxy[i][4]) );
        painter.drawText( rectangle, Qt::AlignHCenter, QString::number( print_offset_noxy[i][0] ) );
      }
    }
    painter.end();
  }
}

// -------------------------------------------------------------------------
//                                                             Help Function
// -------------------------------------------------------------------------

/*
  // 由于 librviz 内嵌 QT UI 中， 窗口关闭后Ubuntu系统闪退 !! 故暂时不使用， 使用现成的 rviz.launch
  // init_rviz()
  // [Ref - QT之rviz组件的开发] https://blog.csdn.net/yoga_wyj/article/details/123099518
  // 创建rviz显示容器，其本质是一个qwidget容器
  rviz::RenderPanel *render_panel = new rviz::RenderPanel;
  // 设置布局,添加显示容器到UI界面
  ui->verticalLayout->addWidget(render_panel);
  // type2 - A.setParent(&w);
  // render_panel->setParent(ui->widget);

  // 初始化rviz控制对象
  rviz::VisualizationManager *manager = new rviz::VisualizationManager(render_panel);
  // 初始化camera, 实现camera视图的 放大，缩小和平移
  render_panel->initialize(manager->getSceneManager(), manager);
  // 初始化rviz 控制对象 显示
  manager->initialize();
  manager->removeAllDisplays();
  manager->startUpdate();
  //添加网格图层
  ...
 */

} // namespace map_widget
