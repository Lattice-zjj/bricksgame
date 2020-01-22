#include "mywidget.h"
#include "ui_mywidget.h"
#include "ball.h"
#include "brick.h"
#include "knock.h"
#include <QTime>
#include <QTimer>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QPushButton>
#include <fstream>
#include <sstream>
#include <iostream>
#include <QFont>
using namespace std;

#define M 1000000
myWidget::myWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myWidget)
{
    ui->setupUi(this);
    isWin = false;
    isFailure = false;
    is_begin = false;
    frameWidth = 540;
    frameHeight = 337.5;
    score = 0;
    numscore=0;
    rows = 5;
    columns = 12;
    createPaddle();
    createBall();


    timer = new QTimer(this );
    timer->setInterval( 5 );

    connect( timer, SIGNAL( timeout() ), this, SLOT( moveBall() ) );
    setWindowTitle("打砖块");
    QString btnStyle="QPushButton{\
            color: rgb(255, 255, 255);\
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(166,164,208), stop:0.3 rgb(171,152,230), stop:1 rgb(152,140,220));\
            border:1px;\
            border-radius:5px; \
            padding:2px 4px;  \
        }\
        QPushButton:hover{\
            color: rgb(255, 255, 255); \
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(130,120,226), stop:0.3 rgb(120,130,230), stop:1 rgb(125,140,226));\
            border:1px;  \
            border-radius:5px; \
            padding:2px 4px; \
        }\
        QPushButton:pressed{    \
            color: rgb(255, 255, 255); \
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(240,156,121), stop:0.3 rgb(220,160,140), stop:1 rgb(230,140,120));  \
            border:1px;  \
            border-radius:5px; \
            padding:2px 4px; \
        }";
    ui->begin->setStyleSheet(btnStyle);
    ui->highscore->setStyleSheet(btnStyle);
    ui->end->setStyleSheet(btnStyle);
    ui->textBrowser->setStyleSheet("background-color:rgb(255,255,0);font-size:16px;color:blue");
    QFont font ("Microsoft YaHei",1,75);
    ui->textBrowser->setFont(font);


    resize(540,337.5);

    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    QPalette lcdpat=ui->lcdNumber->palette();
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
    ui->lcdNumber->setPalette(lcdpat);
    ui->lcdNumber->setStyleSheet("background-color:rgb(255,255,0)");


    ui->lcdNumber_2->setSegmentStyle(QLCDNumber::Flat);
    QPalette lcdpat1=ui->lcdNumber_2->palette();
    lcdpat1.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
    ui->lcdNumber_2->setPalette(lcdpat1);
    ui->lcdNumber_2->setStyleSheet("background-color:rgb(255,0,255);");


    ui->lcdNumber_3->setSegmentStyle(QLCDNumber::Flat);
    QPalette lcdpat2=ui->lcdNumber->palette();
    lcdpat2.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
    ui->lcdNumber_3->setPalette(lcdpat2);
    ui->lcdNumber_3->setStyleSheet("background-color:rgb(255,0,255);");


    ui->lcdNumber_4->setSegmentStyle(QLCDNumber::Flat);
    QPalette lcdpat3=ui->lcdNumber_4->palette();
    lcdpat3.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
    ui->lcdNumber_4->setPalette(lcdpat3);
    ui->lcdNumber_4->setStyleSheet("background-color:rgb(255,0,255);");

    //显示第一个页面
    ui->stackedWidget->setCurrentWidget(ui->pageSet);
    ui->labelOver->setMovie(&overMovie);
    ui->labelOver->setScaledContents(true);
    ui->labelWin->setMovie(&winMovie);
    ui->labelWin->setScaledContents(true);

    //初始化数据
    for (int i=0;i<100;i++) scorexi[i]=0;

    up();

    update1();
    //失败动画
    overMovie.setFileName("://image/over.gif");

    winMovie.setFileName("://image/win.gif");

    ui->helper1->setStyleSheet("border-image:url(://image/helper1.JPG);");
    ui->helper2->setStyleSheet("border-image:url(://image/helper2.JPG);");
    ui->helper3->setStyleSheet("border-image:url(://image/helper3.JPG);");





    connect(ui->begin,&QPushButton::released,
            [=](){
        int ret=QMessageBox::question(this,"确定","are you sure?",QMessageBox::Ok,QMessageBox::Cancel);
        switch (ret)
        {
            case QMessageBox::Ok:
                ui->stackedWidget->setCurrentWidget(ui->page_2);
                on_startGame();
                begin();
                jie=2;
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }
    });



    connect(this, SIGNAL( scoreChanged( int ) ), this, SLOT( on_changeScore( int ) ) );

    connect(ui->highscore,&QPushButton::released,
            [=](){
        ui->stackedWidget->setCurrentWidget(ui->page);
    });

    connect(ui->back,&QPushButton::released,
            [=](){
        ui->stackedWidget->setCurrentWidget(ui->pageSet);
    });
    QString st="";
    int p;
    if (numscore>9) p=9;else p=numscore;
    for (int i=1;i<=p;i++){
        st=st+QString::number(i)+"."+QString::number(scorexi[i]);
        for (int j=1;j<=11-QString::number(scorexi[i]).length()-2;j++) st=st+" ";
    }
    ui->textBrowser->setText(st);


}

