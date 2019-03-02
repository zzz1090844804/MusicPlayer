#include "widget.h"
#include "ui_widget.h"

#define  LOCALMUSIC

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //qDebug()<<manager->supportedSchemes();  //检查支持的协议

    manager   =  new  QNetworkAccessManager;
    manager2  =  new  QNetworkAccessManager;
    manager3  =  new  QNetworkAccessManager;
    request   =  new  QNetworkRequest;
    request2  =  new  QNetworkRequest;
    request3  =  new  QNetworkRequest;

    SongList      = new QVector<SongInfo*>;     //创建网络播放列表
    SongLocalList = new QVector<SongInfo*>;//创建本地播放列表

    playList      = new QMediaPlaylist; //创建网络播放列表
    playLocalList = new QMediaPlaylist; //创建本地播放列表

    LrcStr        = new QStringList;   //创建歌词行列表
    LrcTimeStr    = new QStringList;   //创建歌词行列表

    timerpic      = new QTimer;         //专辑图片旋转定时器

    InitWholeUI();                      //初始化整体界面

    InitSignalSlots();                  //初始化信号与槽的函数

    LoadLocalSongFromMysql();           //从数据库加载本地音乐

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



/**************************************初始化********************************************/
//切换背景绑定下拉菜单
void Widget::SwitchBackground()
{
    backmenu  = new QMenu;
    back1     = new QAction(backmenu);  //默认背景一
    back2     = new QAction(backmenu);  //默认背景二
    back3     = new QAction(backmenu);  //默认背景三
    customize = new QAction(backmenu);  //自定义

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

    ui->pushButton_ChangeSkin->setStyleSheet("QPushButton::menu-indicator{image:none;} QPushButton{border-image: url(:/icons/icon/skin.png);}"); //去掉下拉三角
}
//初始化整体界面
void Widget::InitWholeUI()
{
    BackgroundPicPath = ":/background/background/background_1.png";  //背景
    //去掉标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置APP图标
    this->setWindowIcon(QIcon(":/icons/icon/WinIcon.jpg"));
    //设置APP名称
    this->setWindowTitle("MusicPlayer");

    //搜索     按钮属性设置
    ui->pushButton->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/search.png);}"); //无边框

    //暂停     按钮属性设置
    ui->pushButton_2->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/mediaPause.png);}");
    ui->pushButton_2->hide();

    //上一首   按钮属性设置
    ui->pushButton_3->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/playPreSong.png);}");
    //播放    按钮属性设置
    ui->pushButton_4->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/mediaPlay.png);}");


    //下一首  按钮属性设置
    ui->pushButton_5->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/playNextSong.png);}");

    //播放模式 按钮属性设置
    playmode = "循环顺序播放";
    //ui->pushButton_6->setAutoFillBackground(true);
    //ui->pushButton_6->setIcon(QIcon(":/icons/icon/inOrder.png"));
    //ui->pushButton_6->setToolTip(playmode);
    ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/inOrder.png);}");

    //添加本地音乐  按钮属性设置
    ui->pushButton_13->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/addSong.png);}");

    //打开网络音乐列表  按钮属性设置
    ui->pushButton_14->setStyleSheet("QPushButton{border:none;} QPushButton::hover{background-color: rgba(255, 255, 255,150);}");

    //打开本地音乐列表  按钮属性设置
    ui->pushButton_15->setStyleSheet("QPushButton{border:none;} QPushButton::hover{background-color: rgba(255, 255, 255,150);}");

    //切换到专辑页面
    ui->pushButton_ShowLrc->setStyleSheet("QPushButton{border:none;}");
    ui->pushButton_ShowLrc->hide();

    //声音      按钮属性设置
    ui->pushButton_Mute->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/volume.png);}");

    //背景
    SwitchBackground();


    //网络播放列表
    ui->listWidget->setStyleSheet("QListWidget{"
                                  "background-color:transparent;"
                                  "border:none;"
                                  "}");

    //本地播放列表
    ui->listWidget_locallist->setStyleSheet("QListWidget{"
                                  "background-color:transparent;"
                                  "border:none;"
                                  "}");
    //专辑简要信息
    ui->textBrowser_SongInfo->setStyleSheet("background-color:transparent;border:none;");

    //显示歌词
    ui->listWidget_SongLrc->setStyleSheet("QListWidget{"
                                   "background-color:transparent;"
                                   "border:none;"
                                   "}");

    //搜索输入框
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

    //播放源选择
    ui->comboBox->setStyleSheet("QComboBox{"
                                "background-color:transparent;"
                                "font:bold 14px;"
                                "color:rgba(0,0,0,100);"
                                "background-color: rgba(255, 255, 255,150);"
                                "}");

}
//初始化信号与槽的函数
void Widget::InitSignalSlots()
{
    //获取歌曲的时间
    connect(&musicplayer,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(currentMediaChanged_slot()));
    connect(&musicplayer,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged_slot(qint64)));                  // 歌曲进度改变
    connect(&musicplayer,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(mediaStatusChanged_slot(QMediaPlayer::MediaStatus)));//歌曲状态改变

    connect(manager ,&QNetworkAccessManager::finished,this,&Widget::search_slot);
    connect(manager2,&QNetworkAccessManager::finished,this,&Widget::searchLrc_slot);
    connect(manager3,&QNetworkAccessManager::finished,this,&Widget::searchPic_slot);
