#include "widget.h"
#include "ui_widget.h"

#define  LOCALMUSIC

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //qDebug()<<manager->supportedSchemes();  //���֧�ֵ�Э��

    manager   =  new  QNetworkAccessManager;
    manager2  =  new  QNetworkAccessManager;
    manager3  =  new  QNetworkAccessManager;
    request   =  new  QNetworkRequest;
    request2  =  new  QNetworkRequest;
    request3  =  new  QNetworkRequest;

    SongList      = new QVector<SongInfo*>;     //�������粥���б�
    SongLocalList = new QVector<SongInfo*>;//�������ز����б�

    playList      = new QMediaPlaylist; //�������粥���б�
    playLocalList = new QMediaPlaylist; //�������ز����б�

    LrcStr        = new QStringList;   //����������б�
    LrcTimeStr    = new QStringList;   //����������б�

    timerpic      = new QTimer;         //ר��ͼƬ��ת��ʱ��

    InitWholeUI();                      //��ʼ���������

    InitSignalSlots();                  //��ʼ���ź���۵ĺ���

    LoadLocalSongFromMysql();           //�����ݿ���ر�������

}

Widget::~Widget()
{
    delete ui;
    delete manager;
    delete manager2;
    delete manager3;
    delete request;
    reply->deleteLater();
    delete request2;
    reply2->deleteLater();
    delete request3;
    reply3->deleteLater();
}



/**************************************��ʼ��********************************************/
//�л������������˵�
void Widget::SwitchBackground()
{
    backmenu  = new QMenu;
    back1     = new QAction(backmenu);  //Ĭ�ϱ���һ
    back2     = new QAction(backmenu);  //Ĭ�ϱ�����
    back3     = new QAction(backmenu);  //Ĭ�ϱ�����
    customize = new QAction(backmenu);  //�Զ���

    back1->setToolTip("back1");
    back2->setToolTip("back2");
    back3->setToolTip("back3");
    customize->setToolTip("customize");

    back1->setIconText("back1");
    back2->setIconText("back2");
    back3->setIconText("back3");
    customize->setIconText("customize");
    back1->setIcon(QIcon(":/icons/icon/background_1.png"));
    back2->setIcon(QIcon(":/icons/icon/background_1.png"));
    back3->setIcon(QIcon(":/icons/icon/background_1.png"));
    customize->setIcon(QIcon(":/icons/icon/customize.png"));

    backmenu->addAction(back1    );
    backmenu->addAction(back2    );
    backmenu->addAction(back3    );
    backmenu->addAction(customize);
    ui->pushButton_ChangeSkin->setMenu(backmenu);

    ui->pushButton_ChangeSkin->setStyleSheet("QPushButton::menu-indicator{image:none;} QPushButton{border-image: url(:/icons/icon/skin.png);}"); //ȥ����������
}
//��ʼ���������
void Widget::InitWholeUI()
{
    BackgroundPicPath = ":/background/background/background_1.png";  //����
    //ȥ��������
    this->setWindowFlags(Qt::FramelessWindowHint);
    //����APPͼ��
    this->setWindowIcon(QIcon(":/icons/icon/WinIcon.jpg"));
    //����APP����
    this->setWindowTitle("MusicPlayer");

    //����     ��ť��������
    ui->pushButton->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/search.png);}"); //�ޱ߿�

    //��ͣ     ��ť��������
    ui->pushButton_2->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/mediaPause.png);}");
    ui->pushButton_2->hide();

    //��һ��   ��ť��������
    ui->pushButton_3->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/playPreSong.png);}");
    //����    ��ť��������
    ui->pushButton_4->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/mediaPlay.png);}");


    //��һ��  ��ť��������
    ui->pushButton_5->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/playNextSong.png);}");

    //����ģʽ ��ť��������
    playmode = "ѭ��˳�򲥷�";
    //ui->pushButton_6->setAutoFillBackground(true);
    //ui->pushButton_6->setIcon(QIcon(":/icons/icon/inOrder.png"));
    //ui->pushButton_6->setToolTip(playmode);
    ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/inOrder.png);}");

    //��ӱ�������  ��ť��������
    ui->pushButton_13->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/addSong.png);}");

    //�����������б�  ��ť��������
    ui->pushButton_14->setStyleSheet("QPushButton{border:none;} QPushButton::hover{background-color: rgba(255, 255, 255,150);}");

    //�򿪱��������б�  ��ť��������
    ui->pushButton_15->setStyleSheet("QPushButton{border:none;} QPushButton::hover{background-color: rgba(255, 255, 255,150);}");

    //�л���ר��ҳ��
    ui->pushButton_ShowLrc->setStyleSheet("QPushButton{border:none;}");
    ui->pushButton_ShowLrc->hide();

    //����      ��ť��������
    ui->pushButton_Mute->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/volume.png);}");

    //����
    SwitchBackground();


    //���粥���б�
    ui->listWidget->setStyleSheet("QListWidget{"
                                  "background-color:transparent;"
                                  "border:none;"
                                  "}");

    //���ز����б�
    ui->listWidget_locallist->setStyleSheet("QListWidget{"
                                  "background-color:transparent;"
                                  "border:none;"
                                  "}");
    //ר����Ҫ��Ϣ
    ui->textBrowser_SongInfo->setStyleSheet("background-color:transparent;border:none;");

    //��ʾ���
    ui->listWidget_SongLrc->setStyleSheet("QListWidget{"
                                   "background-color:transparent;"
                                   "border:none;"
                                   "}");

    //���������
    ui->lineEdit->setStyleSheet("QLineEdit{"
                                "background-color:transparent;"
                                "border:none;"
                                "background-color: rgba(255, 255, 255,150);"
                                "border-style:outset;"
                                "border-width:0px;"
                                "border-radius:10px;"
                                "border-color: rgba(0,0,0,100);"
                                "font:bold 14px;"
                                "color:rgba(0,0,0,100);"
                                "padding:6px;"
                                "}"
                                "QLineEdit:hover{border-radius: 10px ; border-color: blue}");
    //ui->lineEdit->setPlaceholderText(QStringLiteral("Search"));

    //����Դѡ��
    ui->comboBox->setStyleSheet("QComboBox{"
                                "background-color:transparent;"
                                "font:bold 14px;"
                                "color:rgba(0,0,0,100);"
                                "background-color: rgba(255, 255, 255,150);"
                                "}");

}
//��ʼ���ź���۵ĺ���
void Widget::InitSignalSlots()
{
    //��ȡ������ʱ��
    connect(&musicplayer,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(currentMediaChanged_slot()));
    connect(&musicplayer,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged_slot(qint64)));                  // �������ȸı�
    connect(&musicplayer,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(mediaStatusChanged_slot(QMediaPlayer::MediaStatus)));//����״̬�ı�

    connect(manager ,&QNetworkAccessManager::finished,this,&Widget::search_slot);
    connect(manager2,&QNetworkAccessManager::finished,this,&Widget::searchLrc_slot);
    connect(manager3,&QNetworkAccessManager::finished,this,&Widget::searchPic_slot);