void myWidget::helper1(){
    if (numhelp1!=0){

    helper1TimerId = startTimer(5000);
    is_begin=true;
    numhelp1--;
    if (numhelp1==0){
        ui->helper1->hide();
        ui->lcdNumber_2->hide();
    }
    }
}


void myWidget::helper2(){
    if (numhelp2!=0){

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for (int i=1;i<=min(5,bricks.size());i++){
        int num=qrand()%(bricks.size());
        Brick *temp = bricks.takeAt( num );
        delete temp;
        score=score+100;
    }
    numhelp2--;

    if (numhelp2==0){
        ui->helper2->hide();
        ui->lcdNumber_4->hide();
    }
    }
}

void myWidget::helper3(){
    if (numhelp3!=0){


    helper3TimerId = startTimer(5000);

    QRectF change=knock->getShape();
    QRectF later=QRectF(change.left(),change.top(),change.width()+20,change.height());
    knock->setShape(later);
    numhelp3--;

    if (numhelp3==0){
        ui->helper3->hide();
        ui->lcdNumber_3->hide();
    }
    }
}

void myWidget::on_startGame(){
    isWin = false;
    isFailure = false;
    is_begin = false;
    frameWidth = 540;
    frameHeight = 337.5;
    score = 0;
    rows = 5;
    columns = 12;
    createBricks();
    createPaddle();
    createBall();


    timer = new QTimer(this );
    timer->setInterval( 5 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( moveBall() ) );
}
myWidget::~myWidget()
{
    delete ui;
    delete ball;
    delete knock;
    delete timer;
}


void myWidget::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap("://image/back.JPG"));

    if (jie==2){
        QFont font( "Courier", 20, QFont::DemiBold );
        QFontMetrics fm( font );
        p.setFont( font );
        p.translate( QPoint(0,0) );

        p.setPen( Qt::NoPen );
        p.setBrush( Qt::green );
        p.drawEllipse( ball->getShape() );
        p.fillRect( knock->getShape(), Qt::blue);
        foreach( Brick *brick, bricks )
        p.fillRect( brick->getShape(), brick->getColor() );
        ui->lcdNumber_2->display(numhelp1);
        ui->lcdNumber_3->display(numhelp3);
        ui->lcdNumber_4->display(numhelp2);
    }


}

void myWidget::timerEvent(QTimerEvent *e){
    if (e->timerId()==gameTimerId)
    {
        gameTime--;

        //设置进度条
        ui->progressBar->setValue(gameTime);

        if (0==gameTime || isFailure)
        {
            lose();
        }
        if (isWin) win();
    }

    else if (e->timerId()==overTimerId)
    {
        //停止动画
        overMovie.stop();
        killTimer(overTimerId);

        ui->stackedWidget->setCurrentWidget(ui->pageSet);
        isFailure=false;

    }
    else if (e->timerId()==winTimerId)
    {
        winMovie.stop();
        killTimer(winTimerId);

        ui->stackedWidget->setCurrentWidget(ui->pageSet);
        isWin=false;
    }else if (e->timerId()==helper1TimerId){
        setBallSpeed(0.5);
        killTimer(helper1TimerId);
        is_begin=false;
    }else if (e->timerId()==helper3TimerId){

        killTimer(helper3TimerId);

        QRectF origin=knock->getShape();
        QRectF final=QRectF(origin.left(),origin.top(),origin.width()-20,origin.height());
        knock->setShape(final);
    }
}


