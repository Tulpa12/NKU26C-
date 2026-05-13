#include "gamescene.h"
#include "unit.h"
#include "worker.h"
#include "soldier.h"
#include "enemy.h"
#include "building.h"
#include "resource.h"
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>
#include <cmath>

GameScene::GameScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_base(nullptr)
    , m_timer(new QTimer(this))
    , m_selectedUnit(nullptr)
    , m_gold(100)
    , m_enemySpawnTimer(0)
    , m_waveNumber(0)
    , m_gameOver(false)
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
    m_base = new CommandCenter(QPointF(600, 450));
    addItem(m_base);

    m_resources.append(new ResourceNode(500, QPointF(200, 200)));
    m_resources.append(new ResourceNode(500, QPointF(1050, 280)));
    m_resources.append(new ResourceNode(500, QPointF(480, 780)));
    for (auto* r : m_resources)
        addItem(r);

    Worker* w1 = new Worker(QPointF(560, 400), m_base);
    Worker* w2 = new Worker(QPointF(640, 490), m_base);
    m_workers.append(w1);
    m_workers.append(w2);
    addItem(w1);
    addItem(w2);

    Soldier* s1 = new Soldier(QPointF(520, 420));
    m_soldiers.append(s1);
    addItem(s1);

    m_enemySpawnTimer = 160; // First wave at ~8 seconds
    m_timer->start(50);
    emit statsChanged(m_gold, m_workers.size(), m_soldiers.size(), QString::fromUtf8("无"));
}

bool GameScene::spawnWorker()
{
    if (m_gold < 50)
        return false;
    m_gold -= 50;

    QPointF bp = m_base->scenePos();
    Worker* w = new Worker(QPointF(bp.x() + (std::rand() % 80) - 40,
                                    bp.y() + (std::rand() % 80) - 40), m_base);
    m_workers.append(w);
    addItem(w);

    emit statsChanged(m_gold, m_workers.size(), m_soldiers.size(),
                      m_selectedUnit ? QString::fromUtf8("单位") : QString::fromUtf8("无"));
    return true;
}

bool GameScene::spawnSoldier()
{
    if (m_gold < 30)
        return false;
    m_gold -= 30;

    QPointF bp = m_base->scenePos();
    Soldier* s = new Soldier(QPointF(bp.x() + (std::rand() % 80) - 40,
                                      bp.y() + (std::rand() % 80) - 40));
    m_soldiers.append(s);
    addItem(s);

    emit statsChanged(m_gold, m_workers.size(), m_soldiers.size(),
                      m_selectedUnit ? QString::fromUtf8("单位") : QString::fromUtf8("无"));
    return true;
}

