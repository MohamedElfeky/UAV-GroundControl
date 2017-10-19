#include "gcsdisplay.h"
#include "ui_gcsdisplay.h"
#if 1
GCSDisplay::GCSDisplay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GCSDisplay)
{
    ui->setupUi(this);
    this->setWindowTitle("GCS (Ground Control System)");//���ó������
    this->setWindowState(Qt::WindowMaximized);//���ô���Ϊȫ��
    int hight = QApplication::desktop()->height();
    int width = QApplication::desktop()->width();
    mReadini = new ReadIni("prame.ini");
    this->setGeometry(0,0,width,hight);
    this->SetQMLCommunicate();
    this->SetToolBar();
    this->SetDockWidget();
    this->SetFlyDataWidget();
    this->SetStatusDisplay();
    this->SetCommandDisplay();
    this->SetUAVLogoDisplay();
    this->SetMagDisplay();

    this->SetMenuWidget();
    this->ChangeSize(this->width(),this->height());
    this->SetButton();
    this->connect(mGoogleMapWeb,SIGNAL(SendLocation(QString,QString)),mQMLCom,SLOT(ChangeLocation(QString,QString)));
    this->connect(ui->FPVaction,SIGNAL(changed()),this,SLOT(DisplayChanged()));
    this->connect(ui->Serialaction,SIGNAL(toggled(bool)),mMenuWidget,SLOT(SerialSettingShow(bool)));
    this->connect(ui->FlyPrepareaction,SIGNAL(toggled(bool)),mMenuWidget,SLOT(FlyPrepareShow(bool)));
    this->connect(mMenuWidget,SIGNAL(sigGetAppRect()),this,SLOT(SendRect()));
    this->connect(ui->Exitaction,SIGNAL(toggled(bool)),this,SLOT(close()));

}



void GCSDisplay::SetQMLCommunicate()
{
    mQMLCom = new QMLCommunicate;
    qmlRegisterType<QMLCommunicate>("an.qt.QMLCommunicate",1,0,"mQMLCom");
    mQMLCom->SetReadIni(mReadini);
}

void GCSDisplay::SetDockWidget(void)
{
    mWebWidget = new QWidget(this);
    mVLCWidget = new QWidget(this);
    mFlyDataWidget = new QWidget(this);
    mCommandWidget = new QWidget(this);
    mVLCPlayer = new VLCPlayer;
    mVLCPlayer->SetPlayWidget(mVLCWidget);
    mGCSStatusWidget = new QWidget(this);
    mGCSCommandWidget = new QWidget(this);
    mGCSUAVLogoWidget = new QWidget(this);
    mWebView = new QWebEngineView(mWebWidget);
    mGoogleMapWeb = new Googlemapweb(mWebView,mWebWidget);
//    mVLCPlayer->openFile(QString("rtmp://live.hkstv.hk.lxdns.com/live/hks"));
//        mVLCPlayer->openFile(QString("D:/\GOPR0859_A01.MP4"));
    mGCSMagWidget = new QWidget(mWebWidget);

    mMenuWidget = new MenuWidgetManager(mQMLCom,this);

//    mSerialSettingWidget->hide();
    //this->setCentralWidget(mWebWidget);
}



void GCSDisplay::onRestoreCatalogueView()
{
    mVLCWidget->setMinimumWidth(0);
    mFlyDataWidget->setMinimumWidth(0);
}

void GCSDisplay::DisplayChanged()
{
    this->ChangeSize(this->width(),this->height());
}

void GCSDisplay::RecordRect()
{

}


