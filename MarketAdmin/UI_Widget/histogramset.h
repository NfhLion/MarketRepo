#ifndef HISTOGRAMSET_H
#define HISTOGRAMSET_H

#include <QWidget>
#include <QDebug>

#include <QVector>
#include <qwt_plot.h>
#include <QStringList>

#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_barchart.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>

///绘制刻度
class DistroScaleDraw: public QwtScaleDraw
{
public:
    DistroScaleDraw( Qt::Orientation orientation, const QStringList &labels );

    //用于显示横坐标下面对应的文本
    virtual QwtText label( double value ) const;

private:
    QStringList d_labels;
};

///自定义套件继承qwtplotbarchart来使用柱状图
class DistroChartItem: public QwtPlotBarChart
{
public:
    DistroChartItem();

    void addDistro( const QString &distro, const QColor &color );

    virtual QwtColumnSymbol *specialSymbol(int index, const QPointF& ) const;

    virtual QwtText barTitle(int sampleIndex) const;

private:
    QList<QColor> d_colors;
    QStringList   d_distros;
};

#endif // HISTOGRAMSET_H
