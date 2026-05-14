#ifndef ARCHER_H
#define ARCHER_H

#include "soldier.h"

class Archer : public Soldier {      //继承自sodier，
public:
    Archer(const QPointF& pos, QGraphicsItem* parent = nullptr);

    void updateUnit() override;
    double searchRange() const override { return m_attackMove ? 220 : 180; }   //扩大搜索范围

    enum { Type = QGraphicsItem::UserType + 7 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    static constexpr int SAFE_DISTANCE = 60;    //安全距离（风筝阈值）
};

#endif
