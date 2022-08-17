
#include "../include/map_widget/bd_map_widget.hpp"

#define NNN 11

namespace map_widget {

using namespace Qt;

// -------------------------------------------------------------------------
//  bd_map_widget 初始化声明
// -------------------------------------------------------------------------

bd_map_widget::bd_map_widget(int argc, char** argv, QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::bd_map_widget)
  , bd_map_qnode(argc,argv)
{
  ui->setupUi(this);

  bd_map_qnode.init();

  // init()_1;
  m_pWebView = ui->webView;
  m_pWebChannel = new QWebChannel(this);

  // init falg
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    flag_show_track_first_time[temp_i] = true;
  }

  // loadHtml();
  // // 显示APP 路径 /home/zy/zy_new_mavros/cq_gcs/devel/lib/map_widget;
  // // ui->label->setText(qApp->applicationDirPath());
  // 获取程序所在路径，切换到同目录下的 地图html
  // 导入的是 ..cq_gcs/devel.. 的名称
  QString htmlPath = QCoreApplication::applicationDirPath();
  QString temp_path_y = "devel";
  int temp_path_1 = htmlPath.indexOf(temp_path_y);
  temp_path_y = "file://" + htmlPath.left(temp_path_1) + "src/cluster_gcs/offline_bmap/BMap_offline.html";

  // 通过ROS导入
  // QString temp_path_y;
  // QString htmlPath = QString::fromStdString(bd_map_qnode.rospackage_path);
  // htmlPath.remove(0,1);
  // temp_path_y = "file:///home" + QString::fromStdString(bd_map_qnode.rospackage_path) + "/offline_bmap/BMap_offline.html";
  qDebug() << "Map_html path = " << temp_path_y;

  m_pWebView->load(QUrl(temp_path_y));

  // 注册一个qtui对象  html端通过此名称向qt发送消息
  m_pWebChannel->registerObject(QString("qtui"), this);
  m_pWebView->page()->setWebChannel(m_pWebChannel);

  //--------------------------------------------------------------------------
  initConncetion(); //初始化槽函数

  ui->HomeGPS_checkBox_Set_Home->setCheckable(1);
  ui->HomeGPS_checkBox_Set_Home->setChecked(0);
  ui->HomeGPS_checkBox_Set_Home->setCheckable(0);

  ui->HomeGPS_pushButton_Set_centerLonLat->setEnabled(0);

  //默认图标颜色 黑
  ui->icon_color_radiobutton_b->setChecked(1);

  // 测距工具
  ui->DisGPS_pushButton_open->setEnabled(1);
  ui->DisGPS_pushButton_close->setEnabled(0);

  // 航迹显示 all
  ui->showtrack_pushButton_all_on->setEnabled(0);
  ui->showtrack_pushButton_all_off->setEnabled(0);
  ui->showtrack_pushButton_all_clean->setEnabled(0);

  // 航迹显示 单架
  ui->showtrack_pushButton_select_clean->setEnabled(0);

  // 添加 checkBox_showtrack[]
  signalMapper = new QSignalMapper(this);
  for (int iii=1; iii<NNN; iii++)
  {
    checkBox_showtrack[iii] = new QCheckBox;
    checkBox_showtrack[iii]->setText( QString("%1").arg(iii) );
    checkBox_showtrack[iii]->setChecked(0);
    checkBox_showtrack[iii]->setEnabled(0);
    ui->gridLayout_track->addWidget(checkBox_showtrack[iii], ((iii-1)/4), ((iii-1)%4) );
    // connect(checkBox_showtrack[iii], SIGNAL(clicked()), signalMapper, SLOT( map() ) );             //原始信号传递给signalmapper
    connect(checkBox_showtrack[iii], SIGNAL(stateChanged(int)), signalMapper, SLOT( map() ) );        //原始信号传递给signalmapper
    signalMapper->setMapping (checkBox_showtrack[iii], iii);
  }
  connect(signalMapper, SIGNAL(mapped (const int) ), this, SLOT( doClicked_showtrack(const int) ) );  //将转发的信号连接到最终的槽函数

