#include "homecommodity.h"
#include "ui_homecommodity.h"

#include "globalvariable.h"
#include "filesocket.h"
#include "config.h"

#include <QMessageBox>
#include <QImage>
#include <QScrollBar>

HomeCommodity::HomeCommodity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeCommodity)
{
    ui->setupUi(this);
    m_x = 0;
    m_searchCond = Search_None;
    m_homeItemList = new HomeItemList;

    ui->pb_select->setIcon(QPixmap(":/ico/select.png"));
    ui->pb_break->setIcon(QPixmap(":/ico/shuaxin.png"));
    ui->pb_ack->setIcon(QPixmap(":/ico/back.png"));
    ui->pb_ok->setIcon(QPixmap(":/ico/shenqing.png"));

//    ui->tableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
//                                                          "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
//                                                          "QScrollBar::handle:hover{background:gray;}"
//                                                          "QScrollBar::sub-line{background:transparent;}"
//                                                          "QScrollBar::add-line{background:transparent;}");

    //设置纵向滚动条
     ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background-color:rgb(218,222,223); width:10px;}"
                                                         "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");
}

HomeCommodity::~HomeCommodity()
{
    delete m_homeItemList;
    m_homeItemList = NULL;
    delete ui;
}

///远程获取商家所有商品信息
void HomeCommodity::GetAllCommodityInfo()
{
    QJsonObject json;
    json.insert("type", CMD_AllCommodity_D);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    MsgSocket::GetMsgSocket()->SendMessage(data);
}

///判断并设置商品value
void HomeCommodity::SetAllCommodityInfo(const int &result_cmd, const QJsonValue &value)
{
    if(result_cmd == CMD_Sucess_Y){

        m_array = value.toArray();
        GlobalVariable::g_allCommodityArray = m_array;
        int k = 0;
        ui->tableWidget->clear();
        GlobalVariable::g_compathMap.clear();
        GlobalVariable::g_comimageMap.clear();
        m_homeItemList->clear();
        ui->tableWidget->setRowCount(m_array.size() / 5 + 1);
        ui->tableWidget->setColumnCount(5);
        for(int i = 0 ; i < ui->tableWidget->rowCount(); i++)
        {
            for(int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                if(k < m_array.size())
                {
                    QJsonObject node = m_array.at(k).toObject();

                    HomeItem *hitem = new HomeItem;
                    hitem->SetId(node.value("goods_id").toString());
                    hitem->SetName(node.value("goods_name").toString());
                    hitem->SetOldPrice(node.value("goods_old_price").toString());
                    hitem->SetNewPrice(node.value("goods_new_price").toString());
                    hitem->SetCallNum(node.value("goods_call_num").toString());
                    hitem->SetNumber(k + 1);
                    m_homeItemList->append(hitem);       ///这里注意必须要用append,否则报错
                    GlobalVariable::g_compathMap.insert(node.value("goods_id").toString(),
                                                        node.value("goods_head").toString());
                    GlobalVariable::g_comimageMap.insert(node.value("goods_id").toString(), QImage());

                    ui->tableWidget->setCellWidget(i, j, hitem);

                    connect(hitem, &HomeItem::SignalAllCommodityInfo,
                            this, &HomeCommodity::SlotAllCommodity);

                    k++;
                }
            }
        }
        GetAllCommodityImage();


    }else{

    }
}

///远程获取商家所有商品的图片
void HomeCommodity::GetAllCommodityImage()
{
    if(m_x < m_array.size()){
        QJsonObject node = m_array.at(m_x).toObject();
        QString id = node.value("goods_id").toString();
        QString path = node.value("goods_head").toString();
        qDebug() << "GetAllCommodityImage---" << id << path;

        QJsonObject json;
        json.insert("type", CMD_UploadFile_G);
        json.insert("com_id", id);
        json.insert("path", path);
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();
        FileSocket::GetFileSocket()->SendMessage(data);
    }else{
        m_x = 0;
        SetAllCommidityItem();
    }
    return ;
}

///设置商品图片
void HomeCommodity::SetCommodityImage()
{
    HomeItem *item = m_homeItemList->at(m_x);
    QString id = m_homeItemList->at(m_x)->GetId();
    QString fileName = GlobalVariable::g_compathMap[id];
    QImage image(fileName);
    item->SetHead(image);

    GlobalVariable::g_comimageMap[id] = image;

    m_x++;
    GetAllCommodityImage();

    return ;
}

///设置所有商品的Item
void HomeCommodity::SetAllCommidityItem()
{
    //设置商品信息item
    int k = 0;
    ui->tableWidget->setRowCount(m_array.size() / 5 + 1);
    ui->tableWidget->setColumnCount(5);

    for(int i = 0 ; i < ui->tableWidget->rowCount(); i++)
    {
        for(int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            if(k < m_array.size())
            {
                QJsonObject node = m_array.at(k).toObject();
                HomeItem *hitem = new HomeItem;
                hitem->SetId(node.value("goods_id").toString());
                hitem->SetName(node.value("goods_name").toString());
                hitem->SetOldPrice(node.value("goods_old_price").toString());
                hitem->SetNewPrice(node.value("goods_new_price").toString());
                hitem->SetCallNum(node.value("goods_call_num").toString());
                hitem->SetNumber(k + 1);
                hitem->SetHead(GlobalVariable::g_comimageMap[node.value("goods_id").toString()]);
                ui->tableWidget->setCellWidget(i, j, hitem);

                connect(hitem, &HomeItem::SignalAllCommodityInfo,
                        this, &HomeCommodity::SlotAllCommodity);

                k++;
            }
        }
    }
}

