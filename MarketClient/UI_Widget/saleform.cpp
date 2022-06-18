#include "saleform.h"
#include "ui_saleform.h"

#include "control.h"
#include "msgsocket.h"
#include "config.h"
#include "execsql.h"
#include "globalvariable.h"

#include <QMessageBox>
#include <QScrollBar>

SaleForm::SaleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaleForm)
{
    ui->setupUi(this);
    ui->pb_break->setIcon(QPixmap(":/ico/shuaxin.png"));
    ui->pb_select->setIcon(QPixmap(":/ico/select.png"));

    m_array = ExecSql::GetExecSql()->SearchForm(Config::m_strUserName);

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(9);

    // set header lables
    QStringList headers;
    headers << "订单编号" << "商品编号" << "商品名称" << "数量" << "合计" << "订单状态" << "时间" << "支付方式" << "种类";

    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式

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
}

SaleForm::~SaleForm()
{
    delete ui;
}

void SaleForm::SetTabelWidget()
{
    QJsonArray array = m_array;
    for(int i = 0; i < array.size(); i++){
        QJsonObject obj = array.at(i).toObject();
        QString name;
        for(int j = 0; j < GlobalVariable::g_allCommodityArray.size(); j++){
            QJsonObject comobj = GlobalVariable::g_allCommodityArray.at(j).toObject();
            if(obj.value("gid").toString() == comobj.value("goods_id").toString()){
                name = comobj.value("goods_name").toString();
                break;
            }
        }

        QTableWidgetItem *item = new QTableWidgetItem(obj.value("id").toString());
        ui->tableWidget->setItem(i, 0, item);
        item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
        item = new QTableWidgetItem(obj.value("gid").toString());
        item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
        ui->tableWidget->setItem(i, 1, item);
        item = new QTableWidgetItem(name);
        item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
        ui->tableWidget->setItem(i, 2, item);
        item = new QTableWidgetItem(obj.value("num").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 3, item);
        item = new QTableWidgetItem(obj.value("price").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 4, item);
        item = new QTableWidgetItem(obj.value("state").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 5, item);
        item = new QTableWidgetItem(obj.value("time").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 6, item);
        item = new QTableWidgetItem(obj.value("mode").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 7, item);
        item = new QTableWidgetItem(obj.value("value").toString());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 8, item);
    }
}

void SaleForm::on_pb_break_clicked()
{
    m_array = ExecSql::GetExecSql()->SearchForm(Config::m_strUserName);
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(9);

    // set header lables
    QStringList headers;
    headers << "订单编号" << "商品编号" << "商品名称" << "数量" << "合计" << "订单状态" << "时间" << "支付方式" << "种类";

    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    SetTabelWidget();
}
