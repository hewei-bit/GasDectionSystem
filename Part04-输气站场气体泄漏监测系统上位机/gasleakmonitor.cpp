#include "gasleakmonitor.h"
#include "ui_gasleakmonitor.h"

GasLeakMonitor::GasLeakMonitor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GasLeakMonitor)
{
    ui->setupUi(this); 

    //时间运行
    TimeRun();
    //收发文本显示初始化
    sendRecvInit();
    timer_1 = new QTimer(this);
    //串口初始化
    Port_init();

    //连接数据库
    //createConnection();
    openDatabase();
    openTable();

    //从数据库提取数据画图
    //打开数据库
    plot_data_init();

    //添加图像
    humi_plot();
    temp_plot();
    CH4_plot();

    connect(this,&GasLeakMonitor::humi_plot_signal,this,
            &GasLeakMonitor::humi_plot_Slot);
    connect(this,&GasLeakMonitor::temp_plot_signal,this,
            &GasLeakMonitor::temp_plot_Slot);
    connect(this,&GasLeakMonitor::CH4_plot_signal,this,
            &GasLeakMonitor::CH4_plot_Slot);


    //模拟从串口接收数据
    //sql_test_run();
    //系统运行
    //sys_test_run();
}

GasLeakMonitor::~GasLeakMonitor()
{
    delete ui;
}

/************************0.时间运行与初始化**************************/
/**********/
/**********/
/**********/
/**********/
void GasLeakMonitor::TimeRun()
{

//    mtimer = new QTimer(this);
//    connect(mtimer,&QTimer::timeout,this,&GasLeakMonitor::showtime);
//    mtimer->setInterval(1000);
//    mtimer->start(1000);

    mtimethread = new MyTimeThread(this);
    mtimethread->setObjectName("mtimethread");
    connect(mtimethread,&MyTimeThread::send,this,&GasLeakMonitor::showtime);
    mtimethread->start();
}

void GasLeakMonitor::showtime()
{
    time1 = QTime::currentTime();
    ui->Time_label->setText(time1.toString("hh:mm:ss"));
    data1 = QDate::currentDate();
    ui->Date_label->setText(data1.toString("yyyy-MM-dd"));
}

void GasLeakMonitor::Port_init()
{
    ui->port_1_comboBox->clear();
    serialinfos = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo serialinfo,serialinfos)
    {
        qDebug() << serialinfo.portName();
        qDebug() << serialinfo.description();
        qDebug() << serialinfo.manufacturer();
        qDebug() << serialinfos.size();
        ui->port_1_comboBox->addItem(serialinfo.portName());
    }
}

void GasLeakMonitor::sendRecvInit()
{
    portIsOpen = false;
    Hex_recv = false;
    Hex_send = false;
    text_recv = false;
    text_send = false;
    autoSend = false;
    show_recvtime = false;
    send_newline = false;

    //设置自动发送最大值
    ui->auto_send_count_spinBox->setMaximum(1000000);
    //默认接受text文本
    ui->text_recv_radioBtn->setChecked(true);
    //ui->hex_recv_radioBtn->setChecked(true);
    ui->text_send_radioBtn->setChecked(true);
    //ui->hex_send_radioBtn->setChecked(true);
}

/*********************1.串口设置相关串口收发数据*********************/
/*****/
/*****/
/*****/
/*****/
void GasLeakMonitor::MyJsonParse(NODE &node,QByteArray &ba)
{
    //用cjson库进行json解析
    //QByteArray   -------------data()-------------------->char*
    cJSON *messageJson = cJSON_Parse(ba.toStdString().c_str());
    if(NULL == messageJson)
    {
          //出错处理
    }
    cJSON *reason = cJSON_GetObjectItem(messageJson, "reason");
    cJSON *node_1 = cJSON_GetObjectItem(messageJson,"node_1");
    cJSON *node1_DeviceID = cJSON_GetObjectItem(node_1,"DeviceID");
    cJSON *node1_LORA_ADD = cJSON_GetObjectItem(node_1,"LORA_ADD");
    cJSON *node1_LORA_CHN = cJSON_GetObjectItem(node_1,"LORA_CHN");
    cJSON *node1_Temp = cJSON_GetObjectItem(node_1,"Temp");
    cJSON *node1_humi = cJSON_GetObjectItem(node_1,"humi");
    cJSON *node1_CH4_concentration = cJSON_GetObjectItem(node_1,"CH4_concentration");

    qDebug() << reason->valuestring;
    qDebug() << node1_DeviceID->valueint;
    qDebug() << node1_LORA_ADD->valueint;
    qDebug() << node1_LORA_CHN->valueint;
    qDebug() << node1_Temp->valuestring;
    qDebug() << node1_humi->valuestring;
    qDebug() << node1_CH4_concentration->valuestring;

    node.IDint = node1_DeviceID->valueint;
    node.tempstr = node1_Temp->valuestring;
    node.tempdouble = node.tempstr.toDouble();
    node.humistr = node1_humi->valuestring;
    node.humidouble = node.humistr.toDouble();
    node.CH4_str = node1_humi->valuestring;
    node.CH4double = node.CH4_str.toDouble();
}

