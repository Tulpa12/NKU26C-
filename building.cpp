#include "building.h"
#include "texturemanager.h"
#include <QPainter>
#include <QPen>
#include <QFont>

CommandCenter::CommandCenter(const QPointF& pos, QGraphicsItem* parent)
    : QGraphicsRectItem(-42, -42, 84, 84, parent)
    , m_hp(300)
    , m_maxHp(300)
{
    setPos(pos);
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
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const QPixmap& tex = TextureManager::instance().baseTex();
    painter->drawPixmap(rect().toRect(), tex);

    // Health bar
    if (m_hp < m_maxHp) {
        double w = 84;
        double barH = 6;
        double y = -50;

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawRect(QRectF(-42, y, w, barH));

        double ratio = static_cast<double>(m_hp) / m_maxHp;
        painter->setBrush(Qt::green);
        painter->drawRect(QRectF(-42, y, w * ratio, barH));
    }
}
