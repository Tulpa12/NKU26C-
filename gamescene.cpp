#include "gamescene.h"
#include "worker.h"
#include "soldier.h"
#include "enemy.h"
#include "bossenemy.h"
#include "building.h"
#include "resource.h"
#include "texturemanager.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QFont>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>
#include <cmath>

GameScene::GameScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_base(nullptr)
    , m_timer(new QTimer(this))
    , m_hudText(nullptr)
    , m_gold(100)
    , m_enemySpawnTimer(0)
    , m_waveNumber(0)
    , m_maxWaves(15)
    , m_gameOver(false)
    , m_victory(false)
    , m_paused(false)
{
    setSceneRect(0, 0, 1200, 900);
    QPixmap bg = TextureManager::instance().backgroundTex();
    setBackgroundBrush(QBrush(bg));
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    connect(m_timer, &QTimer::timeout, this, &GameScene::gameTick);
}

GameScene::~GameScene()
{
    m_timer->stop();
}

void GameScene::togglePause()
{
    if (m_gameOver) return;
    m_paused = !m_paused;
    if (m_paused) {
        m_timer->stop();
    } else {
        m_timer->start(50);
    }
    updateHUD();
    emit pauseChanged(m_paused);
}

void GameScene::reloadBackground()
{
    QPixmap bg = TextureManager::instance().backgroundTex();
    setBackgroundBrush(QBrush(bg));
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

    // HUD
    m_hudText = new QGraphicsTextItem();
    m_hudText->setPos(10, 10);
    m_hudText->setDefaultTextColor(Qt::white);
    m_hudText->setFont(QFont("Arial", 13, QFont::Bold));
    m_hudText->setZValue(100);
    addItem(m_hudText);
    updateHUD();

    m_enemySpawnTimer = 160;
    m_timer->start(50);
    emit statsChanged(m_gold, m_workers.size(), m_soldiers.size(), 0, m_waveNumber);
}

bool GameScene::spawnWorker()
{
    if (m_gold < 50) return false;
    m_gold -= 50;
    QPointF bp = m_base->scenePos();
    Worker* w = new Worker(QPointF(bp.x() + (std::rand() % 80) - 40,
                                    bp.y() + (std::rand() % 80) - 40), m_base);
    m_workers.append(w);
    addItem(w);
    int sel = selectedItems().size();
    emit statsChanged(m_gold, m_workers.size(), m_soldiers.size(), sel, m_waveNumber);
    return true;
}

bool GameScene::spawnSoldier()
{
    if (m_gold < 30) return false;
    m_gold -= 30;
    QPointF bp = m_base->scenePos();
    Soldier* s = new Soldier(QPointF(bp.x() + (std::rand() % 80) - 40,
                                      bp.y() + (std::rand() % 80) - 40));
    m_soldiers.append(s);
    addItem(s);
    int sel = selectedItems().size();
    emit statsChanged(m_gold, m_workers.size(), m_soldiers.size(), sel, m_waveNumber);
    return true;
}

