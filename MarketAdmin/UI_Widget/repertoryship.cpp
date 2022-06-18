#include "repertoryship.h"
#include "ui_repertoryship.h"

#include "control.h"
#include "msgsocket.h"
#include "config.h"
#include "execsql.h"
#include "globalvariable.h"
#include <QPushButton>
#include <QList>
#include <QMessageBox>
#include <QScrollBar>

RepertoryShip::RepertoryShip(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepertoryShip)
{
    ui->setupUi(this);


    connect(this, &RepertoryShip::SignalDealShipmentInfo,
            this, &RepertoryShip::SlotDealShipmentInfo);

    ui->pb_select->setIcon(QPixmap(":/ico/select.png"));
    ui->pb_break->setIcon(QPixmap(":/ico/shuaxin.png"));
    ui->pb_ack->setIcon(QPixmap(":/ico/back.png"));
    ui->pb_analyze->setIcon(QPixmap(":/ico/shuaxin.png"));
    ui->pb_dhok->setIcon(QPixmap(":/ico/save.png"));
    ui->pb_dhcancel->setIcon(QPixmap(":/ico/delete.png"));

    //设置横向滚动条
    ui->tableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                          "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                          "QScrollBar::handle:hover{background:gray;}"
                                                          "QScrollBar::sub-line{background:transparent;}"
                                                          "QScrollBar::add-line{background:transparent;}");

    //设置纵向滚动条
    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background-color:rgb(218,222,223); width:10px;}"
                                                         "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");

    //设置横向滚动条
    ui->tableWidget_dh->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                          "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                          "QScrollBar::handle:hover{background:gray;}"
                                                          "QScrollBar::sub-line{background:transparent;}"
                                                          "QScrollBar::add-line{background:transparent;}");

    //设置纵向滚动条
    ui->tableWidget_dh->verticalScrollBar()->setStyleSheet("QScrollBar{background-color:rgb(218,222,223); width:10px;}"
                                                         "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");
}

RepertoryShip::~RepertoryShip()
{
    delete ui;
}

///远程获取所有的调货信息
void RepertoryShip::GetRepertoryShipment()
{
    QJsonObject json;
    json.insert("type", CMD_GetRepertoryShip_M);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

///设置
void RepertoryShip::SetRepertoryShipment(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonArray array = value.toArray();
        QJsonArray com_array = GlobalVariable::g_allCommodityArray;
        m_array = array;

        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(8);

        // set header lables
        QStringList headers;
        headers << "调货单号" << "申请方" << "发货方" << "申请商品" << "申请数" << "时间" << "状态" << "操作";

        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->setRowCount(array.size());
//        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
        //设置这两行自适应宽度
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式

        for(int i = 0; i < array.size(); i++){
            QJsonObject obj = array.at(i).toObject();
            QJsonObject com_obj;
            for(int j = 0; j < com_array.size(); j++){
                com_obj = com_array.at(j).toObject();
                if(obj.value("shipment_gid").toString() == com_obj.value("goods_id").toString()){
                    break;
                }
            }
            QTableWidgetItem *item = new QTableWidgetItem(obj.value("shipment_id").toString());
            ui->tableWidget->setItem(i, 0, item);
            item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
            item = new QTableWidgetItem(obj.value("shipment_qname").toString());
            item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
            ui->tableWidget->setItem(i, 1, item);
            item = new QTableWidgetItem(obj.value("shipment_fname").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, 2, item);
            item = new QTableWidgetItem(com_obj.value("goods_name").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, 3, item);
            item = new QTableWidgetItem(obj.value("shipment_num").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, 4, item);
            item = new QTableWidgetItem(obj.value("shipment_time").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, 5, item);
            item = new QTableWidgetItem(obj.value("shipment_state").toString());
            if(obj.value("shipment_state").toString() == "审核中"){
                item->setForeground(QBrush(QColor(255, 0, 0)));
            }else if(obj.value("shipment_state").toString() == "调货中"){
                item->setForeground(QBrush(QColor(255, 255, 0)));
            }else if(obj.value("shipment_state").toString() == "已取消"){
                item->setForeground(QBrush(QColor(195, 195, 195)));
            }else if(obj.value("shipment_state").toString() == "已收到"){
                item->setForeground(QBrush(QColor(0, 255, 0)));
            }
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, 6, item);


            QPushButton *pBtn = new QPushButton();
            pBtn->setText("处理");
            if(obj.value("shipment_state").toString() == "审核中"){
                pBtn->setEnabled(true);
                connect(pBtn, &QPushButton::clicked, [=]{
                    emit SignalDealShipmentInfo(obj.value("shipment_id").toString(),
                                                com_obj.value("goods_name").toString());
                });
            }else{
                pBtn->setEnabled(false);
            }
            // 在QTableWidget中添加控件
            ui->tableWidget->setCellWidget(i,7,pBtn);
        }
    }
}


///设置处理界面的信息
void RepertoryShip::SlotDealShipmentInfo(const QString &id, const QString &goods_name)
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->le_dhsite->clear();
    ui->lb_dhid->setText(id);
    ui->lb_spname->setText(goods_name);

    QJsonObject obj;
    for(int i = 0; i < m_array.size(); i++){
        obj = m_array.at(i).toObject();
        if(id == obj.value("shipment_id").toString()){
            break;
        }
    }
    ui->lb_sdid->setText(obj.value("shipment_qid").toString());
    ui->lb_sdname->setText(obj.value("shipment_qname").toString());
    ui->lb_spnum->setText(obj.value("shipment_num").toString());

    QJsonArray shop_array = ExecSql::GetExecSql()->SelectAllShopFromMarketInfo();
    for(int i = 0; i <shop_array.size(); i++){
        if(obj.value("shipment_qname").toString()
                == shop_array.at(i).toObject().value("name").toString()){
            ui->lb_sdsite->setText(shop_array.at(i).toObject().value("site").toString());
            break;
        }
    }

    //组装新的json
    QJsonObject json;
    json.insert("type", CMD_GetCommodityForID_N);
    json.insert("com_id", obj.value("shipment_gid").toString());
    json.insert("com_num", obj.value("shipment_num").toString());
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