//�л�����
    connect(back1,    &QAction::triggered,this,&Widget::SwitchBackground1_slot);
    connect(back2,    &QAction::triggered,this,&Widget::SwitchBackground2_slot);
    connect(back3,    &QAction::triggered,this,&Widget::SwitchBackground3_slot);
    connect(customize,&QAction::triggered,this,&Widget::customizeBackground_slot);
//�л� ר����ʺ�ͼƬ
    connect(this,&Widget::updataSonglrcShow_signal,this,&Widget::updataSonglrcShow_slot);
    connect(this,&Widget::updataSongpicShow_signal,this,&Widget::updataSongpicShow_slot);
//��ʱ��
    connect(timerpic,&QTimer::timeout,this,&Widget::SongPicAnimationShow_slot);


}


/**************************************�ۺ���********************************************/
void Widget::positionChanged_slot(qint64 position) //���Ÿ������ȸı䣬�ı�ʱ����ʾ����ʸı�
{
    //�����������ı�
    ui->horizontalSlider->setValue(position);

    //����ʱ��ı�
    int h = position/3600000;
    int m = (position%3600000)/60000;
    int s = ((position%3600000)%60000)/1000;
    int ms = ((position%3600000)%60000)%1000;
    QTime CurrentTime (h,m,s,ms);
    ui->label_CurrentTime->setText(CurrentTime.toString("mm:ss:zzz"));

    //��ǰ��ʸı�
    switch (CurrentPlaylist) {
    case PLAYLIST:
        for(int i = 0;i < LrcTimeStr->count();i++)
        {
            if(CurrentTime <= QTime::fromString(LrcTimeStr->at(i),"mm:ss:zzz"))  //�ҵ�Ҫ���ڲ��ŵĸ��
            {
                if(i == 0)//��һ��
                {
                    ui->listWidget_SongLrc->item(i)->setTextColor("red");
                    ui->listWidget_SongLrc->item(i)->setSelected(true);                     //�۽�
                    ui->listWidget_SongLrc->scrollToItem(ui->listWidget_SongLrc->item(i));  //�������Զ��۽�
                    ui->listWidget_SongLrc->item(i)->setTextAlignment(Qt::AlignCenter);     //����
                    //ui->listWidget_SongLrc->setResizeMode(QListView::Adjust);  //����Ӧ
                }
                else
                {
                    ui->listWidget_SongLrc->item(i-1)->setTextColor("red");
                    ui->listWidget_SongLrc->item(i-1)->setSelected(true);
                    ui->listWidget_SongLrc->scrollToItem(ui->listWidget_SongLrc->item(i-1));
                    ui->listWidget_SongLrc->item(i-1)->setTextAlignment(Qt::AlignCenter); //����
                    //ui->listWidget_SongLrc->setResizeMode(QListView::Adjust);  //����Ӧ
                }
                break;
            }
            else //���Ź��ĸ�� ��ʸ�ԭ
            {
                ui->listWidget_SongLrc->item(i)->setTextColor("black");
                ui->listWidget_SongLrc->item(i)->setSelected(false);
                ui->listWidget_SongLrc->scrollToItem(ui->listWidget_SongLrc->item(i));
                ui->listWidget_SongLrc->item(i)->setTextAlignment(Qt::AlignCenter); //����
                //ui->listWidget_SongLrc->setResizeMode(QListView::Adjust);  //����Ӧ
            }
        }
        break;
    case PLAYLOCALLIST:

        break;
    default:
        break;
    }
}
void Widget::mediaStatusChanged_slot(QMediaPlayer::MediaStatus status)  //������״̬�仯
{
    switch (status)
    {
    case QMediaPlayer::BufferedMedia:       //����������ȫ���嵱ǰý�塣  �޸���ʱ�� �ͽ����� �Լ������Ϣ
        {
            qDebug() << "BufferedMedia";

            ui->pushButton_4->hide();
            ui->pushButton_2->show();

            //��������
            ui->horizontalSlider_volumn->setValue(musicplayer.volume());

            //���ý������ķ�Χ
            ui->horizontalSlider->setMinimum(0);
            ui->horizontalSlider->setMaximum(musicplayer.duration());

            ui->horizontalSlider->setValue(0);

            int h = musicplayer.duration()/3600000;
            int m = (musicplayer.duration()%3600000)/60000;
            int s = ((musicplayer.duration()%3600000)%60000)/1000;
            int ms = ((musicplayer.duration()%3600000)%60000)%1000;
            QTime Totaltime(h,m,s,ms);     //����ʱ��
            ui->label_TotalTIme->setText(Totaltime.toString("mm:ss:zzz"));
            ui->label_CurrentTime->setText("00:00:000");

            switch (CurrentPlaylist) {
            case PLAYLIST:
            {
                QString SongName   = SongList->at(playList->currentIndex())->SongName;
                QString SingerName = SongList->at(playList->currentIndex())->SingerName;

                ui->label_CurrentSongInfo->setText(SingerName+" - "+SongName);
                ui->label_CurrentSongInfo->setAlignment(Qt::AlignCenter);
                ui->label_CurrentSongInfo->setStyleSheet("QLabel{color:green}");

                ui->textBrowser_SongInfo->setTextColor(QColor(Qt::blue));
                ui->textBrowser_SongInfo->setText(SingerName+"\n"+SongName);
                break;
            }
            case PLAYLOCALLIST:
            {
                QString     SongNameStr   = SongLocalList->at(playLocalList->currentIndex())->SongName;
                QStringList SongNamelist  = SongNameStr.split("-");

                ui->label_CurrentSongInfo->setText(SongNameStr);
                ui->label_CurrentSongInfo->setAlignment(Qt::AlignCenter);
                ui->label_CurrentSongInfo->setStyleSheet("QLabel{color:green}");

                ui->textBrowser_SongInfo->setTextColor(QColor(Qt::blue));
                ui->textBrowser_SongInfo->setText(SongNamelist.at(0)+"\n"+SongNamelist.at(1));
                break;
            }
            default:
                break;
            }
            break;
        }
    default:
        break;
    }

}
void Widget::currentMediaChanged_slot() //�����л�  ���ͻ�ȡͼƬ�͸�ʵ�����
{
    qDebug() << "mediaChanged_slot";
    switch (CurrentPlaylist)
    {
    case PLAYLIST:
    {
        int index = playList->currentIndex();
        qDebug() << "current playlist index = " <<index;
        if(index >= 0 && index < SongList->size())
        {

            ui->listWidget->item(index)->setSelected(true);
            //��ȡ���
            QString LrcUrl = SongList->at(index)->LrcUrl;
            request2->setUrl(QUrl(LrcUrl));
            reply2 = manager2->get(*request2);

            //��ȡͼƬ
            QString PicUrl = SongList->at(index)->PicUrl;
            request3->setUrl(QUrl(PicUrl));
            reply3 = manager3->get(*request3);
        }
    }
        break;
    case PLAYLOCALLIST:
    {
        int index = playLocalList->currentIndex();
        ui->listWidget_locallist->item(index)->setSelected(true);
        qDebug() << "current listWidget_locallist index = " <<index;
        emit updataSonglrcShow_slot();
        emit updataSongpicShow_slot();
    }
        break;
    default:
        break;
    }
}

