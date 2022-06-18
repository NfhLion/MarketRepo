#include "homecommodity.h"
#include "ui_homecommodity.h"

#include "globalvariable.h"
#include "filesocket.h"

#include <QMessageBox>

HomeCommodity::HomeCommodity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeCommodity)
{
    ui->setupUi(this);
    m_x = 0;
    m_searchCond = Search_None;
    m_homeItemList = new HomeItemList;

    GetAllCommodityInfo();
}

HomeCommodity::~HomeCommodity()
{
    for(int i = 0; i < m_homeItemList->length(); i++){
        if(m_homeItemList->at(i) != NULL){
            delete m_homeItemList->at(i);
        }
    }
    delete m_homeItemList;
    m_homeItemList = NULL;
    delete ui;
}

///远程获取商家所有商品信息
void HomeCommodity::GetAllCommodityInfo()
{
    QJsonObject json;
    //json.insert("msg_type", MsgText);
    json.insert("type", CMD_AllCommodity_D);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    MsgSocket::GetMsgSocket()->SendMessage(data);
}

///判断并设置商品value
void HomeCommodity::SetAllCommodityInfo(const int &result_cmd, const QJsonValue &value)
{
    if(result_cmd == CMD_Sucess_Y){
        //设置商品信息item
        ui->m_listWidget->clear();
        QJsonArray array = value.toArray();
        for(int i = 0; i < array.size(); i++)
        {
            QJsonObject node = array.at(i).toObject();

            ui->m_listWidget->addItem("");
            HomeItem *hitem = new HomeItem;
            hitem->SetId(node.value("goods_id").toString());
            hitem->SetName(node.value("goods_name").toString());
            hitem->SetOldPrice(node.value("goods_old_price").toString());
            hitem->SetNewPrice(node.value("goods_new_price").toString());
            hitem->SetCallNum(node.value("goods_call_num").toString());
            hitem->SetNumber(i + 1);
            m_homeItemList->append(hitem);       ///这里注意必须要用append,否则报错
            ui->m_listWidget->item(i)->setSizeHint(QSize(1070,125));
            ui->m_listWidget->setItemWidget(ui->m_listWidget->item(i), hitem);

            GlobalVariable::g_compathMap.insert(node.value("goods_id").toString(),
                                                node.value("goods_head").toString());

            GlobalVariable::g_comimageMap.insert(node.value("goods_id").toString(), QImage());
            connect(m_homeItemList->at(i), &HomeItem::SignalAllCommodityInfo,
                    this, &HomeCommodity::SlotAllCommodity);
        }


        GetAllCommodityImage();

    }else{

    }
}

///远程获取商家所有商品的图片
void HomeCommodity::GetAllCommodityImage()
{
    if(m_x < m_homeItemList->length()){
        QString id = m_homeItemList->at(m_x)->GetId();
        QString path = GlobalVariable::g_compathMap[id];
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

///刷新页面
void HomeCommodity::on_pb_break_clicked()
{
    ui->m_listWidget->clear();
    for(int i = 0; i < m_homeItemList->length(); i++){
        if(m_homeItemList->at(i) != NULL){
            delete m_homeItemList->at(i);
        }
    }
    m_homeItemList->clear();

    GlobalVariable::g_compathMap.clear();
    GlobalVariable::g_comimageMap.clear();
    GetAllCommodityInfo();
}

///显示商品具体信息
void HomeCommodity::SlotAllCommodity(int number)
{
    ui->stackedWidget->setCurrentIndex(1);
    HomeItem *item = m_homeItemList->at(number - 1);
    ui->lb_id->setText(item->GetId());
    ui->lb_name->setText(item->GetName());
    ui->lb_old_price->setText(item->GetOldPrice());
    ui->lb_new_price->setText(item->GetNewPrice());
    ui->lb_call->setText(item->GetCallNum());

    QImage image = item->GetHead();
    QSize laSize = ui->lb_head->size();//label_carema是要显示图片的label的名称
    QImage image1=image.scaled(laSize,Qt::KeepAspectRatio);//重新调整图像大小以适应窗口
    ui->lb_head->setPixmap(QPixmap::fromImage(image1));
}

///返回listwidget页面
void HomeCommodity::on_pb_ack_clicked()
{
   ui->stackedWidget->setCurrentIndex(0);
}

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

void HomeCommodity::on_pb_select_clicked()
{
    if(m_searchCond == Search_None){
        ui->m_listWidget->clear();
        for(int i = 0; i < m_homeItemList->length(); i++){
            QListWidgetItem *item=new QListWidgetItem;
            item->setSizeHint(QSize(1070,125));
            ui->m_listWidget->addItem(item);
            ui->m_listWidget->setItemWidget(item, m_homeItemList->at(i));
        }
    }else if(m_searchCond == Search_ID){
        QString id = ui->le_select->text();
        if(id.isEmpty()){
            QMessageBox::information(this, "查询", "请输入查询属性！");
            return ;
        }
        ui->m_listWidget->clear();
        for(int i = 0; i < m_homeItemList->length(); i++){
            if(id == m_homeItemList->at(i)->GetId()){
                ui->m_listWidget->addItem("");
                qDebug() << "------------------";
                ui->m_listWidget->item(0)->setSizeHint(QSize(1075,125));
                qDebug() << "------------------";
                ui->m_listWidget->setItemWidget(ui->m_listWidget->item(0), m_homeItemList->at(0));
                qDebug() << "------------------";
                break;
            }
        }
    }else if(m_searchCond == Search_Name){
        QString name = ui->le_select->text();
        if(name.isEmpty()){
            QMessageBox::information(this, "查询", "请输入查询属性！");
            return ;
        }
        ui->m_listWidget->clear();
        for(int i = 0; i < m_homeItemList->length(); i++){
            if(name == m_homeItemList->at(i)->GetName()){
                ui->m_listWidget->addItem("");
                ui->m_listWidget->item(0)->setSizeHint(QSize(1075,125));
                ui->m_listWidget->setItemWidget(ui->m_listWidget->item(0), m_homeItemList->at(0));
                break;
            }
        }
    }
    return ;
}
