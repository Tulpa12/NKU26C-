#include "fastenemy.h"
#include "texturemanager.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

FastEnemy::FastEnemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent)
    : Enemy(pos, base, parent)
{
    m_maxHp = 22;
    m_hp = 22;
    m_speed = 3.0;
    m_attackDamage = 4;
    m_attackRange = 30;
    m_attackCooldownMax = 18;
    m_aggroRange = 160;
    setRect(-14, -14, 28, 28);
}

void FastEnemy::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                       QWidget* widget)
{
    const QPixmap& tex = TextureManager::instance().fastenemyTex();
    painter->drawPixmap(rect().toRect(), tex);

    Unit::paint(painter, option, widget);
}
