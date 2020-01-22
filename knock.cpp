#include "knock.h"
#include <QColor>
#include <QPoint>

Knock::Knock(qreal x,qreal y,qreal width,qreal height)
{
    step=10.0;
    shape=new QRectF(x,y,width,height);
}

Knock::~Knock()
{
    delete shape;
}

void Knock::moveLeft()
{
    shape->setLeft(shape->left()-step);
    shape->setRight(shape->right()-step);
}

void Knock::moveRight()
{
    shape->setLeft(shape->left()+step);
    shape->setRight(shape->right()+step);
}


void Knock::setShape(const QRectF &newShape)
{
    shape->setRect(newShape.left(),newShape.top(),newShape.width(),newShape.height());
}


void Knock::setLeft(qreal left)
{
    shape->setLeft(left);
}

void Knock::setRight(qreal right)
{
    shape->setRight(right);
}


void Knock::setStep(qreal newStep)
{
    step=newStep;
}

qreal Knock::getStep() const
{
    return step;
}

QRectF Knock::getShape() const
{
    return QRectF(shape->left(),shape->top(),shape->width(),shape->height());
}

qreal Knock::getLeft() const
{
    return shape->left();
}

qreal Knock::getRight() const
{
    return shape->right();
}
