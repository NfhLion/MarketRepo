#include "salefront.h"
#include "ui_salefront.h"

#include "control.h"
#include "msgsocket.h"
#include "config.h"
#include "execsql.h"
#include "globalvariable.h"

#include <QMessageBox>
#include <QTime>
#include <QScrollBar>

SaleFront::SaleFront(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaleFront)
{
    ui->setupUi(this);
    ui->pb_back->setIcon(QPixmap(":/ico/back.png"));
    ui->pb_cselect->setIcon(QPixmap(":/ico/select.png"));
    ui->pb_mselect->setIcon(QPixmap(":/ico/select.png"));
    ui->pb_clear->setIcon(QPixmap(":/ico/zhuxiao.jpg"));
    ui->pb_ok->setIcon(QPixmap(":/ico/save.png"));

    m_pay.clear();
    m_pay = "现金支付";
    ui->pb_ok->setEnabled(false);
    ui->pb_finish->setEnabled(false);
    ui->lb_shead->setPixmap(QPixmap::fromImage(QImage(":/image/pay_0.png")));

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(9);

    // set header lables
    QStringList headers;
    headers << "商品编号" << "商品名称" << "售价" << "数量" << "折扣" << "会员等级" << "积分" << "合计" << "支付方式";

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
}

SaleFront::~SaleFront()
{
    delete ui;
}

void SaleFront::on_pb_mselect_clicked()
{
    QString str = ui->le_midphone->text();
    if(str.isEmpty()){
        QMessageBox::information(this, "会员搜索", "搜索属性不能为空，请输入会员搜索属性！");
        return;
    }
    QJsonObject json;
    json.insert("type", CMD_SaleSearchMember);
    json.insert("str", str);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
    return ;
}

///设置查询会员的信息
void SaleFront::SetMemberInfo(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonObject obj = value.toObject();
        m_member = obj;
        ui->lb_total->setText(m_member.value("total").toString());
        ui->lb_grade->setText(m_member.value("grade").toString());
    }
    if(m_member.isEmpty()){
        QMessageBox::information(this, "会员搜索", "没有找到此用户！");
    }
    return;
}

void SaleFront::on_pb_cselect_clicked()
{
    QString str = ui->le_cidname->text();
    if(str.isEmpty()){
        QMessageBox::information(this, "商品搜索", "搜索属性不能为空，请输入商品搜索属性！");
        return;
    }

    for(int i = 0; i < GlobalVariable::g_allCommodityArray.size(); i++){
        QJsonObject obj = GlobalVariable::g_allCommodityArray.at(i).toObject();
        if(str == obj.value("goods_id").toString() || str == obj.value("goods_name").toString()){
            m_goods = obj;
            break;
        }
    }
    if(m_goods.isEmpty()){
        QMessageBox::information(this, "商品搜索", "没有找到此商品！");
        return ;
    }

    QString sale = QString("售价:%1元").arg(m_goods.value("goods_new_price").toString());
    QString old  = QString("原价:%1元").arg(m_goods.value("goods_old_price").toString());
    ui->le_sale->setText(sale);
    ui->le_old->setText(old);

    ui->pb_ok->setEnabled(true);
    return;
}

void SaleFront::on_pb_ok_clicked()
{
    if(m_goods.isEmpty()){
        QMessageBox::information(this, "商品搜索", "请查找商品！");
        return;
    }
    QString num = ui->le_num->text();
    if(num.isEmpty()){
        QMessageBox::information(this, "商品数量", "请输入商品数量！");
        return;
    }
    if(m_pay.isEmpty()){
        QMessageBox::information(this, "支付方式", "请选择支付方式！");
        return;
    }

    //会员等级，折扣
    QString grade = ui->lb_grade->text();
    QString discount;
    if(grade == "无"){
        discount = "1";
    }else{
        discount = m_member.value("discount").toString();
    }

    int num_int = num.toInt();
    double discount_num = discount.toDouble();
    int price = m_goods.value("goods_new_price").toString().toInt();
    int all_price = num_int * price * discount_num;
    int all_total;
    if(grade == "无"){
        all_total = 0;
    }else if(grade == "白金会员"){
        all_total = 1000 * num_int;
    }else if(grade == "钻石会员"){
        all_total = 1500 * num_int;
    }

    QTableWidgetItem *item = new QTableWidgetItem(m_goods.value("goods_id").toString());
    ui->tableWidget->setItem(0, 0, item);
    item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
    item = new QTableWidgetItem(m_goods.value("goods_name").toString());
    item->setTextAlignment(Qt::AlignCenter);    //文字居中显示
    ui->tableWidget->setItem(0, 1, item);
    item = new QTableWidgetItem(m_goods.value("goods_new_price").toString());
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 2, item);
    item = new QTableWidgetItem(num);
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 3, item);
    item = new QTableWidgetItem(discount);
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 4, item);
    item = new QTableWidgetItem(grade);
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 5, item);
    item = new QTableWidgetItem(QString::number(all_total));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 6, item);
    item = new QTableWidgetItem(QString::number(all_price));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 7, item);
    item = new QTableWidgetItem(m_pay);
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 8, item);


    ui->le_cidname->setEnabled(false);
    ui->le_midphone->setEnabled(false);
    ui->le_num->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->pb_ok->setEnabled(false);
    ui->pb_finish->setEnabled(true);
}

