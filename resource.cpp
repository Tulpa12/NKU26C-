#include "resource.h"
#include <QPainter>
#include <QFont>
#include <QStyleOptionGraphicsItem>

ResourceNode::ResourceNode(int amount, const QPointF& pos, QGraphicsItem* parent)
    : QGraphicsEllipseItem(-25, -25, 50, 50, parent)
    , m_amount(amount)
{
    setPos(pos);
    setBrush(QColor(255, 215, 0));
    setPen(QPen(QColor(180, 150, 0), 2));
    setZValue(0);
}

int ResourceNode::gather(int amount)
{
    int taken = qMin(amount, m_amount);
    m_amount -= taken;
    if (m_amount <= 0) {
        setBrush(QColor(120, 120, 120));
        setPen(QPen(Qt::darkGray, 2));
    }
    return taken;
}

bool ResourceNode::isDepleted() const
{
    return m_amount <= 0;
}

int ResourceNode::remaining() const
{
    return m_amount;
}

void ResourceNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                         QWidget* widget)
{
    QGraphicsEllipseItem::paint(painter, option, widget);
    if (!isDepleted()) {
        painter->setPen(Qt::black);
        painter->setFont(QFont("Arial", 9, QFont::Bold));
        painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(m_amount));
    }
}