  // 设置初始地图位置
  ui->HomeGPS_lineEdit_lon->setText( QString::number(bd_map_qnode.map_init_lon, 'f', 6) );
  ui->HomeGPS_lineEdit_lat->setText( QString::number(bd_map_qnode.map_init_lat, 'f', 6) );

}


bd_map_widget::~bd_map_widget()
{
  delete ui;
}

// #########################################################################
//                                                            Qt slots -> Js
// #########################################################################

// 初始化信号槽函数
void bd_map_widget::initConncetion()
{

//--------------------------------------
   // 测试用
   // 调用js
   connect(ui->callJSBtn, &QPushButton::clicked, [&]() {
       QString jsStr = QString("addCircle()");
       m_pWebView->page()->runJavaScript(jsStr);
   });

   // 获取js返回值
   connect(ui->getJsRetBtn, &QPushButton::clicked, [&]() {
       auto str = getJsRetString();
       ui->label->setText(str);
   });
//--------------------------------------

   // --- ROS ---
   // bd_map_QNode 获取界面信息更新
    connect( &bd_map_qnode, SIGNAL( power( ) ), this, SLOT( slot_power( ) ) );
   // 无人机位置
    connect( &bd_map_qnode, SIGNAL( power_pose( ) ), this, SLOT( slot_power_pose( ) ) );
   // 无人机轨迹
    connect( &bd_map_qnode, SIGNAL( power_track_current( ) ), this, SLOT( slot_power_track_current( ) ) );
}

// 获取web端返回值
QString bd_map_widget::getJsRetString()
{
    QEventLoop loop;
    QString jsStr = "getInfo();";
    QString retStr{}; // 返回值
    // 通过loop循环等到回调上来数据再继续
    m_pWebView->page()->runJavaScript(jsStr, [&](const QVariant &v)
    {   retStr = v.toString();
        loop.quit();
    });
    loop.exec();
    return retStr;
}

// -------------------------------------------------------------------------
//                                                          ROS solt 链接信号
// -------------------------------------------------------------------------
// qt gui 界面数据刷新
void bd_map_widget::slot_power( )
{

}


void bd_map_widget::slot_power_track_current()
{
  // 告知 js track航迹 path 增加一个 point
  for (int temp_i = 1; temp_i < NNN; temp_i++) {
    QString qtemp_x, qtemp_y, qtemp_num, strVal;
    if (bd_map_qnode.DataS_drone_gps_flag[temp_i]) { // 存储有飞机的位置信息更新
      qtemp_x = QString::number(bd_map_qnode.DataS_drone_gps_lon[temp_i], 'f', 6);
      qtemp_y = QString::number(bd_map_qnode.DataS_drone_gps_lat[temp_i], 'f', 6);
      qtemp_num = QString::number(temp_i);

      strVal = QString("plane_track_add_point(\"%1\",\"%2\",\"%3\");").arg(qtemp_x).arg(qtemp_y).arg(qtemp_num);
      m_pWebView->page()->runJavaScript(strVal);
    }
  }
}


