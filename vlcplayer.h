#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include <QThread>
#include <vlc/vlc.h>
#include <vlc/libvlc.h>
#include <QDebug>
#include <QMessageBox>
#include <QSlider>
#include <QPushButton>
#include <QPalette>
#include <QFileDialog>

class VLCPlayer : public QThread
{
    Q_OBJECT
public:
    explicit VLCPlayer(QWidget *parent = 0);
    void SetPlayWidget(QWidget * PlayWidget);
    void SetStartButton(QPushButton * StartButton);
    void SetSetButton(QPushButton * SetButton);
    void SetValumeSlider(QSlider * ValumeSlider);
    void SetPlayPointSlider(QSlider * PlayPointSlider);
    bool CheckPlaying();
    ~VLCPlayer();
signals:

public:
    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;

public slots:
    void play();
    void stop();
    void Pause();
    void changeVolume(int vol);
    void openFile(QString strURL);
    void LoadRTSPStream(QString strURL);
private:

    QWidget * mPlayWidgetView; //���Ž���ؼ�
    QPushButton * mStartButton;//��ʼ����
    QPushButton * mSetButton;//���ð���
    QSlider    * mValumeSlider;//������
    QSlider    * mPlayPointSlider;//���Ž�����
    QWidget * mParentWidget;
};

#endif // VLCPLAYER_H
