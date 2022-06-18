#include "localcommodity.h"
#include "ui_localcommodity.h"

#include <QMessageBox>
#include <QScrollBar>

LocalCommodity::LocalCommodity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalCommodity)
{
    ui->setupUi(this);

    ui->pb_select->setIcon(QPixmap(":/ico/select.png"));
    ui->pb_break->setIcon(QPixmap(":/ico/shuaxin.png"));
    ui->pb_delete->setIcon(QPixmap(":/ico/delete.png"));
    ui->pb_shipment->setIcon(QPixmap(":/ico/shenqing.png"));

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

    d_barChartItem = NULL;
    GetLocalCommodity();
}

LocalCommodity::~LocalCommodity()
{
    delete ui;
}

///远程获取本地商品
void LocalCommodity::GetLocalCommodity()
{
    if(m_array.isEmpty()){
        m_array = ExecSql::GetExecSql()->SearchLocalCommodity(Config::m_strUserName);
        if(m_array.isEmpty()){
            QJsonObject json;
            json.insert("type", CMD_GLocalCommodity_U);
            json.insert("shop_id", Config::m_strUserName);
            QJsonDocument doc(json);
            QByteArray data = doc.toJson();
            MsgSocket::GetMsgSocket()->SendMessage(data);
        }else{
            SetLocalCommodityTableWidget();
        }
    }
}

void LocalCommodity::SetLocalCommodity(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonArray array = value.toArray();
        if(array.isEmpty()){
            return ;
        }

        if(m_array.isEmpty()){
            for(int i = 0; i < array.size(); i++){
                QJsonObject obj = array.at(i).toObject();
                ExecSql::GetExecSql()->InsertCommodity(obj.value("goods_num_id").toString(), obj.value("goods_num_gid").toString(),
                                                    obj.value("goods_num_mid").toString(),obj.value("goods_num_all").toString(),
                                                    obj.value("goods_num_sale").toString(),
                                                    obj.value("goods_num_residue").toString(),obj.value("goods_num_shipment").toString());
            }
        }

        m_array = array;
        SetLocalCommodityTableWidget();
    }
}

void LocalCommodity::SetLocalCommodityTableWidget()
{
    QJsonArray array = m_array;
    QJsonArray com_array = GlobalVariable::g_allCommodityArray;

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(6);

    // set header lables
    QStringList headers;
    headers << "商品编号" << "商品名称" << "总数" << "售出数" << "调货数" << "剩余数";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setRowCount(array.size());

    for(int i = 0; i < array.size(); i++){
        QJsonObject obj = array.at(i).toObject();
        QJsonObject com_obj;
        for(int j = 0; j < com_array.size(); j++){
            com_obj = com_array.at(j).toObject();
            if(obj.value("goods_num_gid").toString() == com_obj.value("goods_id").toString()){
                break;
            }
        }
        QTableWidgetItem *item = new QTableWidgetItem(obj.value("goods_num_gid").toString());
        ui->tableWidget->setItem(i, 0, item);
        item = new QTableWidgetItem(com_obj.value("goods_name").toString());
        ui->tableWidget->setItem(i, 1, item);
        item = new QTableWidgetItem(obj.value("goods_num_all").toString());
        ui->tableWidget->setItem(i, 2, item);
        item = new QTableWidgetItem(obj.value("goods_num_sale").toString());
        ui->tableWidget->setItem(i, 3, item);
        item = new QTableWidgetItem(obj.value("goods_num_shipment").toString());
        ui->tableWidget->setItem(i, 4, item);
        item = new QTableWidgetItem(obj.value("goods_num_residue").toString());
        ui->tableWidget->setItem(i, 5, item);
    }
}

///获取行
void LocalCommodity::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    QString name = ui->tableWidget->item(row, 1)->text();
    int all_num = ui->tableWidget->item(row, 2)->text().toInt();
    int sale_num = ui->tableWidget->item(row, 3)->text().toInt();
    int shipment_num = ui->tableWidget->item(row, 4)->text().toInt();
    int residu_num = ui->tableWidget->item(row, 5)->text().toInt();
    ui->le_id->setText(ui->tableWidget->item(row, 0)->text());
    ui->le_name->setText(name);
    ui->le_all->setText(ui->tableWidget->item(row, 2)->text());
    ui->le_residue->setText(ui->tableWidget->item(row, 5)->text());

    SetBarchart(name, all_num, sale_num, shipment_num, residu_num);
}

