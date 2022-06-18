#ifndef FGGROUPLISTWIDGET_H
#define FGGROUPLISTWIDGET_H

#include <QTreeWidget>
class FgGroupListWidget : public QTreeWidget
{
    Q_OBJECT
public:
    FgGroupListWidget(QWidget* parent = Q_NULLPTR);

    QTreeWidgetItem* appendGroupItem(QString groupName);
    QTreeWidgetItem* insertGroupItem(int index, QString &groupName);

    void addChildItem(QTreeWidgetItem *pGroupItem, QWidget* pWidget);

private slots:
    void onItemClicked(QTreeWidgetItem *item, int);
    void onItemExpanded(QTreeWidgetItem *item);
    void onItemCollapsed(QTreeWidgetItem *item);
};

#endif // FGGROUPLISTWIDGET_H