void GasLeakMonitor::read_data()
{
    //由于read_All函数不能一次性接收完整个json数据包，需要分几次接收
    //因此需要声明为静态变量使其保存在内存中，多次接收分开传输的json数据
    static QByteArray messageArray;
    //接收这次接收到的数据
    QByteArray array = myserial_1.readAll();
    //qDebug() << array;

    /* 接受的数据模板
    {
        "node_1":{
            "DeviceID":10,
            "LORA_ADD":10,
            "LORA_CHN":23,
            "Temp":"00.0",
            "humi":"00.0",
            "CH4_concentration":"gq0.val=06???nd0.valgq0.val=06???nd0.val=00000???BLE"
        }
    }
    */

    //当数据不为空时，都加入messageArray之中组合成完整的数据包
    if(!array.isEmpty())
    {
        messageArray.append(array);
        //在数据包尾部加入#字符作为数据结尾，检测这段数据中是否含有#
        if(array.contains("#"))
        {
            //去除#字符，得到json数据包
            messageArray = messageArray.mid(0,messageArray.indexOf('#'));
            //检查数据包是否完整,不打印转义字符
            qDebug().noquote() << messageArray;

#if 1
            //MyJsonParse(node1,messageArray);
            //用cjson库进行json解析
            //QByteArray   -------------data()-------------------->char*
            cJSON *messageJson = cJSON_Parse(messageArray.toStdString().c_str());
            if(NULL == messageJson)
            {
                  //出错处理
            }
            cJSON *reason = cJSON_GetObjectItem(messageJson, "reason");
            cJSON *node_1 = cJSON_GetObjectItem(messageJson,"node_1");
            cJSON *node1_DeviceID = cJSON_GetObjectItem(node_1,"DeviceID");
            cJSON *node1_LORA_ADD = cJSON_GetObjectItem(node_1,"LORA_ADD");
            cJSON *node1_LORA_CHN = cJSON_GetObjectItem(node_1,"LORA_CHN");
            cJSON *node1_Temp = cJSON_GetObjectItem(node_1,"Temp");
            cJSON *node1_humi = cJSON_GetObjectItem(node_1,"humi");
            cJSON *node1_CH4_concentration = cJSON_GetObjectItem(node_1,"CH4_concentration");

            qDebug() << reason->valuestring;
            qDebug() << node1_DeviceID->valueint;
            qDebug() << node1_LORA_ADD->valueint;
            qDebug() << node1_LORA_CHN->valueint;
            qDebug() << node1_Temp->valuestring;
            qDebug() << node1_humi->valuestring;
            qDebug() << node1_CH4_concentration->valuestring;

            node1.IDint = node1_DeviceID->valueint;
            node1.LORA_ADRint = node1_LORA_ADD->valueint;
            node1.LORA_ADRstr = QString::number(node1.LORA_ADRint);
            node1.LORA_CHNint = node1_LORA_CHN->valueint;
            node1.LORA_CHNstr = QString::number(node1.LORA_CHNint);
            node1.tempstr = node1_Temp->valuestring;
            node1.tempdouble = node1.tempstr.toDouble();
            node1.humistr = node1_humi->valuestring;
            node1.humidouble = node1.humistr.toDouble();

            node1.CH4_str = node1_CH4_concentration->valuestring;
            QString gq0val = "gq0.val=";
            int lightIndex = node1.CH4_str.indexOf(gq0val);
            qDebug() << "lightIndex:" << lightIndex;
            QString lightStr = node1.CH4_str.mid(lightIndex+8,2);
            node1.lightdouble = lightStr.toDouble();
            qDebug() << "light:" << node1.lightdouble;

            QString nd0val = "nd0.val=";
            int concenIndex = node1.CH4_str.indexOf(nd0val);
            qDebug() << "concenIndex:" << concenIndex;
            QString concenStr = node1.CH4_str.mid(concenIndex+8,5);
            node1.CH4double = concenStr.toDouble();
            qDebug() <<  "CH4:" << node1.CH4double;

            //获取时间
            QString DT = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            //执行数据库语句
            QSqlQuery query;
            query.prepare("insert into Record(DeviceID,DATETIME,LORA_ADDR,LORA_CHN,HUMIDITY,temperature,CH4_concentration) "
                          "values(?,?,?,?,?,?,?)");
            query.addBindValue(node1.IDint);
            query.addBindValue(DT);
            query.addBindValue(node1.LORA_ADRstr);
            query.addBindValue(node1.LORA_CHNstr);
            query.addBindValue(node1.humistr);
            query.addBindValue(node1.tempstr);
            query.addBindValue(concenStr);
            query.exec();

            //显示
            ui->temp_tB_1->setAlignment(Qt::AlignCenter);
            ui->temp_tB_1->setText(node1.tempstr);
            ui->humi_tB_1->setText(node1.humistr);
            ui->CH4_tB_1->setText(concenStr);

            emit humi_plot_signal(node1.humidouble,node1.IDint);
            emit temp_plot_signal(node1.tempdouble,node1.IDint);
            emit CH4_plot_signal(node1.CH4double,node1.IDint);
#endif
            //一次接收完成，清除buf
            messageArray.clear();
        }
    }

    QString data;
    //若显示接收时间
    if(ui->recv_time_checkBox->isChecked())
    {
        QTime nowTime = QTime::currentTime();
        data += "/n" + nowTime.toString("HH:mm::ss")+"/n";
    }

    //选择格式
    if(ui->text_recv_radioBtn->isChecked())
        data = QString(array);
    else if(ui->hex_recv_radioBtn->isChecked())
        data = QString(array.toHex());

    //重新输入文本
    ui->recv_textBrowser->insertPlainText(data);

    //改变光标位置
    QTextCursor cursor = ui->recv_textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->recv_textBrowser->setTextCursor(cursor);

    //统计
    ui->sRNumLabel->setNum(ui->sRNumLabel->text().toInt() + array.size());
}