//切换背景
    connect(back1,    &QAction::triggered,this,&Widget::SwitchBackground1_slot);
    connect(back2,    &QAction::triggered,this,&Widget::SwitchBackground2_slot);
    connect(back3,    &QAction::triggered,this,&Widget::SwitchBackground3_slot);
    connect(customize,&QAction::triggered,this,&Widget::customizeBackground_slot);
//切换 专辑歌词和图片
    connect(this,&Widget::updataSonglrcShow_signal,this,&Widget::updataSonglrcShow_slot);
    connect(this,&Widget::updataSongpicShow_signal,this,&Widget::updataSongpicShow_slot);
//定时器
    connect(timerpic,&QTimer::timeout,this,&Widget::SongPicAnimationShow_slot);


}


/**************************************槽函数********************************************/
void Widget::positionChanged_slot(qint64 position) //播放歌曲进度改变，改变时间显示，歌词改变
{
    //歌曲进度条改变
    ui->horizontalSlider->setValue(position);

    //歌曲时间改变
    int h = position/3600000;
    int m = (position%3600000)/60000;
    int s = ((position%3600000)%60000)/1000;
    int ms = ((position%3600000)%60000)%1000;
    QTime CurrentTime (h,m,s,ms);
    ui->label_CurrentTime->setText(CurrentTime.toString("mm:ss:zzz"));

    //当前歌词改变
    switch (CurrentPlaylist) {
    case PLAYLIST:
        for(int i = 0;i < LrcTimeStr->count();i++)
        {
            if(CurrentTime <= QTime::fromString(LrcTimeStr->at(i),"mm:ss:zzz"))  //找到要正在播放的歌词
            {
                if(i == 0)//第一行
                {
                    ui->listWidget_SongLrc->item(i)->setTextColor("red");
                    ui->listWidget_SongLrc->item(i)->setSelected(true);                     //聚焦
                    ui->listWidget_SongLrc->scrollToItem(ui->listWidget_SongLrc->item(i));  //滚动条自动聚焦
                    ui->listWidget_SongLrc->item(i)->setTextAlignment(Qt::AlignCenter);     //居中
                    //ui->listWidget_SongLrc->setResizeMode(QListView::Adjust);  //自适应
                }
                else
                {
                    ui->listWidget_SongLrc->item(i-1)->setTextColor("red");
                    ui->listWidget_SongLrc->item(i-1)->setSelected(true);
                    ui->listWidget_SongLrc->scrollToItem(ui->listWidget_SongLrc->item(i-1));
                    ui->listWidget_SongLrc->item(i-1)->setTextAlignment(Qt::AlignCenter); //居中
                    //ui->listWidget_SongLrc->setResizeMode(QListView::Adjust);  //自适应
                }
                break;
            }
            else //播放过的歌词 歌词复原
            {
                ui->listWidget_SongLrc->item(i)->setTextColor("black");
                ui->listWidget_SongLrc->item(i)->setSelected(false);
                ui->listWidget_SongLrc->scrollToItem(ui->listWidget_SongLrc->item(i));
                ui->listWidget_SongLrc->item(i)->setTextAlignment(Qt::AlignCenter); //居中
                //ui->listWidget_SongLrc->setResizeMode(QListView::Adjust);  //自适应
            }
        }
        break;
    case PLAYLOCALLIST:

        break;
    default:
        break;
    }
}
void Widget::mediaStatusChanged_slot(QMediaPlayer::MediaStatus status)  //播放器状态变化
{
    switch (status)
    {
    case QMediaPlayer::BufferedMedia:       //播放器已完全缓冲当前媒体。  修改总时长 和进度条 以及相关信息
        {
            qDebug() << "BufferedMedia";

            ui->pushButton_4->hide();
            ui->pushButton_2->show();

            //设置音量
            ui->horizontalSlider_volumn->setValue(musicplayer.volume());

            //设置进度条的范围
            ui->horizontalSlider->setMinimum(0);
            ui->horizontalSlider->setMaximum(musicplayer.duration());

            ui->horizontalSlider->setValue(0);

            int h = musicplayer.duration()/3600000;
            int m = (musicplayer.duration()%3600000)/60000;
            int s = ((musicplayer.duration()%3600000)%60000)/1000;
            int ms = ((musicplayer.duration()%3600000)%60000)%1000;
            QTime Totaltime(h,m,s,ms);     //音乐时长
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
void Widget::currentMediaChanged_slot() //歌曲切换  发送获取图片和歌词的请求
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
            //获取歌词
            QString LrcUrl = SongList->at(index)->LrcUrl;
            request2->setUrl(QUrl(LrcUrl));
            reply2 = manager2->get(*request2);

            //获取图片
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

//全局搜索
void Widget::search_slot()
{
    QByteArray reply_content = reply->readAll();

    ResultParseJson(reply_content);     //搜索结果解析
}
//搜索歌词
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
//搜索专辑图片
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
    else //保存失败 如果有数据 多次尝试重新保存
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


//切换背景图片1
void Widget::SwitchBackground1_slot()
{
    BackgroundPicPath = ":/background/background/background_1.png";
}
//切换背景图片2
void Widget::SwitchBackground2_slot()
{
    BackgroundPicPath = ":/background/background/background_2.jpg";
}
//切换背景图片3
void Widget::SwitchBackground3_slot()
{
    BackgroundPicPath = ":/background/background/background_3.jpg";
}
//自定义背景图片
void Widget::customizeBackground_slot()
{
    QString picpath= QFileDialog::getOpenFileName(this,tr("File"),"",tr("Image(*.jpg *.png *.bmp)")); //选择文件
    picpath = QDir::toNativeSeparators(picpath);
    if(!picpath.isEmpty())
    {
        BackgroundPicPath = picpath;
    }

}


//更新歌词显示槽函数
void Widget::updataSonglrcShow_slot()
{
    qDebug() <<"enter updataSonglrcShow_slot...";
    switch (CurrentPlaylist)
    {
    case PLAYLIST:
    {
            //切换歌词
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
            //显示歌词
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
//更新图片显示槽函数
void Widget::updataSongpicShow_slot()
{
    qDebug() <<"enter updataSongpicShow_slot...";
    switch (CurrentPlaylist)
    {
    case PLAYLIST:
        {
            qDebug()<<"IsRecPic = "<<IsRecPic;
            //更新图片
            if(IsRecPic)
            {
                timerpic->start(100); //开始计时
                spin_degree = 0;      //专辑图片初始旋转角度

                ui->pushButton_ShowLrc->show();
                timerpic->start(100);
                ui->pushButton_ShowLrc->setStyleSheet("QPushButton{border-image :url(D://tmp1.jpg);}" );
                ui->label_ShowSongPic->setStyleSheet("QLabel{border-radius:128px;border-image :none;}" );
                //ui->label_ShowSongPic->setStyleSheet("QLabel{border-radius:128px;border-image :url(D://tmp1.jpg);}" );
                IsRecPic = false;
            }
            else
            {
                timerpic->stop();     //停止计时

                ui->pushButton_ShowLrc->show();
                ui->pushButton_ShowLrc->setStyleSheet("QPushButton{border-image :url(:/icons/icon/NoPicture.jpg);}" );

                ui->label_ShowSongPic->setStyleSheet("QLabel{border-radius:128px;"
                                                    "border-image :url(:/icons/icon/NoPicture.jpg);}" );

                ui->label_ShowSongPic->clear();  //清除上一次的图片显示
            }

        }
        break;
    case PLAYLOCALLIST:
        {
            timerpic->stop();     //停止计时

            //显示图片    
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

//专辑图片动画显示  旋转
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

/**************************************搜索与解析********************************************/
/* 歌曲格式：
 * [00:00.000] 作曲 : 郭顶\n
 * [00:01.000] 作词 : 薛之谦\n
 * [00:10.91]东打一下西戳一下\n
 * [00:13.30]动物未必需要尖牙\n
 * [00:15.84]示爱的方法有礼貌或是我管它\n
 * [00:21.08]要将情人一口吞下\n
 * [00:23.57]还要显得温文尔雅\n
 * [00:26.33]螳螂委屈的展示旧伤疤\n
 * [00:31.30]求偶时候一惊一乍\n
 * [00:33.79]因为害怕时常倒挂\n
 * [00:36.46]走投无路的情况下舍弃了尾巴\n
 * [00:41.77]如果不能将它同化就寄生于它 大不了一同腐化\n
 * [00:50.99]努力进化 笑动物世界都太假\n
 * [00:56.78]祖先 已磨去爪牙\n
 * [01:01.38]相爱相杀 一定有更好的办法\n
 * [01:07.34]攀比一下 谁先跪下\n
 * [01:11.65]不再进化 动物世界里都太傻\n
 * [01:17.58]为情表现到浮夸\n
 * [01:22.82]得到了你就该丢下 人性来不及粉刷\n
 * [01:27.79]所以啊 人总患孤寡\n
 * [01:54.31]麋鹿本来约在树下\n
 * [01:56.79]说好一起浪迹天涯\n
 * [01:59.29]系上铃铛还在往那个方向挣扎\n
 * [02:04.12]如果有只豺狼它英勇披上婚纱 同伴笑他读过童话\n
 * [02:22.46]别再进化 别让动物世界太假\n
 * [02:28.64]我们 该露出爪牙\n
 * [02:32.79]相爱相杀 别再想更好的办法\n
 * [02:38.38]优胜劣汰 自舔伤疤\n
 * [02:43.07]假装进化 拼命想和动物有差\n
 * [02:48.54]玩一出高贵优雅\n
 * [02:53.45]在人们腐烂的欲望下 兽性来不及抹杀\n
 * [02:58.60]算了吧 懒得去挣扎\n
 * [03:04.09]人类用沙 想捏出梦里通天塔\n
 * [03:10.01]为贪念不惜代价\n
 * [03:15.01]驾驭着昂贵的木马 巢穴一层层叠加\n
 * [03:19.79]最后啊 却一丝不挂    别害怕 我们都孤寡\n
 * [03:31.24]制作人：郭顶\n
 * [03:32.04]编曲/钢琴/贝斯：陈迪\n
 * [03:32.64]鼓：王斌\n
 * [03:33.26]弦乐：国际首席爱乐乐团\n
 * [03:33.82]第一小提琴：李朋 王大毛庞阔 张浩 杨爽 李曦 刘潇 高言 杨思宇 倪冰雪\n
 * [03:34.35]第二小提琴：简蓓 阎红 张晨迪 唐昕 侯宇红 张雷 徐文超\n
 * [03:34.90]中提琴：何辉 毕芳 武文豪 陈欣欣 王羽沛\n
 * [03:35.47]大提琴：张平 郎莹 陈俊杰 孙艺 邵鑫\n
 * [03:36.00]低音提琴：周旭 段然\n
 * [03:36.55]录音：汝文博 （Big J Studio. beijing.)\n
 * [03:37.09]混音：赵靖（Big J Studio. beijing.）\n
 * [03:37.86]母带：Tom Coyne （Sterlingsound NYC.）\n"
*/
//歌词解析自定义显示
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
            QStringList songlrc = SearchSongLrc.split("[");  //以"["拆分字符串  将各个子串放入字符串列表中
            qDebug() << "songlrc size =  "<<songlrc.size();
            for(int i = 0 ;  i < songlrc.size() ; ++i)
            {
                //qDebug() << "i =  "<<i;
                QString RowLrc = songlrc.at(i);

                if(RowLrc.contains(":") && RowLrc.contains(".") && RowLrc.contains("]"))  //筛选数据每一行数据格式 是 时间+歌词
                {
                   //获取时间字符串
                   QString timestr = RowLrc.split("]").at(0);
                   //将毫秒转为3个字符 "51"  ===>  "051"
                   QString ms = timestr.split(".").at(1);
                   ms = QString("%1").arg(ms.toInt(),3,10,QLatin1Char('0'));
                   //存入时间字符串列表中
                   LrcTimeStr->append(timestr.split(".").at(0)+":"+ms);
                   //将歌词存入歌词字符串列表中
                   LrcStr->append(QString(RowLrc.split("]").at(1)).remove("\n"));
                }
            }
         }

        //歌词显示
        for(int i = 0 ; i < LrcStr->size() ; ++i)
        {
            ui->listWidget_SongLrc->insertItem(i,LrcStr->at(i));
        }
    }
    qDebug()<<"end LrcParseShow.......";
}
//发送搜索请求
void Widget::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0); //切换到搜索列表

    QString str = ui->lineEdit->text();
    int limit = 20; //设置歌曲数量
    //选择播放器的源
    int SelectSourceIndex = ui->comboBox->currentIndex();
    switch (SelectSourceIndex) {
    case 0://网易云API
        request->setUrl(QUrl(QString("https://api.bzqll.com/music/netease/search?key=579621905&s=%1&type=song&limit=%2&offset=0").arg(str).arg(limit)));
        break;
    case 1://酷狗API
        request->setUrl(QUrl(QString("https://api.bzqll.com/music/kugou/search?key=579621905&s=%1&limit=%2&type=song&offset=0").arg(str).arg(limit)));
        break;
    case 2://QQAPI
        request->setUrl(QUrl(QString("https://api.bzqll.com/music/tencent/search?key=579621905&s=%1&limit=%2&type=song&offset=0").arg(str).arg(limit)));
        break;
    default:
        break;
    }

    reply = manager->get(*request); //发起get请求
}
/* 返回的json示例
{
"result":"SUCCESS",
"code":200,
"data":[
        {
        "id":"001rTpCK2nGJuK",
        "name":"内容",
        "time":254,
        "singer":"李亚宗",
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
//解析搜素结果
void Widget::ResultParseJson(QByteArray &reply_content)
{
    //将结果转化为标准字符串
    QString res = QString::fromStdString(reply_content.toStdString()).toUtf8();

    QJsonDocument document;
    QJsonParseError ParseError;
    //创建QJSON文档
    document = QJsonDocument::fromJson(res.toUtf8(),&ParseError);

    if(!document.isNull() && ParseError.error == QJsonParseError::NoError)  //文件非空且解析没错
    {
        if(document.isObject()) //是否为json对象
        {
            QJsonObject object = document.object(); //读取QJSON文档对象
            if(!object.isEmpty())
            {
                if(object.contains("result"))   //解析请求结果
                {
                    QJsonValue value = object.value("result");
                    if(value.isString())
                    {
                        QString result = value.toString();
                        qDebug() <<"result = "<<result ;
                    }

                }
                if(object.contains("code"))    //解析请求返回代码
                {
                    QJsonValue value = object.value("code");
                    if(value.isDouble())
                    {
                        int code = value.toDouble();
                        qDebug() <<"code = "<<code ;
                        if(code != 200) //搜索失败
                        {
                            QMessageBox::warning(this,"","search failed");
                            return;
                        }
                    }
                }
                if(object.contains("data"))     //解析数组
                {
                    CleanWebPlayList();         //清空网络播放列表
                    SearchSongLrc.clear();      //清空歌词

                    QJsonValue value = object.value("data"); //解析data数组
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
                                if(data_object.contains("id"))  //解析歌曲id
                                {
                                    QJsonValue value = data_object.value("id");
                                    if(value.isString())
                                    {
                                        QString id = value.toString();
                                        qDebug() <<"id = "<<id ;

                                        song->SongID = id;
                                    }
                                }
                                if(data_object.contains("name")) //解析歌曲名
                                {
                                    QJsonValue value = data_object.value("name");
                                    if(value.isString())
                                    {
                                        QString name = value.toString();
                                        qDebug() <<"name = "<<name ;

                                        song->SongName = name;
                                    }
                                }
                                if(data_object.contains("singer")) //解析歌手名
                                {
                                    QJsonValue value = data_object.value("singer");
                                    if(value.isString())
                                    {
                                        QString singer = value.toString();
                                        qDebug() <<"singer = "<<singer ;

                                        song->SingerName = singer;
                                    }
                                }
                                if(data_object.contains("url")) //解析歌曲URL
                                {
                                    QJsonValue value = data_object.value("url");
                                    if(value.isString())
                                    {
                                        QString url = value.toString();
                                        qDebug() <<"url = "<<url ;

                                        song->SongUrl = url;
                                    }
                                }
                                if(data_object.contains("pic")) //解析图片URL
                                {
                                    QJsonValue value = data_object.value("pic");
                                    if(value.isString())
                                    {
                                        QString pic = value.toString();
                                        qDebug() <<"pic = "<<pic ;

                                        song->PicUrl = pic;
                                    }
                                }
                                if(data_object.contains("lrc")) //解析歌词URL
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
                            //暂存歌曲信息
                            SongList->append(song);
                            //SongList->insert(i,song);

                            //添加到显示列表中
                            QString showinfo = song->SingerName + "              "+song->SongName;
                            ui->listWidget->addItem(showinfo);

                            //添加到播放列表中
                            QString SongUrl = song->SongUrl.append("&br=999000");
                            playList ->addMedia(QUrl(SongUrl));
                        }

                    }

                }


            }
        }
    }
}
//清空歌曲网络播放列表
void Widget::CleanWebPlayList()
{
    if(!SongList->isEmpty())
    {

        //清空歌曲搜索信息列表
        //  先释放内存
        for(int i = 0 ; i < SongList->size(); ++i)
        {
            SongInfo *tmp = SongList->at(i);
            delete tmp;
        }
        //  再清空列表
        SongList->clear();

        //清空显示列表
        ui->listWidget->clear();

        //清空播放列表
        playList->clear();

        qDebug() <<"SongList size = " <<SongList->size();
        qDebug() <<"playList mediaCount size = " <<playList->mediaCount();
    }
}
//延时函数  --ms
void Widget::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/**************************************播放控制********************************************/
//播放
void Widget::on_pushButton_4_clicked()
{
    ui->pushButton_4->hide();
    ui->pushButton_2->show();
    int page = ui->stackedWidget->currentIndex();
    switch (page)
    {
    case 0 ://网络列表
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
    case 1://本地列表
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
        else  //继续
        {
            musicplayer.play();
        }
        break;
    default:
        break;
    }

    if(musicplayer.state() == QMediaPlayer::PlayingState) //继续旋转
    {
        timerpic->start(100);
    }
}
//暂停
void Widget::on_pushButton_2_clicked()
{
    ui->pushButton_2->hide();
    ui->pushButton_4->show();

    musicplayer.pause();
    //qDebug() << "musicplayer.state  ="<<musicplayer.state();
    if(musicplayer.state() == QMediaPlayer::PausedState)    //暂停旋转
    {
        timerpic->stop();
    }
}
//下一首
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
        //playList->next();  //这个是列表顺序播放 不会循环
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
        //playLocalList->next();//这个是列表顺序播放 不会循环
        break;
    }
    default:
        break;
    }
}
//上一首
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
        // playList->previous();//这个是列表顺序播放 不会循环
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
        //playLocalList->previous();//这个是列表顺序播放  不会循环
        break;
    }
    default:
        break;
    }
}
//切换列表播放方式
void Widget::on_pushButton_6_clicked()
{
    static int i = 0;

    if(++i == 4)
    {
        i = 0;
    }
    if(i == 0)
    {
       playmode = "列表循环播放";

       ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/inOrder.png);}");
       //ui->pushButton_6->setIcon(QIcon(":/icons/icon/inOrder.png"));
       ui->pushButton_6->setToolTip(QString(playmode.toUtf8()));

       playList->setPlaybackMode(QMediaPlaylist::Loop); //列表循环播放
    }
    else if(i == 1)
    {
        playmode = "列表顺序播放";

        ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/order_play.png);}");
        //ui->pushButton_6->setIcon(QIcon(":/icons/icon/order_play.png"));
        ui->pushButton_6->setToolTip(QString(playmode.toUtf8()));

        playList->setPlaybackMode(QMediaPlaylist::Sequential); //列表顺序播放
    }
    else if(i == 2)
    {
        playmode = "随机播放";

        ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/shuffle.png);}");
        //ui->pushButton_6->setIcon(QIcon(":/icons/icon/shuffle.png"));
        ui->pushButton_6->setToolTip(QString(playmode.toUtf8()));

        playList->setPlaybackMode(QMediaPlaylist::Random);//随机播放
    }
    else if(i == 3)
    {
        playmode = "单曲循环";

        ui->pushButton_6->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/singleCycle.png);}");
        //ui->pushButton_6->setIcon(QIcon(":/icons/icon/singleCycle.png"));
        ui->pushButton_6->setToolTip(QString(playmode.toUtf8()));

        playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//单曲循环
    }
}
//双击播放音乐  --网络列表
void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    //int CurrentRow = ui->listWidget->currentRow();  //双击的是第几行   行号从0开始
    int CurrentRow = index.row(); //双击的是第几行   行号从0开始
    if(CurrentRow != playList->currentIndex())
    {
        //播放音乐
        musicplayer.stop();
        musicplayer.setPlaylist(playList);
        CurrentPlaylist = PLAYLIST;
        playList->setCurrentIndex(CurrentRow);//播放音乐
        musicplayer.play();
    }
}
//双击播放音乐  --本地列表
void Widget::on_listWidget_locallist_doubleClicked(const QModelIndex &index)
{

    int CurrentRow = index.row();   //双击的是第几行   行号从0开始
    if(CurrentRow != playLocalList->currentIndex())
    {
        //播放音乐
        musicplayer.setPlaylist(playLocalList);
        CurrentPlaylist = PLAYLOCALLIST;
        playLocalList->setCurrentIndex(CurrentRow);
        musicplayer.play();
        qDebug()<< musicplayer.currentMedia().canonicalUrl();
    }
}
//调整播放进度
void Widget::on_horizontalSlider_sliderMoved(int position)
{
    musicplayer.setPosition((qint64)(position));
}
//调整声音大小
void Widget::on_horizontalSlider_volumn_valueChanged(int value)
{
     musicplayer.setVolume(value);
}




