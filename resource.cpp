#include "resource.h"
#include "texturemanager.h"
#include <QPainter>
#include <QFont>

ResourceNode::ResourceNode(int amount, const QPointF& pos, QGraphicsItem* parent)
    : QGraphicsEllipseItem(-27, -27, 54, 54, parent)
    , m_amount(amount)
{
    setPos(pos);
    setZValue(0);
}

int ResourceNode::gather(int amount)
{
    int taken = qMin(amount, m_amount);
    m_amount -= taken;
    return taken;
}

bool ResourceNode::isDepleted() const { return m_amount <= 0; }
int ResourceNode::remaining() const { return m_amount; }

void ResourceNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                         QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const QPixmap& tex = TextureManager::instance().goldmineTex();
    painter->drawPixmap(rect().toRect(), tex);

    if (!isDepleted()) {
        painter->setPen(Qt::black);
        painter->setFont(QFont("Arial", 10, QFont::Bold));
        painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(m_amount));
    }
}
