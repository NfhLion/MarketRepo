#include "repertorylocal.h"
#include "ui_repertorylocal.h"

#include "control.h"
#include "msgsocket.h"
#include "config.h"
#include "globalvariable.h"
#include <QPushButton>
#include <QScrollBar>

RepertoryLocal::RepertoryLocal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepertoryLocal)
{
    ui->setupUi(this);

    d_barChartItem = NULL;

    ui->pb_select->setIcon(QPixmap(":/ico/select.png"));
    ui->pb_break->setIcon(QPixmap(":/ico/shuaxin.png"));
    ui->pb_ack->setIcon(QPixmap(":/ico/back.png"));

    //设置纵向滚动条
    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background-color:rgb(218,222,223); width:10px;}"
                                                         "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");
}

RepertoryLocal::~RepertoryLocal()
{
    delete ui;
}

///远程获取商家库存
void RepertoryLocal::GetRepertoryLocal()
{
    QJsonObject json;
    json.insert("type", CMD_GetRepertoryLocal_L);
    json.insert("mer_id", Config::m_strUserName);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);

    return ;
}

void RepertoryLocal::SetRepertoryLocal(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        m_array = value.toArray();
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

//            QPushButton *pBtn = new QPushButton();
//            // 在QTableWidget中添加控件
//            ui->tableWidget->setCellWidget(i,5,pBtn);
        }
    }
}

///获取行
void RepertoryLocal::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    QString name = ui->tableWidget->item(row, 1)->text();
    int all_num = ui->tableWidget->item(row, 2)->text().toInt();
    int sale_num = ui->tableWidget->item(row, 3)->text().toInt();
    int shipment_num = ui->tableWidget->item(row, 4)->text().toInt();
    int residu_num = ui->tableWidget->item(row, 5)->text().toInt();
    SetBarchart(name, all_num, sale_num, shipment_num, residu_num);
}


///绘制柱状图的主要函数
void RepertoryLocal::SetBarchart(QString name, int all_num, int sale_num, int shipment_num, int residu_num)
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

void RepertoryLocal::setOrientation(QStringList d_distros)
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


void RepertoryLocal::on_pb_break_clicked()
{
    GetRepertoryLocal();
}
