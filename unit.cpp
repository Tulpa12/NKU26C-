#include "unit.h"
#include <QPen>
#include <cmath>

Unit::Unit(int maxHp, double speed, const QPointF& pos, const QRectF& rect,
           QGraphicsItem* parent)
    : QGraphicsRectItem(rect, parent)
    , m_hp(maxHp)
    , m_maxHp(maxHp)
    , m_speed(speed)
    , m_selected(false)
{
    setPos(pos);
    setZValue(1);
}

void Unit::takeDamage(int dmg)
{
    m_hp -= dmg;
    if (m_hp < 0) m_hp = 0;
}

bool Unit::isDead() const { return m_hp <= 0; }
int Unit::health() const { return m_hp; }
int Unit::maxHealth() const { return m_maxHp; }

void Unit::setSelected(bool sel)
{
    m_selected = sel;
    update();
}

bool Unit::isSelected() const { return m_selected; }

bool Unit::moveTowards(const QPointF& target)
{
    QPointF current = scenePos();
    QPointF dir = target - current;
    double dist = std::sqrt(dir.x() * dir.x() + dir.y() * dir.y());

    if (dist < m_speed) {
        setPos(target);
        return true;
    }
    dir /= dist;
    setPos(current + dir * m_speed);
    return false;
}

void Unit::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                  QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawRect(rect());

    if (m_selected) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 1.5, Qt::DashLine));
        painter->drawRect(rect().adjusted(-4, -4, 4, 4));
    }

    if (m_hp < m_maxHp) {
        double w = rect().width() * 1.2;
        double barH = 4;
        double x = -w / 2 + rect().width() / 2;
        double y = rect().top() - 8;

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawRect(QRectF(x, y, w, barH));

        double ratio = static_cast<double>(m_hp) / m_maxHp;
        painter->setBrush(Qt::green);
        painter->drawRect(QRectF(x, y, w * ratio, barH));
    }
}