///绘制柱状图的主要函数
void LocalCommodity::SetBarchart(QString name, int all_num, int sale_num, int shipment_num, int residu_num)
{
    struct S_PageHit
    {
        QString distro;
        int hits;
        QColor color;

    };
    QList<S_PageHit> pageHits;
    pageHits.append(S_PageHit{"售出数", sale_num, QColor( "DodgerBlue" )});
    pageHits.append(S_PageHit{"调货数", shipment_num, QColor( "FireBrick" )});
    pageHits.append(S_PageHit{ "总数", all_num, QColor( "#d70751" ) });
    pageHits.append(S_PageHit{ "剩余数", residu_num, QColor( "SteelBlue" ) });


    ui->plot->setTitle(QString("%1统计表").arg(name));
    setAutoFillBackground( true );
    //setPalette( QColor( "Linen" ) );//设置背景调色板颜色

    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth( 2 );
    canvas->setFrameStyle( QFrame::Box | QFrame::Sunken );
    canvas->setBorderRadius( 10 );

    QPalette canvasPalette( QColor( "Plum" ) );
    canvasPalette.setColor( QPalette::Foreground, QColor( "Indigo" ) );
    canvas->setPalette( canvasPalette );

    ui->plot->setCanvas( canvas );

    if(d_barChartItem != NULL){
        delete d_barChartItem;
        d_barChartItem = NULL;
    }

    d_barChartItem = new DistroChartItem();

    QVector< double > samples;
    QStringList d_distros;

    for (int i = 0; i < pageHits.length(); i++ )
    {
        d_distros.append(pageHits[i].distro);
        samples += pageHits[i].hits;

        d_barChartItem->addDistro(pageHits[i].distro, pageHits[i].color);//设置右上角
    }

    d_barChartItem->setSamples( samples );

    d_barChartItem->attach( ui->plot );

    ui->plot->insertLegend( new QwtLegend() );


    ui->plot->setAutoReplot( true );
    setOrientation(d_distros);

}

void LocalCommodity::setOrientation(QStringList d_distros)
{
    const Qt::Orientation orientation = Qt::Vertical;

    int axis1 = QwtPlot::xBottom;
    int axis2 = QwtPlot::yLeft;

    if ( orientation == Qt::Horizontal )
        qSwap( axis1, axis2 );

    d_barChartItem->setOrientation( orientation );

    ui->plot->setAxisTitle( axis1, "类型" );
    ui->plot->setAxisMaxMinor( axis1, 3 );
    ui->plot->setAxisScale(axis1, 0, d_barChartItem->dataSize() - 1, 1.0);
    ui->plot->setAxisScaleDraw( axis1, new DistroScaleDraw( orientation, d_distros ) );

    ui->plot->setAxisTitle( axis2, "数量/个" );
    ui->plot->setAxisMaxMinor( axis2, 3 );

    QwtScaleDraw *scaleDraw = new QwtScaleDraw();
    scaleDraw->setTickLength( QwtScaleDiv::MediumTick, 4 );
    ui->plot->setAxisScaleDraw( axis2, scaleDraw );

    ui->plot->plotLayout()->setCanvasMargin( 0 );
    ui->plot->replot();
}


void LocalCommodity::on_pb_shipment_clicked()
{
    QString id = ui->le_id->text();
    if(id.isEmpty()){
        QMessageBox::information(this, "申请调货", "请通过表格选择需要的货物！");
        return;
    }
    QString num = ui->le_num->text();
    if(num.isEmpty()){
        QMessageBox::information(this, "申请调货", "请输入需要的货物数量！");
        return;
    }
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

void LocalCommodity::on_pb_break_clicked()
{
    if(m_array.isEmpty()){
        m_array = ExecSql::GetExecSql()->SearchLocalCommodity(Config::m_strUserName);
        if(m_array.isEmpty()){
            QJsonObject json;
            json.insert("type", CMD_GLocalCommodity_U);
            json.insert("shop_id", Config::m_strUserName);
            QJsonDocument doc(json);
            QByteArray data = doc.toJson();
            MsgSocket::GetMsgSocket()->SendMessage(data);
        }else{
            SetLocalCommodityTableWidget();
        }
    }else{
        SetLocalCommodityTableWidget();
    }
}
