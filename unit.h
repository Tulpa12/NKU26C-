#ifndef UNIT_H
#define UNIT_H

#include <QGraphicsRectItem>
#include <QPainter>

class Unit : public QGraphicsRectItem {
public:
    Unit(int maxHp, double speed, const QPointF& pos, const QRectF& rect,
         QGraphicsItem* parent = nullptr);
    virtual ~Unit() = default;

    void takeDamage(int dmg);
    bool isDead() const;
    int health() const;
    int maxHealth() const;

    virtual void updateUnit() = 0;

    enum { Type = QGraphicsItem::UserType + 10 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

protected:
    bool moveTowards(const QPointF& target);
    int m_hp;
    int m_maxHp;
    double m_speed;
};

#endif
