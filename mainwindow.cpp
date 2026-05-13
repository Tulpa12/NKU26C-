#include "mainwindow.h"
#include "gamescene.h"

#include <QGraphicsView>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_scene(nullptr)
    , m_view(nullptr)
{
    setWindowTitle(QString::fromUtf8("Mini RTS - V2.0"));

    m_scene = new GameScene(this);

    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setCentralWidget(m_view);

    statusBar()->showMessage(QString::fromUtf8("金矿: 100 | 工人: 2 | 士兵: 1 | 选中: 无"));

    connect(m_scene, &GameScene::statsChanged, this,
            [this](int gold, int workers, int soldiers, const QString& sel) {
        statusBar()->showMessage(
            QString::fromUtf8("金矿: %1 | 工人: %2 | 士兵: %3 | 选中: %4")
                .arg(gold).arg(workers).arg(soldiers).arg(sel));
    });

    connect(m_scene, &GameScene::gameOver, this, [this](bool victory) {
        Q_UNUSED(victory);
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("游戏结束"));
        msgBox.setText(QString::fromUtf8("你的基地被摧毁了！"));
        msgBox.setInformativeText(QString::fromUtf8(
            "敌人摧毁了指挥中心。\n"
            "你在本局存活了数波进攻。\n\n"
            "请查看状态栏查看存活波数。"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    });

    // Toolbar
    QToolBar* toolbar = addToolBar(QString::fromUtf8("游戏"));
    QAction* workerAction = toolbar->addAction(QString::fromUtf8("训练工人 (50金)"));
    QAction* soldierAction = toolbar->addAction(QString::fromUtf8("训练士兵 (30金)"));
    QAction* helpAction = toolbar->addAction(QString::fromUtf8("帮助"));

    connect(workerAction, &QAction::triggered, this, [this]() {
        if (!m_scene->spawnWorker())
            statusBar()->showMessage(QString::fromUtf8("金矿不足! 需要50金"), 2000);
    });

    connect(soldierAction, &QAction::triggered, this, [this]() {
        if (!m_scene->spawnSoldier())
            statusBar()->showMessage(QString::fromUtf8("金矿不足! 需要30金"), 2000);
    });

    connect(helpAction, &QAction::triggered, this, [this]() {
        QMessageBox::information(this,
            QString::fromUtf8("操作说明"),
            QString::fromUtf8(
                "【V2.0 新增】\n"
                "左键点击单位 → 选中工人/士兵\n"
                "右键空地 → 移动选中单位\n"
                "右键金矿(工人) → 采集资源\n"
                "右键敌人(士兵) → 攻击敌人\n"
                "\n"
                "工人（蓝）: 采集金矿，自动往返\n"
                "士兵（绿）: 自动攻击附近敌人\n"
                "敌人（红）: 从地图边缘出现，攻击基地\n"
                "\n"
                "金矿采完会枯竭，击杀敌人获得15金\n"
                "保护基地！基地被摧毁游戏结束"));
    });

    m_scene->startGame();
}

MainWindow::~MainWindow()
{
}
