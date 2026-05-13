#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QTimer>

class Worker;
class Soldier;
class Enemy;
class CommandCenter;
class ResourceNode;

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
    int enemyCount() const { return m_enemies.size(); }
    bool isGameOver() const { return m_gameOver; }

signals:
    void statsChanged(int gold, int workers, int soldiers, const QString& selected);
    void gameOver(bool victory);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void gameTick();

private:
    void selectUnit(Unit* unit);
    void spawnEnemy();
    void cleanDeadUnits();
    void checkGameOver();

    CommandCenter* m_base;
    QList<Worker*> m_workers;
    QList<Soldier*> m_soldiers;
    QList<Enemy*> m_enemies;
    QList<ResourceNode*> m_resources;
    QTimer* m_timer;
    Unit* m_selectedUnit;
    int m_gold;
    int m_enemySpawnTimer;
    int m_waveNumber;
    bool m_gameOver;
};

#endif
