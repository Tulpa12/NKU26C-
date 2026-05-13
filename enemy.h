#ifndef ENEMY_H
#define ENEMY_H

#include "unit.h"
#include <QPointer>

class Soldier;
class CommandCenter;

class Enemy : public Unit {
public:
    Enemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent = nullptr);

    void updateUnit() override;
    CommandCenter* baseTarget() const { return m_baseTarget; }

    enum { Type = QGraphicsItem::UserType + 5 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    enum State { MOVING_TO_TARGET, ATTACKING_UNIT, ATTACKING_BASE };

    State m_state;
    CommandCenter* m_baseTarget;
    QPointer<Soldier> m_attackTarget;
    int m_attackCooldown;

    static constexpr int ATTACK_DAMAGE = 5;
    static constexpr int ATTACK_RANGE = 35;
    static constexpr int ATTACK_COOLDOWN = 30;
    static constexpr int AGGRO_RANGE = 100;
};

#endif