void bd_map_widget::slot_power_pose( )
{
    // int temp_sys;
    // 在地图中 更新无人机的GPS位置

  for (int temp_i = 1; temp_i < NNN; temp_i++) {
    QString qtemp_x, qtemp_y, qtemp_num, strVal;
    if (bd_map_qnode.DataS_drone_gps_update_flag[temp_i])
    { // 有飞机位置信息更新
      // 首次有位置 -> ui 界面开启按钮
      if ( flag_show_track_first_time[temp_i])
      { // 置 0
        flag_show_track_first_time[temp_i] = false;
        // 按钮开启
        ui->showtrack_pushButton_all_on->setEnabled(1);
        ui->showtrack_pushButton_all_off->setEnabled(1);
        ui->showtrack_pushButton_all_clean->setEnabled(1);
        ui->showtrack_pushButton_select_clean->setEnabled(1);
        // 按钮开启
        checkBox_showtrack[temp_i]->setEnabled(1);

        // 列表添加无人机编号选项
        ui->comboBox_select_clean_no->clear();
        for (int temp_j = 1; temp_j < NNN; temp_j++)
        {
          if ( flag_show_track_first_time[temp_j] == false )
          {
            ui->comboBox_select_clean_no->addItem( QString("%1").arg(temp_j) );
          }
        }
      } // if ( flag_show_track_first_time[temp_i]) {

      qtemp_x = QString::number(bd_map_qnode.DataS_drone_gps_lon[temp_i], 'f', 6);
      qtemp_y = QString::number(bd_map_qnode.DataS_drone_gps_lat[temp_i], 'f', 6);
      qtemp_num = QString::number(temp_i);

      strVal = QString("Set_drone_GPS(\"%1\",\"%2\",\"%3\");").arg(qtemp_x).arg(qtemp_y).arg(qtemp_num);
      m_pWebView->page()->runJavaScript(strVal);

      bd_map_qnode.DataS_drone_gps_update_flag[temp_i] = false;
    }
  }
}

// -------------------------------------------------------------------------
//                                                                Home 点相关
// -------------------------------------------------------------------------
// const QString & jsMsg web端通知消息
// 接收来自 webhtml 的消息
// 当前点的 GPS 坐标
void bd_map_widget::recieveJsMessage_Point_GPS(const QString& jsMsg)
{
    flag_Set_marker1 = 1;
    flag_Set_marker1_2 = 0; // 防止重复生成
    // ui->label->setText(jsMsg);

    QString x = jsMsg;
    QString y = ",";
    int temp = x.indexOf(y);               // returns 0
    int temp2 = x.length();

    if ( temp > 11)                       //经度 取 小数点后7位
    {   x = jsMsg.left(11);}
    else
    {   x = jsMsg.left(temp);}

    if ( temp2-temp-1 > 10 )              //纬度 取 小数点后7位
    {   y = jsMsg.mid(temp+1, 10);}
    else
    {   y = jsMsg.mid(temp+1, temp2-temp-1);}

    ui->HomeGPS_lineEdit_lon->setText(x);
    ui->HomeGPS_lineEdit_lat->setText(y);
}

// 视野回到 Home 中心位置
void bd_map_widget::on_HomeGPS_pushButton_Set_centerLonLat_clicked()
{
  //判断有无设置 Home_GPS 点
  if( !flag_Set_Home_GPS )
    { QMessageBox::warning(NULL, QStringLiteral("提示"), QStringLiteral("请先设置 Home 点"),
                 QMessageBox::NoButton, QMessageBox::Yes);
      return;
    }
  QString strVal = QString("Set_centerLonLat(\"%1\",\"%2\");").arg(Str_Data_Gps_lon).arg(Str_Data_Gps_lat);
  // qDebug() << "strVal set init : " << strVal;
  m_pWebView->page()->runJavaScript(strVal);
}