/**************************************本地音乐播放********************************************/
//添加本地音乐
void Widget::on_pushButton_13_clicked()
{
    QSqlQuery query;

    QStringList list= QFileDialog::getOpenFileNames(this,tr("文件"),"D:/",tr("音频文件(*.mp3)")); //选择文件
    for(int i = 0 ; i < list.size() ; ++i)
    {
        QString path = QDir::toNativeSeparators(list.at(i)); //音频文件的绝对路径
        if(!path.isEmpty())
        {
            SongInfo *song =  new SongInfo;
            song->SongUrl  =  path;
            qDebug()<<"path = "<<path;

            QString songfilename = path;
            songfilename.replace("\\","\\\\\\\\");  //解决mysql吃掉 反斜杠 的问题
            qDebug()<<"songfilename = "<<songfilename;

            QString name=path.split("\\").last();
            name.remove(QString(".mp3"));
            song->SongName = name;
            qDebug()<<"name = "<<name;

            bool result = query.exec(QString("insert into localMusic values (%1,'%2','%3','%4','%5')").arg(qrand()%10000).arg(name).arg(songfilename).arg("").arg(""));//添加到数据库中
            if(result == true)  //歌曲不重复
            {
                SongLocalList->append(song);  //添加临时本地播放列表
                playLocalList->addMedia(QUrl::fromLocalFile(path));
            }
            else//歌曲重复
            {
              QMessageBox::critical(this,nullptr,"The current song already exists!!!");
            }
        }
    }
    ShowLocalSong();
}
//从数据库加载本地音乐
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
//显示本地音乐
void Widget::ShowLocalSong()
{
    ui->stackedWidget->setCurrentIndex(1);
    for(int i = 0 ; i <SongLocalList->size() ; ++i )//清除本地列表显示
    {
        QListWidgetItem *item = ui->listWidget_locallist->takeItem(0);
        delete item;
    }
    for(int i = 0 ; i <SongLocalList->size() ; ++i )//更新本地列表显示
    {
        ui->listWidget_locallist->insertItem(i,SongLocalList->at(i)->SongName);
    }
}
//清空本地播放列表
void Widget::ClearSongLocalPlayList()
{
    for(int i = 0 ; i <SongLocalList->size() ; ++i )
    {
        SongInfo*tmp = SongLocalList->back();
        SongLocalList->pop_back();
        delete tmp;  //释放内存
    }
    //清空显示列表
    ui->listWidget_locallist->clear();
    //清空播放列表
    playLocalList->clear();
}
//显示网络播放列表
void Widget::on_pushButton_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
//显示本地播放列表
void Widget::on_pushButton_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
//静音与取消静音
void Widget::on_pushButton_Mute_clicked()
{
    static bool MuteFlag = true;
    if(MuteFlag)   //静音
    {
        FrontOfMuteVolume = musicplayer.volume(); //保存静音前的声音大小

        musicplayer.setVolume(0);
        ui->horizontalSlider_volumn->setValue(0);
        ui->pushButton_Mute->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/muteVolume.png);}");


        MuteFlag = false;
    }
    else          //取消静音
    {
        musicplayer.setVolume(FrontOfMuteVolume);
        ui->horizontalSlider_volumn->setValue(FrontOfMuteVolume);

        ui->pushButton_Mute->setStyleSheet("QPushButton{border:none;border-image: url(:/icons/icon/volume.png);}");

        MuteFlag = true;
    }
}
//切换专辑页面
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



/**************************************窗口控制********************************************/
//关闭
void Widget::on_pushButton_WinClose_clicked()
{
    int res = QMessageBox::critical(this,"","You are sure to close the program?","Yes","No");
    if(res == 0)  //yes
    {
        this->close();
    }
}
//最小化
void Widget::on_pushButton_WinMinsize_clicked()
{
    this->showMinimized();
}
//鼠标事件 -- 压下
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
     {
         IsDrag = true;
         MousePositon = event->globalPos() -  this->pos();
         event->accept();
     }
}
//鼠标事件 -- 移动
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(IsDrag && (event->buttons() == Qt::LeftButton))
    {
        move(event->globalPos() - MousePositon);
        event->accept();
    }
}
//鼠标事件 -- 释放
void Widget::mouseReleseEvent(QMouseEvent *event)
{
     IsDrag = false;
     event->accept();
}
//设置背景图片  --画图事件
void Widget::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    painter.drawPixmap(0,0,width(),height(),QPixmap(BackgroundPicPath)); //通过画图来设置背景图片

    /*this->setStyleSheet("Widget{"
                        "border-image: url(D://tmp1.jpg);"          //通过setStyleSheet
                        "}");*/
    update();
}


