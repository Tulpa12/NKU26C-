#ifndef FASTENEMY_H
#define FASTENEMY_H

#include "enemy.h"

class FastEnemy : public Enemy {
public:
    FastEnemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent = nullptr);

    enum { Type = QGraphicsItem::UserType + 9 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
};

#endif