void GCSDisplay::ChangeSize(int iWidth,int iHeight)
{
    this->RecordRect();
    this->ChangeFlyDataRect(0.005*iWidth,0.08*iHeight,0.15*iWidth,0.7*iHeight);

    mCommandWidget->setGeometry(0.2*iWidth,0.8*iHeight,0.8*iWidth,0.2*iHeight);
    this->ChangeFlyStatusRect(0,0.8*iHeight,0.15*iWidth,0.2*iHeight);
    this->ChangeCommandRect(0.15*iWidth,0.8*iHeight,0.65*iWidth,0.2*iHeight);
    this->ChangeUAVLogoRect(0.8*iWidth,0.8*iHeight,0.2*iWidth,0.2*iHeight);
    mMenuWidget->setGeometry(0.3*iWidth,0.3*iHeight,0.4*iWidth,0.4*iHeight);
    if(ui->FPVaction->isChecked())//��ʾFPV
    {
        //mWebWidget->setGeometry(0.15*iWidth,0.08*iHeight,0.55*iWidth,0.7*iHeight);
        mWebWidget->setGeometry(0,0.08*iHeight,0.55*iWidth,0.7*iHeight);
        mGoogleMapWeb->ChangeRect(0,0,0.5*iWidth,0.7*iHeight);
        this->ChangeMagRect(0.45*iWidth,0.04*iHeight,0.06*iWidth,0.1*iHeight);
        mVLCWidget->show();

        mVLCWidget->setGeometry(0.7*iWidth,0.08*iHeight,0.3*iWidth,0.7*iHeight);
        mVLCPlayer->openFile(QString("rtmp://live.hkstv.hk.lxdns.com/live/hks"));
        //mVLCPlayer->openFile(QString("D:/\GOPR0859_A01.MP4"));
//        mVLCPlayer->FullWidget(0.5*iWidth,0.7*iHeight);
    }
    else//����FPV
    {
        //mWebWidget->setGeometry(0.15*iWidth,0.08*iHeight,0.85*iWidth,0.7*iHeight);
        mWebWidget->setGeometry(0,0.08*iHeight,0.85*iWidth,0.7*iHeight);
        this->ChangeMagRect(0.77*iWidth,0.04*iHeight,0.08*iWidth,0.05*iHeight);
        mGoogleMapWeb->ChangeRect(0,0,0.85*iWidth,0.7*iHeight);
        mVLCWidget->hide();
        mVLCPlayer->stop();
    }
}

void GCSDisplay::SetButton()
{
//    QPushButton * p1= new QPushButton(mCommondWidget);
    mCommandWidget->setStyleSheet("QPushButton#evilButton { background-color: red; \
                                  border-style: outset;   \
                                  border-width: 2px;   \
                                  border-radius: 10px;   \
                                  border-color: beige;   \
                                  font: bold 14px;    \
                                  min-width: 10em;   \
                                  padding: 6px;   \
                              }");
}

void GCSDisplay::resizeEvent(QResizeEvent *event)
{
    qDebug()<<"ParentWidget:"<<this->width()<<" "<<this->height();
    this->ChangeSize(this->width(),this->height());
}

QRect GCSDisplay::SendRect()
{
    return this->rect();
}

void GCSDisplay::SetToolBar()
{
    ChangeMouseAction = new QAction(QString::fromLocal8Bit("���"), this);
    ChangeMouseAction->setShortcut(QKeySequence::Open);
    ChangeMouseAction->setStatusTip(QString::fromLocal8Bit("�ı����Ϊ���"));
    ChangeMouseAction->setIcon(QIcon("://new/toolbar/png/mousepass.png")); // Add code.
    //ui->menu->addMenu(tr("&File"));
    //ui->menu->addAction(ChangeMouseAction);
    ui->toolBar->addAction(ChangeMouseAction);


    ZoomAddAction = new QAction(QString::fromLocal8Bit("�Ŵ�"),this);      //ToolBar �Ŵ�
    ZoomAddAction->setStatusTip(QString::fromLocal8Bit("�Ŵ��ͼ"));
    ZoomAddAction->setIcon(QIcon("://new/toolbar/png/ZoomAdd.png"));
    ui->toolBar->addAction(ZoomAddAction);

    ZoomSubAction = new QAction(QString::fromLocal8Bit("��С"),this);      //ToolBar ��С
    ZoomSubAction->setStatusTip(QString::fromLocal8Bit("��С��ͼ"));
    ZoomSubAction->setIcon(QIcon("://new/toolbar/png/ZoomSub.png"));
    ui->toolBar->addAction(ZoomSubAction);

    ScalePlateAction = new QAction(QString::fromLocal8Bit("���"),this);      //ToolBar ���
    ScalePlateAction->setStatusTip(QString::fromLocal8Bit("��������֮��ľ���"));
    ScalePlateAction->setIcon(QIcon("://new/toolbar/png/ScalePlate.png"));
    ui->toolBar->addAction(ScalePlateAction);

    ChangeMoveAction = new QAction(QString::fromLocal8Bit("�ƶ�"),this);      //ToolBar �ƶ�
    ChangeMoveAction->setStatusTip(QString::fromLocal8Bit("������Ϊ���϶�"));
    ChangeMoveAction->setIcon(QIcon("://new/toolbar/png/ChangeMove.png"));
    ui->toolBar->addAction(ChangeMoveAction);

    CoursePlotAction = new QAction(QString::fromLocal8Bit("������"),this);      //ToolBar ������
    CoursePlotAction->setStatusTip(QString::fromLocal8Bit("������"));
    CoursePlotAction->setIcon(QIcon("://new/toolbar/png/CoursePlot.png"));
    ui->toolBar->addAction(CoursePlotAction);


    ShowGridAction = new QAction(QString::fromLocal8Bit("��ʾ����"),this);      //ToolBar ��ʾ����
    ShowGridAction->setStatusTip(QString::fromLocal8Bit("��ʾ����"));
    ShowGridAction->setIcon(QIcon("://new/toolbar/png/ShowGrid.png"));
    ui->toolBar->addAction(ShowGridAction);

    FindUAVAction = new QAction(QString::fromLocal8Bit("��λ���˻�"),this);      //ToolBar ��λ���˻���ǰλ��
    FindUAVAction->setStatusTip(QString::fromLocal8Bit("��λ�����˻���ǰλ��"));
    FindUAVAction->setIcon(QIcon("://new/toolbar/png/FindUAV.png"));
    ui->toolBar->addAction(FindUAVAction);

    ClearRouteAction = new QAction(QString::fromLocal8Bit("�������"),this);      //ToolBar �������
    ClearRouteAction->setStatusTip(QString::fromLocal8Bit("�����ǰ����"));
    ClearRouteAction->setIcon(QIcon("://new/toolbar/png/ClearRoute.png"));
    ui->toolBar->addAction(ClearRouteAction);

    RouteDisplayAction = new QAction(QString::fromLocal8Bit("��ʾ����"),this);      //ToolBar ��ʾ����
    RouteDisplayAction->setStatusTip(QString::fromLocal8Bit("��ʾ���˻��켣"));
    RouteDisplayAction->setIcon(QIcon("://new/toolbar/png/RouteDisplay.png"));
    ui->toolBar->addAction(RouteDisplayAction);

    RecordDataAction = new QAction(QString::fromLocal8Bit("��¼����"),this);      //ToolBar ��¼����
    RecordDataAction->setStatusTip(QString::fromLocal8Bit("�����ʼ��¼����"));
    RecordDataAction->setIcon(QIcon("://new/toolbar/png/RecordData.png"));
    ui->toolBar->addAction(RecordDataAction);

    SettingAction = new QAction(QString::fromLocal8Bit("����"),this);      //ToolBar ����
    SettingAction->setStatusTip(QString::fromLocal8Bit("����������ý���"));
    SettingAction->setIcon(QIcon("://new/toolbar/png/Setting.png"));
    ui->toolBar->addAction(SettingAction);
    QSize size(30,30);
    ui->toolBar->setIconSize(size);
}

