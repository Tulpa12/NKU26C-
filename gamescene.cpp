#include "gamescene.h"
#include "worker.h"
#include "building.h"
#include "resource.h"
#include <QGraphicsSceneMouseEvent>
#include <cstdlib>
#include <ctime>

GameScene::GameScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_base(nullptr)
    , m_timer(new QTimer(this))
    , m_selectedWorker(nullptr)
    , m_gold(100)
{
    setSceneRect(0, 0, 1200, 900);
    setBackgroundBrush(QColor(50, 130, 50));
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    connect(m_timer, &QTimer::timeout, this, &GameScene::gameTick);
}

GameScene::~GameScene()
{
    m_timer->stop();
}

void GameScene::startGame()
{
    // Command center at map center
    m_base = new CommandCenter(QPointF(600, 450));
    addItem(m_base);

    // Gold mines
    m_resources.append(new ResourceNode(500, QPointF(200, 200)));
    m_resources.append(new ResourceNode(500, QPointF(1050, 280)));
    m_resources.append(new ResourceNode(500, QPointF(480, 780)));
    for (auto* r : m_resources)
        addItem(r);

    // Starting workers
    Worker* w1 = new Worker(QPointF(560, 400), m_base);
    Worker* w2 = new Worker(QPointF(640, 490), m_base);
    m_workers.append(w1);
    m_workers.append(w2);
    addItem(w1);
    addItem(w2);

    m_timer->start(50); // 20 FPS
    emit statsChanged(m_gold, m_workers.size(), QString::fromUtf8("无"));
}

bool GameScene::spawnWorker()
{
    if (m_gold < 50)
        return false;
    m_gold -= 50;

    QPointF bp = m_base->scenePos();
    double rx = (std::rand() % 80) - 40;
    double ry = (std::rand() % 80) - 40;
    Worker* w = new Worker(QPointF(bp.x() + rx, bp.y() + ry), m_base);
    m_workers.append(w);
    addItem(w);

    emit statsChanged(m_gold, m_workers.size(),
                      m_selectedWorker ? QString::fromUtf8("工人") : QString::fromUtf8("无"));
    return true;
}

void GameScene::selectWorker(Worker* worker)
{
    if (m_selectedWorker)
        m_selectedWorker->setSelected(false);
    m_selectedWorker = worker;
    if (worker)
        worker->setSelected(true);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());

    if (event->button() == Qt::LeftButton) {
        Worker* worker = nullptr;
        if (item)
            worker = qgraphicsitem_cast<Worker*>(item);
        if (worker) {
            selectWorker(worker);
        } else {
            selectWorker(nullptr);
        }
        emit statsChanged(m_gold, m_workers.size(),
                          m_selectedWorker ? QString::fromUtf8("工人") : QString::fromUtf8("无"));
    } else if (event->button() == Qt::RightButton) {
        if (m_selectedWorker) {
            ResourceNode* resource = nullptr;
            if (item)
                resource = qgraphicsitem_cast<ResourceNode*>(item);
            if (resource && !resource->isDepleted()) {
                m_selectedWorker->gatherFrom(resource);
            } else {
                m_selectedWorker->moveTo(event->scenePos());
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void GameScene::gameTick()
{
    for (auto* w : m_workers) {
        w->updateUnit();
        m_gold += w->takeDepositedGold();
    }

    emit statsChanged(m_gold, m_workers.size(),
                      m_selectedWorker ? QString::fromUtf8("工人") : QString::fromUtf8("无"));
}
