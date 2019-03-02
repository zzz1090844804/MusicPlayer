#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

//network
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonValue>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>

#include <QPaintEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QPainter>

#include <QFile>
#include <QFileDialog>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>
#include <QSqlRecord>
#include <QSqlTableModel>

#include <QMessageBox>
#include <QMenu>

#include <QString>
#include <QDebug>
#include <vector>

#include <QTimer>

#define SONGLRCLIST 1


#define QT_EVENTS

struct SongInfo
{
    QString SongID;
    QString SongName;
    QString SingerName;
    QString SongUrl;
    QString PicUrl;
    QString LrcUrl;

    SongInfo()
    {
        SongID     = "";
        SongName   = "";
        SingerName = "";
        SongUrl    = "";
        PicUrl     = "";
        LrcUrl     = "";
    }
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public:
    void ResultParseJson(QByteArray &reply_contente); //json解析

    void CleanWebPlayList(); //清空歌曲播放列表

    void LrcParseShow();  //歌词解析自定义显示

    void InitWholeUI();   //初始化整体界面

    void ShowLocalSong();  //显示本地音乐
    void LoadLocalSongFromMysql();  //从数据库加载本地音乐
    void ClearSongLocalPlayList();  //清空本地播放列表

    void SwitchBackground();        //切换背景

    void Sleep(int msec); //自定义延时函数


public:QT_EVENTS

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleseEvent(QMouseEvent *event);

    void InitSignalSlots(); //初始化信号与槽的函数

signals:
    void updataSonglrcShow_signal();    //更新歌词显示信号

    void updataSongpicShow_signal();    //更新图片显示信号

public slots:
    void currentMediaChanged_slot();                    //歌曲切换

    void positionChanged_slot(qint64 position);         //歌曲进度

    void mediaStatusChanged_slot(QMediaPlayer::MediaStatus status);  //播放状态

    void search_slot();                                 //全局搜索

    void searchLrc_slot();                              //搜索歌词

    void searchPic_slot();                              //搜索专辑图片

    void SwitchBackground1_slot();                      //切换背景图片1
    void SwitchBackground2_slot();                      //切换背景图片2
    void SwitchBackground3_slot();                      //切换背景图片3
    void customizeBackground_slot();                    //自定义背景图片

    void updataSonglrcShow_slot();                    //更新歌词显示槽函数

    void updataSongpicShow_slot();                    //更新图片显示槽函数

    void SongPicAnimationShow_slot();               //专辑图片动画显示


private slots:
    void on_pushButton_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_volumn_valueChanged(int value);

    void on_listWidget_locallist_doubleClicked(const QModelIndex &index);

    void on_pushButton_WinClose_clicked();

    void on_pushButton_WinMinsize_clicked();

    void on_pushButton_Mute_clicked();

    void on_pushButton_ShowLrc_clicked();

private:
    Ui::Widget *ui;

    //搜索
    QNetworkAccessManager *manager;
    QNetworkRequest       *request;
    QNetworkReply         *reply;
    //获取歌词
    QNetworkAccessManager *manager2;
    QNetworkRequest       *request2;
    QNetworkReply         *reply2;
    //获取图片
    QNetworkAccessManager *manager3;
    QNetworkRequest       *request3;
    QNetworkReply         *reply3;


private:
    QMediaPlayer            musicplayer;    //音乐播放器
    QMediaPlaylist          *playList;      //网络音乐播放列表
    QMediaPlaylist          *playLocalList; //本地音乐播放列表

    int                     FrontOfMuteVolume ;  //静音前的声音大小

    QString                 playmode; //播放模式
private:
    QVector<SongInfo*> *SongList;      //暂存搜索到 歌曲信息
    QVector<SongInfo*> *SongLocalList; //本地播放列表

    enum CURRENTPLAYLIST{PLAYLIST,PLAYLOCALLIST};
    CURRENTPLAYLIST     CurrentPlaylist;//当前播放列表

    QString            SearchSongLrc;//搜索返回的未解析歌词
    QStringList        *LrcStr;       //单首歌词行列表
    QStringList        *LrcTimeStr;   //单首歌 对应时间行列表

    bool               IsRecLrc;      //是否收到歌词
    bool               IsRecPic;      //是否收到图片


    QTimer             *timerpic;     //定时器
    int                spin_degree;   //旋转角度


private:                             //窗口
    bool            IsDrag;          //窗口是否开始移动
    QPoint          MousePositon;    //鼠标的实时坐标

    QString            BackgroundPicPath; //背景图片
    QMenu              *backmenu  ;
    QAction            *back1     ;
    QAction            *back2     ;
    QAction            *back3     ;
    QAction            *customize ;



};

#endif // WIDGET_H
