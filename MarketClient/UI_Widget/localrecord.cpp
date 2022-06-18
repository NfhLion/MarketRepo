#include "localrecord.h"
#include "ui_localrecord.h"


#include "control.h"
#include "msgsocket.h"
#include "config.h"
#include "execsql.h"
#include "globalvariable.h"
#include <QScrollBar>

LocalRecord::LocalRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalRecord)
{
    ui->setupUi(this);

    ui->pb_select->setIcon(QPixmap(":/ico/select.png"));
    ui->pb_break->setIcon(QPixmap(":/ico/shuaxin.png"));

    //设置纵向滚动条
    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background-color:rgb(218,222,223); width:10px;}"
                                                         "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");

    connect(this, &LocalRecord::SignalCanelShipmentInfo, this, &LocalRecord::SlotCanelShipmentInfo);
    connect(this, &LocalRecord::SignalConfirmShipment, this, &LocalRecord::SlotConfirmShipment);
}

LocalRecord::~LocalRecord()
{
    delete ui;
}

///根据自己的ID获取自己的货单
void LocalRecord::GetShipmentForID()
{
    QJsonObject json;
    json.insert("type", CMD_GetShipmentForID_O);
    json.insert("id", Config::m_strUserName);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

///设置自己的货单
void LocalRecord::SetShipmentForID(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonArray array = value.toArray();
        if(array.isEmpty()){
            return ;
        }
        QJsonArray com_array = GlobalVariable::g_allCommodityArray;
        m_array = array;

        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(9);

        // set header lables
        QStringList headers;
        headers << "调货单号" << "申请方" << "发货方" << "申请商品" << "申请数" << "时间" << "状态" << "操作" << "操作";

        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->setRowCount(array.size());
//        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
        //设置这几行自适应宽度
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
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


            QPushButton *pBq = new QPushButton();
            pBq->setText("取消调货");
            if(obj.value("shipment_state").toString() == "审核中"){
                pBq->setEnabled(true);
                connect(pBq, &QPushButton::clicked, [=]{
                    emit SignalCanelShipmentInfo(obj.value("shipment_id").toString(),
                                                 obj.value("shipment_fid").toString());
                });
            }else{
                pBq->setEnabled(false);
            }
            // 在QTableWidget中添加控件
            ui->tableWidget->setCellWidget(i,7,pBq);

            QPushButton *pBs = new QPushButton();
            pBs->setText("确认收到");
            if(obj.value("shipment_state").toString() == "调货中"){
                pBs->setEnabled(true);
                connect(pBs, &QPushButton::clicked, [=]{
                    emit SignalConfirmShipment(obj.value("shipment_id").toString(),
                                               obj.value("shipment_qid").toString(),
                                               obj.value("shipment_fid").toString(),
                                               obj.value("shipment_gid").toString(),
                                               obj.value("shipment_num").toString());
                });
            }else{
                pBs->setEnabled(false);
            }
            // 在QTableWidget中添加控件
            ui->tableWidget->setCellWidget(i,8,pBs);
        }
    }

}

void LocalRecord::SlotCanelShipmentInfo(const QString ship_id, const QString ship_fid)
{
    QJsonObject json;
    json.insert("type", CMD_CanelShipment_S);
    json.insert("ship_id", ship_id);
    json.insert("ship_fid",ship_fid);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

void LocalRecord::SlotConfirmShipment(const QString ship_id, const QString ship_qid, const QString ship_fid,
                                      const QString ship_comid, const QString ship_comnum)
{
    QJsonObject json;
    json.insert("type", CMD_ConfirmShipment_T);
    json.insert("ship_id", ship_id);
    json.insert("ship_qid",ship_qid);
    json.insert("ship_fid",ship_fid);
    json.insert("ship_comid", ship_comid);
    json.insert("ship_comnum",ship_comnum);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

void LocalRecord::on_pb_break_clicked()
{
    GetShipmentForID();
}


