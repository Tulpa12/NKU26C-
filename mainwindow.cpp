#include "mainwindow.h"
#include "gamescene.h"
#include "soldier.h"
#include "texturemanager.h"

#include <QGraphicsView>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QComboBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_scene(nullptr)
    , m_view(nullptr)
{
    setWindowTitle(QString::fromUtf8("Mini RTS - V3.0"));

    m_scene = new GameScene(this);

    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setCentralWidget(m_view);

    statusBar()->showMessage(QString::fromUtf8("波次: 0/15 | 金矿: 100 | 工人: 2 | 士兵: 1 | 选中: 0"));

    // Toolbar
    QToolBar* toolbar = addToolBar(QString::fromUtf8("游戏"));

    // Pause button (declared early so other connects can reference it)
    QAction* pauseAction = toolbar->addAction(QString::fromUtf8("暂停"));
    pauseAction->setCheckable(true);
    pauseAction->setShortcut(QKeySequence(Qt::Key_Space));
    toolbar->addSeparator();

    // Texture pack selector
    toolbar->addWidget(new QLabel(QString::fromUtf8(" 皮肤: ")));
    QComboBox* skinBox = new QComboBox();
    QStringList packs = TextureManager::instance().availablePacks();
    if (packs.isEmpty())
        packs.append(QString::fromUtf8("默认"));
    skinBox->addItems(packs);
    skinBox->setCurrentText(TextureManager::instance().currentPack().isEmpty()
                                ? packs.first()
                                : TextureManager::instance().currentPack());
    connect(skinBox, &QComboBox::currentTextChanged, this, [this](const QString& pack) {
        TextureManager::instance().loadPack(pack);
        m_scene->reloadBackground();
        m_scene->update();
    });
    toolbar->addWidget(skinBox);
    toolbar->addSeparator();

    QAction* workerAction = toolbar->addAction(QString::fromUtf8("训练工人 (50金)"));
    QAction* soldierAction = toolbar->addAction(QString::fromUtf8("训练士兵 (30金)"));
    toolbar->addSeparator();

    QAction* amoveAction = toolbar->addAction(QString::fromUtf8("A-移动: 关"));
    amoveAction->setCheckable(true);

    toolbar->addSeparator();
    QAction* helpAction = toolbar->addAction(QString::fromUtf8("帮助"));

    // === Connections ===

    connect(m_scene, &GameScene::statsChanged, this,
            [this](int gold, int workers, int soldiers, int selected, int wave) {
        statusBar()->showMessage(
            QString::fromUtf8("波次: %1/%2 | 金矿: %3 | 工人: %4 | 士兵: %5 | 选中: %6")
                .arg(wave).arg(15).arg(gold).arg(workers).arg(soldiers).arg(selected));
    });

    connect(m_scene, &GameScene::gameOver, this, [this, pauseAction](bool victory) {
        pauseAction->setChecked(false);
        pauseAction->setText(QString::fromUtf8("暂停"));
        pauseAction->setEnabled(false);
        QMessageBox msgBox;
        if (victory) {
            msgBox.setWindowTitle(QString::fromUtf8("胜利!"));
            msgBox.setText(QString::fromUtf8("恭喜！你成功守住了基地！"));
            msgBox.setInformativeText(QString::fromUtf8("你存活了全部15波进攻。"));
        } else {
            msgBox.setWindowTitle(QString::fromUtf8("游戏结束"));
            msgBox.setText(QString::fromUtf8("你的基地被摧毁了！"));
            msgBox.setInformativeText(QString::fromUtf8("敌人摧毁了指挥中心。"));
        }
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    });

    connect(pauseAction, &QAction::toggled, this, [this, pauseAction](bool checked) {
        m_scene->togglePause();
        pauseAction->setText(checked ? QString::fromUtf8("继续") : QString::fromUtf8("暂停"));
    });

    connect(m_scene, &GameScene::pauseChanged, this, [pauseAction](bool paused) {
        pauseAction->setChecked(paused);
        pauseAction->setText(paused ? QString::fromUtf8("继续") : QString::fromUtf8("暂停"));
    });

    connect(amoveAction, &QAction::toggled, this, [this, amoveAction](bool checked) {
        for (auto* item : m_scene->items()) {
            Soldier* s = qgraphicsitem_cast<Soldier*>(item);
            if (s) s->setAttackMove(checked);
        }
        amoveAction->setText(checked ? QString::fromUtf8("A-移动: 开") : QString::fromUtf8("A-移动: 关"));
    });

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
            QString::fromUtf8("操作说明 V3.0"),
            QString::fromUtf8(
                "【快捷键】\n"
                "空格键 → 暂停 / 继续\n"
                "\n"
                "【贴图系统】\n"
                "工具栏「皮肤」下拉切换贴图包\n"
                "将你的PNG图片放入 images/新文件夹/\n"
                "文件名: worker.png, soldier.png, enemy.png,\n"
                " boss.png, base.png, goldmine.png, background.png\n"
                "\n"
                "【操作】\n"
                "左键拖拽 → 框选单位\n"
                "右键空地 → 移动选中单位\n"
                "右键金矿 → 采集\n"
                "右键敌人 → 攻击\n"
                "A-移动 → 行军途中自动攻击\n"
                "\n"
                "【目标】存活15波! 每5波出Boss"));
    });

    m_scene->startGame();
}

MainWindow::~MainWindow()
{
}