void myWidget::begin()
{

    gameTime = 300;


    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(gameTime);
    ui->progressBar->setValue(gameTime);

    gameTimerId=0;
    gameTimerId=startTimer(1000);

    numscore=0;
    numhelp1=1;
    numhelp2=1;
    numhelp3=1;

    up();
    setBallSpeed(0.5);

    setPaddleStep( 2 * 10.0 );

    moveBall();
    timer->start();
}

void myWidget::lose(){
    //关闭计时器
    killTimer(gameTimerId);

    QMessageBox::warning(this,"失败","你输了！！");

    overMovie.start();
    timer->stop();

    //切换失败动画页面
    ui->stackedWidget->setCurrentWidget(ui->pageOver);
    jie=3;
    numscore++;
    scorexi[numscore]=score;

    overTimerId = startTimer(5000);
    for (int i=1;i<=numscore-1;i++)
        for (int j=i+1;j<=numscore;j++)
            if (scorexi[i]<scorexi[j]){
                int t=scorexi[i];scorexi[i]=scorexi[j];scorexi[j]=t;
            }
    update1();
}
void myWidget::win(){
    killTimer(gameTimerId);
    QMessageBox::information(this,"胜利","恭喜你，你赢了！！");
    timer->stop();
    isWin=false;
    winMovie.start();
    jie=4;
    numscore++;
    scorexi[numscore]=score;
    ui->stackedWidget->setCurrentWidget(ui->pageWin);

    winTimerId=startTimer(5000);
    for (int i=1;i<=numscore-1;i++)
        for (int j=i+1;j<=numscore;j++)
            if (scorexi[i]<scorexi[j]){
                int t=scorexi[i];scorexi[i]=scorexi[j];scorexi[j]=t;
            }
    update1();
}

void myWidget::update1() {
    ofstream outfile1("score.txt");
    outfile1 << "分数" << endl;
    for (int i = 1; i <= numscore; i++) {
        outfile1 << scorexi[i] << endl;
    }
    QString st="";
    int p=0;
    if (numscore>9) p=9;else p=numscore;
    for (int i=1;i<=p;i++){
        st=st+QString::number(i)+"."+QString::number(scorexi[i]);
        for (int j=1;j<=11-QString::number(scorexi[i]).length()-2;j++) st=st+" ";
    }
    ui->textBrowser->setText(st);
    if (numscore>9){
        for (int i=10;i<=numscore;i++) scorexi[i]=0;
        numscore=9;
    }
    outfile1.close();
}


void myWidget::createBricks()
{
    qreal gap = 1.0;
    qreal brickWidth = frameWidth / ( columns + 1 ) - gap;
    qreal brickHeight = frameHeight / 4 / ( rows + 1 ) - gap;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    for( int r = 0; r < rows; ++r )
    for( int c = 0; c < columns; ++c )
    {

        int num=qrand()% 5;
        Brick *brick = new Brick(brickWidth/3 + c * (brickWidth + gap ),
                       brickHeight / 2 + r * ( brickHeight + gap ),
                       brickWidth, brickHeight ,getcolor(num));
        brick->setkind(num+1);
        bricks.append( brick );
    }
}
int myWidget::randomInt( int high )
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int k=qrand()%(high+1);

    return k;
}

QColor myWidget::getcolor(int x){
    if (x==0) return QColor(255,255,0);
    else if (x==1) return QColor(0,255,0);
    else if (x==2) return QColor(255,0,0);
    else if (x==3) return QColor(160,32,240);
    else return QColor(0,0,255);
}

void myWidget::createPaddle()
{
    qreal paddleWidth=frameWidth/10;
    qreal paddleHeight=paddleWidth/5;

    knock = new Knock(frameWidth/2-paddleWidth/2,frameHeight-paddleHeight*2-7,paddleWidth,paddleHeight);
}

void myWidget::createBall()
{
    qreal ballSide = knock->getShape().height();

    ball = new Ball(frameWidth/2-ballSide/2,knock->getShape().top()-ballSide,ballSide);
}