// 设置 Home GPS
void bd_map_widget::on_HomeGPS_pushButton_Set_Home_clicked()
{
  //判断输入的是一个正确的小数 abs()<=180 未判断
  if(!ui->HomeGPS_lineEdit_lon->text().contains(QRegExp("^(\\-|\\+)?\\d+(\\.\\d+)?$")))
    { QMessageBox::warning(NULL, QStringLiteral("提示"), QStringLiteral("请确保'经度'输入正确数字"),
                 QMessageBox::NoButton, QMessageBox::Yes);
      return;
    }
  if(!ui->HomeGPS_lineEdit_lat->text().contains(QRegExp("^(\\-|\\+)?\\d+(\\.\\d+)?$")))
    { QMessageBox::warning(NULL, QStringLiteral("提示"), QStringLiteral("请确保'纬度'输入正确数字"),
                 QMessageBox::NoButton, QMessageBox::Yes);
      return;
    }
  // QString allStr  = QDateTime::currentDateTime().toString();

  // 存储
  Str_Data_Gps_lon = ui->HomeGPS_lineEdit_lon->text();
  Str_Data_Gps_lat = ui->HomeGPS_lineEdit_lat->text();

  ui->HomeGPS_checkBox_Set_Home->setText("已设置Home");
  ui->HomeGPS_checkBox_Set_Home->setCheckable(1);
  ui->HomeGPS_checkBox_Set_Home->setChecked(1);
  ui->HomeGPS_pushButton_Set_centerLonLat->setEnabled(1);
  // ui->HomeGPS_checkBox_Set_Home->setCheckable(0);

  QString strVal;
  // 待修改
  if( flag_Set_Home_GPS )
  { strVal = QString("Set_Home_GPS_2(\"%1\",\"%2\");").arg(Str_Data_Gps_lon).arg(Str_Data_Gps_lat);}
  else
  { strVal = QString("Set_Home_GPS(\"%1\",\"%2\");").arg(Str_Data_Gps_lon).arg(Str_Data_Gps_lat);
    flag_Set_Home_GPS = 1;
  }

  // rostopic 发布
  double temp_lon,temp_lat;
  temp_lon = Str_Data_Gps_lon.toDouble();
  temp_lat = Str_Data_Gps_lat.toDouble();
  bd_map_qnode.cb_pb_home_gps(temp_lon, temp_lat);

  m_pWebView->page()->runJavaScript(strVal);
}

void bd_map_widget::on_HomeGPS_pushButton_Obtain_Point_clicked()
{
  QString strVal;
  if ( !flag_Set_marker1_2 ) //flag_Set_marker1_2 = 0 防止重复生成调用
  {
    flag_Set_marker1_2 = 1;
    if (flag_Set_marker1)
    { strVal = QString("addMapEvent_GetCheckPointGPS_2();");}
    else
    { strVal = QString("addMapEvent_GetCheckPointGPS();");}
    m_pWebView->page()->runJavaScript(strVal);
  }
}

// 调整视野
void bd_map_widget::on_HomeGPS_pushButton_Obtain_Point_2_clicked()
{
  if(!ui->HomeGPS_lineEdit_lon->text().contains(QRegExp("^(\\-|\\+)?\\d+(\\.\\d+)?$")))
    { QMessageBox::warning(NULL, QStringLiteral("提示"), QStringLiteral("请确保'经度'输入正确数字"),
                 QMessageBox::NoButton, QMessageBox::Yes);
      return;
    }
  if(!ui->HomeGPS_lineEdit_lat->text().contains(QRegExp("^(\\-|\\+)?\\d+(\\.\\d+)?$")))
    { QMessageBox::warning(NULL, QStringLiteral("提示"), QStringLiteral("请确保'纬度'输入正确数字"),
                 QMessageBox::NoButton, QMessageBox::Yes);
      return;
    }

  // QString strVal = QString("Set_init_centerLonLat(\"%1\",\"%2\",\"%3\");")
  //    .arg(bd_map_qnode.map_init_lon).arg(bd_map_qnode.map_init_lat).arg(18);
  QString strVal = QString("Set_init_centerLonLat(\"%1\",\"%2\",\"%3\");")
      .arg(ui->HomeGPS_lineEdit_lon->text()).arg(ui->HomeGPS_lineEdit_lat->text()).arg(18);
  // qDebug() << "strVal set init : " << strVal;
  m_pWebView->page()->runJavaScript(strVal);

}

// -------------------------------------------------------------------------
//                                                                图标 白&黑色
// -------------------------------------------------------------------------
// 调整图标icon颜色
void bd_map_widget::on_icon_color_radiobutton_b_clicked()
{
  // 设置为黑色
  QString strVal = QString("Set_icon_b();");
  m_pWebView->page()->runJavaScript(strVal);

  // 重新绘制一遍
  replot_all();
}

