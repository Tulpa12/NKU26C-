#include "soldier.h"
#include "enemy.h"
#include <QPainter>
#include <QPen>
#include <cmath>

Soldier::Soldier(const QPointF& pos, QGraphicsItem* parent)
    : Unit(60, 2.5, pos, QRectF(-11, -11, 22, 22), parent)
    , m_state(IDLE)
    , m_attackTarget(nullptr)
    , m_attackCooldown(0)
{
    setBrush(QColor(60, 180, 60));
    setPen(QPen(Qt::darkGreen, 1.5));
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

    // Validate current attack target
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

    // IDLE: auto-scan for nearby enemies
    // (Enemy scanning is handled by GameScene)
}

void Soldier::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                    QWidget* widget)
{
    painter->setBrush(QColor(60, 180, 60));
    if (m_selected)
        painter->setPen(QPen(Qt::green, 2.5));
    else
        painter->setPen(QPen(Qt::darkGreen, 1.5));

    Unit::paint(painter, option, widget);
}
