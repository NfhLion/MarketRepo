#include "FgGroupListWidget.h"
#include "FgGroupItem.h"
#include "childitem.h"

FgGroupListWidget::FgGroupListWidget(QWidget *parent):QTreeWidget(parent)
{
    setRootIsDecorated(false);  //隐藏自带指示器
    setIndentation(0);          //设置缩进，让父节点和子节点对齐
    connect(this, &FgGroupListWidget::itemClicked, this, &FgGroupListWidget::onItemClicked);
    connect(this, &FgGroupListWidget::itemExpanded, this, &FgGroupListWidget::onItemExpanded);
    connect(this, &FgGroupListWidget::itemCollapsed, this,&FgGroupListWidget::onItemCollapsed);
}

QTreeWidgetItem *FgGroupListWidget::appendGroupItem(QString groupName)
{
    return insertGroupItem(topLevelItemCount(),groupName);
}

QTreeWidgetItem *FgGroupListWidget::insertGroupItem(int index,QString& groupName)
{
    //分组节点
    QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
    pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    //设置Data用于区分，Item是分组节点还是子节点，0代表分组节点，1代表子节点
    pRootItem->setData(0, Qt::UserRole, 0);
    FgGroupItem *pItemName = new FgGroupItem(this);
    pRootItem->setSizeHint(0,pItemName->size());
    pItemName->setText(groupName);
    //插入分组节点
    insertTopLevelItem(index,pRootItem);
    setItemWidget(pRootItem, 0, pItemName);
    collapseAll();
    pRootItem->setExpanded(true);
    return pRootItem;
}

void FgGroupListWidget::addChildItem(QTreeWidgetItem* pGroupItem,QWidget* pWidget)
{
    QTreeWidgetItem *pChildItem = new QTreeWidgetItem();
    pChildItem->setSizeHint(0,pWidget->size());
    pChildItem->setData(0,Qt::UserRole,1);

    pGroupItem->addChild(pChildItem);
    setItemWidget(pChildItem,0,pWidget);
}

void FgGroupListWidget::onItemClicked(QTreeWidgetItem *item, int)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild)
    {
        item->setExpanded(!item->isExpanded());
    }
}

void FgGroupListWidget::onItemExpanded(QTreeWidgetItem *item)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild)
    {
        FgGroupItem *groupItem = qobject_cast<FgGroupItem*>(this->itemWidget(item, 0));
        if (groupItem)
        {
            groupItem->setExpanded(true);
        }
    }
}

void FgGroupListWidget::onItemCollapsed(QTreeWidgetItem *item)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild)
    {
        FgGroupItem *groupItem = qobject_cast<FgGroupItem*>(this->itemWidget(item, 0));
        if (groupItem)
        {
            groupItem->setExpanded(false);
        }
    }
}
