#include "worker.h"
#include "resource.h"
#include "building.h"
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QStyleOptionGraphicsItem>
#include <cmath>

Worker::Worker(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent)
    : QGraphicsRectItem(-10, -10, 20, 20, parent)
    , m_state(IDLE)
    , m_speed(3.0)
    , m_targetResource(nullptr)
    , m_base(base)
    , m_carriedGold(0)
    , m_harvestTimer(0)
    , m_pendingDeposit(0)
    , m_selected(false)
{
    setPos(pos);
    setBrush(Qt::blue);
    setPen(QPen(Qt::darkBlue, 1));
    setZValue(1);
}

void Worker::moveTo(const QPointF& target)
{
    m_state = MOVING_TO_TARGET;
    m_moveTarget = target;
    m_targetResource = nullptr;
    m_harvestTimer = 0;
}

void Worker::gatherFrom(ResourceNode* resource)
{
    if (!resource || resource->isDepleted())
        return;
    m_state = MOVING_TO_RESOURCE;
    m_targetResource = resource;
    QPointF offset(30, 0);
    m_moveTarget = resource->scenePos() + offset;
    m_harvestTimer = 0;
}

void Worker::setSelected(bool sel)
{
    m_selected = sel;
    update();
}

bool Worker::isSelected() const
{
    return m_selected;
}

int Worker::takeDepositedGold()
{
    int g = m_pendingDeposit;
    m_pendingDeposit = 0;
    return g;
}

void Worker::updateUnit()
{
    if (m_state == IDLE)
        return;

    if (m_state == HARVESTING) {
        m_harvestTimer--;
        if (m_harvestTimer <= 0) {
            if (m_targetResource && !m_targetResource->isDepleted()) {
                m_carriedGold = m_targetResource->gather(CARRY_CAPACITY);
                m_state = MOVING_TO_BASE;
                m_moveTarget = m_base->scenePos();
            } else {
                m_state = IDLE;
            }
        }
        return;
    }

    // Moving states
    QPointF current = scenePos();
    QPointF dir = m_moveTarget - current;
    double dist = std::sqrt(dir.x() * dir.x() + dir.y() * dir.y());

    if (dist < m_speed) {
        setPos(m_moveTarget);

        if (m_state == MOVING_TO_RESOURCE) {
            if (m_targetResource && !m_targetResource->isDepleted()) {
                m_state = HARVESTING;
                m_harvestTimer = HARVEST_TIME;
            } else {
                m_state = IDLE;
            }
        } else if (m_state == MOVING_TO_BASE) {
            m_pendingDeposit = m_carriedGold;
            m_carriedGold = 0;
            m_state = IDLE;
            if (m_targetResource && !m_targetResource->isDepleted()) {
                gatherFrom(m_targetResource);
            }
        } else if (m_state == MOVING_TO_TARGET) {
            m_state = IDLE;
        }
    } else {
        dir /= dist;
        setPos(current + dir * m_speed);
    }
}

void Worker::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget)
{
    // Body color based on state
    QColor bodyColor = Qt::blue;
    if (m_state == HARVESTING)
        bodyColor = QColor(200, 200, 50);
    else if (m_carriedGold > 0)
        bodyColor = QColor(255, 200, 0);

    painter->setBrush(bodyColor);
    if (m_selected) {
        painter->setPen(QPen(Qt::green, 2.5));
    } else {
        painter->setPen(QPen(Qt::darkBlue, 1));
    }
    painter->drawRect(rect());

    // Selection indicator
    if (m_selected) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 1.5, Qt::DashLine));
        painter->drawRect(rect().adjusted(-4, -4, 4, 4));
    }

    // State label
    if (m_state == HARVESTING) {
        painter->setPen(Qt::black);
        painter->setFont(QFont("Arial", 7));
        painter->drawText(rect().adjusted(0, -14, 0, -2), Qt::AlignCenter,
                          QString::fromUtf8("采集"));
    }
}
