#include "histogramset.h"

DistroScaleDraw::DistroScaleDraw(Qt::Orientation orientation, const QStringList &labels)
{
    d_labels.clear();
    d_labels = labels;
    //设置间距、中间刻度、主刻度
    setTickLength( QwtScaleDiv::MinorTick, 0 );
    setTickLength( QwtScaleDiv::MediumTick, 0 );
    setTickLength( QwtScaleDiv::MajorTick, 2 );

    //不显示x轴的主轴线
    enableComponent( QwtScaleDraw::Backbone, false );

    //设置x轴刻度下文字的旋转角度、对齐方式
    if ( orientation == Qt::Vertical )
    {
        setLabelRotation( 0.0 );    //旋转角度选择不旋转
    }
    else
    {
        setLabelRotation( -20.0 );
    }

    setLabelAlignment( Qt::AlignVCenter | Qt::AlignVCenter );   //对齐方式选择中心对齐
}

QwtText DistroScaleDraw::label(double value) const
{
    QwtText lbl;

    const int index = qRound( value );
    if ( index >= 0 && index < d_labels.size() )
    {
        lbl = d_labels[ index ];
    }

    return lbl;
}

DistroChartItem::DistroChartItem():
    QwtPlotBarChart( "Page Hits" )
{
    setLegendMode( QwtPlotBarChart::LegendBarTitles );
    //设置项大小
    setLegendIconSize( QSize( 10, 14 ) );
    //setLayoutPolicy( AutoAdjustSamples );
    //setLayoutHint( 4.0 ); // minimum width for a single bar

    //setSpacing( 10 ); // spacing between bars
}

void DistroChartItem::addDistro(const QString &distro, const QColor &color)
{
    d_colors += color;
    d_distros += distro;
    itemChanged();
}

QwtColumnSymbol *DistroChartItem::specialSymbol(int index, const QPointF &) const
{
    //自定义项符号线度、样式、背景色
    QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
    symbol->setLineWidth( 2 );
    symbol->setFrameStyle( QwtColumnSymbol::Raised );

    QColor c( Qt::white );
    if ( index >= 0 && index < d_colors.size() )
        c = d_colors[index];

    symbol->setPalette(c);

    return symbol;
}

QwtText DistroChartItem::barTitle(int sampleIndex) const
{
    QwtText title;
    if ( sampleIndex >= 0 && sampleIndex < d_distros.size())
        title = d_distros[sampleIndex];
    return title;
}
