#include "enemy.h"
#include "soldier.h"
#include "building.h"
#include "texturemanager.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <cmath>

Enemy::Enemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent)
    : Unit(40, 1.5, pos, QRectF(-16, -16, 32, 32), parent)
    , m_state(MOVING_TO_TARGET)
    , m_baseTarget(base)
    , m_attackTarget(nullptr)
    , m_attackCooldown(0)
    , m_attackDamage(5)
    , m_attackRange(35)
    , m_attackCooldownMax(30)
    , m_aggroRange(100)
{
}

void Enemy::scaleStats(double hpMul, double dmgMul)
{
    m_maxHp = static_cast<int>(m_maxHp * hpMul);
    m_hp = m_maxHp;
    m_attackDamage = static_cast<int>(m_attackDamage * dmgMul);
}

void Enemy::updateUnit()
{
    if (isDead())
        return;

    if (m_attackCooldown > 0)
        m_attackCooldown--;

    if (m_attackTarget && m_attackTarget->isDead())
        m_attackTarget = nullptr;

    Soldier* nearbySoldier = nullptr;
    double closestDist = m_aggroRange;
    for (auto* item : scene()->items()) {
        Soldier* s = dynamic_cast<Soldier*>(item);
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

    if (m_state == ATTACKING_UNIT) {
        if (!m_attackTarget) {
            m_state = MOVING_TO_TARGET;
            return;
        }

        double dx = scenePos().x() - m_attackTarget->scenePos().x();
        double dy = scenePos().y() - m_attackTarget->scenePos().y();
        double dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= m_attackRange) {
            if (m_attackCooldown <= 0) {
                m_attackTarget->takeDamage(m_attackDamage);
                m_attackCooldown = m_attackCooldownMax;
            }
        } else {
            moveTowards(m_attackTarget->scenePos());
        }
        return;
    }

    if (m_baseTarget && !m_baseTarget->isDestroyed()) {
        double dx = scenePos().x() - m_baseTarget->scenePos().x();
        double dy = scenePos().y() - m_baseTarget->scenePos().y();
        double dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= m_attackRange + 30) {
            m_state = ATTACKING_BASE;
            if (m_attackCooldown <= 0) {
                m_baseTarget->takeDamage(m_attackDamage);
                m_attackCooldown = m_attackCooldownMax;
            }
            return;
        }
    }

    m_state = MOVING_TO_TARGET;
    if (m_baseTarget)
        moveTowards(m_baseTarget->scenePos());
}

void Enemy::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                  QWidget* widget)
{
    const QPixmap& tex = TextureManager::instance().enemyTex();
    painter->drawPixmap(rect().toRect(), tex);

    Unit::paint(painter, option, widget);
}
