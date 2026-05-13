#include "heavyenemy.h"
#include "texturemanager.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

HeavyEnemy::HeavyEnemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent)
    : Enemy(pos, base, parent)
{
    m_maxHp = 100;
    m_hp = 100;
    m_speed = 1.0;
    m_attackDamage = 10;
    m_attackRange = 40;
    m_attackCooldownMax = 35;
    m_aggroRange = 80;
    setRect(-20, -20, 40, 40);
}

void HeavyEnemy::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                        QWidget* widget)
{
    const QPixmap& tex = TextureManager::instance().heavyenemyTex();
    painter->drawPixmap(rect().toRect(), tex);

    Unit::paint(painter, option, widget);
}
