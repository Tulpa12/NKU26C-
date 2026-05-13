#include "tank.h"
#include "texturemanager.h"
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>

Tank::Tank(const QPointF& pos, QGraphicsItem* parent)
    : Soldier(pos, parent)
{
    m_maxHp = 130;
    m_hp = 130;
    m_speed = 1.3;
    m_attackDamage = 16;
    m_attackRange = 45;
    m_attackCooldownMax = 30;
    setRect(-18, -18, 36, 36);
}

void Tank::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                  QWidget* widget)
{
    const QPixmap& tex = TextureManager::instance().tankTex();
    painter->drawPixmap(rect().toRect(), tex);

    Unit::paint(painter, option, widget);

    if (m_attackMove) {
        painter->setPen(QPen(Qt::red, 2));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(rect().adjusted(-6, -6, 6, 6));
    }
}
