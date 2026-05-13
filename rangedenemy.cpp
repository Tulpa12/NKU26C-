#include "rangedenemy.h"
#include "texturemanager.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

RangedEnemy::RangedEnemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent)
    : Enemy(pos, base, parent)
{
    m_maxHp = 30;
    m_hp = 30;
    m_speed = 1.8;
    m_attackDamage = 7;
    m_attackRange = 120;
    m_attackCooldownMax = 30;
    m_aggroRange = 150;
    setRect(-15, -15, 30, 30);
}

void RangedEnemy::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                        QWidget* widget)
{
    const QPixmap& tex = TextureManager::instance().rangedenemyTex();
    painter->drawPixmap(rect().toRect(), tex);

    Unit::paint(painter, option, widget);
}
