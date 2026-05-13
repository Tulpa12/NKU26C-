#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GameScene;
class QGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    GameScene* m_scene;
    QGraphicsView* m_view;
};

#endif // MAINWINDOW_H