void GameScene::selectUnit(Unit* unit)
{
    if (m_selectedUnit)
        m_selectedUnit->setSelected(false);
    m_selectedUnit = unit;
    if (unit)
        unit->setSelected(true);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_gameOver) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());

    if (event->button() == Qt::LeftButton) {
        Worker* worker = qgraphicsitem_cast<Worker*>(item);
        Soldier* soldier = qgraphicsitem_cast<Soldier*>(item);
        QString selName = QString::fromUtf8("无");
        if (worker) {
            selectUnit(worker);
            selName = QString::fromUtf8("工人");
        } else if (soldier) {
            selectUnit(soldier);
            selName = QString::fromUtf8("士兵");
        } else {
            selectUnit(nullptr);
        }
        emit statsChanged(m_gold, m_workers.size(), m_soldiers.size(), selName);
    } else if (event->button() == Qt::RightButton) {
        if (m_selectedUnit) {
            Worker* worker = qgraphicsitem_cast<Worker*>(m_selectedUnit);
            Soldier* soldier = qgraphicsitem_cast<Soldier*>(m_selectedUnit);

            if (worker) {
                ResourceNode* resource = qgraphicsitem_cast<ResourceNode*>(item);
                if (resource && !resource->isDepleted()) {
                    worker->gatherFrom(resource);
                } else {
                    worker->moveTo(event->scenePos());
                }
            } else if (soldier) {
                Enemy* enemy = qgraphicsitem_cast<Enemy*>(item);
                if (enemy && !enemy->isDead()) {
                    soldier->attackTarget(enemy);
                } else {
                    soldier->moveTo(event->scenePos());
                }
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void GameScene::spawnEnemy()
{
    m_waveNumber++;
    int count = 1 + m_waveNumber / 3; // increases over time

    for (int i = 0; i < count; i++) {
        // Random spawn at map edges
        double x, y;
        int edge = std::rand() % 4;
        switch (edge) {
        case 0: x = 20;               y = std::rand() % 860 + 20; break;
        case 1: x = 1180;             y = std::rand() % 860 + 20; break;
        case 2: x = std::rand() % 1160 + 20; y = 20;              break;
        case 3: x = std::rand() % 1160 + 20; y = 880;             break;
        }

        Enemy* e = new Enemy(QPointF(x, y), m_base);
        m_enemies.append(e);
        addItem(e);
    }
}

void GameScene::cleanDeadUnits()
{
    // Collect dead units
    QList<Worker*> deadWorkers;
    for (auto* w : m_workers) {
        if (w->isDead()) deadWorkers.append(w);
    }

    QList<Soldier*> deadSoldiers;
    for (auto* s : m_soldiers) {
        if (s->isDead()) deadSoldiers.append(s);
    }

    QList<Enemy*> deadEnemies;
    for (auto* e : m_enemies) {
        if (e->isDead()) deadEnemies.append(e);
    }

    // Clear cross-references before deletion (prevent dangling pointers)
    for (auto* ds : deadSoldiers) {
        for (auto* e : m_enemies) {
            if (e->attackTarget() == ds)
                e->clearAttackTarget();
        }
    }
    for (auto* de : deadEnemies) {
        for (auto* s : m_soldiers) {
            if (s->currentTarget() == de)
                s->clearTarget();
        }
    }

    // Delete dead workers
    for (auto* w : deadWorkers) {
        m_workers.removeOne(w);
        if (m_selectedUnit == w) selectUnit(nullptr);
        removeItem(w);
        delete w;
    }

    // Delete dead soldiers
    for (auto* s : deadSoldiers) {
        m_soldiers.removeOne(s);
        if (m_selectedUnit == s) selectUnit(nullptr);
        removeItem(s);
        delete s;
    }

    // Delete dead enemies
    for (auto* e : deadEnemies) {
        m_enemies.removeOne(e);
        removeItem(e);
        m_gold += 15;
        delete e;
    }
}

void GameScene::checkGameOver()
{
    if (m_base->isDestroyed() && !m_gameOver) {
        m_gameOver = true;
        m_timer->stop();
        emit gameOver(false);
    }
}

void GameScene::gameTick()
{
    if (m_gameOver)
        return;

    // Update workers
    for (auto* w : m_workers)
        w->updateUnit();

    // Collect gold from workers
    for (auto* w : m_workers)
        m_gold += w->takeDepositedGold();

    // Soldier auto-target: scan for enemies and auto-engage idle soldiers
    for (auto* s : m_soldiers) {
        if (s->isDead()) continue;
        if (s->currentTarget() == nullptr) {
            // Find nearest enemy for idle soldiers
            Enemy* nearest = nullptr;
            double bestDist = 120; // scan range
            for (auto* e : m_enemies) {
                if (e->isDead()) continue;
                double dx = s->scenePos().x() - e->scenePos().x();
                double dy = s->scenePos().y() - e->scenePos().y();
                double dist = std::sqrt(dx * dx + dy * dy);
                if (dist < bestDist) {
                    bestDist = dist;
                    nearest = e;
                }
            }
            if (nearest) {
                s->attackTarget(nearest);
            }
        }
        s->updateUnit();
    }

    // Update enemies
    for (auto* e : m_enemies)
        e->updateUnit();

    // Cleanup
    cleanDeadUnits();
    checkGameOver();

    // Enemy spawning
    m_enemySpawnTimer--;
    if (m_enemySpawnTimer <= 0) {
        spawnEnemy();
        m_enemySpawnTimer = 200; // every 10 seconds
    }

    QString selName = QString::fromUtf8("无");
    if (m_selectedUnit) {
        if (qgraphicsitem_cast<Worker*>(m_selectedUnit))
            selName = QString::fromUtf8("工人");
        else if (qgraphicsitem_cast<Soldier*>(m_selectedUnit))
            selName = QString::fromUtf8("士兵");
    }
    emit statsChanged(m_gold, m_workers.size(), m_soldiers.size(), selName);
}