void GasLeakMonitor::on_send_Btn_clicked()
{
    //自动发送检查
    if(autoSend == true)
    {
        timer_1->start();
        return;
    }

    //输入空为空，则发送换行符
    if(ui->send_textBrowser->document()->toPlainText() == "")
    {
        myserial_1.write("\r\n");
        return;
    }

    char*  ch;
    QString sendData = ui->send_textBrowser->document()->toPlainText();
    QByteArray ba = sendData.toLatin1();
    ch=ba.data();

    if(send_newline == true)//发送新行检查
    {
        sendData += "\r\n";
    }
    qDebug() << sendData;
    myserial_1.write(ch,sendData.count());//发送数据

    ui->sTNumLabel->setNum(ui->sTNumLabel->text().toInt() + sendData.size());
    //ui->send_textBrowser->clear();

    ui->recv_textBrowser->moveCursor(QTextCursor::End);
}

bool GasLeakMonitor::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->send_textBrowser)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->key() == Qt::Key_Return)
            {
                on_send_Btn_clicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

void GasLeakMonitor::on_check_serial_Btn_1_clicked()
{
    ui->port_1_comboBox->clear();
    serialinfos = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo serialinfo,serialinfos)
    {
        qDebug() << serialinfo.portName();
        qDebug() << serialinfo.description();
        qDebug() << serialinfo.manufacturer();
        qDebug() << serialinfos.size();
        ui->port_1_comboBox->addItem(serialinfo.portName());
    }
}

void GasLeakMonitor::on_open_serial_Btn_1_clicked()
{
    if(!portIsOpen)
    {
        //获取选项
        ui->Parity_1_comboBox->currentText();
        ui->StopBits_1_comboBox->currentText();
        ui->port_1_comboBox->currentText();
        ui->BaudRate_1_comboBox->currentText();
        ui->DataBits_1_comboBox->currentText();

        //[1]配置串口
        myserial_1.setPortName(ui->port_1_comboBox->currentText());
        myserial_1.setBaudRate(myserial_1.selectBaudRate(ui->BaudRate_1_comboBox->currentText()));
        myserial_1.setDataBits(myserial_1.selectDataBits(ui->DataBits_1_comboBox->currentText()));
        myserial_1.setParity(myserial_1.selectParityBits(ui->Parity_1_comboBox->currentText()));
        myserial_1.setStopBits(myserial_1.selectStopBits(ui->StopBits_1_comboBox->currentText()));

        //[2]打开串口
        if(!myserial_1.open(QIODevice::ReadWrite))
        {
            qDebug() << "open port fail";
            QString tips = QString("端口打开失败！/n错误码：%1").arg(myserial_1.error());
            QMessageBox::information(this,"错误提示",tips);
            return;
        }

        //[3] 建立与readyRead() 信号的槽函数
        connect(&myserial_1,&QSerialPort::readyRead,this,&GasLeakMonitor::read_data);


        //改变状态栏
        ui->stateLabel->setText("串口已打开");
        ui->stateLabel->setStyleSheet("color::rgb(30,230,50);");

        //关闭按钮
        ui->open_serial_Btn_1->setText("关闭串口");
        ui->open_serial_Btn_1->setStyleSheet("background-color:rgb(0,255,0);");

        //锁定状态
        ui->port_1_comboBox->setEnabled(false);
        ui->BaudRate_1_comboBox->setEnabled(false);
        ui->DataBits_1_comboBox->setEnabled(false);
        ui->StopBits_1_comboBox->setEnabled(false);
        ui->Parity_1_comboBox->setEnabled(false);

        ui->refresh_Btn_1->setEnabled(false);
        ui->send_Btn->setEnabled(true);

        portIsOpen = true;
    }
    else
    {
        myserial_1.close();

        //状态栏
        ui->stateLabel->setText("串口未打开");
        ui->stateLabel->setStyleSheet("color:rgb(0,0,0);");

        //改变按钮
        ui->open_serial_Btn_1->setText("开启串口");
        ui->open_serial_Btn_1->setStyleSheet("background-color:rgb(255,0,0);");

        //锁定状态
        ui->port_1_comboBox->setEnabled(true);
        ui->BaudRate_1_comboBox->setEnabled(true);
        ui->DataBits_1_comboBox->setEnabled(true);
        ui->StopBits_1_comboBox->setEnabled(true);
        ui->Parity_1_comboBox->setEnabled(true);

        ui->refresh_Btn_1->setEnabled(true);
        ui->send_Btn->setEnabled(false);

        portIsOpen=false;
    }
}

void GasLeakMonitor::on_refresh_Btn_1_clicked()
{
    Port_init();
}

void GasLeakMonitor::on_check_serial_Btn_2_clicked()
{
    ui->port_2_comboBox->clear();
    serialinfos = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo serialinfo,serialinfos)
    {
        qDebug() << serialinfo.portName();
        qDebug() << serialinfo.description();
        qDebug() << serialinfo.manufacturer();
        qDebug() << serialinfos.size();
        ui->port_2_comboBox->addItem(serialinfo.portName());
    }
}

void GasLeakMonitor::on_open_serial_Btn_2_clicked()
{

}

void GasLeakMonitor::on_refresh_Btn_2_clicked()
{
    Port_init();
}

void GasLeakMonitor::on_send_clear_Btn_clicked()
{
    ui->send_textBrowser->clear();
}

void GasLeakMonitor::on_sNumClearBtn_clicked()
{
    ui->sRNumLabel->setNum(0);
    ui->sTNumLabel->setNum(0);
}

