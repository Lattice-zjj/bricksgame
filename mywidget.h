#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "ball.h"
#include "knock.h"
#include "brick.h"
#include <QWidget>
#include <QMovie>


namespace Ui {
class myWidget;
}

class myWidget : public QWidget
{
    Q_OBJECT

public:
    explicit myWidget(QWidget *parent = 0);
    ~myWidget();
    void win();
    void lose();
    void begin();

    void setBallSpeed( qreal );
    void setBricksNumbers( int , int );
    void setPaddleStep( qreal );
    void on_startGame();
    QColor getcolor(int x);
    int randomInt( int high );
    void up();
    void update1();

public slots:
    void on_changeScore( int );
    void helper1();
    void helper2();
    void helper3();

signals:
    void finished();
    void scoreChanged( int );


protected:
    void timerEvent(QTimerEvent *e);
    void paintEvent(QPaintEvent * );
    void keyPressEvent( QKeyEvent * );


private slots:
    void moveBall();
    void moveKnockLeft();
    void moveKnockRight();


private:
    Ui::myWidget *ui;

    int gameTime;
    int gameTimerId;
    int overTimerId;
    int winTimerId;
    int helper1TimerId;
    int helper2TimerId;
    int helper3TimerId;
    int numhelp1;
    int numhelp2;
    int numhelp3;
    int jie;

    QMovie overMovie;
    QMovie winMovie;

    void createBricks();
    void createPaddle();
    void createBall();

    QRectF adjustShape( QRectF, qreal, qreal );
    QColor randomColor();

    Ball *ball;
    Knock *knock;
    QList< Brick * > bricks;
    QTimer *timer;

    int rows;
    int columns;
    int score;
    bool isWin;
    bool isFailure;
    bool is_begin;
    int scorexi[100];
    int numscore;

    double frameWidth;
    double frameHeight;


};

#endif // MYWIDGET_H