//ȫ������
void Widget::search_slot()
{
    QByteArray reply_content = reply->readAll();

    ResultParseJson(reply_content);     //�����������
}
//�������
void Widget::searchLrc_slot()
{
    qDebug()<<"Getting lyrics...";

    IsRecLrc = false;

    QByteArray reply2_content = reply2->readAll();
    SearchSongLrc = (QString)reply2_content;

    if(!SearchSongLrc.isEmpty())
    {
        IsRecLrc = true;
    }
    emit updataSonglrcShow_signal();
    //qDebug() << "SearchSongLrc      = " <<SearchSongLrc;
    qDebug() << "SearchSongLrc size = " <<SearchSongLrc.size();


}
//����ר��ͼƬ
void Widget::searchPic_slot()
{
    qDebug()<<"Getting Pic...";
    IsRecPic = false;
    QByteArray reply3_content = reply3->readAll();

    qDebug() << "reply3_content size = " <<reply3_content.size();
    QPixmap pix;
    pix.loadFromData(reply3_content,"JPG");
    if(pix.save("D://tmp1.jpg","JPG",100))
    {
       IsRecPic = true;
    }
    else //����ʧ�� ��������� ��γ������±���
    {
        if(!reply3_content.isEmpty())
        {
            int count = 0;
            while(!pix.save("D://tmp1.jpg","JPG",100))
            {
                if(++count == 5)
                    break;
            }
            if(count < 5)
            {
                IsRecPic = true;
            }
            qDebug() << "count = " <<count;
        }
    }
    emit updataSongpicShow_signal();
}


//�л�����ͼƬ1
void Widget::SwitchBackground1_slot()
{
    BackgroundPicPath = ":/background/background/background_1.png";
}
//�л�����ͼƬ2
void Widget::SwitchBackground2_slot()
{
    BackgroundPicPath = ":/background/background/background_2.jpg";
}
//�л�����ͼƬ3
void Widget::SwitchBackground3_slot()
{
    BackgroundPicPath = ":/background/background/background_3.jpg";
}
//�Զ��屳��ͼƬ
void Widget::customizeBackground_slot()
{
    QString picpath= QFileDialog::getOpenFileName(this,tr("File"),"",tr("Image(*.jpg *.png *.bmp)")); //ѡ���ļ�
    picpath = QDir::toNativeSeparators(picpath);
    if(!picpath.isEmpty())
    {
        BackgroundPicPath = picpath;
    }

}