void GCSDisplay::SetMenuWidget()
{
    mMenuWidget->SetMenuWidget();
}



void GCSDisplay::SetStatusDisplay()
{
    QQuickView *view = new QQuickView();
    mStatusDisplayQMLcontainer = QWidget::createWindowContainer(view,mGCSStatusWidget);
    view->setSource(QUrl(QStringLiteral("qrc:/gcsstatusdisplay.qml")));
    QQmlContext* context=view->rootContext();
     context->setContextProperty("mQMLCom",mQMLCom);
     mStatusDisplayQMLcontainer->setParent(mGCSStatusWidget);
     //mStatusDisplayQMLcontainer->setGeometry(0,0,600,600);
}


void GCSDisplay::SetCommandDisplay()
{
    QQuickView *view = new QQuickView();
    mCommandDisplayQMLcontainer = QWidget::createWindowContainer(view,mGCSCommandWidget);

    view->setSource(QUrl(QStringLiteral("qrc:/gcscommanddisplay.qml")));
    QQmlContext* context=view->rootContext();
     context->setContextProperty("mQMLCom",mQMLCom);
     mCommandDisplayQMLcontainer->setParent(mGCSCommandWidget);
}



void GCSDisplay::SetUAVLogoDisplay()
{
#if 0
    QQuickView *view = new QQuickView();
    mUAVLogoDisplayQMLcontainer = QWidget::createWindowContainer(view,mGCSUAVLogoWidget);

    view->setSource(QUrl(QStringLiteral("qrc:/gcsuavlogodisplay.qml")));
    QQmlContext* context=view->rootContext();
     context->setContextProperty("mQMLCom",mQMLCom);
     mUAVLogoDisplayQMLcontainer->setParent(mGCSUAVLogoWidget);
#else
    mAHRSWidget = new GLWidget(mGCSUAVLogoWidget);
    QColor clearColor;
    clearColor.setHsv( 0xAD , 0xD8, 0xE6);
    mAHRSWidget->setClearColor(clearColor);
    mAHRSWidget->rotateBy(0,0,0);
#endif
}



