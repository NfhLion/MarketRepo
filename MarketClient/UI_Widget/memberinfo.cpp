#include "memberinfo.h"
#include "ui_memberinfo.h"

#include "control.h"
#include "msgsocket.h"
#include "config.h"
#include "execsql.h"
#include "globalvariable.h"

#include <QMessageBox>
#include <QScrollBar>

MemberInfo::MemberInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemberInfo)
{
    ui->setupUi(this);

    ui->pb_break->setIcon(QPixmap(":/ico/shuaxin.png"));
    ui->pb_select->setIcon(QPixmap(":/ico/select.png"));
    ui->pb_yselect->setIcon(QPixmap(":/ico/select.png"));

    //设置纵向滚动条
    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background-color:rgb(218,222,223); width:10px;}"
                                                         "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");
}

MemberInfo::~MemberInfo()
{
    delete ui;
}

///获取会员信息
void MemberInfo::GetMemberInfo()
{

    if(m_array.isEmpty()){
        m_array = ExecSql::GetExecSql()->SearchMember(Config::m_strUserName);
        if(m_array.isEmpty()){
            QJsonObject json;
            json.insert("type", CMD_GetMyMemmber);
            json.insert("shop_id", Config::m_strUserName);
            QJsonDocument doc(json);
            QByteArray data = doc.toJson();
            MsgSocket::GetMsgSocket()->SendMessage(data);
        }else{
            SetMemberTableWidget();
        }
    }
}

///设置本地会员信息
void MemberInfo::SetMemberInfo(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonArray array = value.toArray();
        if(array.isEmpty()){
            return ;
        }

        if(m_array.isEmpty()){
            for(int i = 0; i < array.size(); i++){
                QJsonObject obj = array.at(i).toObject();
                ExecSql::GetExecSql()->InsertMember(obj.value("id").toString(), obj.value("name").toString(), obj.value("phone").toString(),
                                                    obj.value("shopid").toString(), obj.value("time").toString(), obj.value("grade").toString(),
                                                    obj.value("total").toString(), obj.value("price").toString(), obj.value("discount").toString());
            }
        }

        m_array = array;
        SetMemberTableWidget();
    }
}

void MemberInfo::SetMemberTableWidget()
{
    QJsonArray array = m_array;
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(9);

    // set header lables
    QStringList headers;
    headers << "会员卡号" << "会员名" << "会员电话" << "申请地" << "申请时间" << "会员等级" << "积分" << "消费" << "折扣";

    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setRowCount(array.size());

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
    //设置这几行自适应宽度
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式

    for(int i = 0; i < array.size(); i++){
        QJsonObject obj = array.at(i).toObject();

        QTableWidgetItem *item = new QTableWidgetItem(obj.value("id").toString());
        ui->tableWidget->setItem(i, 0, item);
        item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
        item = new QTableWidgetItem(obj.value("name").toString());
        item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
        ui->tableWidget->setItem(i, 1, item);
        item = new QTableWidgetItem(obj.value("phone").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 2, item);
        item = new QTableWidgetItem(obj.value("shopid").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 3, item);
        item = new QTableWidgetItem(obj.value("time").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 4, item);
        item = new QTableWidgetItem(obj.value("grade").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 5, item);
        item = new QTableWidgetItem(obj.value("total").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 6, item);
        item = new QTableWidgetItem(obj.value("price").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 7, item);
        item = new QTableWidgetItem(obj.value("discount").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 8, item);

    }
}

///远程查询会员信息
void MemberInfo::SetSearchMember(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonObject obj = value.toObject();
        if(obj.isEmpty()){
            QMessageBox::information(this, "远程查询", "没有该会员用户的信息！");
            return;
        }else{
            ui->tableWidget->clear();
            ui->tableWidget->setColumnCount(9);

            // set header lables
            QStringList headers;
            headers << "会员卡号" << "会员名" << "会员电话" << "申请地" << "申请时间" << "会员等级" << "积分" << "消费" << "折扣";

            ui->tableWidget->setHorizontalHeaderLabels(headers);
            ui->tableWidget->setRowCount(1);

            ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
            //设置这几行自适应宽度
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

            ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式

            QTableWidgetItem *item = new QTableWidgetItem(obj.value("id").toString());
            ui->tableWidget->setItem(0, 0, item);
            item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
            item = new QTableWidgetItem(obj.value("name").toString());
            item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
            ui->tableWidget->setItem(0, 1, item);
            item = new QTableWidgetItem(obj.value("phone").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0, 2, item);
            item = new QTableWidgetItem(obj.value("shopid").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0, 3, item);
            item = new QTableWidgetItem(obj.value("time").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0, 4, item);
            item = new QTableWidgetItem(obj.value("grade").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0, 5, item);
            item = new QTableWidgetItem(obj.value("total").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0, 6, item);
            item = new QTableWidgetItem(obj.value("price").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0, 7, item);
            item = new QTableWidgetItem(obj.value("discount").toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0, 8, item);
        }
    }
}

void MemberInfo::on_pb_break_clicked()
{
    m_array = ExecSql::GetExecSql()->SearchMember(Config::m_strUserName);
    SetMemberTableWidget();
}

///远程搜索
void MemberInfo::on_pb_yselect_clicked()
{
    QString str = ui->le_yselect->text();
    if(str.isEmpty()){
        QMessageBox::information(this, "远程搜索", "搜索属性不能为空，请输入远程搜索属性！");
        return;
    }
    QJsonObject json;
    json.insert("type", CMD_SearchMember);
    json.insert("str", str);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}
