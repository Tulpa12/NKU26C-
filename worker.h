#ifndef WORKER_H
#define WORKER_H

#include <QGraphicsRectItem>
#include <QPointF>

class ResourceNode;
class CommandCenter;

class Worker : public QGraphicsRectItem {
public:
    enum State { IDLE, MOVING_TO_TARGET, MOVING_TO_RESOURCE, HARVESTING, MOVING_TO_BASE };

    Worker(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent = nullptr);

    void moveTo(const QPointF& target);
    void gatherFrom(ResourceNode* resource);
    void setSelected(bool sel);
    bool isSelected() const;
    void updateUnit();
    int takeDepositedGold();

    enum { Type = QGraphicsItem::UserType + 3 };
    int type() const override { return Type; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    State m_state;
    double m_speed;
    QPointF m_moveTarget;
    ResourceNode* m_targetResource;
    CommandCenter* m_base;
    int m_carriedGold;
    int m_harvestTimer;
    int m_pendingDeposit;
    bool m_selected;

    static constexpr int HARVEST_TIME = 30;  // ticks
    static constexpr int CARRY_CAPACITY = 10;
};

#endif
