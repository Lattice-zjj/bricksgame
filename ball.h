#ifndef BALL_H
#define BALL_H

#include <QRect>
#include <QColor>

class Ball
{
public:
    Ball(qreal,qreal,qreal);
    ~Ball();

    void move();
    QRectF rect() const;

    void setShape(const QRectF &);
    void setDirX(qreal);
    void setDirY(qreal);
    void setSpeed(qreal);

    QRectF getShape() const;
    qreal getDirX() const;
    qreal getDirY() const;
    qreal getSpeed() const;




private:
    QRectF *shape;
    qreal dirX;
    qreal dirY;
    qreal speed;

};

#endif // BALL_H