void GasLeakMonitor::on_clear_recv_box_Btn_clicked()
{
    ui->recv_textBrowser->clear();
}

void GasLeakMonitor::on_sql_tableWidget_cellActivated(int row, int column)
{

}

void GasLeakMonitor::on_check_Btn_clicked()
{
    char*  ch;
    QString sendData = "check\r\n";
    QByteArray ba = sendData.toLatin1();
    ch=ba.data();
    myserial_1.write(ch,sendData.count());
    //http_weather();
}

void GasLeakMonitor::http_weather()
{
    manager = new QNetworkAccessManager();

    connect(manager,&QNetworkAccessManager::finished,this,&GasLeakMonitor::readWeatherData);

    QUrl url("http://apis.juhe.cn/simpleWeather/query?"
             "city=%E5%B9%BF%E5%B7%9E"
             "&key=6eaaa433d136ff59653d126c67270943");
    QNetworkRequest request(url);
    manager->get(request);
}

void GasLeakMonitor::readWeatherData(QNetworkReply *reply)
{
    QByteArray array = reply->readAll();
    qDebug() << array;
    QJsonDocument doc = QJsonDocument::fromJson(array);

    /*
    {
        "reason":"查询成功!",
        "result":{
            "city":"广州",
            "realtime":{
                "temperature":"29",
                "humidity":"75",
                "info":"多云",
                "wid":"01",
                "direct":"东南风",
                "power":"2级",
                "aqi":"15"
            }
    }
    */

    //获取整个文本
    QJsonObject object =  doc.object();
    //是否成功
    QString reason = object.value("reason").toString();
    qDebug().noquote() << reason;
    QJsonObject resultObject = object.value("result").toObject();
    //所在城市
    QString mcity = resultObject.value("city").toString();
    qDebug() << mcity;
    //实时信息
    QJsonObject realtimeObject = resultObject.value("realtime").toObject();

    QString mtemperature = realtimeObject.value("temperature").toString();
    QString minfo = realtimeObject.value("info").toString();
    QString mhumidity = realtimeObject.value("humidity").toString();
    QString winddirect = realtimeObject.value("direct").toString();
    QString windpower = realtimeObject.value("power").toString();

    qDebug() << minfo;
    qDebug() << mtemperature;
    qDebug() << mhumidity;
    qDebug() << winddirect;
    qDebug() << windpower;
}

/***********************3.画图界面相关************************/
/*****/
/*****/
/*****/
/*****/
/*****/
void GasLeakMonitor::humi_realtime_plot_set()
{
    // 设定右上角图形标注可见
    ui->humi_data_realtime_plot->legend->setVisible(true);
    // 设定右上角图形标注的字体
    ui->humi_data_realtime_plot->legend->setFont(QFont("Helvetica",9));
    ui->humi_data_realtime_plot->addGraph();
    ui->humi_data_realtime_plot->graph(0)->setPen(QPen(Qt::red));
    ui->humi_data_realtime_plot->graph(0)->setName("节点0");
    ui->humi_data_realtime_plot->addGraph();
    ui->humi_data_realtime_plot->graph(1)->setPen(QPen(Qt::blue));
    ui->humi_data_realtime_plot->graph(1)->setName("节点1");
    ui->humi_data_realtime_plot->addGraph();
    ui->humi_data_realtime_plot->graph(2)->setPen(QPen(Qt::green));
    ui->humi_data_realtime_plot->graph(2)->setName("节点2");
    //坐标轴使用时间刻度
//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->humi_data_realtime_plot->xAxis->setTicker(timeTicker);
    //四边安上坐标轴
    ui->humi_data_realtime_plot->axisRect()->setupFullAxesBox();
    //设置坐标范围和
    ui->humi_data_realtime_plot->xAxis->setLabel("时间");
    ui->humi_data_realtime_plot->xAxis->setRange(0,20);
    ui->humi_data_realtime_plot->yAxis->setLabel("湿度");
    ui->humi_data_realtime_plot->yAxis->setRange(20,150);
    // 使上下轴、左右轴范围同步
    connect(ui->humi_data_realtime_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->humi_data_realtime_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->humi_data_realtime_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->humi_data_realtime_plot->yAxis2, SLOT(setRange(QCPRange)));

}

void GasLeakMonitor::temp_realtime_plot_set()
{
    // 设定右上角图形标注可见
    ui->temp_data_realtime_plot->legend->setVisible(true);
    // 设定右上角图形标注的字体
    ui->temp_data_realtime_plot->legend->setFont(QFont("Helvetica",9));
    ui->temp_data_realtime_plot->addGraph();
    ui->temp_data_realtime_plot->graph(0)->setPen(QPen(Qt::red));
    ui->temp_data_realtime_plot->graph(0)->setName("节点0");
    ui->temp_data_realtime_plot->addGraph();
    ui->temp_data_realtime_plot->graph(1)->setPen(QPen(Qt::blue));
    ui->temp_data_realtime_plot->graph(1)->setName("节点1");
    ui->temp_data_realtime_plot->addGraph();
    ui->temp_data_realtime_plot->graph(2)->setPen(QPen(Qt::green));
    ui->temp_data_realtime_plot->graph(2)->setName("节点2");
    //坐标轴使用时间刻度
//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->temp_data_realtime_plot->xAxis->setTicker(timeTicker);
    //四边安上坐标轴
    ui->temp_data_realtime_plot->axisRect()->setupFullAxesBox();
    //设置坐标范围和
    ui->temp_data_realtime_plot->xAxis->setLabel("时间");
    ui->temp_data_realtime_plot->xAxis->setRange(0,20);
    ui->temp_data_realtime_plot->yAxis->setLabel("温度");
    ui->temp_data_realtime_plot->yAxis->setRange(0,40);
    // 使上下轴、左右轴范围同步
    connect(ui->temp_data_realtime_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->temp_data_realtime_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->temp_data_realtime_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->temp_data_realtime_plot->yAxis2, SLOT(setRange(QCPRange)));
}