///
void RepertoryShip::SetCommodityTableWidget(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonArray array = value.toArray();
        m_comarray = array;

        ui->tableWidget_dh->clear();
        ui->tableWidget_dh->setColumnCount(6);

        // set header lables
        QStringList headers;
        headers << "商品名称" << "调货地" << "调货地地址" << "总数" << "调货数" << "剩余数";

        ui->tableWidget_dh->setHorizontalHeaderLabels(headers);
        ui->tableWidget_dh->setRowCount(array.size());
        ui->tableWidget_dh->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
        //设置这两行自适应宽度
        ui->tableWidget_dh->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->tableWidget_dh->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        ui->tableWidget_dh->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式

        QJsonObject com_obj;
        QJsonArray com_array = GlobalVariable::g_allCommodityArray;
        for(int i = 0; i < array.size(); i++){
            QJsonObject obj = array.at(i).toObject();

            for(int j = 0; j < com_array.size(); j++){
                com_obj = com_array.at(j).toObject();

                if(obj.value("com_id").toString() == com_obj.value("goods_id").toString()){
                    break;
                }
            }


            QTableWidgetItem *item = new QTableWidgetItem(com_obj.value("goods_name").toString());
            ui->tableWidget_dh->setItem(i, 0, item);
            item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
            item = new QTableWidgetItem(obj.value("com_mname").toString());
            item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
            ui->tableWidget_dh->setItem(i, 1, item);
            item = new QTableWidgetItem(obj.value("com_msite").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_dh->setItem(i, 2, item);
            item = new QTableWidgetItem(obj.value("com_all").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_dh->setItem(i, 3, item);
            item = new QTableWidgetItem(obj.value("com_shipment").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_dh->setItem(i, 4, item);
            item = new QTableWidgetItem(obj.value("com_residue").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_dh->setItem(i, 5, item);
        }
    }
}

void RepertoryShip::on_pb_ack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

///分析最优调货地,暂且不用选择框
struct ShopValue{
    QString name;
    int num_all;
    int num_shipment;
    int num_residue;
};
struct ShopSort{//时间原因先用最简单的选择，这里是后期优化的地方
    bool operator()(ShopValue &shv1, ShopValue &shv2)
    {
        return shv1.num_residue > shv2.num_residue;
    }
};

void RepertoryShip::on_pb_analyze_clicked()
{

    QList<ShopValue> shopValueList;
    QJsonObject obj;
    for(int i = 0; i < m_comarray.size(); i++){
        obj = m_comarray.at(i).toObject();
        ShopValue shv;
        shv.name = obj.value("com_mname").toString();
        shv.num_all = obj.value("com_all").toString().toInt();
        shv.num_shipment = obj.value("com_shipment").toString().toInt();
        shv.num_residue = obj.value("com_residue").toString().toInt();
        shopValueList.append(shv);
    }
    qSort(shopValueList.begin(), shopValueList.end(), ShopSort());
    QList<ShopValue>::iterator it = shopValueList.begin();
    ui->le_dhsite->setText(it->name);
}

void RepertoryShip::on_pb_dhcancel_clicked()
{
    ui->le_dhsite->clear();
}

///确认调货
void RepertoryShip::on_pb_dhok_clicked()
{
    if(QString(ui->le_dhsite->text()).isEmpty()){
        QMessageBox::information(this, "确认调货", "请先选择最优调货地并填写！");
        return ;
    }

    QString ship_id = ui->lb_dhid->text();
    QString ship_qid = ui->lb_sdid->text();
    QString ship_fid;
    QString fid_shipment;
    QString fid_residue;
    QString ship_comid;
    QString ship_comnum = ui->lb_spnum->text();
    for(int i = 0; i < m_comarray.size(); i++){
        QJsonObject obj = m_comarray.at(i).toObject();
        if(ui->le_dhsite->text() == obj.value("com_mname").toString()){
            ship_fid = obj.value("com_mid").toString();
            fid_shipment = QString("%1").arg(obj.value("com_shipment").toString().toInt() + ship_comnum.toInt());
            fid_residue = QString("%1").arg(obj.value("com_residue").toString().toInt() - ship_comnum.toInt());
            break;
        }
    }
    for(int i = 0; i < GlobalVariable::g_allCommodityArray.size(); i++){
        QJsonObject obj = GlobalVariable::g_allCommodityArray.at(i).toObject();
        if(ui->lb_spname->text() == obj.value("goods_name").toString()){
            ship_comid = obj.value("goods_id").toString();
            break;
        }
    }


    QJsonObject json;
    json.insert("type", CMD_OKUpdateShipment_Q);
    json.insert("ship_id", ship_id);
    json.insert("ship_qid", ship_qid);
    json.insert("ship_fid", ship_fid);
    json.insert("fid_shipment", fid_shipment);
    json.insert("fid_residue", fid_residue);
    json.insert("ship_comid", ship_comid);
    json.insert("ship_comnum", ship_comnum);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
    return ;
}
