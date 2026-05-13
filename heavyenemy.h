#ifndef HEAVYENEMY_H
#define HEAVYENEMY_H

#include "enemy.h"

class HeavyEnemy : public Enemy {
public:
    HeavyEnemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent = nullptr);

    enum { Type = QGraphicsItem::UserType + 11 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
};

#endif
