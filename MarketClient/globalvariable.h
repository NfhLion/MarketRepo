#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H

#include <QQueue>
#include <QMap>
#include <QImage>
#include <QJsonArray>


typedef QQueue<QByteArray>     ByteArryQueue;
typedef QMap<QString, QImage>  CommodityImageMap;
typedef QMap<QString, QString> CommodityPathMap;

class GlobalVariable
{
public:
    static ByteArryQueue     g_msgQueue;             //消息队列
    static CommodityImageMap g_comimageMap;          //商品图片Map
    static CommodityPathMap  g_compathMap;           //商品图片路径
    static QJsonArray        g_allCommodityArray;    //所有商品的信息
};

#endif // GLOBALVARIABLE_H
