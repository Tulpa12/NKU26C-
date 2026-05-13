#include "worker.h"
#include "resource.h"
#include "building.h"
#include <QPainter>
#include <QPen>
#include <QFont>

Worker::Worker(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent)
    : Unit(30, 3.0, pos, QRectF(-10, -10, 20, 20), parent)
    , m_state(IDLE)
    , m_targetResource(nullptr)
    , m_base(base)
    , m_carriedGold(0)
    , m_harvestTimer(0)
    , m_pendingDeposit(0)
{
    setBrush(Qt::blue);
    setPen(QPen(Qt::darkBlue, 1));
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
    m_moveTarget = resource->scenePos() + QPointF(30, 0);
    m_harvestTimer = 0;
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

    bool arrived = moveTowards(m_moveTarget);

    if (arrived) {
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
    }
}

void Worker::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget)
{
    QColor bodyColor = Qt::blue;
    if (m_state == HARVESTING)
        bodyColor = QColor(200, 200, 50);
    else if (m_carriedGold > 0)
        bodyColor = QColor(255, 200, 0);

    painter->setBrush(bodyColor);
    if (m_selected)
        painter->setPen(QPen(Qt::green, 2.5));
    else
        painter->setPen(QPen(Qt::darkBlue, 1));

    Unit::paint(painter, option, widget);

    if (m_state == HARVESTING) {
        painter->setPen(Qt::black);
        painter->setFont(QFont("Arial", 7));
        painter->drawText(rect().adjusted(0, -14, 0, -2), Qt::AlignCenter,
                          QString::fromUtf8("采集"));
    }
}