void GasLeakMonitor::CH4_realtime_plot_set()
{
    // 设定右上角图形标注可见
    ui->CH4_data_realtime_plot->legend->setVisible(true);
    // 设定右上角图形标注的字体
    ui->CH4_data_realtime_plot->legend->setFont(QFont("Helvetica",9));
    ui->CH4_data_realtime_plot->addGraph();
    ui->CH4_data_realtime_plot->graph(0)->setPen(QPen(Qt::red));
    ui->CH4_data_realtime_plot->graph(0)->setName("节点0");
    ui->CH4_data_realtime_plot->addGraph();
    ui->CH4_data_realtime_plot->graph(1)->setPen(QPen(Qt::blue));
    ui->CH4_data_realtime_plot->graph(1)->setName("节点1");
    ui->CH4_data_realtime_plot->addGraph();
    ui->CH4_data_realtime_plot->graph(2)->setPen(QPen(Qt::green));
    ui->CH4_data_realtime_plot->graph(2)->setName("节点2");
    //坐标轴使用时间刻度
//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->CH4_data_realtime_plot->xAxis->setTicker(timeTicker);
    //四边安上坐标轴
    ui->CH4_data_realtime_plot->axisRect()->setupFullAxesBox();
    //设置坐标范围和
    ui->CH4_data_realtime_plot->xAxis->setLabel("时间");
    ui->CH4_data_realtime_plot->xAxis->setRange(0,20);
    ui->CH4_data_realtime_plot->yAxis->setLabel("浓度");
    ui->CH4_data_realtime_plot->yAxis->setRange(0,10000);
    // 使上下轴、左右轴范围同步
    connect(ui->CH4_data_realtime_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->CH4_data_realtime_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->CH4_data_realtime_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->CH4_data_realtime_plot->yAxis2, SLOT(setRange(QCPRange)));
}

int GasLeakMonitor::count_for_record(QString &sqlquery)
{
    int num = 0;

    if(!sqlQuery.exec(sqlquery))
        qDebug() << "Error:Failed to SELECT record"<< database.lastError();
    else
        qDebug() << "success";

    //计算数据数量
    while(sqlQuery.next()){
        num++;
    }
    return num;
}


void GasLeakMonitor::Get_data_from_record(QString &sqlquery,QString &parameter,QVector<double> &dataX,QVector<double> &dataY)
{
    if(!sqlQuery.exec(sqlquery)){
        qDebug() << "Error:Failed to SELECT record"<< database.lastError();
    }else{
        qDebug() << "success";
    }
    int para = 0;
    if(parameter == "humi"){
        para = 4;
    }
    else if(parameter == "temp"){
        para = 5;
    }
    else if(parameter == "CH4"){
        para = 6;
    }
    //将数据存入vector
    int j = 0;
    while(sqlQuery.next()){
        dataX[j] = j;
        dataY[j] = sqlQuery.value(para).toInt();
        j++;
    }
}

void GasLeakMonitor::resize_buffer()
{
    humiY_0.resize(num_node_0);
    humiX_0.resize(num_node_0);
    humiY_1.resize(num_node_1);
    humiX_1.resize(num_node_1);
    humiY_2.resize(num_node_2);
    humiX_2.resize(num_node_2);

    TempY_0.resize(num_node_0);
    TempX_0.resize(num_node_0);
    TempY_1.resize(num_node_1);
    TempX_1.resize(num_node_1);
    TempY_2.resize(num_node_2);
    TempX_2.resize(num_node_2);

    CH4Y_0.resize(num_node_0);
    CH4X_0.resize(num_node_0);
    CH4Y_1.resize(num_node_1);
    CH4X_1.resize(num_node_1);
    CH4Y_2.resize(num_node_2);
    CH4X_2.resize(num_node_2);
}

void GasLeakMonitor::plot_data_init()
{
    num_all = count_for_record(sqlSelect);
    num_node_0 = count_for_record(sqlSelect_0);
    num_node_1 = count_for_record(sqlSelect_1);
    num_node_2 = count_for_record(sqlSelect_2);

    qDebug() << "num_all: "<< num_all<< " "<<num_node_0<<" "<<num_node_1<<" "<<num_node_2<<" ";

    resize_buffer();

    Get_data_from_record(sqlSelect_0,humi,humiX_0,humiY_0);
    Get_data_from_record(sqlSelect_1,humi,humiX_1,humiY_1);
    Get_data_from_record(sqlSelect_2,humi,humiX_2,humiY_2);

    Get_data_from_record(sqlSelect_0,temp,TempX_0,TempY_0);
    Get_data_from_record(sqlSelect_1,temp,TempX_1,TempY_1);
    Get_data_from_record(sqlSelect_2,temp,TempX_2,TempY_2);

    Get_data_from_record(sqlSelect_0,CH4,CH4X_0,CH4Y_0);
    Get_data_from_record(sqlSelect_1,CH4,CH4X_1,CH4Y_1);
    Get_data_from_record(sqlSelect_2,CH4,CH4X_2,CH4Y_2);

    humi_realtime_plot_set();
    temp_realtime_plot_set();
    CH4_realtime_plot_set();
}


