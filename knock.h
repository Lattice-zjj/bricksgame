#ifndef _KNOCK_H_
#define _KNOCK_H_

#include <QRect>
#include <QColor>

class Knock
{
public:
    Knock( qreal, qreal, qreal, qreal);
    ~Knock();

    QRectF getShape() const;
    qreal getLeft() const;
    qreal getRight() const;
    qreal getStep() const;

    void setShape( const QRectF & );
    void setLeft( qreal );
    void setRight( qreal );
    void setStep( qreal );

    void moveLeft();
    void moveRight();

private:
    QRectF *shape;
    QColor color;
    qreal step;
};


#endif
