#include "memberwidget.h"
#include "ui_memberwidget.h"

MemberWidget::MemberWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemberWidget)
{
    ui->setupUi(this);

    m_memberInfo = new MemberInfo(ui->widget);
    m_insertMember = new InsertMember(ui->widget);
    m_memberInfo->show();
    m_insertMember->hide();

    ui->pb_localcom->setIcon(QPixmap(":/ico/zhuce.png"));
    ui->pb_come->setIcon(QPixmap(":/ico/member.png"));
}

MemberWidget::~MemberWidget()
{
    delete ui;
}

MemberInfo *MemberWidget::GetMemberInfoWidget()
{
    return m_memberInfo;
}

InsertMember *MemberWidget::GetInsertMember()
{
    return m_insertMember;
}

void MemberWidget::on_pb_come_clicked()
{
    m_memberInfo->show();
    m_insertMember->hide();
}

void MemberWidget::on_pb_localcom_clicked()
{
    m_memberInfo->hide();
    m_insertMember->show();
}
