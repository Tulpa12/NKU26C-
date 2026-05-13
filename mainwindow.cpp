#include "mainwindow.h"
#include "gamescene.h"

#include <QGraphicsView>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_scene(nullptr)
    , m_view(nullptr)
{
    setWindowTitle(QString::fromUtf8("Mini RTS - V1.0"));

    m_scene = new GameScene(this);

    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setCentralWidget(m_view);

    // Status bar
    statusBar()->showMessage(QString::fromUtf8("金矿: 100 | 工人: 2 | 选中: 无"));

    connect(m_scene, &GameScene::statsChanged, this, [this](int gold, int workers, const QString& sel) {
        statusBar()->showMessage(
            QString::fromUtf8("金矿: %1 | 工人: %2 | 选中: %3")
                .arg(gold)
                .arg(workers)
                .arg(sel));
    });

    // Toolbar
    QToolBar* toolbar = addToolBar(QString::fromUtf8("游戏"));
    QAction* trainAction = toolbar->addAction(QString::fromUtf8("训练工人 (50金)"));
    connect(trainAction, &QAction::triggered, this, [this]() {
        if (!m_scene->spawnWorker()) {
            statusBar()->showMessage(QString::fromUtf8("金矿不足! 需要50金"), 2000);
        }
    });

    QAction* helpAction = toolbar->addAction(QString::fromUtf8("帮助"));
    connect(helpAction, &QAction::triggered, this, [this]() {
        QMessageBox::information(this,
            QString::fromUtf8("操作说明"),
            QString::fromUtf8("左键点击蓝色方块 → 选中工人\n"
                              "右键点击空地 → 移动工人\n"
                              "右键点击金矿 → 采集资源\n"
                              "工人自动往返基地运送金矿\n"
                              "训练工人消耗50金矿"));
    });

    m_scene->startGame();
}

MainWindow::~MainWindow()
{
}