//���¸����ʾ�ۺ���
void Widget::updataSonglrcShow_slot()
{
    qDebug() <<"enter updataSonglrcShow_slot...";
    switch (CurrentPlaylist)
    {
    case PLAYLIST:
    {
            //�л����
            qDebug()<<"IsRecLrc = "<<IsRecLrc;
            if(playList->currentIndex() >= 0 && playList->currentIndex() < playList->mediaCount())
            {
                if(IsRecLrc == true)
                {
                    LrcParseShow();
                    IsRecLrc = false;
                }
                else
                {
                    ui->listWidget_SongLrc->clear();
                    ui->listWidget_SongLrc->addItem("No LRC ...");
                    ui->listWidget_SongLrc->item(0)->setTextAlignment(Qt::AlignCenter);
                    ui->listWidget_SongLrc->item(0)->setTextColor(QColor(Qt::yellow));
                }
            }
            break;
    }
    case PLAYLOCALLIST:
        {
            //��ʾ���
            ui->listWidget_SongLrc->clear();
            ui->listWidget_SongLrc->addItem("No LRC ...");
            ui->listWidget_SongLrc->item(0)->setTextAlignment(Qt::AlignCenter);
            ui->listWidget_SongLrc->item(0)->setTextColor(QColor(Qt::yellow));
        }
        break;
    default:
        break;
    }
}
//����ͼƬ��ʾ�ۺ���
void Widget::updataSongpicShow_slot()
{
    qDebug() <<"enter updataSongpicShow_slot...";
    switch (CurrentPlaylist)
    {
    case PLAYLIST:
        {
            qDebug()<<"IsRecPic = "<<IsRecPic;
            //����ͼƬ
            if(IsRecPic)
            {
                timerpic->start(100); //��ʼ��ʱ
                spin_degree = 0;      //ר��ͼƬ��ʼ��ת�Ƕ�

                ui->pushButton_ShowLrc->show();
                timerpic->start(100);
                ui->pushButton_ShowLrc->setStyleSheet("QPushButton{border-image :url(D://tmp1.jpg);}" );
                ui->label_ShowSongPic->setStyleSheet("QLabel{border-radius:128px;border-image :none;}" );
                //ui->label_ShowSongPic->setStyleSheet("QLabel{border-radius:128px;border-image :url(D://tmp1.jpg);}" );
                IsRecPic = false;
            }
            else
            {
                timerpic->stop();     //ֹͣ��ʱ

                ui->pushButton_ShowLrc->show();
                ui->pushButton_ShowLrc->setStyleSheet("QPushButton{border-image :url(:/icons/icon/NoPicture.jpg);}" );

                ui->label_ShowSongPic->setStyleSheet("QLabel{border-radius:128px;"
                                                    "border-image :url(:/icons/icon/NoPicture.jpg);}" );

                ui->label_ShowSongPic->clear();  //�����һ�ε�ͼƬ��ʾ
            }

        }
        break;
    case PLAYLOCALLIST:
        {
            timerpic->stop();     //ֹͣ��ʱ

            //��ʾͼƬ    
            ui->pushButton_ShowLrc->show();
            ui->label_ShowSongPic->setStyleSheet("QLabel{border-radius:128px;"
                                                 "border-image :url(:/icons/icon/NoPicture.jpg);}" );
            ui->pushButton_ShowLrc->setStyleSheet("QPushButton{border-image :url(:/icons/icon/NoPicture.jpg);}");
        }
        break;
    default:
        break;
    }
}

//ר��ͼƬ������ʾ  ��ת
void Widget::SongPicAnimationShow_slot()
{
   //qDebug() <<"degree = "<< degree;

   QPixmap pic("D://tmp1.jpg");
   QMatrix matrix;
   matrix.rotate(spin_degree);
   ui->label_ShowSongPic->setPixmap(pic.transformed(matrix));
   if(spin_degree++ == 360)
   {
       spin_degree = 0;
   }
}

