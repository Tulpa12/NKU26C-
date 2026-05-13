#ifndef BOSSENEMY_H
#define BOSSENEMY_H

#include "enemy.h"

class BossEnemy : public Enemy {
public:
    BossEnemy(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent = nullptr);

    enum { Type = QGraphicsItem::UserType + 6 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
};

#endif
