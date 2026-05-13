#include "soldier.h"
#include "enemy.h"
#include "texturemanager.h"
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <cmath>

Soldier::Soldier(const QPointF& pos, QGraphicsItem* parent)
    : Unit(60, 2.5, pos, QRectF(-16, -16, 32, 32), parent)
    , m_state(IDLE)
    , m_attackTarget(nullptr)
    , m_attackCooldown(0)
    , m_attackMove(false)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void Soldier::moveTo(const QPointF& target)
{
    m_state = MOVING;
    m_moveTarget = target;
    m_attackTarget = nullptr;
}

void Soldier::attackTarget(Enemy* enemy)
{
    if (!enemy || enemy->isDead())
        return;
    m_state = ATTACKING;
    m_attackTarget = enemy;
}

void Soldier::updateUnit()
{
    if (isDead())
        return;

    if (m_attackCooldown > 0)
        m_attackCooldown--;

    if (m_attackTarget && m_attackTarget->isDead()) {
        m_attackTarget = nullptr;
        m_state = IDLE;
    }

    if (m_state == ATTACKING) {
        if (!m_attackTarget) {
            m_state = IDLE;
            return;
        }

        double dx = scenePos().x() - m_attackTarget->scenePos().x();
        double dy = scenePos().y() - m_attackTarget->scenePos().y();
        double dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= ATTACK_RANGE) {
            if (m_attackCooldown <= 0) {
                m_attackTarget->takeDamage(ATTACK_DAMAGE);
                m_attackCooldown = ATTACK_COOLDOWN;
            }
        } else {
            moveTowards(m_attackTarget->scenePos());
        }
        return;
    }

    if (m_state == MOVING) {
        if (moveTowards(m_moveTarget)) {
            m_state = IDLE;
        }
        return;
    }
}

void Soldier::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                    QWidget* widget)
{
    const QPixmap& tex = TextureManager::instance().soldierTex();
    painter->drawPixmap(rect().toRect(), tex);

    Unit::paint(painter, option, widget);

    // Attack-move indicator
    if (m_attackMove) {
        painter->setPen(QPen(Qt::red, 2));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(rect().adjusted(-6, -6, 6, 6));
    }
}