void GasLeakMonitor::humi_plot_Slot(double new_humiY,int ID)
{
    qDebug()<<"new_humiY:"<<new_humiY<<endl;
    static QTime time(QTime::currentTime());

    double key = time.elapsed()/1000.0 ; // 开始到现在的时间，单位秒
    //key = key + num_all;
    static double lastPointKey = 0;
    if(key - lastPointKey > 0.002){
        // 添加数据到graph
        ui->humi_data_realtime_plot->graph(ID)->addData(key,new_humiY);
        //记录当前时刻
        lastPointKey = key;
    }
    // 曲线能动起来的关键在这里，设定x轴范围为最近10个时刻
    ui->humi_data_realtime_plot->xAxis->setRange(key,30,Qt::AlignRight);
    // 绘图
    ui->humi_data_realtime_plot->replot();
}

void GasLeakMonitor::temp_plot_Slot(double new_tempY,int ID)
{
    qDebug()<<"new_tempY:"<<new_tempY<<endl;
    static QTime time(QTime::currentTime());

    double key = time.elapsed()/1000.0 ; // 开始到现在的时间，单位秒
//    key = key + num_all;
    static double lastPointKey = 0;
    if(key - lastPointKey > 0.002){
        // 添加数据到graph
        ui->temp_data_realtime_plot->graph(ID)->addData(key,new_tempY);
        //记录当前时刻
        lastPointKey = key;
    }
    // 曲线能动起来的关键在这里，设定x轴范围为最近200个时刻
    ui->temp_data_realtime_plot->xAxis->setRange(key,20,Qt::AlignRight);
    // 绘图
    ui->temp_data_realtime_plot->replot();
}

void GasLeakMonitor::CH4_plot_Slot(double new_CH4Y,int ID)
{
    qDebug()<<"new_CH4Y:"<<new_CH4Y<<endl;
    static QTime time(QTime::currentTime());

    double key = time.elapsed()/1000.0 ; // 开始到现在的时间，单位秒
//    key = key + num_all;
    static double lastPointKey = 0;
    if(key - lastPointKey > 0.002){
        // 添加数据到graph
        ui->CH4_data_realtime_plot->graph(ID)->addData(key,new_CH4Y);
        //记录当前时刻
        lastPointKey = key;
    }
    // 曲线能动起来的关键在这里，设定x轴范围为最近200个时刻
    ui->CH4_data_realtime_plot->xAxis->setRange(key,20,Qt::AlignRight);
    // 绘图
    ui->CH4_data_realtime_plot->replot();
}

void GasLeakMonitor::humi_plot()
{
    // 设定右上角图形标注可见
    ui->humi_data_plot->legend->setVisible(true);
    // 设定右上角图形标注的字体
    ui->humi_data_plot->legend->setFont(QFont("Helvetica",9));
    ui->humi_data_plot->addGraph();
    ui->humi_data_plot->graph(0)->setPen(QPen(Qt::red));
    ui->humi_data_plot->graph(0)->setName("节点0");
    ui->humi_data_plot->addGraph();
    ui->humi_data_plot->graph(1)->setPen(QPen(Qt::blue));
    ui->humi_data_plot->graph(1)->setName("节点1");
    ui->humi_data_plot->addGraph();
    ui->humi_data_plot->graph(2)->setPen(QPen(Qt::green));
    ui->humi_data_plot->graph(2)->setName("节点2");
    //坐标轴使用时间刻度
//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->humi_data_plot->xAxis->setTicker(timeTicker);
    //四边安上坐标轴
    ui->humi_data_plot->axisRect()->setupFullAxesBox();
    //设置坐标范围和
    ui->humi_data_plot->xAxis->setLabel("时间");
    ui->humi_data_plot->xAxis->setRange(0,150);
    ui->humi_data_plot->yAxis->setLabel("湿度");
    ui->humi_data_plot->yAxis->setRange(20,150);
    // 使上下轴、左右轴范围同步
    connect(ui->humi_data_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->humi_data_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->humi_data_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->humi_data_plot->yAxis2, SLOT(setRange(QCPRange)));
    ui->humi_data_plot->graph(0)->setData(humiX_0,humiY_0);
    ui->humi_data_plot->graph(1)->setData(humiX_1,humiY_1);
    ui->humi_data_plot->graph(2)->setData(humiX_2,humiY_2);

    ui->humi_data_plot->plottable();
}

void GasLeakMonitor::temp_plot()
{
    // 设定右上角图形标注可见
    ui->temp_data_plot->legend->setVisible(true);
    // 设定右上角图形标注的字体
    ui->temp_data_plot->legend->setFont(QFont("Helvetica",9));
    ui->temp_data_plot->addGraph();
    ui->temp_data_plot->graph(0)->setPen(QPen(Qt::red));
    ui->temp_data_plot->graph(0)->setName("节点0");
    ui->temp_data_plot->addGraph();
    ui->temp_data_plot->graph(1)->setPen(QPen(Qt::blue));
    ui->temp_data_plot->graph(1)->setName("节点1");
    ui->temp_data_plot->addGraph();
    ui->temp_data_plot->graph(2)->setPen(QPen(Qt::green));
    ui->temp_data_plot->graph(2)->setName("节点2");
    //坐标轴使用时间刻度
//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->temp_data_plot->xAxis->setTicker(timeTicker);
    //四边安上坐标轴
    ui->temp_data_plot->axisRect()->setupFullAxesBox();
    ui->temp_data_plot->xAxis->setLabel("时间");
    ui->temp_data_plot->xAxis->setRange(0,150);
    ui->temp_data_plot->yAxis->setLabel("温度");
    ui->temp_data_plot->yAxis->setRange(0,50);
    // 使上下轴、左右轴范围同步
    connect(ui->temp_data_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->temp_data_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->temp_data_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->temp_data_plot->yAxis2, SLOT(setRange(QCPRange)));
    ui->temp_data_plot->graph(0)->setData(TempX_0,TempY_0);
    ui->temp_data_plot->graph(1)->setData(TempX_1,TempY_1);
    ui->temp_data_plot->graph(2)->setData(TempX_2,TempY_2);

    ui->temp_data_plot->plottable();
}

