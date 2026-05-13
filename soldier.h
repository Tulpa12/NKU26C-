#ifndef SOLDIER_H
#define SOLDIER_H

#include "unit.h"

class Enemy;

class Soldier : public Unit {
public:
    enum State { IDLE, MOVING, ATTACKING };

    Soldier(const QPointF& pos, QGraphicsItem* parent = nullptr);

    void moveTo(const QPointF& target);
    void attackTarget(Enemy* enemy);
    void updateUnit() override;
    void setAttackMove(bool enabled) { m_attackMove = enabled; }
    bool isAttackMove() const { return m_attackMove; }

    Enemy* currentTarget() const { return m_attackTarget; }
    void clearTarget() { m_attackTarget = nullptr; m_state = IDLE; }

    enum { Type = QGraphicsItem::UserType + 4 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    State m_state;
    QPointF m_moveTarget;
    Enemy* m_attackTarget;
    int m_attackCooldown;
    bool m_attackMove;

    static constexpr int ATTACK_DAMAGE = 8;
    static constexpr int ATTACK_RANGE = 50;
    static constexpr int ATTACK_COOLDOWN = 20;
};

#endif
