#ifndef RESOURCE_H
#define RESOURCE_H

#include <QGraphicsEllipseItem>

class ResourceNode : public QGraphicsEllipseItem {
public:
    ResourceNode(int amount, const QPointF& pos, QGraphicsItem* parent = nullptr);

    int gather(int amount);
    bool isDepleted() const;
    int remaining() const;

    enum { Type = QGraphicsItem::UserType + 1 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    int m_amount;
};

#endif