///刷新页面
void HomeCommodity::on_pb_break_clicked()
{
    ui->tableWidget->clear();
    m_homeItemList->clear();

    GlobalVariable::g_compathMap.clear();
    GlobalVariable::g_comimageMap.clear();
    GetAllCommodityInfo();
}

///显示单个商品具体信息
void HomeCommodity::SlotAllCommodity(int number)
{
    ui->stackedWidget->setCurrentIndex(1);
    QJsonObject obj = m_array.at(number - 1).toObject();
    ui->lb_id->setText(obj.value("goods_id").toString());
    ui->lb_name->setText(obj.value("goods_name").toString());
    ui->lb_old_price->setText(obj.value("goods_old_price").toString());
    ui->lb_new_price->setText(obj.value("goods_new_price").toString());
    ui->lb_call->setText(obj.value("goods_call_num").toString());

    QImage image = GlobalVariable::g_comimageMap[obj.value("goods_id").toString()];
    QSize laSize = ui->lb_head->size();//label_carema是要显示图片的label的名称
    QImage image1=image.scaled(laSize,Qt::KeepAspectRatio);//重新调整图像大小以适应窗口
    ui->lb_head->setPixmap(QPixmap::fromImage(image1));
}

///返回listwidget页面
void HomeCommodity::on_pb_ack_clicked()
{
   ui->stackedWidget->setCurrentIndex(0);
}

///设置查询条件
void HomeCommodity::on_comboBox_currentIndexChanged(int index)
{
    m_searchCond = index;
    if(m_searchCond == Search_None)
    {
        ui->le_select->setEnabled(false);
    }else
    {
        ui->le_select->setEnabled(true);
    }
}

///查询
void HomeCommodity::on_pb_select_clicked()
{
    if(m_searchCond == Search_None){
        ui->tableWidget->clear();
        SetAllCommidityItem();

    }else if(m_searchCond == Search_ID){
        QString id = ui->le_select->text();
        if(id.isEmpty()){
            QMessageBox::information(this, "查询", "请输入查询属性！");
            return ;
        }

        int i = 0;
        QJsonObject node;
        for(i = 0; i < m_array.size(); i++){
            node = m_array.at(i).toObject();
            if(id == node.value("goods_id").toString()){
                break;
            }
        }
        if(i >= m_array.size()){
            QMessageBox::information(this, "查询错误", "您输入的编号不存在，请核对后重新输入！");
            return ;
        }else{
            ui->tableWidget->clear();
            ui->tableWidget->setRowCount(1);
            ui->tableWidget->setColumnCount(1);

            HomeItem *hitem = new HomeItem;
            hitem->SetId(node.value("goods_id").toString());
            hitem->SetName(node.value("goods_name").toString());
            hitem->SetOldPrice(node.value("goods_old_price").toString());
            hitem->SetNewPrice(node.value("goods_new_price").toString());
            hitem->SetCallNum(node.value("goods_call_num").toString());
            hitem->SetNumber(i + 1);
            hitem->SetHead(GlobalVariable::g_comimageMap[node.value("goods_id").toString()]);

            connect(hitem, &HomeItem::SignalAllCommodityInfo,
                    this, &HomeCommodity::SlotAllCommodity);
            ui->tableWidget->setCellWidget(0, 0, hitem);
        }

    }else if(m_searchCond == Search_Name){
        QString name = ui->le_select->text();
        if(name.isEmpty()){
            QMessageBox::information(this, "查询", "请输入查询属性！");
            return ;
        }

        int i = 0;
        QJsonObject node;
        for(i = 0; i < m_array.size(); i++){
            node = m_array.at(i).toObject();
            if(name == node.value("goods_name").toString()){
                break;
            }
        }
        if(i >= m_array.size()){
            QMessageBox::information(this, "查询错误", "您输入的名称不存在，请核对后重新输入！");
            return ;
        }else{
            ui->tableWidget->clear();
            ui->tableWidget->setRowCount(1);
            ui->tableWidget->setColumnCount(1);

            HomeItem *hitem = new HomeItem;
            hitem->SetId(node.value("goods_id").toString());
            hitem->SetName(node.value("goods_name").toString());
            hitem->SetOldPrice(node.value("goods_old_price").toString());
            hitem->SetNewPrice(node.value("goods_new_price").toString());
            hitem->SetCallNum(node.value("goods_call_num").toString());
            hitem->SetNumber(i + 1);
            hitem->SetHead(GlobalVariable::g_comimageMap[node.value("goods_id").toString()]);

            connect(hitem, &HomeItem::SignalAllCommodityInfo,
                    this, &HomeCommodity::SlotAllCommodity);
            ui->tableWidget->setCellWidget(0, 0, hitem);
        }
    }
    return ;
}

///申请调货
void HomeCommodity::on_pb_ok_clicked()
{
    QString id = ui->le_id->text();
    QString num = ui->le_num->text();
    QJsonObject json;
    json.insert("type", CMD_InsertCommodity_P);
    json.insert("s_qid", Config::m_strUserName);
    json.insert("s_fid", QString("MER10001"));
    json.insert("s_comid", id);
    json.insert("s_comnum", num);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}
