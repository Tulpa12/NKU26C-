#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QTimer>

class Worker;
class CommandCenter;
class ResourceNode;

class GameScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GameScene(QObject* parent = nullptr);
    ~GameScene();

    void startGame();
    bool spawnWorker();
    int totalGold() const { return m_gold; }
    int workerCount() const { return m_workers.size(); }

signals:
    void statsChanged(int gold, int workers, const QString& selected);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void gameTick();

private:
    void selectWorker(Worker* worker);

    CommandCenter* m_base;
    QList<Worker*> m_workers;
    QList<ResourceNode*> m_resources;
    QTimer* m_timer;
    Worker* m_selectedWorker;
    int m_gold;
};

#endif