void GameScene::commandSelectedUnits(const QPointF& scenePos, QGraphicsItem* clickedItem)
{
    QList<QGraphicsItem*> sel = selectedItems();
    ResourceNode* resource = qgraphicsitem_cast<ResourceNode*>(clickedItem);
    Enemy* enemy = qgraphicsitem_cast<Enemy*>(clickedItem);

    // Calculate formation offset for multi-unit move
    int idx = 0;
    int cols = static_cast<int>(std::ceil(std::sqrt(static_cast<double>(sel.size()))));

    for (auto* item : sel) {
        Worker* worker = qgraphicsitem_cast<Worker*>(item);
        Soldier* soldier = qgraphicsitem_cast<Soldier*>(item);

        if (worker) {
            if (resource && !resource->isDepleted()) {
                worker->gatherFrom(resource);
            } else {
                // Formation spread
                double ox = (idx % cols - cols / 2.0) * 25;
                double oy = (idx / cols - cols / 2.0) * 25;
                worker->moveTo(scenePos + QPointF(ox, oy));
            }
        } else if (soldier) {
            if (enemy && !enemy->isDead()) {
                soldier->attackTarget(enemy);
            } else {
                double ox = (idx % cols - cols / 2.0) * 25;
                double oy = (idx / cols - cols / 2.0) * 25;
                soldier->moveTo(scenePos + QPointF(ox, oy));
            }
        }
        idx++;
    }
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_gameOver || m_paused) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    if (event->button() == Qt::RightButton) {
        QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
        QList<QGraphicsItem*> sel = selectedItems();
        if (!sel.isEmpty()) {
            commandSelectedUnits(event->scenePos(), item);
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void GameScene::spawnEnemy()
{
    m_waveNumber++;
    updateHUD();

    int count = 1 + m_waveNumber / 3;
    for (int i = 0; i < count; i++) {
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

void GameScene::spawnBoss()
{
    double x = std::rand() % 1160 + 20;
    double y = std::rand() % 2 ? 20 : 880;
    BossEnemy* boss = new BossEnemy(QPointF(x, y), m_base);
    m_enemies.append(boss);
    addItem(boss);
}

void GameScene::cleanDeadUnits()
{
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

    // Clear cross-references
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

    for (auto* w : deadWorkers) {
        m_workers.removeOne(w);
        removeItem(w);
        delete w;
    }

    for (auto* s : deadSoldiers) {
        m_soldiers.removeOne(s);
        removeItem(s);
        delete s;
    }

    for (auto* e : deadEnemies) {
        m_enemies.removeOne(e);
        bool isBoss = (qgraphicsitem_cast<BossEnemy*>(e) != nullptr);
        m_gold += isBoss ? 50 : 15;
        removeItem(e);
        delete e;
    }
}

void GameScene::checkGameOver()
{
    if (m_base->isDestroyed() && !m_gameOver) {
        m_gameOver = true;
        m_timer->stop();
        updateHUD();
        emit gameOver(false);
    }
    if (m_waveNumber >= m_maxWaves && m_enemies.isEmpty() && !m_gameOver) {
        m_gameOver = true;
        m_victory = true;
        m_timer->stop();
        updateHUD();
        emit gameOver(true);
    }
}

void GameScene::updateHUD()
{
    if (!m_hudText) return;
    QString status;
    if (m_paused)
        status = QString::fromUtf8("暂停中");
    else if (m_victory)
        status = QString::fromUtf8("胜利!");
    else if (m_base->isDestroyed())
        status = QString::fromUtf8("失败!");
    else
        status = QString::fromUtf8("第 %1 / %2 波").arg(m_waveNumber).arg(m_maxWaves);

    m_hudText->setPlainText(QString::fromUtf8("金矿: %1    %3    HP: %2")
                                .arg(m_gold)
                                .arg(m_base ? m_base->health() : 0)
                                .arg(status));
}

void GameScene::gameTick()
{
    if (m_gameOver || m_paused)
        return;

    for (auto* w : m_workers)
        w->updateUnit();

    for (auto* w : m_workers)
        m_gold += w->takeDepositedGold();

    // Soldier auto-target
    for (auto* s : m_soldiers) {
        if (s->isDead()) continue;
        if (s->currentTarget() == nullptr) {
            Enemy* nearest = nullptr;
            double bestDist = s->isAttackMove() ? 150 : 120;
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
            if (nearest)
                s->attackTarget(nearest);
        }
        s->updateUnit();
    }

    for (auto* e : m_enemies)
        e->updateUnit();

    cleanDeadUnits();
    checkGameOver();

    // Enemy spawning (stop after max waves reached)
    if (!m_gameOver && m_waveNumber < m_maxWaves) {
        m_enemySpawnTimer--;
        if (m_enemySpawnTimer <= 0) {
            spawnEnemy();
            // Boss every 5 waves
            if (m_waveNumber % 5 == 0)
                spawnBoss();
            m_enemySpawnTimer = 200;
        }
    }

    updateHUD();
    int sel = selectedItems().size();
    emit statsChanged(m_gold, m_workers.size(), m_soldiers.size(), sel, m_waveNumber);
}