void bd_map_widget::on_icon_color_radiobutton_w_clicked()
{
  // 设置为白色
  QString strVal = QString("Set_icon_w();");
  m_pWebView->page()->runJavaScript(strVal);

  // 重新绘制一遍
  replot_all();
}

void bd_map_widget::replot_all()
{
  // 重新绘制 home 点
  if( flag_Set_Home_GPS )
  {
    QString strVal;
    strVal = QString("Set_Home_GPS_2(\"%1\",\"%2\");").arg(Str_Data_Gps_lon).arg(Str_Data_Gps_lat);
    m_pWebView->page()->runJavaScript(strVal);
  }
  // 待添加 航点
}

// -------------------------------------------------------------------------
//                                                      多架无人机轨迹 显示&隐藏
// -------------------------------------------------------------------------
void bd_map_widget::on_showtrack_pushButton_all_on_clicked()
{
  for (int iii=1; iii<NNN; iii++)
  {
    checkBox_showtrack[iii]->setChecked(1);
  }
}

void bd_map_widget::on_showtrack_pushButton_all_off_clicked()
{
  for (int iii=1; iii<NNN; iii++)
  {
    checkBox_showtrack[iii]->setChecked(0);
  }
}

// -------------------------------------------------------------------------
//                                                      单架无人机轨迹 显示&隐藏
// -------------------------------------------------------------------------
void bd_map_widget::doClicked_showtrack(const int btnname)
{
  bool cstate = checkBox_showtrack[btnname]->isChecked();
  QString strVal;
  if ( cstate ) {
    strVal = QString("plane_track_show(%1);").arg(btnname);   // isChecked      cstate==2 被选中
    m_pWebView->page()->runJavaScript(strVal);
  } else {
    strVal = QString("plane_track_hide(%1);").arg(btnname);   // ==0 被取消
    m_pWebView->page()->runJavaScript(strVal);
  }
  // QMessageBox::information(this, "Clicked", QString("no=%1,clicked=%2").arg(btnname).arg(cstate) + "is clicked!");//显示被按下的btn名称。
}

// -------------------------------------------------------------------------
//                                                        多/单架无人机轨迹清除
// -------------------------------------------------------------------------
void bd_map_widget::on_showtrack_pushButton_all_clean_clicked()
{
  QString jsStr = QString("plane_track_clean_all()");
  m_pWebView->page()->runJavaScript(jsStr);
}

void bd_map_widget::on_showtrack_pushButton_select_clean_clicked()
{
  QString test;
  test = ui->comboBox_select_clean_no->currentText();
  int cstate;
  cstate = test.toInt();
  QString jsStr = QString("plane_track_clean(\"%1\")").arg(cstate);
  m_pWebView->page()->runJavaScript(jsStr);
}

// -------------------------------------------------------------------------
//                                                                地图测距功能
// -------------------------------------------------------------------------
// 开启地图测距
void bd_map_widget::on_DisGPS_pushButton_open_clicked()
{
  QString strVal = QString("Set_myDis_open();");
  m_pWebView->page()->runJavaScript(strVal);
  ui->DisGPS_pushButton_open->setEnabled(0);
  ui->DisGPS_pushButton_close->setEnabled(1);
  // 关闭其他按钮
  ui->groupBox_HomeGPS->setEnabled(0);
  ui->groupBox_track->setEnabled(0);
  // ui->groupBox_boundary->setEnabled(0);
}

// 关闭地图测距
void bd_map_widget::on_DisGPS_pushButton_close_clicked()
{
  QString strVal = QString("Set_myDis_close();");
  m_pWebView->page()->runJavaScript(strVal);
  ui->DisGPS_pushButton_open->setEnabled(1);
  ui->DisGPS_pushButton_close->setEnabled(0);
  // 开启其他窗口按钮
  ui->groupBox_HomeGPS->setEnabled(1);
  ui->groupBox_track->setEnabled(1);
  // ui->groupBox_boundary->setEnabled(1);
}

// -------------------------------------------------------------------------
//                                                             Help Function
// -------------------------------------------------------------------------


} // namespace map_widget
