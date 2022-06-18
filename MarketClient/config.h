#ifndef CONFIG_H
#define CONFIG_H
#include <QObject>

class Config
{
public:
    //=======================系统配置部分=========================//
    static QString m_strAppPath;         // 应用程序路径
    static QString m_strDataPath;        // 数据保存路径
    static QString m_strRecvPath;        // 接收文件目录
    static QString m_strDatabasePath;    // 数据库目录
    static QString m_strConfPath;        // 配置目录
    static QString m_strSoundPath;       // 音频目录
    static QString m_strHeadPath;        // 头像目录
    static QString m_strGoodsPath;       // 商品图片目录

    static QString m_strIniFile;         // 配置文件名字

    static QString m_nMsgHostAddr;        // 消息服务器IP配置
    static QString m_nFileHostAddr;       //文件服务器IP配置
    static int     m_nMsgPort;           // 消息服务器端口配置
    static int     m_nFilePort;          // 文件服务器端口配置

    static QString m_strUserName;       // 用户名
    static QString m_strPassword;       // 用户密码
    static QString m_strHeadFile;       // 头像文件

    static int     m_nId;

    static int     m_nWinX;
    static int     m_nWinY;

    //=======================函数功能部分=========================//
    // 初始化
    static void InitApp(const QString &appPath);
    // 创建配置文件
    static void CreatorSettingFile();
    // 读配置文件，加载系统配置
    static void ReadSettingFile();

    // 读取/修改配置文件
    static void SetSettingFile(const QString &group, const QString &key, const QVariant &value);
    static QVariant GetSettingKeyValue(const QString &group, const QString &key, const QVariant &value);
    // 检查目录
    static void CheckDirs();
    static void CheckSound();

    static void SaveConfig();
};

#endif // CONFIG_H
