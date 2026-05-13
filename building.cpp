#include "building.h"
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QStyleOptionGraphicsItem>

CommandCenter::CommandCenter(const QPointF& pos, QGraphicsItem* parent)
    : QGraphicsRectItem(-40, -40, 80, 80, parent)
{
    setPos(pos);
    setBrush(QColor(200, 170, 50));
    setPen(QPen(Qt::black, 3));
    setZValue(0);
}

void CommandCenter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                          QWidget* widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 8, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter,
                      QString::fromUtf8("基地"));
}