void GasLeakMonitor::CH4_plot()
{
    // 设定右上角图形标注可见
    ui->CH4_data_plot->legend->setVisible(true);
    // 设定右上角图形标注的字体
    ui->CH4_data_plot->legend->setFont(QFont("Helvetica",9));
    ui->CH4_data_plot->addGraph();
    ui->CH4_data_plot->graph(0)->setPen(QPen(Qt::red));
    ui->CH4_data_plot->graph(0)->setName("节点0");
    ui->CH4_data_plot->addGraph();
    ui->CH4_data_plot->graph(1)->setPen(QPen(Qt::blue));
    ui->CH4_data_plot->graph(1)->setName("节点1");
    ui->CH4_data_plot->addGraph();
    ui->CH4_data_plot->graph(2)->setPen(QPen(Qt::green));
    ui->CH4_data_plot->graph(2)->setName("节点2");
    //坐标轴使用时间刻度
//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->CH4_data_plot->xAxis->setTicker(timeTicker);
    //四边安上坐标轴
    ui->CH4_data_plot->axisRect()->setupFullAxesBox();
    ui->CH4_data_plot->xAxis->setLabel("时间");
    ui->CH4_data_plot->xAxis->setRange(0,150);
    ui->CH4_data_plot->yAxis->setLabel("浓度");
    ui->CH4_data_plot->yAxis->setRange(0,10000);
    // 使上下轴、左右轴范围同步
    connect(ui->CH4_data_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->CH4_data_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->CH4_data_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->CH4_data_plot->yAxis2, SLOT(setRange(QCPRange)));
    ui->CH4_data_plot->graph(0)->setData(CH4X_0,CH4Y_0);
    ui->CH4_data_plot->graph(1)->setData(CH4X_1,CH4Y_1);
    ui->CH4_data_plot->graph(2)->setData(CH4X_2,CH4Y_2);
    ui->CH4_data_plot->plottable();
}



/***********************4.数据查询界面相关************************/
/*****/
/*****/
/*****/
/*****/
void GasLeakMonitor::openDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");

    database.setDatabaseName(dbname);

    if(!database.open())
    {
        qCritical("Can't open database: %s(%s)",
                  database.lastError().text().toLocal8Bit().data(),
                  qt_error_string().toLocal8Bit().data());
    }
}

