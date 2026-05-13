#include "enemy.h"
#include "soldier.h"
#include "building.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <cmath>

Enemy::Enemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent)
    : Unit(40, 1.5, pos, QRectF(-10, -10, 20, 20), parent)
    , m_state(MOVING_TO_TARGET)
    , m_baseTarget(base)
    , m_attackTarget(nullptr)
    , m_attackCooldown(0)
{
    setBrush(Qt::red);
    setPen(QPen(QColor(150, 0, 0), 1));
}

void Enemy::updateUnit()
{
    if (isDead())
        return;

    if (m_attackCooldown > 0)
        m_attackCooldown--;

    // Validate targets
    if (m_attackTarget && m_attackTarget->isDead())
        m_attackTarget = nullptr;

    // Always check for nearby soldiers (aggro)
    Soldier* nearbySoldier = nullptr;
    double closestDist = AGGRO_RANGE;
    for (auto* item : scene()->items()) {
        Soldier* s = qgraphicsitem_cast<Soldier*>(item);
        if (!s || s->isDead())
            continue;
        double dx = scenePos().x() - s->scenePos().x();
        double dy = scenePos().y() - s->scenePos().y();
        double dist = std::sqrt(dx * dx + dy * dy);
        if (dist < closestDist) {
            closestDist = dist;
            nearbySoldier = s;
        }
    }

    if (nearbySoldier) {
        m_attackTarget = nearbySoldier;
        m_state = ATTACKING_UNIT;
    }

    // Attack logic
    if (m_state == ATTACKING_UNIT) {
        if (!m_attackTarget) {
            m_state = MOVING_TO_TARGET;
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

    // Attack base if in range
    if (m_baseTarget && !m_baseTarget->isDestroyed()) {
        double dx = scenePos().x() - m_baseTarget->scenePos().x();
        double dy = scenePos().y() - m_baseTarget->scenePos().y();
        double dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= ATTACK_RANGE + 30) {
            m_state = ATTACKING_BASE;
            if (m_attackCooldown <= 0) {
                m_baseTarget->takeDamage(ATTACK_DAMAGE);
                m_attackCooldown = ATTACK_COOLDOWN;
            }
            return;
        }
    }

    // Default: move toward base
    m_state = MOVING_TO_TARGET;
    if (m_baseTarget)
        moveTowards(m_baseTarget->scenePos());
}

void Enemy::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                  QWidget* widget)
{
    painter->setBrush(Qt::red);
    painter->setPen(QPen(QColor(150, 0, 0), 1));

    Unit::paint(painter, option, widget);
}
