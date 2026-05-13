#include "building.h"
#include <QPainter>
#include <QPen>
#include <QFont>

CommandCenter::CommandCenter(const QPointF& pos, QGraphicsItem* parent)
    : QGraphicsRectItem(-40, -40, 80, 80, parent)
    , m_hp(300)
    , m_maxHp(300)
{
    setPos(pos);
    setBrush(QColor(200, 170, 50));
    setPen(QPen(Qt::black, 3));
    setZValue(0);
}

void CommandCenter::takeDamage(int dmg)
{
    m_hp -= dmg;
    if (m_hp < 0) m_hp = 0;
}

bool CommandCenter::isDestroyed() const { return m_hp <= 0; }
int CommandCenter::health() const { return m_hp; }
int CommandCenter::maxHealth() const { return m_maxHp; }

void CommandCenter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                          QWidget* widget)
{
    QGraphicsRectItem::paint(painter, option, widget);

    // Label
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 8, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter,
                      QString::fromUtf8("基地"));

    // Health bar
    if (m_hp < m_maxHp) {
        double w = 80;
        double barH = 6;
        double y = -50;

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawRect(QRectF(-40, y, w, barH));

        double ratio = static_cast<double>(m_hp) / m_maxHp;
        painter->setBrush(Qt::green);
        painter->drawRect(QRectF(-40, y, w * ratio, barH));
    }
}
