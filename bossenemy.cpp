#include "bossenemy.h"
#include "texturemanager.h"
#include <QPainter>
#include <QPen>
#include <QFont>

BossEnemy::BossEnemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent)
    : Enemy(pos, base, parent)
{
    setRect(-24, -24, 48, 48);
    m_maxHp = 150;
    m_hp = 150;
    m_speed = 1.0;
    m_attackDamage = 12;
    m_attackRange = 45;
    m_attackCooldownMax = 25;
    m_aggroRange = 150;
}

void BossEnemy::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                      QWidget* widget)
{
    const QPixmap& tex = TextureManager::instance().bossTex();
    painter->drawPixmap(rect().toRect(), tex);

    Unit::paint(painter, option, widget);
}
