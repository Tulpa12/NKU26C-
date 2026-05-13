#ifndef TANK_H
#define TANK_H

#include "soldier.h"

class Tank : public Soldier {
public:
    Tank(const QPointF& pos, QGraphicsItem* parent = nullptr);

    double searchRange() const override { return m_attackMove ? 120 : 90; }

    enum { Type = QGraphicsItem::UserType + 8 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
};

#endif