void SaleFront::on_comboBox_currentIndexChanged(int index)
{
    if(index == PAY_0){
        m_pay = "现金支付";
        ui->lb_shead->setPixmap(QPixmap::fromImage(QImage(":/image/pay_0.png")));
    }else if(index == PAY_1){
        m_pay = "微信支付";
        ui->lb_shead->setPixmap(QPixmap::fromImage(QImage(":/image/pay_1.png")));
    }else{
        m_pay = "支付宝支付";
        ui->lb_shead->setPixmap(QPixmap::fromImage(QImage(":/image/pay_2.png")));
    }
}

void SaleFront::on_pb_clear_clicked()
{
    m_pay.clear();
    m_pay = "现金支付";

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(9);

    // set header lables
    QStringList headers;
    headers << "商品编号" << "商品名称" << "售价" << "数量" << "折扣" << "会员等级" << "积分" << "合计" << "支付方式";

    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式

    ui->le_cidname->setEnabled(true);
    ui->le_midphone->setEnabled(true);
    ui->le_num->setEnabled(true);
    ui->comboBox->setEnabled(true);

    QString sale = QString("售价:%1元").arg(QString("0.00"));
    QString old  = QString("原价:%1元").arg(QString("0.00"));
    ui->le_sale->setText(sale);
    ui->le_old->setText(old);
    ui->lb_total->setText("0");
    ui->lb_grade->setText("无");
    ui->lb_shead->setPixmap(QPixmap::fromImage(QImage(":/image/pay_0.png")));

    ui->le_cidname->clear();
    ui->le_midphone->clear();
    ui->le_num->clear();
    ui->pb_finish->setEnabled(false);
}

///结算
void SaleFront::on_pb_finish_clicked()
{
    QString comid = ui->tableWidget->item(0, 0)->text();
    QString cliid = "XIANXIA";
    QString shid = Config::m_strUserName;
    QString comnum = ui->tableWidget->item(0, 3)->text();
    QString price = ui->tableWidget->item(0, 7)->text();
    QString state = "已支付";
    QString time;
    QTime timeNow = QTime::currentTime();
    time = (QString)((QDate::currentDate().toString(tr("yyyy-MM-dd")))
                           + tr(" %1").arg(timeNow.toString()));

    QString mode = m_pay;
    QString value = "线下";
    QString mem_id, mem_total, mem_price;
    if(!m_member.isEmpty()){
        mem_id = m_member.value("id").toString();
        int mtotal = m_member.value("total").toString().toInt();
        int jtotal = ui->tableWidget->item(0, 6)->text().toInt();
        mem_total =  QString::number(mtotal + jtotal);

        int mprice = m_member.value("price").toString().toInt();
        int jprice = ui->tableWidget->item(0, 7)->text().toInt();
        mem_price =  QString::number(mprice + jprice);
    }else{
        mem_id = "null";
        mem_total = "null";
    }

    QJsonObject json;
    json.insert("type", CMD_FinishSale);
    json.insert("gid", comid);
    json.insert("cid", cliid);
    json.insert("mid", shid);
    json.insert("num", comnum);
    json.insert("price", price);
    json.insert("state", state);
    json.insert("time", time);
    json.insert("mode", mode);
    json.insert("value", value);
    json.insert("mem_id", mem_id);
    json.insert("total", mem_total);
    json.insert("mem_price", mem_price);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

///结算完成后
void SaleFront::SetFinishSale(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonObject obj = value.toObject();
        QString comid = ui->tableWidget->item(0, 0)->text();
        QString cliid = "XIANXIA";
        QString shid = Config::m_strUserName;
        QString comnum = ui->tableWidget->item(0, 3)->text();
        QString price = ui->tableWidget->item(0, 7)->text();
        QString state = "已支付";
        QString time;
        QTime timeNow = QTime::currentTime();
        time = (QString)((QDate::currentDate().toString(tr("yyyy-MM-dd")))
                               + tr(" %1").arg(timeNow.toString()));

        QString mode = m_pay;
        QString value = "线下";
        QString mem_id, mem_total, mem_price;
        if(obj.value("mem_id").toString() != "null"){
            mem_id = m_member.value("id").toString();
            int mtotal = m_member.value("total").toString().toInt();
            int jtotal = ui->tableWidget->item(0, 6)->text().toInt();
            mem_total =  QString::number(mtotal + jtotal);

            int mprice = m_member.value("price").toString().toInt();
            int jprice = ui->tableWidget->item(0, 7)->text().toInt();
            mem_price =  QString::number(mprice + jprice);

            //更新本地会员
            ExecSql::GetExecSql()->UpdateMember(mem_id, mem_total, mem_price);
        }

        ExecSql::GetExecSql()->InsertForm(obj.value("id").toString(), comid, cliid, shid, comnum, price, state, time, mode, value);
        QJsonObject comobj = ExecSql::GetExecSql()->SearchCommodity(comid, shid);
        if(!comobj.isEmpty()){
            QString nsale, nresidue;
            nsale = QString::number(comobj.value("sale_num").toString().toInt() + comnum.toInt());
            nresidue = QString::number(comobj.value("residue_num").toString().toInt() - comnum.toInt());
            ExecSql::GetExecSql()->UpdateCommodity(comid, shid, nsale, nresidue);
        }
        QMessageBox::information(this, "订单结算", "订单结算成功！");
        return;
    }
    QMessageBox::information(this, "订单结算", "订单结算失败！");
}
