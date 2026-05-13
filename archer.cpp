#include "archer.h"
#include "enemy.h"
#include "texturemanager.h"
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <cmath>

Archer::Archer(const QPointF& pos, QGraphicsItem* parent)
    : Soldier(pos, parent)
{
    m_maxHp = 35;
    m_hp = 35;
    m_speed = 2.2;
    m_attackDamage = 12;
    m_attackRange = 130;
    m_attackCooldownMax = 25;
    setRect(-16, -16, 32, 32);
}

void Archer::updateUnit()
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

        if (dist <= m_attackRange) {
            // Kite: back away if enemy is too close
            if (dist < SAFE_DISTANCE && dist > 0) {
                QPointF away = scenePos() + QPointF(dx / dist * m_speed * 2,
                                                     dy / dist * m_speed * 2);
                setPos(away);
            }
            if (m_attackCooldown <= 0) {
                m_attackTarget->takeDamage(m_attackDamage);
                m_attackCooldown = m_attackCooldownMax;
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

void Archer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                    QWidget* widget)
{
    const QPixmap& tex = TextureManager::instance().archerTex();
    painter->drawPixmap(rect().toRect(), tex);

    Unit::paint(painter, option, widget);

    if (m_attackMove) {
        painter->setPen(QPen(Qt::red, 2));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(rect().adjusted(-6, -6, 6, 6));
    }
}
