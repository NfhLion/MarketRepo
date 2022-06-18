#include "config.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QDebug>

// 应用程序配置目录
QString Config::m_strAppPath         = "./";
QString Config::m_strDataPath        = "";
QString Config::m_strRecvPath        = "";
QString Config::m_strDatabasePath    = "";
QString Config::m_strConfPath        = "";
QString Config::m_strHeadPath        = "";
QString Config::m_strGoodsPath       = "";
QString Config::m_strSoundPath       = "";
// 配置文件
QString Config::m_strIniFile         = "config.ini";

// 服务器相关配置
QString Config::m_nMsgHostAddr       = "192.168.0.109";
QString Config::m_nFileHostAddr      = "192.168.0.109";
int     Config::m_nMsgPort           = 10240;
int     Config::m_nFilePort          = 10241;

QString Config::m_strUserName        = "zhangsan";
QString Config::m_strPassword        = "123456";
QString Config::m_strHeadFile        = "head0.png";

int     Config::m_nId                = -1;
int     Config::m_nWinX              = 0;
int     Config::m_nWinY              = 0;

// 初始化
void Config::InitApp(const QString &appPath)
{
    m_strAppPath        = appPath + "/../../MarketAdmin/";

    m_strDataPath       = m_strAppPath  + "ConfigDir/";
    m_strRecvPath       = m_strDataPath + "RecvFiles/";
    m_strDatabasePath   = m_strDataPath + "Database/";
    m_strConfPath       = m_strDataPath + "Conf/";
    m_strHeadPath       = m_strDataPath + "Head/";
    m_strGoodsPath      = m_strDataPath + "GoodsImage/";
    m_strSoundPath      = m_strDataPath + "Sound/";
    m_strIniFile        = m_strConfPath + "config.ini";

    // 检查目录
    CheckDirs();

    // 检测音频文件
    CheckSound();

    // 创建配置文件
    CreatorSettingFile();

    //保存配置
    SaveConfig();

    // 加载系统配置
    ReadSettingFile();
}

///创建配置文件
void Config::CreatorSettingFile() {
    // 写入配置文件
    QSettings settings(m_strIniFile, QSettings::IniFormat);
    QString strGroups = settings.childGroups().join("");
    if (!QFile::exists(m_strIniFile) || (strGroups.isEmpty()))
    {

        /*系统设置*/
        settings.beginGroup("UserCfg");
        settings.setValue("User",   m_strUserName);
        settings.setValue("Passwd", m_strPassword);
        settings.setValue("Head", m_strHeadPath + m_strHeadFile);
        settings.endGroup();

        /*其他配置*/
        settings.beginGroup("Server");
        settings.setValue("MsgHostAddr", m_nMsgHostAddr);
        settings.setValue("FileHostAddr", m_nFileHostAddr);
        settings.setValue("MsgPort",  m_nMsgPort);
        settings.setValue("FilePort",  m_nFilePort);
        settings.endGroup();
        settings.sync();

    }
}


///读取系统配置信息
void Config::ReadSettingFile()
{
    QSettings settings(m_strIniFile, QSettings::IniFormat);
    settings.beginGroup("UserCfg");
    m_strUserName = settings.value("User", "zhangsan").toString();
    m_strPassword = settings.value("Passwd", "123456").toString();
    m_strHeadFile = settings.value("Head", m_strHeadPath + m_strHeadFile).toString();
    settings.endGroup();

    settings.beginGroup("Server");
    m_nMsgHostAddr  = settings.value("MsgHostAddr", "192.168.0.109").toString();
    m_nFileHostAddr = settings.value("FileHostAddr", "192.168.0.109").toString();
    m_nMsgPort      = settings.value("MsgPort", 10240)  .toInt();
    m_nFilePort     = settings.value("FilePort", 10240)  .toInt();
    settings.endGroup();
}

///写入配置文件
void Config::SetSettingFile(const QString &group, const QString &key, const QVariant &value)
{
    QSettings settings(m_strIniFile, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.sync();
}

///读取配置参数
QVariant Config::GetSettingKeyValue(const QString &group, const QString &key, const QVariant &value)
{
    QSettings settings(m_strIniFile, QSettings::IniFormat);
    settings.beginGroup(group);
    return settings.value(key, value);
}


///检查数据存储目录
void Config::CheckDirs()
{
    // 数据文件夹
    QDir dir(m_strDataPath);
    if (!dir.exists()) {
        dir.mkdir(m_strDataPath);
    }

    // 接收文件目录
    dir.setPath(m_strRecvPath);
    if (!dir.exists()) {
        dir.mkdir(m_strRecvPath);
    }

    // 数据库目录
    dir.setPath(m_strDatabasePath);
    if (!dir.exists()) {
        dir.mkdir(m_strDatabasePath);
    }

    // 配置文件目录
    dir.setPath(m_strConfPath);
    if (!dir.exists()) {
        dir.mkdir(m_strConfPath);
    }

    // 头像检测目录
    dir.setPath(m_strHeadPath);
    if (!dir.exists()) {
        dir.mkdir(m_strHeadPath);
    }

    // 商品图片检测目录
    dir.setPath(m_strGoodsPath);
    if (!dir.exists()) {
        dir.mkdir(m_strGoodsPath);
    }

    // 音频目录
    dir.setPath(m_strSoundPath);
    if (!dir.exists()) {
        dir.mkdir(m_strSoundPath);
    }
}


 ///音频文件检测
void Config::CheckSound()
{
    if (!QFile::exists(Config::m_strSoundPath + "message.wav")) {
        QFile::copy(":/sound/resource/sound/message.wav", Config::m_strSoundPath + "message.wav");
    }

    if (!QFile::exists(Config::m_strSoundPath + "msg.wav")) {
        QFile::copy(":/sound/resource/sound/msg.wav", Config::m_strSoundPath + "msg.wav");
    }

    if (!QFile::exists(Config::m_strSoundPath + "ringin.wav")) {
        QFile::copy(":/sound/resource/sound/ringin.wav", Config::m_strSoundPath + "ringin.wav");
    }

    if (!QFile::exists(Config::m_strSoundPath + "system.wav")) {
        QFile::copy(":/sound/resource/sound/system.wav", Config::m_strSoundPath + "system.wav");
    }

    if (!QFile::exists(Config::m_strSoundPath + "userlogon.wav")) {
        QFile::copy(":/sound/resource/sound/userlogon.wav", Config::m_strSoundPath + "userlogon.wav");
    }
}

///保存配置
void Config::SaveConfig()
{
    QSettings settings(m_strIniFile, QSettings::IniFormat);

    /*系统设置*/
    settings.beginGroup("UserCfg");
    settings.setValue("User",   m_strUserName);
    settings.setValue("Passwd", m_strPassword);
    settings.setValue("Head", m_strHeadPath + m_strHeadFile);
    settings.endGroup();

    /*其他配置*/
    settings.beginGroup("Server");
    settings.setValue("MsgHostAddr", m_nMsgHostAddr);
    settings.setValue("FileHostAddr", m_nFileHostAddr);
    settings.setValue("MsgPort",  m_nMsgPort);
    settings.setValue("FilePort",  m_nFilePort);
    settings.endGroup();
    settings.sync();
}