void GCSDisplay::SetFlyDataWidget()
{
#if 0
    QQuickWidget *mFlyDataQMLcontainer = new QQuickWidget();
    mFlyDataQMLcontainer = QWidget::createWindowContainer(view,mFlyDataWidget);
//    container->setGeometry(0,17,300,580);
    view->setSource(QUrl(QStringLiteral("qrc:/flydata.qml")));
    QQmlContext* context=view->rootContext();
     context->setContextProperty("mQMLCom",mQMLCom);
     mFlyDataQMLcontainer->setParent(mFlyDataWidget);
#endif
     mFlyDataQMLcontainer = new QQuickWidget(mFlyDataWidget);

     mFlyDataQMLcontainer->setSource(QUrl(QStringLiteral("qrc:/flydata.qml")));
     QQmlContext* context=mFlyDataQMLcontainer->rootContext();
      context->setContextProperty("mQMLCom",mQMLCom);
     mFlyDataQMLcontainer->setAttribute(Qt::WA_AlwaysStackOnTop, true); //��������ʹQML����͸��
     mFlyDataQMLcontainer->setClearColor(QColor(Qt::transparent));

//     mMapDisplayView->setAttribute(Qt::WA_AlwaysStackOnTop, true); //��������ʹQML����͸��
//     mMapDisplayView->setClearColor(QColor(Qt::transparent));
}

void GCSDisplay::SetMagDisplay()
{
    mMapDisplayView = new QQuickWidget(mGCSMagWidget);

    mMapDisplayView->setSource(QUrl(QStringLiteral("qrc:/MagneticCompass.qml")));
    QQmlContext* context=mMapDisplayView->rootContext();
     context->setContextProperty("mQMLCom",mQMLCom);
    mMapDisplayView->setAttribute(Qt::WA_AlwaysStackOnTop, true); //��������ʹQML����͸��
    mMapDisplayView->setClearColor(QColor(Qt::transparent));

     //view->setColor(QColor(Qt::transparent));
#if 0
     mGCSMagWidget->setAutoFillBackground( true );
     QPalette plt = this->mGCSMagWidget->palette();
     plt.setColor( QPalette::Window, Qt::red );
     mGCSMagWidget->setPalette( plt );

#else
     mGCSMagWidget->setAutoFillBackground( true );
     mGCSMagWidget->setAttribute(Qt::WA_TranslucentBackground);//����͸��
     mGCSMagWidget->setStyleSheet("background-color: rgb(222, 222, 222,100);");
     QPalette pal = this->mGCSMagWidget->palette();
     pal.setColor(QPalette::Background, QColor(0x00,0x00,0x00,0x00));
     mGCSMagWidget->setPalette(pal);

#endif
}

void GCSDisplay::ChangeFlyDataRect(int x,int y,int iWidth,int iHeight)
{
    mFlyDataWidget->setGeometry(x,y,iWidth,iHeight);

    mFlyDataQMLcontainer->resize(iWidth,iHeight);
    mQMLCom->ChangeSize(iWidth,iHeight);

}

void GCSDisplay::ChangeMenuWidgetRect(int iWidth,int iHeight)
{

}

void GCSDisplay::ChangeFlyStatusRect(int x,int y,int iWidth,int iHeight)
{
    mGCSStatusWidget->setGeometry(x,y,iWidth,iHeight);
    mQMLCom->ChangeStatusDisplaySize(iWidth,iHeight);
    mStatusDisplayQMLcontainer->resize(iWidth,iHeight);
}

void GCSDisplay::ChangeCommandRect(int x,int y,int iWidth,int iHeight)
{
    mGCSCommandWidget->setGeometry(x,y,iWidth,iHeight);
    mQMLCom->ChangeCommandDisplaySize(iWidth,iHeight);
    mCommandDisplayQMLcontainer->resize(iWidth,iHeight);
}

void GCSDisplay::ChangeUAVLogoRect(int x,int y,int iWidth,int iHeight)
{
    mGCSUAVLogoWidget->setGeometry(x,y,iWidth,iHeight);
    mQMLCom->ChangeUAVLogoDisplaySize(iWidth,iHeight);
    mAHRSWidget->setGeometry(0,0,iWidth,iHeight);
//    mUAVLogoDisplayQMLcontainer->resize(iWidth,iHeight);
}

void GCSDisplay::ChangeMagRect(int x,int y,int iWidth,int iHeight)
{
    mQMLCom->ChangeMagDisplaySize(iWidth,iWidth);
    mGCSMagWidget->setGeometry(x,y,iWidth,iWidth);
    mMapDisplayView->setGeometry(0,0,iWidth,iWidth);

}

void GCSDisplay::testChanged(void)
{
    QMessageBox::information(NULL,"test", "111111111", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

GCSDisplay::~GCSDisplay()
{
    delete ui;
}
#endif
