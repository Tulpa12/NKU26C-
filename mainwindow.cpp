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

    statusBar()->showMessage(QString::fromUtf8("波次: 0/20 | 金矿: 100 | 工人: 2 | 士兵: 1 | 选中: 0"));

    // Toolbar
    QToolBar* toolbar = addToolBar(QString::fromUtf8("游戏"));

    // Pause button (declared early so other connects can reference it)
    QAction* pauseAction = toolbar->addAction(QString::fromUtf8("暂停"));
    pauseAction->setCheckable(true);
    pauseAction->setShortcut(QKeySequence(Qt::Key_Space));
    toolbar->addSeparator();

    // Unit skin selector
    toolbar->addWidget(new QLabel(QString::fromUtf8(" 皮肤: ")));
    QComboBox* skinBox = new QComboBox();
    QStringList skinPacks = TextureManager::instance().availableSkinPacks();
    if (skinPacks.isEmpty())
        skinPacks.append(QString::fromUtf8("默认"));
    skinBox->addItems(skinPacks);
    skinBox->setCurrentText(TextureManager::instance().currentSkin().isEmpty()
                                ? skinPacks.first()
                                : TextureManager::instance().currentSkin());
    connect(skinBox, &QComboBox::currentTextChanged, this, [this](const QString& pack) {
        TextureManager::instance().loadSkinPack(pack);
        m_scene->update();
    });
    toolbar->addWidget(skinBox);

    // Background selector
    toolbar->addWidget(new QLabel(QString::fromUtf8(" 背景: ")));
    QComboBox* bgBox = new QComboBox();
    QStringList bgFiles = TextureManager::instance().availableBackgrounds();
    bgBox->addItems(bgFiles);
    QString curBg = TextureManager::instance().currentBackground();
    if (!curBg.isEmpty())
        bgBox->setCurrentText(curBg);
    connect(bgBox, &QComboBox::currentTextChanged, this, [this](const QString& file) {
        TextureManager::instance().loadBackgroundFile(file);
        m_scene->reloadBackground();
    });
    toolbar->addWidget(bgBox);
    toolbar->addSeparator();

    QAction* workerAction = toolbar->addAction(QString::fromUtf8("训练工人 (50金)"));
    QAction* soldierAction = toolbar->addAction(QString::fromUtf8("训练士兵 (30金)"));
    QAction* archerAction = toolbar->addAction(QString::fromUtf8("训练弓箭手 (35金)"));
    QAction* tankAction = toolbar->addAction(QString::fromUtf8("训练坦克 (50金)"));
    toolbar->addSeparator();

    QAction* amoveAction = toolbar->addAction(QString::fromUtf8("A-移动: 关"));
    amoveAction->setCheckable(true);

    toolbar->addSeparator();
    QAction* helpAction = toolbar->addAction(QString::fromUtf8("帮助"));

    // === Connections ===

    connect(m_scene, &GameScene::statsChanged, this,
            [this](int gold, int workers, int soldiers, int selected, int wave) {
        statusBar()->showMessage(
            QString::fromUtf8("波次: %1/20 | 金矿: %2 | 工人: %3 | 士兵: %4 | 选中: %5")
                .arg(wave).arg(gold).arg(workers).arg(soldiers).arg(selected));
    });

    connect(m_scene, &GameScene::gameOver, this, [this, pauseAction](bool victory) {
        pauseAction->setChecked(false);
        pauseAction->setText(QString::fromUtf8("暂停"));
        pauseAction->setEnabled(false);
        QMessageBox msgBox;
        if (victory) {
            msgBox.setWindowTitle(QString::fromUtf8("胜利!"));
            msgBox.setText(QString::fromUtf8("恭喜！你成功守住了基地！"));
            msgBox.setInformativeText(QString::fromUtf8("你存活了全部20波进攻。"));
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
            Soldier* s = dynamic_cast<Soldier*>(item);
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

    connect(archerAction, &QAction::triggered, this, [this]() {
        if (!m_scene->spawnArcher())
            statusBar()->showMessage(QString::fromUtf8("金矿不足! 需要35金"), 2000);
    });

    connect(tankAction, &QAction::triggered, this, [this]() {
        if (!m_scene->spawnTank())
            statusBar()->showMessage(QString::fromUtf8("金矿不足! 需要50金"), 2000);
    });

    connect(helpAction, &QAction::triggered, this, [this]() {
        QMessageBox::information(this,
            QString::fromUtf8("操作说明 V3.0"),
            QString::fromUtf8(
                "【快捷键】\n"
                "空格键 → 暂停 / 继续\n"
                "\n"
                "【兵种】\n"
                "工人(W) - 采集金矿, 30HP\n"
                "士兵(S) - 近战, 60HP, 30金\n"
                "弓箭手(A) - 远程风筝, 35HP, 35金\n"
                "坦克(T) - 重装近战, 130HP, 50金\n"
                "\n"
                "【敌人】\n"
                "普通敌人(E) - 40HP\n"
                "快速敌人(F) - 22HP, 高速\n"
                "重型敌人(H) - 100HP, 高伤害\n"
                "Boss - 150HP, 每5波出现\n"
                "\n"
                "【贴图系统】\n"
                "工具栏「皮肤」切换单位贴图\n"
                "工具栏「背景」独立切换背景图\n"
                "将PNG放入 images/新文件夹/ 即可识别\n"
                "单位贴图: worker.png, soldier.png, archer.png, tank.png,\n"
                " enemy.png, fastenemy.png, heavyenemy.png,\n"
                " rangedenemy.png, boss.png, base.png, goldmine.png\n"
                "背景贴图: background.png\n"
                "\n"
                "【操作】\n"
                "左键拖拽 → 框选单位\n"
                "右键空地 → 移动选中单位\n"
                "右键金矿 → 采集\n"
                "右键敌人 → 攻击\n"
                "A-移动 → 行军途中自动攻击\n"
                "\n"
                "【目标】存活20波! 每5波出Boss\n"
                "每波前有3秒倒计时准备"));
    });

    m_scene->startGame();
}

MainWindow::~MainWindow()
{
}
