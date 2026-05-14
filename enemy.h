#ifndef ENEMY_H
#define ENEMY_H

#include "unit.h"

class Soldier;
class CommandCenter;

class Enemy : public Unit {      //继承自unit
public:
    Enemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent = nullptr);

    void updateUnit() override;
    void scaleStats(double hpMul, double dmgMul);             //动态难度变化
    CommandCenter* baseTarget() const { return m_baseTarget; }

    Soldier* attackTarget() const { return m_attackTarget; }
    void clearAttackTarget() { m_attackTarget = nullptr; m_state = MOVING_TO_TARGET; }

    enum { Type = QGraphicsItem::UserType + 5 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

protected:
    enum State { MOVING_TO_TARGET, ATTACKING_UNIT, ATTACKING_BASE };  //移动，攻击士兵，攻击基地

    State m_state;
    CommandCenter* m_baseTarget;
    Soldier* m_attackTarget;
    int m_attackCooldown;
    int m_attackDamage;
    int m_attackRange;
    int m_attackCooldownMax;
    int m_aggroRange;
};

#endif
