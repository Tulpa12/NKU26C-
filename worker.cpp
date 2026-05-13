#include "worker.h"
#include "resource.h"
#include "building.h"
#include "texturemanager.h"
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QStyleOptionGraphicsItem>

Worker::Worker(const QPointF& pos, CommandCenter* base, QGraphicsItem* parent)
    : Unit(30, 3.0, pos, QRectF(-16, -16, 32, 32), parent)
    , m_state(IDLE)
    , m_targetResource(nullptr)
    , m_base(base)
    , m_carriedGold(0)
    , m_harvestTimer(0)
    , m_pendingDeposit(0)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
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
    m_moveTarget = resource->scenePos() + QPointF(35, 0);
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
    // Draw texture
    const QPixmap& tex = TextureManager::instance().workerTex();
    painter->drawPixmap(rect().toRect(), tex);

    // Carrying gold overlay
    if (m_carriedGold > 0 && m_state != HARVESTING) {
        painter->setBrush(QColor(255, 200, 0, 80));
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect());
    }

    Unit::paint(painter, option, widget);

    // State label
    if (m_state == HARVESTING) {
        painter->setPen(Qt::black);
        painter->setFont(QFont("Arial", 8, QFont::Bold));
        painter->drawText(rect().adjusted(0, -18, 0, -2), Qt::AlignCenter,
                          QString::fromUtf8("采集"));
    }
}