void GasLeakMonitor::openTable()
{
    // 每次进入先清空
    ui->sql_tableWidget->clear();
    ui->sql_tableWidget->setRowCount(0);
    ui->sql_tableWidget->clearContents();

    // 设置表头
    QStringList header;
    header<<tr("DeviceID")<<tr("日期时间")<<tr("LORA地址")<<tr("LORA信道")<<tr("湿度")<<tr("温度")<<tr("CH4浓度");
    ui->sql_tableWidget->setHorizontalHeaderLabels(header);
    //qDebug() <<header;
    ui->sql_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 打开数据库
    sqlQuery = QSqlQuery(database);
    // 先显示数据库所有的数据
    QString sqlall = QString("SELECT * FROM Record");
    if(!sqlQuery.exec(sqlall))
    {
        qDebug() << "Error:Failed to SELECT record"<< database.lastError();
    }else
    {
        qDebug() << "success";
    }

    int i = 0;
    ui->sql_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    while(sqlQuery.next())
    {
        ui->sql_tableWidget->insertRow(i);
        ui->sql_tableWidget->setItem(i,0,new QTableWidgetItem(sqlQuery.value(0).toString()));
        ui->sql_tableWidget->setItem(i,1,new QTableWidgetItem(sqlQuery.value(1).toString()));
        ui->sql_tableWidget->setItem(i,2,new QTableWidgetItem(sqlQuery.value(2).toString()));
        ui->sql_tableWidget->setItem(i,3,new QTableWidgetItem(sqlQuery.value(3).toString()));
        ui->sql_tableWidget->setItem(i,4,new QTableWidgetItem(sqlQuery.value(4).toString()));
        ui->sql_tableWidget->setItem(i,5,new QTableWidgetItem(sqlQuery.value(5).toString()));
        ui->sql_tableWidget->setItem(i,6,new QTableWidgetItem(sqlQuery.value(6).toString()));

        ui->sql_tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->sql_tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->sql_tableWidget->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->sql_tableWidget->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->sql_tableWidget->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->sql_tableWidget->item(i,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->sql_tableWidget->item(i,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        i++;
    }
}

void GasLeakMonitor::on_query_Btn_clicked()
{
    // 看是否添加了限定条件
//    if(){
//    }
//    else if

    // 查询的起时间startDateTime,和止时间endDateTime回到查询语句中。
    QDateTime StartDateTime = ui->START_dateTimeEdit->dateTime();
    QDateTime EndDateTime = ui->ENDdateTimeEdit->dateTime();
    QString StartDateTimeStr = StartDateTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << StartDateTimeStr;
    QString EndDateTimeStr = EndDateTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << EndDateTimeStr;

#if 1
    // 方法一使用 QSqlTableModel
    QSqlTableModel *model = new QSqlTableModel(ui->sql_tableView);
    model ->setTable("record");//数据记录表
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("设备ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("日期时间"));
    model->setHeaderData(2, Qt::Horizontal, tr("LORA地址"));
    model->setHeaderData(3, Qt::Horizontal, tr("LORA信道"));
    model->setHeaderData(4, Qt::Horizontal, tr("湿度"));
    model->setHeaderData(5, Qt::Horizontal, tr("温度"));
    model->setHeaderData(6, Qt::Horizontal, tr("CH4浓度"));
    QString SqlStr = QString("DATETIME >= '%1' and DATETIME <= '%2'").arg(StartDateTimeStr).arg(EndDateTimeStr);
    // QString SqlStr = QString("DATETIME >= '2020-12-17 00:00:00' and DATETIME <= '2020-12-20 00:00:00'");
    model ->setFilter(SqlStr);//按这个方式查询
    model ->select();
    ui->sql_tableView->setModel(model);//用tableview控件显示查询后的结果
    ui->sql_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif


#if 0
    QSqlQueryModel *model = new QSqlQueryModel(ui->sql_tableView);
    //model->setQuery(QString("DATETIME >= '%1' and DATETIME <= '%2'").arg(StartDateTimeStr).arg(EndDateTimeStr));
    model->setQuery(QString("DATETIME >= '2020-12-17 00:00:00' and DATETIME <= '2020-12-20 00:00:00'"));
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("设备ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("日期时间"));
    model->setHeaderData(2, Qt::Horizontal, tr("LORA地址"));
    model->setHeaderData(3, Qt::Horizontal, tr("LORA信道"));
    model->setHeaderData(4, Qt::Horizontal, tr("湿度"));
    model->setHeaderData(5, Qt::Horizontal, tr("温度"));
    model->setHeaderData(6, Qt::Horizontal, tr("CH4浓度"));

    ui->sql_tableView->setModel(model);//数据放置进去
    ui->sql_tableView->verticalHeader()->hide();//不显示序号
#endif
}

void GasLeakMonitor::sql_test_run()
{
//    sql_timer = new QTimer(this);
//    connect(sql_timer,SIGNAL(timeout()),this,SLOT(sql_test()));
//    sql_timer->start(1000);

    msqltestthread = new MysqltestThread(this);
    msqltestthread->setObjectName("msqlthread");
    connect(msqltestthread,&MysqltestThread::send,this,&GasLeakMonitor::sql_test);
    msqltestthread->start();
}

void GasLeakMonitor::sys_test_run()
{
    msystestthread = new MySysTestThread(this);
    msystestthread->setObjectName("msystestthread");
    connect(msystestthread,&MySysTestThread::send,this,&GasLeakMonitor::send_check);
    msystestthread->start();
}

//发送
void GasLeakMonitor::send_check()
{
    char*  ch;
    QString sendData = "check\r\n";
    QByteArray ba = sendData.toLatin1();
    ch=ba.data();
    myserial_1.write(ch,sendData.count());
}


void GasLeakMonitor::sql_test()
{
#if 0
    sqlInsert = QString("insert into Record(DeviceID,DATETIME,LORA_ADDR,LORA_CHN,HUMIDITY,temperature,CH4_concentration) "
                                "values('%1','%2','%3','%4' ,'%5','%6','%7')")
            .arg(IDstr).arg(DT).arg("10").arg("23").arg(humistr).arg(tempstr).arg(CH4str);
    qDebug()<<sqlInsert;
   if(!sqlQuery.exec(sqlInsert))
    {
        qDebug() << "Error:Failed to SELECT record"<< database.lastError();
    }else
    {
        qDebug() << "success";
    }
#endif

#if 1
    QString DT = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    // 设置随机数种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int IDint = qrand() % 3;
    QString IDstr = QString::number(IDint);
    int humiint = qrand() % 100;
    QString humistr = QString::number(humiint);
    int tempint = (qrand() % 10)+20;
    QString tempstr = QString::number(tempint);
    int CH4int = qrand() % 9000; //产生十以内的随机数(0-9)
    QString CH4str = QString::number(CH4int);

    qDebug() <<DT<<IDstr<<humistr<<tempstr<<CH4str;
    QSqlQuery query;
    query.prepare("insert into Record(DeviceID,DATETIME,LORA_ADDR,LORA_CHN,HUMIDITY,temperature,CH4_concentration) "
                  "values(?,?,?,?,?,?,?)");
    query.addBindValue(IDstr);
    query.addBindValue(DT);
    query.addBindValue("10");
    query.addBindValue("23");
    query.addBindValue(humistr);
    query.addBindValue(tempstr);
    query.addBindValue(CH4str);
    query.exec();
#endif

    ui->temp_tB_1->setAlignment(Qt::AlignCenter);

    ui->temp_tB_1->setText(tempstr);
    ui->humi_tB_1->setText(humistr);
    ui->CH4_tB_1->setText(CH4str);

    emit humi_plot_signal(humiint,IDint);
    emit temp_plot_signal(tempint,IDint);
    emit CH4_plot_signal(CH4int,IDint);

    count++;
    qDebug() << "count: "<<count;
    if(count >= 20)
    {
        qDebug() << "msqltestthread->quit()";
        disconnect(msqltestthread,&MysqltestThread::send,this,&GasLeakMonitor::sql_test);
        msqltestthread->exit(0);
//        disconnect(sql_timer,SIGNAL(timeout()),this,SLOT(sql_test()));
    }
}



