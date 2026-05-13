#ifndef RANGEDENEMY_H
#define RANGEDENEMY_H

#include "enemy.h"

class RangedEnemy : public Enemy {
public:
    RangedEnemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent = nullptr);

    enum { Type = QGraphicsItem::UserType + 12 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
};

#endif
