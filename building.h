#ifndef BUILDING_H
#define BUILDING_H

#include <QGraphicsRectItem>

class CommandCenter : public QGraphicsRectItem {
public:
    CommandCenter(const QPointF& pos, QGraphicsItem* parent = nullptr);

    enum { Type = QGraphicsItem::UserType + 2 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
};

#endif
