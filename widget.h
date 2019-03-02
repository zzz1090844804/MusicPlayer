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
    void ResultParseJson(QByteArray &reply_contente); //json����

    void CleanWebPlayList(); //��ո��������б�

    void LrcParseShow();  //��ʽ����Զ�����ʾ

    void InitWholeUI();   //��ʼ���������

    void ShowLocalSong();  //��ʾ��������
    void LoadLocalSongFromMysql();  //�����ݿ���ر�������
    void ClearSongLocalPlayList();  //��ձ��ز����б�

    void SwitchBackground();        //�л�����

    void Sleep(int msec); //�Զ�����ʱ����


public:QT_EVENTS

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleseEvent(QMouseEvent *event);

    void InitSignalSlots(); //��ʼ���ź���۵ĺ���

signals:
    void updataSonglrcShow_signal();    //���¸����ʾ�ź�

    void updataSongpicShow_signal();    //����ͼƬ��ʾ�ź�

public slots:
    void currentMediaChanged_slot();                    //�����л�

    void positionChanged_slot(qint64 position);         //��������

    void mediaStatusChanged_slot(QMediaPlayer::MediaStatus status);  //����״̬

    void search_slot();                                 //ȫ������

    void searchLrc_slot();                              //�������

    void searchPic_slot();                              //����ר��ͼƬ

    void SwitchBackground1_slot();                      //�л�����ͼƬ1
    void SwitchBackground2_slot();                      //�л�����ͼƬ2
    void SwitchBackground3_slot();                      //�л�����ͼƬ3
    void customizeBackground_slot();                    //�Զ��屳��ͼƬ

    void updataSonglrcShow_slot();                    //���¸����ʾ�ۺ���

    void updataSongpicShow_slot();                    //����ͼƬ��ʾ�ۺ���

    void SongPicAnimationShow_slot();               //ר��ͼƬ������ʾ


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

    //����
    QNetworkAccessManager *manager;
    QNetworkRequest       *request;
    QNetworkReply         *reply;
    //��ȡ���
    QNetworkAccessManager *manager2;
    QNetworkRequest       *request2;
    QNetworkReply         *reply2;
    //��ȡͼƬ
    QNetworkAccessManager *manager3;
    QNetworkRequest       *request3;
    QNetworkReply         *reply3;


private:
    QMediaPlayer            musicplayer;    //���ֲ�����
    QMediaPlaylist          *playList;      //�������ֲ����б�
    QMediaPlaylist          *playLocalList; //�������ֲ����б�

    int                     FrontOfMuteVolume ;  //����ǰ��������С

    QString                 playmode; //����ģʽ
private:
    QVector<SongInfo*> *SongList;      //�ݴ������� ������Ϣ
    QVector<SongInfo*> *SongLocalList; //���ز����б�

    enum CURRENTPLAYLIST{PLAYLIST,PLAYLOCALLIST};
    CURRENTPLAYLIST     CurrentPlaylist;//��ǰ�����б�

    QString            SearchSongLrc;//�������ص�δ�������
    QStringList        *LrcStr;       //���׸�����б�
    QStringList        *LrcTimeStr;   //���׸� ��Ӧʱ�����б�

    bool               IsRecLrc;      //�Ƿ��յ����
    bool               IsRecPic;      //�Ƿ��յ�ͼƬ


    QTimer             *timerpic;     //��ʱ��
    int                spin_degree;   //��ת�Ƕ�


private:                             //����
    bool            IsDrag;          //�����Ƿ�ʼ�ƶ�
    QPoint          MousePositon;    //����ʵʱ����

    QString            BackgroundPicPath; //����ͼƬ
    QMenu              *backmenu  ;
    QAction            *back1     ;
    QAction            *back2     ;
    QAction            *back3     ;
    QAction            *customize ;



};

#endif // WIDGET_H
