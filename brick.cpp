#include "brick.h"

Brick::Brick(qreal x,qreal y,qreal width,qreal height,QColor brickColor)
{
    shape=new QRectF(x,y,width,height);
    color=brickColor;
}

Brick::~Brick()
{
    delete shape;
}


void Brick::setShape(const QRectF &newShape)
{
    shape->setRect(newShape.left(),newShape.top(),newShape.width(),newShape.height());
}


void Brick::setColor(const QColor &newColor)
{
    color=newColor;
}


void Brick::setkind(const int &x)
{
    kind=x;
}

int Brick::getkind() const
{
    return kind;
}


QColor Brick::getColor() const
{
    return color;
}


QRectF Brick::getShape() const
{
    return QRectF(shape->left(),shape->top(),shape->width(),shape->height());
}
