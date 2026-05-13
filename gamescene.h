#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include "unit.h"

class Worker;
class Soldier;
class Enemy;
class CommandCenter;
class ResourceNode;
class QGraphicsTextItem;

class GameScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GameScene(QObject* parent = nullptr);
    ~GameScene();

    void startGame();
    bool spawnWorker();
    bool spawnSoldier();
    int totalGold() const { return m_gold; }
    int workerCount() const { return m_workers.size(); }
    int soldierCount() const { return m_soldiers.size(); }
    bool isGameOver() const { return m_gameOver; }
    void reloadBackground();
    void togglePause();
    bool isPaused() const { return m_paused; }

signals:
    void statsChanged(int gold, int workers, int soldiers, int selected, int wave);
    void gameOver(bool victory);
    void pauseChanged(bool paused);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void gameTick();

private:
    void spawnEnemy();
    void spawnBoss();
    void cleanDeadUnits();
    void checkGameOver();
    void commandSelectedUnits(const QPointF& scenePos, QGraphicsItem* clickedItem);
    void updateHUD();

    CommandCenter* m_base;
    QList<Worker*> m_workers;
    QList<Soldier*> m_soldiers;
    QList<Enemy*> m_enemies;
    QList<ResourceNode*> m_resources;
    QTimer* m_timer;
    QGraphicsTextItem* m_hudText;
    int m_gold;
    int m_enemySpawnTimer;
    int m_waveNumber;
    int m_maxWaves;
    bool m_gameOver;
    bool m_victory;
    bool m_paused;
};

#endif