/**************************************���������********************************************/
/* ������ʽ��
 * [00:00.000] ���� : ����\n
 * [00:01.000] ���� : Ѧ֮ǫ\n
 * [00:10.91]����һ������һ��\n
 * [00:13.30]����δ����Ҫ����\n
 * [00:15.84]ʾ���ķ�������ò�����ҹ���\n
 * [00:21.08]Ҫ������һ������\n
 * [00:23.57]��Ҫ�Ե����Ķ���\n
 * [00:26.33]���ί����չʾ���˰�\n
 * [00:31.30]��żʱ��һ��һէ\n
 * [00:33.79]��Ϊ����ʱ������\n
 * [00:36.46]��Ͷ��·�������������β��\n
 * [00:41.77]������ܽ���ͬ���ͼ������� ����һͬ����\n
 * [00:50.99]Ŭ������ Ц�������綼̫��\n
 * [00:56.78]���� ��ĥȥצ��\n
 * [01:01.38]�మ��ɱ һ���и��õİ취\n
 * [01:07.34]�ʱ�һ�� ˭�ȹ���\n
 * [01:11.65]���ٽ��� ���������ﶼ̫ɵ\n
 * [01:17.58]Ϊ����ֵ�����\n
 * [01:22.82]�õ�����͸ö��� ������������ˢ\n
 * [01:27.79]���԰� ���ܻ��¹�\n
 * [01:54.31]��¹����Լ������\n
 * [01:56.79]˵��һ���˼�����\n
 * [01:59.29]ϵ�������������Ǹ���������\n
 * [02:04.12]�����ֻ������Ӣ�����ϻ�ɴ ͬ��Ц������ͯ��\n
 * [02:22.46]���ٽ��� ���ö�������̫��\n
 * [02:28.64]���� ��¶��צ��\n
 * [02:32.79]�మ��ɱ ��������õİ취\n
 * [02:38.38]��ʤ��̭ �����˰�\n
 * [02:43.07]��װ���� ƴ����Ͷ����в�\n
 * [02:48.54]��һ���߹�����\n
 * [02:53.45]�����Ǹ��õ������� ����������Ĩɱ\n
 * [02:58.60]���˰� ����ȥ����\n
 * [03:04.09]������ɳ ���������ͨ����\n
 * [03:10.01]Ϊ̰�ϧ����\n
 * [03:15.01]��Ԧ�Ű����ľ�� ��Ѩһ������\n
 * [03:19.79]��� ȴһ˿����    ���� ���Ƕ��¹�\n
 * [03:31.24]�����ˣ�����\n
 * [03:32.04]����/����/��˹���µ�\n
 * [03:32.64]�ģ�����\n
 * [03:33.26]���֣�������ϯ��������\n
 * [03:33.82]��һС���٣����� ����ë���� �ź� ��ˬ ���� ���� ���� ��˼�� �߱�ѩ\n
 * [03:34.35]�ڶ�С���٣����� �ֺ� �ų��� ��� ����� ���� ���ĳ�\n
 * [03:34.90]�����٣��λ� �Ϸ� ���ĺ� ������ ������\n
 * [03:35.47]�����٣���ƽ ��Ө �¿��� ���� ����\n
 * [03:36.00]�������٣����� ��Ȼ\n
 * [03:36.55]¼�������Ĳ� ��Big J Studio. beijing.)\n
 * [03:37.09]�������Ծ���Big J Studio. beijing.��\n
 * [03:37.86]ĸ����Tom Coyne ��Sterlingsound NYC.��\n"
*/
//��ʽ����Զ�����ʾ
void Widget::LrcParseShow()
{
    if(!playList->isEmpty() && !SearchSongLrc.isEmpty())
    {
        qDebug()<<"start LrcParseShow.......";

        ui->listWidget_SongLrc->clear();

        LrcStr->clear();
        LrcTimeStr->clear();

        if(SearchSongLrc.isEmpty())
        {
            ui->listWidget_SongLrc->addItem("NO LRC...");
        }
        else
        {
            QStringList songlrc = SearchSongLrc.split("[");  //��"["����ַ���  �������Ӵ������ַ����б���
            qDebug() << "songlrc size =  "<<songlrc.size();
            for(int i = 0 ;  i < songlrc.size() ; ++i)
            {
                //qDebug() << "i =  "<<i;
                QString RowLrc = songlrc.at(i);

                if(RowLrc.contains(":") && RowLrc.contains(".") && RowLrc.contains("]"))  //ɸѡ����ÿһ�����ݸ�ʽ �� ʱ��+���
                {
                   //��ȡʱ���ַ���
                   QString timestr = RowLrc.split("]").at(0);
                   //������תΪ3���ַ� "51"  ===>  "051"
                   QString ms = timestr.split(".").at(1);
                   ms = QString("%1").arg(ms.toInt(),3,10,QLatin1Char('0'));
                   //����ʱ���ַ����б���
                   LrcTimeStr->append(timestr.split(".").at(0)+":"+ms);
                   //����ʴ������ַ����б���
                   LrcStr->append(QString(RowLrc.split("]").at(1)).remove("\n"));
                }
            }
         }

        //�����ʾ
        for(int i = 0 ; i < LrcStr->size() ; ++i)
        {
            ui->listWidget_SongLrc->insertItem(i,LrcStr->at(i));
        }
    }
    qDebug()<<"end LrcParseShow.......";
}
//������������
void Widget::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0); //�л��������б�

    QString str = ui->lineEdit->text();
    int limit = 20; //���ø�������
    //ѡ�񲥷�����Դ
    int SelectSourceIndex = ui->comboBox->currentIndex();
    switch (SelectSourceIndex) {
    case 0://������API
        request->setUrl(QUrl(QString("https://api.bzqll.com/music/netease/search?key=579621905&s=%1&type=song&limit=%2&offset=0").arg(str).arg(limit)));
        break;
    case 1://�ṷAPI
        request->setUrl(QUrl(QString("https://api.bzqll.com/music/kugou/search?key=579621905&s=%1&limit=%2&type=song&offset=0").arg(str).arg(limit)));
        break;
    case 2://QQAPI
        request->setUrl(QUrl(QString("https://api.bzqll.com/music/tencent/search?key=579621905&s=%1&limit=%2&type=song&offset=0").arg(str).arg(limit)));
        break;
    default:
        break;
    }

    reply = manager->get(*request); //����get����
}
/* ���ص�jsonʾ��
{
"result":"SUCCESS",
"code":200,
"data":[
        {
        "id":"001rTpCK2nGJuK",
        "name":"����",
        "time":254,
        "singer":"������",
        "url":"https://api.bzqll.com/music/tencent/url?id=001rTpCK2nGJuK&key=579621905",
        "pic":"https://api.bzqll.com/music/tencent/pic?id=001rTpCK2nGJuK&key=579621905",
        "lrc":"https://api.bzqll.com/music/tencent/lrc?id=001rTpCK2nGJuK&key=579621905"
        },
        {
        "id":"0045jgZb2Ak85A",
        "name":"When You're Bored",
        "time":193,
        "singer":"Joywave",
        "url":"https://api.bzqll.com/music/tencent/url?id=0045jgZb2Ak85A&key=579621905",
        "pic":"https://api.bzqll.com/music/tencent/pic?id=0045jgZb2Ak85A&key=579621905",
        "lrc":"https://api.bzqll.com/music/tencent/lrc?id=0045jgZb2Ak85A&key=579621905"
        }
    ]
}
*/
//�������ؽ��
void Widget::ResultParseJson(QByteArray &reply_content)
{
    //�����ת��Ϊ��׼�ַ���
    QString res = QString::fromStdString(reply_content.toStdString()).toUtf8();

    QJsonDocument document;
    QJsonParseError ParseError;
    //����QJSON�ĵ�
    document = QJsonDocument::fromJson(res.toUtf8(),&ParseError);

    if(!document.isNull() && ParseError.error == QJsonParseError::NoError)  //�ļ��ǿ��ҽ���û��
    {
        if(document.isObject()) //�Ƿ�Ϊjson����
        {
            QJsonObject object = document.object(); //��ȡQJSON�ĵ�����
            if(!object.isEmpty())
            {
                if(object.contains("result"))   //����������
                {
                    QJsonValue value = object.value("result");
                    if(value.isString())
                    {
                        QString result = value.toString();
                        qDebug() <<"result = "<<result ;
                    }

                }
                if(object.contains("code"))    //�������󷵻ش���
                {
                    QJsonValue value = object.value("code");
                    if(value.isDouble())
                    {
                        int code = value.toDouble();
                        qDebug() <<"code = "<<code ;
                        if(code != 200) //����ʧ��
                        {
                            QMessageBox::warning(this,"","search failed");
                            return;
                        }
                    }
                }
                if(object.contains("data"))     //��������
                {
                    CleanWebPlayList();         //������粥���б�
                    SearchSongLrc.clear();      //��ո��

                    QJsonValue value = object.value("data"); //����data����
                    if(value.isArray())
                    {
                        QJsonArray data  = value.toArray();
                        int nsize = data.size();
                        for(int i =0  ;  i < nsize ; ++i)
                        {
                            SongInfo *song = new SongInfo;

                            QJsonValue value = data.at(i);
                            if(value.isObject() && !value.isNull())
                            {
                                QJsonObject data_object = value.toObject();
                                if(data_object.contains("id"))  //��������id
                                {
                                    QJsonValue value = data_object.value("id");
                                    if(value.isString())
                                    {
                                        QString id = value.toString();
                                        qDebug() <<"id = "<<id ;

                                        song->SongID = id;
                                    }
                                }
                                if(data_object.contains("name")) //����������
                                {
                                    QJsonValue value = data_object.value("name");
                                    if(value.isString())
                                    {
                                        QString name = value.toString();
                                        qDebug() <<"name = "<<name ;

                                        song->SongName = name;
                                    }
                                }
                                if(data_object.contains("singer")) //����������
                                {
                                    QJsonValue value = data_object.value("singer");
                                    if(value.isString())
                                    {
                                        QString singer = value.toString();
                                        qDebug() <<"singer = "<<singer ;

                                        song->SingerName = singer;
                                    }
                                }
                                if(data_object.contains("url")) //��������URL
                                {
                                    QJsonValue value = data_object.value("url");
                                    if(value.isString())
                                    {
                                        QString url = value.toString();
                                        qDebug() <<"url = "<<url ;

                                        song->SongUrl = url;
                                    }
                                }
                                if(data_object.contains("pic")) //����ͼƬURL
                                {
                                    QJsonValue value = data_object.value("pic");
                                    if(value.isString())
                                    {
                                        QString pic = value.toString();
                                        qDebug() <<"pic = "<<pic ;

                                        song->PicUrl = pic;
                                    }
                                }
                                if(data_object.contains("lrc")) //�������URL
                                {
                                    QJsonValue value = data_object.value("lrc");
                                    if(value.isString())
                                    {
                                        QString lrc = value.toString();
                                        qDebug() <<"lrc = "<<lrc ;

                                        song->LrcUrl = lrc;
                                    }
                                }

                            }
                            //�ݴ������Ϣ
                            SongList->append(song);
                            //SongList->insert(i,song);

                            //��ӵ���ʾ�б���
                            QString showinfo = song->SingerName + "              "+song->SongName;
                            ui->listWidget->addItem(showinfo);

                            //��ӵ������б���
                            QString SongUrl = song->SongUrl.append("&br=999000");
                            playList ->addMedia(QUrl(SongUrl));
                        }

                    }

                }


            }
        }
    }
}
//��ո������粥���б�
void Widget::CleanWebPlayList()
{
    if(!SongList->isEmpty())
    {

        //��ո���������Ϣ�б�
        //  ���ͷ��ڴ�
        for(int i = 0 ; i < SongList->size(); ++i)
        {
            SongInfo *tmp = SongList->at(i);
            delete tmp;
        }
        //  ������б�
        SongList->clear();

        //�����ʾ�б�
        ui->listWidget->clear();

        //��ղ����б�
        playList->clear();

        qDebug() <<"SongList size = " <<SongList->size();
        qDebug() <<"playList mediaCount size = " <<playList->mediaCount();
    }
}
//��ʱ����  --ms
void Widget::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/**************************************���ſ���********************************************/
//����
void Widget::on_pushButton_4_clicked()
{
    ui->pushButton_4->hide();
    ui->pushButton_2->show();
    int page = ui->stackedWidget->currentIndex();
    switch (page)
    {
    case 0 ://�����б�
        qDebug() << "playList_CurrentRow = " << playList->currentIndex();
        qDebug() << "CurrentRow = " << ui->listWidget->currentRow();
        if(ui->listWidget->currentRow() != playList->currentIndex())
        {
            musicplayer.stop();
            musicplayer.setPlaylist(playList);
            CurrentPlaylist = PLAYLIST;
            int CurrentRow = ui->listWidget->currentRow();
            playList->setCurrentIndex(CurrentRow);
            musicplayer.play();
        }
        else
        {
           musicplayer.play();
        }
        break;
    case 1://�����б�
        qDebug() << "playLocalList_CurrentRow = " << playLocalList->currentIndex();
        qDebug() << "local_CurrentRow = " << ui->listWidget_locallist->currentRow();
        if(ui->listWidget_locallist->currentRow() != playLocalList->currentIndex())  //
        {
            musicplayer.stop();
            musicplayer.setPlaylist(playLocalList);
            CurrentPlaylist = PLAYLOCALLIST;
            int CurrentRow = ui->listWidget_locallist->currentRow();
            playLocalList->setCurrentIndex(CurrentRow);
            musicplayer.play();
        }
        else  //����
        {
            musicplayer.play();
        }
        break;
    default:
        break;
    }

    if(musicplayer.state() == QMediaPlayer::PlayingState) //������ת
    {
        timerpic->start(100);
    }
}
//��ͣ
void Widget::on_pushButton_2_clicked()
{
    ui->pushButton_2->hide();
    ui->pushButton_4->show();

    musicplayer.pause();
    //qDebug() << "musicplayer.state  ="<<musicplayer.state();
    if(musicplayer.state() == QMediaPlayer::PausedState)    //��ͣ��ת
    {
        timerpic->stop();
    }
}
//��һ��
void Widget::on_pushButton_5_clicked()
{
    switch (CurrentPlaylist) {
    case PLAYLIST:
    {
        int index = playList->currentIndex();
        if(index == playList->mediaCount()-1)
        {
            index = 0;
        }
        else
        {
            index++;
        }
        playList->setCurrentIndex(index);
        musicplayer.play();
        ui->listWidget->item(index)->setSelected(true);
        //playList->next();  //������б�˳�򲥷� ����ѭ��
        break;
    }
    case PLAYLOCALLIST:
    {
        int index = playLocalList->currentIndex();
        if(index == playLocalList->mediaCount()-1)
        {
            index = 0;
        }
        else
        {
            index++;
        }
        playLocalList->setCurrentIndex(index);
        musicplayer.play();
        ui->listWidget_locallist->item(index)->setSelected(true);
        //playLocalList->next();//������б�˳�򲥷� ����ѭ��
        break;
    }
    default:
        break;
    }
}
//��һ��
void Widget::on_pushButton_3_clicked()
{
    switch (CurrentPlaylist) {
    case PLAYLIST:
    {
        int index = playList->currentIndex();
        if(index == 0)
        {
             index = playList->mediaCount()-1;
        }
        else
        {
             index--;
        }
        playList->setCurrentIndex(index);
        musicplayer.play();
        ui->listWidget->item(index)->setSelected(true);
        // playList->previous();//������б�˳�򲥷� ����ѭ��
        break;
    }
    case PLAYLOCALLIST:
    {
        int index = playLocalList->currentIndex();
        if(index == 0)
        {
             index = playLocalList->mediaCount()-1;
        }
        else
        {
             index--;
        }
        playLocalList->setCurrentIndex(index);
        musicplayer.play();
        ui->listWidget_locallist->item(index)->setSelected(true);
        //playLocalList->previous();//������б�˳�򲥷�  ����ѭ��
        break;
    }
    default:
        break;
    }
}
//�л��б��ŷ�ʽ
void Widget::on_pushButton_6_clicked()
{
    static int i = 0;

    if(++i == 4)
    {
        i = 0;
    }
    if(i == 0)
    {
       playmode = "�б�ѭ������";

       ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/inOrder.png);}");
       //ui->pushButton_6->setIcon(QIcon(":/icons/icon/inOrder.png"));
       ui->pushButton_6->setToolTip(QString(playmode.toUtf8()));

       playList->setPlaybackMode(QMediaPlaylist::Loop); //�б�ѭ������
    }
    else if(i == 1)
    {
        playmode = "�б�˳�򲥷�";

        ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/order_play.png);}");
        //ui->pushButton_6->setIcon(QIcon(":/icons/icon/order_play.png"));
        ui->pushButton_6->setToolTip(QString(playmode.toUtf8()));

        playList->setPlaybackMode(QMediaPlaylist::Sequential); //�б�˳�򲥷�
    }
    else if(i == 2)
    {
        playmode = "�������";

        ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/shuffle.png);}");
        //ui->pushButton_6->setIcon(QIcon(":/icons/icon/shuffle.png"));
        ui->pushButton_6->setToolTip(QString(playmode.toUtf8()));

        playList->setPlaybackMode(QMediaPlaylist::Random);//�������
    }
    else if(i == 3)
    {
        playmode = "����ѭ��";

        ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/singleCycle.png);}");
        //ui->pushButton_6->setIcon(QIcon(":/icons/icon/singleCycle.png"));
        ui->pushButton_6->setToolTip(QString(playmode.toUtf8()));

        playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//����ѭ��
    }
}
//˫����������  --�����б�
void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    //int CurrentRow = ui->listWidget->currentRow();  //˫�����ǵڼ���   �кŴ�0��ʼ
    int CurrentRow = index.row(); //˫�����ǵڼ���   �кŴ�0��ʼ
    if(CurrentRow != playList->currentIndex())
    {
        //��������
        musicplayer.stop();
        musicplayer.setPlaylist(playList);
        CurrentPlaylist = PLAYLIST;
        playList->setCurrentIndex(CurrentRow);//��������
        musicplayer.play();
    }
}
//˫����������  --�����б�
void Widget::on_listWidget_locallist_doubleClicked(const QModelIndex &index)
{

    int CurrentRow = index.row();   //˫�����ǵڼ���   �кŴ�0��ʼ
    if(CurrentRow != playLocalList->currentIndex())
    {
        //��������
        musicplayer.setPlaylist(playLocalList);
        CurrentPlaylist = PLAYLOCALLIST;
        playLocalList->setCurrentIndex(CurrentRow);
        musicplayer.play();
        qDebug()<< musicplayer.currentMedia().canonicalUrl();
    }
}
//�������Ž���
void Widget::on_horizontalSlider_sliderMoved(int position)
{
    musicplayer.setPosition((qint64)(position));
}
//����������С
void Widget::on_horizontalSlider_volumn_valueChanged(int value)
{
     musicplayer.setVolume(value);
}