void myWidget::moveBall()
{
    ball->move();
    QRectF ballShape = ball->getShape();
    QRectF knockShape = knock->getShape();

    qreal ballLeft = ballShape.left();
    qreal ballRight = ballShape.right();
    qreal ballBottom = ballShape.bottom();
    qreal ballTop = ballShape.top();

    if( ballLeft < rect().left() || ballRight > rect().right()  )
    ball->setDirX( -ball->getDirX() );


    if( ballBottom>knockShape.bottom() )
    {
     if( knockShape.intersects( ballShape ) )
     {
         ball->setDirX( ball->getDirX() );
         ball->setDirY( -ball->getDirY() );
     }
     else
     {
         isFailure = true;
         update();
         return;
     }
    }

    if( ballTop < rect().top() )
    ball->setDirY( -ball->getDirY() );
    else
    {
    for( int i = 0; i < bricks.size(); ++i )
    {
        Brick *brick = bricks.at( i );
        QRectF brickShape = brick->getShape();
        brick->setColor(getcolor(brick->getkind()-1));
        if( ballRight+1  >= brickShape.left() && ballLeft -1 <= brickShape.right() &&ballBottom +1 >= brickShape.top() &&ballTop -1 <= brickShape.bottom() ) {
            brick->setkind(brick->getkind()-1);

            if (ballBottom + 1  >= brickShape.top() &&ballTop + (ballBottom-ballTop)/2 -1 <= brickShape.bottom())
                ball->setDirX( -ball->getDirX() );
            else if (ballRight - (ballBottom-ballTop)/2+1 >= brickShape.left() && ballRight - (ballBottom-ballTop)/2-1 <= brickShape.right())
                ball->setDirY(-ball->getDirY());
            else{
                ball->setDirX(-ball->getDirX());
                ball->setDirY(-ball->getDirY());
            }
        if (brick->getkind()==0)
        {
        Brick *temp = bricks.takeAt(i);
        delete temp;
        score += 100;
        if (is_begin) setBallSpeed((1+score/M)*0.5);
       timer->setInterval( 5-score/M );
        emit scoreChanged( score );
        }
        else{
            brick->setColor(getcolor(brick->getkind()-1));
            score += 100;
            if (is_begin) setBallSpeed((1+score/M)*0.5);
            timer->setInterval(5-score/M);
            emit scoreChanged(score);
        }
        break;
        }
    }

    if( bricks.size() == 0 )
    isWin = true;
    }

    update();
}



void myWidget::moveKnockLeft()
{
    if( knock->getLeft() > rect().left() )
    {
    knock->moveLeft();
    update();
    }
}

void myWidget::moveKnockRight()
{
    if( knock->getRight() < rect().right() )
    {
    knock->moveRight();
    update();
    }
}

void myWidget::keyPressEvent( QKeyEvent *event )
{
    switch( event->key() )
    {
        case Qt::Key_Left:
        moveKnockLeft();
        break;

        case Qt::Key_Right:
        moveKnockRight();
        break;

        case Qt::Key_1:
        helper1();
        break;

        case Qt::Key_2:
        helper2();
        break;

        case Qt::Key_3:
        helper3();
        break;
    }
}

void myWidget::setBallSpeed( qreal speed )
{
    ball->setSpeed( speed );
}

void myWidget::setPaddleStep( qreal step )
{
    knock->setStep( step );
}


void myWidget::on_changeScore( int score )
{
    ui->lcdNumber->display(score);
}

void myWidget::up() {
    ifstream myfile("score.txt");

    if (myfile){

    int i = 1;
    int t = 0;
    char buffer[256];
    while (!myfile.fail()) {

        myfile.getline(buffer, 256);
        if (t == 0) {
            t = 1; continue;
        }
        istringstream iss(buffer);
        int x;
        iss>>x;scorexi[i]=x;
        i++;

    }

    i--;
    numscore=i-1;
    for (int i=1;i<=numscore-1;i++)
        for (int j=i+1;j<=numscore;j++)
            if (scorexi[i]<scorexi[j]){
                int t=scorexi[i];scorexi[i]=scorexi[j];scorexi[j]=t;
            }
    }
}