/**************************************�������ֲ���********************************************/
//��ӱ�������
void Widget::on_pushButton_13_clicked()
{
    QSqlQuery query;

    QStringList list= QFileDialog::getOpenFileNames(this,tr("�ļ�"),"D:/",tr("��Ƶ�ļ�(*.mp3)")); //ѡ���ļ�
    for(int i = 0 ; i < list.size() ; ++i)
    {
        QString path = QDir::toNativeSeparators(list.at(i)); //��Ƶ�ļ��ľ���·��
        if(!path.isEmpty())
        {
            SongInfo *song =  new SongInfo;
            song->SongUrl  =  path;
            qDebug()<<"path = "<<path;

            QString songfilename = path;
            songfilename.replace("\\","\\\\\\\\");  //���mysql�Ե� ��б�� ������
            qDebug()<<"songfilename = "<<songfilename;

            QString name=path.split("\\").last();
            name.remove(QString(".mp3"));
            song->SongName = name;
            qDebug()<<"name = "<<name;

            bool result = query.exec(QString("insert into localMusic values (%1,'%2','%3','%4','%5')").arg(qrand()%10000).arg(name).arg(songfilename).arg("").arg(""));//��ӵ����ݿ���
            if(result == true)  //�������ظ�
            {
                SongLocalList->append(song);  //�����ʱ���ز����б�
                playLocalList->addMedia(QUrl::fromLocalFile(path));
            }
            else//�����ظ�
            {
              QMessageBox::critical(this,nullptr,"The current song already exists!!!");
            }
        }
    }
    ShowLocalSong();
}
//�����ݿ���ر�������
void Widget::LoadLocalSongFromMysql()
{
    //QSqlQuery       query;
    QSqlTableModel  model;
    //query.exec(QString("select * from localmusic"));
    model.setTable("localmusic");
    model.select();
    for(int i = 0 ; i < model.rowCount();++i)
    {
        QString name         = model.record(i).value("MusicName").toString();
        QString songfilename = model.record(i).value("SongFileName").toString();
        QString lrcfilename  = model.record(i).value("LrcFileName").toString();

        SongInfo *song  = new SongInfo;
        song->SongName  = name;
        song->SongUrl   = songfilename;
        song->LrcUrl    = lrcfilename;
        SongLocalList->append(song);

        //songfilename.replace("\\\\","\\");
        playLocalList->addMedia(QUrl::fromLocalFile(songfilename));
        qDebug() << "songfilename = " << songfilename;
    }

    ShowLocalSong();
}
//��ʾ��������
void Widget::ShowLocalSong()
{
    ui->stackedWidget->setCurrentIndex(1);
    for(int i = 0 ; i <SongLocalList->size() ; ++i )//��������б���ʾ
    {
        QListWidgetItem *item = ui->listWidget_locallist->takeItem(0);
        delete item;
    }
    for(int i = 0 ; i <SongLocalList->size() ; ++i )//���±����б���ʾ
    {
        ui->listWidget_locallist->insertItem(i,SongLocalList->at(i)->SongName);
    }
}
//��ձ��ز����б�
void Widget::ClearSongLocalPlayList()
{
    for(int i = 0 ; i <SongLocalList->size() ; ++i )
    {
        SongInfo*tmp = SongLocalList->back();
        SongLocalList->pop_back();
        delete tmp;  //�ͷ��ڴ�
    }
    //�����ʾ�б�
    ui->listWidget_locallist->clear();
    //��ղ����б�
    playLocalList->clear();
}
//��ʾ���粥���б�
void Widget::on_pushButton_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
//��ʾ���ز����б�
void Widget::on_pushButton_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
//������ȡ������
void Widget::on_pushButton_Mute_clicked()
{
    static bool MuteFlag = true;
    if(MuteFlag)   //����
    {
        FrontOfMuteVolume = musicplayer.volume(); //���澲��ǰ��������С

        musicplayer.setVolume(0);
        ui->horizontalSlider_volumn->setValue(0);
        ui->pushButton_Mute->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/muteVolume.png);}");


        MuteFlag = false;
    }
    else          //ȡ������
    {
        musicplayer.setVolume(FrontOfMuteVolume);
        ui->horizontalSlider_volumn->setValue(FrontOfMuteVolume);

        ui->pushButton_Mute->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/volume.png);}");

        MuteFlag = true;
    }
}
//�л�ר��ҳ��
void Widget::on_pushButton_ShowLrc_clicked()
{
    int page = ui->stackedWidget_menu->currentIndex();
    if(page == 0)
    {
        ui->stackedWidget_menu->setCurrentIndex(1);
    }
    else if(page == 1)
    {
        ui->stackedWidget_menu->setCurrentIndex(0);

    }
}



/**************************************���ڿ���********************************************/
//�ر�
void Widget::on_pushButton_WinClose_clicked()
{
    int res = QMessageBox::critical(this,"","You are sure to close the program?","Yes","No");
    if(res == 0)  //yes
    {
        this->close();
    }
}
//��С��
void Widget::on_pushButton_WinMinsize_clicked()
{
    this->showMinimized();
}
//����¼� -- ѹ��
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
     {
         IsDrag = true;
         MousePositon = event->globalPos() -  this->pos();
         event->accept();
     }
}
//����¼� -- �ƶ�
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(IsDrag && (event->buttons() == Qt::LeftButton))
    {
        move(event->globalPos() - MousePositon);
        event->accept();
    }
}
//����¼� -- �ͷ�
void Widget::mouseReleseEvent(QMouseEvent *event)
{
     IsDrag = false;
     event->accept();
}
//���ñ���ͼƬ  --��ͼ�¼�
void Widget::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    painter.drawPixmap(0,0,width(),height(),QPixmap(BackgroundPicPath)); //ͨ����ͼ�����ñ���ͼƬ

    /*this->setStyleSheet("Widget{"
                        "border-image: url(D://tmp1.jpg);"          //ͨ��setStyleSheet
                        "}");*/
    update();
}


