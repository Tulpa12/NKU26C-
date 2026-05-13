#include "texturemanager.h"
#include <QPainter>
#include <QApplication>
#include <QDirIterator>
#include <cstdlib>
#include <functional>

TextureManager& TextureManager::instance()
{
    static TextureManager mgr;
    return mgr;
}

TextureManager::TextureManager()
{
    // Find images/ folder relative to executable or source
    m_basePath = QApplication::applicationDirPath() + "/images/";
    if (!QDir(m_basePath).exists()) {
        m_basePath = QApplication::applicationDirPath() + "/../images/";
    }
    if (!QDir(m_basePath).exists()) {
        m_basePath = QApplication::applicationDirPath() + "/../../images/";
    }

    m_skinPath = m_basePath + "skins/";
    m_bgPath = m_basePath + "backgrounds/";

    generateDefaults();

    // Try loading default skin and background
    if (QDir(m_skinPath + "default").exists())
        loadSkinPack("default");

    QString defaultBg = m_bgPath + "default.png";
    if (QFile::exists(defaultBg))
        m_background = QPixmap(defaultBg);
}

QStringList TextureManager::availableSkinPacks() const
{
    QStringList packs;
    QDir dir(m_skinPath);
    if (!dir.exists()) return packs;
    for (const auto& entry : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
        packs.append(entry.fileName());
    return packs;
}

QStringList TextureManager::availableBackgrounds() const
{
    QStringList bgs;
    QDir dir(m_bgPath);
    if (!dir.exists()) return bgs;
    for (const auto& entry : dir.entryInfoList(QDir::Files)) {
        if (entry.suffix().toLower() == "png")
            bgs.append(entry.fileName());
    }
    return bgs;
}

QString TextureManager::currentSkin() const { return m_currentSkin; }
QString TextureManager::currentBackground() const { return m_currentBg; }

void TextureManager::loadSkinPack(const QString& packFolder)
{
    QString path = m_skinPath + packFolder + "/";
    m_currentSkin = packFolder;

    auto loadImg = [&](const QString& name, QPixmap& target) {
        QString file = path + name;
        if (QFile::exists(file)) {
            target = QPixmap(file);
        }
    };

    loadImg("worker.png", m_worker);
    loadImg("soldier.png", m_soldier);
    loadImg("archer.png", m_archer);
    loadImg("tank.png", m_tank);
    loadImg("enemy.png", m_enemy);
    loadImg("fastenemy.png", m_fastenemy);
    loadImg("heavyenemy.png", m_heavyenemy);
    loadImg("rangedenemy.png", m_rangedenemy);
    loadImg("boss.png", m_boss);
    loadImg("base.png", m_base);
    loadImg("goldmine.png", m_goldmine);
}

void TextureManager::loadBackgroundFile(const QString& filename)
{
    QString file = m_bgPath + filename;
    if (QFile::exists(file)) {
        m_background = QPixmap(file);
        m_currentBg = filename;
    }
}

void TextureManager::generateDefaults()
{
    // Worker - blue rounded square
    m_worker = QPixmap(32, 32);
    m_worker.fill(Qt::transparent);
    {
        QPainter p(&m_worker);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(60, 120, 220));
        p.setPen(QPen(QColor(20, 60, 140), 2));
        p.drawRoundedRect(1, 1, 30, 30, 4, 4);
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 14, QFont::Bold));
        p.drawText(QRect(0, 0, 32, 32), Qt::AlignCenter, "W");
    }

    // Soldier - green rounded square
    m_soldier = QPixmap(32, 32);
    m_soldier.fill(Qt::transparent);
    {
        QPainter p(&m_soldier);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(60, 180, 60));
        p.setPen(QPen(QColor(20, 100, 20), 2));
        p.drawRoundedRect(1, 1, 30, 30, 4, 4);
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 14, QFont::Bold));
        p.drawText(QRect(0, 0, 32, 32), Qt::AlignCenter, "S");
    }

    // Archer - cyan rounded square
    m_archer = QPixmap(32, 32);
    m_archer.fill(Qt::transparent);
    {
        QPainter p(&m_archer);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(0, 180, 200));
        p.setPen(QPen(QColor(0, 100, 130), 2));
        p.drawRoundedRect(1, 1, 30, 30, 4, 4);
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 14, QFont::Bold));
        p.drawText(QRect(0, 0, 32, 32), Qt::AlignCenter, "A");
    }

    // Tank - dark blue larger square
    m_tank = QPixmap(36, 36);
    m_tank.fill(Qt::transparent);
    {
        QPainter p(&m_tank);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(40, 60, 160));
        p.setPen(QPen(QColor(20, 30, 100), 3));
        p.drawRoundedRect(1, 1, 34, 34, 5, 5);
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 14, QFont::Bold));
        p.drawText(QRect(0, 0, 36, 36), Qt::AlignCenter, "T");
    }

    // Enemy - red rounded square
    m_enemy = QPixmap(32, 32);
    m_enemy.fill(Qt::transparent);
    {
        QPainter p(&m_enemy);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(220, 40, 40));
        p.setPen(QPen(QColor(140, 10, 10), 2));
        p.drawRoundedRect(1, 1, 30, 30, 4, 4);
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 14, QFont::Bold));
        p.drawText(QRect(0, 0, 32, 32), Qt::AlignCenter, "E");
    }

    // FastEnemy - orange small square
    m_fastenemy = QPixmap(28, 28);
    m_fastenemy.fill(Qt::transparent);
    {
        QPainter p(&m_fastenemy);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(240, 140, 30));
        p.setPen(QPen(QColor(160, 80, 10), 2));
        p.drawRoundedRect(1, 1, 26, 26, 3, 3);
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 11, QFont::Bold));
        p.drawText(QRect(0, 0, 28, 28), Qt::AlignCenter, "F");
    }

    // HeavyEnemy - purple larger square
    m_heavyenemy = QPixmap(40, 40);
    m_heavyenemy.fill(Qt::transparent);
    {
        QPainter p(&m_heavyenemy);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(130, 20, 160));
        p.setPen(QPen(QColor(70, 10, 90), 3));
        p.drawRoundedRect(1, 1, 38, 38, 5, 5);
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 12, QFont::Bold));
        p.drawText(QRect(0, 0, 40, 40), Qt::AlignCenter, "H");
    }

    // RangedEnemy - magenta small square with range indicator
    m_rangedenemy = QPixmap(30, 30);
    m_rangedenemy.fill(Qt::transparent);
    {
        QPainter p(&m_rangedenemy);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(200, 40, 140));
        p.setPen(QPen(QColor(120, 10, 70), 2));
        p.drawRoundedRect(1, 1, 28, 28, 3, 3);
        p.setPen(QPen(Qt::white, 1.5));
        p.drawLine(15, 5, 15, 12);
        p.drawLine(8, 15, 22, 15);
        p.drawEllipse(10, 10, 10, 10);
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.drawText(QRect(0, 16, 30, 14), Qt::AlignCenter, "R");
    }

    // Boss - dark red large
    m_boss = QPixmap(48, 48);
    m_boss.fill(Qt::transparent);
    {
        QPainter p(&m_boss);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(160, 20, 20));
        p.setPen(QPen(Qt::black, 3));
        p.drawRoundedRect(1, 1, 46, 46, 6, 6);
        p.setBrush(Qt::white);
        p.setPen(Qt::NoPen);
        p.drawEllipse(8, 10, 10, 10);
        p.drawEllipse(28, 10, 10, 10);
        p.setBrush(Qt::black);
        p.drawEllipse(11, 12, 4, 6);
        p.drawEllipse(31, 12, 4, 6);
        p.setPen(Qt::yellow);
        p.setFont(QFont("Arial", 9, QFont::Bold));
        p.drawText(QRect(0, 26, 48, 20), Qt::AlignCenter, "BOSS");
    }

    // Base - building
    m_base = QPixmap(84, 84);
    m_base.fill(Qt::transparent);
    {
        QPainter p(&m_base);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(200, 170, 50));
        p.setPen(QPen(QColor(100, 80, 20), 3));
        p.drawRect(10, 20, 64, 54);
        QPolygon roof;
        roof << QPoint(6, 20) << QPoint(42, 2) << QPoint(78, 20);
        p.drawPolygon(roof);
        p.setBrush(QColor(100, 80, 20));
        p.setPen(QPen(QColor(60, 50, 10), 2));
        p.drawRect(32, 46, 20, 28);
        p.setPen(QPen(QColor(100, 80, 20), 2));
        p.drawLine(60, 2, 60, 16);
        p.setBrush(QColor(220, 60, 60));
        p.drawRect(60, 2, 12, 8);
    }

    // Gold mine
    m_goldmine = QPixmap(54, 54);
    m_goldmine.fill(Qt::transparent);
    {
        QPainter p(&m_goldmine);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(255, 215, 0));
        p.setPen(QPen(QColor(180, 150, 0), 2));
        p.drawEllipse(2, 2, 50, 50);
        p.setPen(QColor(140, 100, 0));
        p.setFont(QFont("Arial", 20, QFont::Bold));
        p.drawText(QRect(0, 0, 54, 54), Qt::AlignCenter, "$");
    }

    // Background
    m_background = QPixmap(128, 128);
    m_background.fill(QColor(50, 130, 50));
    {
        QPainter p(&m_background);
        for (int i = 0; i < 150; i++) {
            int x = std::rand() % 128;
            int y = std::rand() % 128;
            int c = 40 + std::rand() % 100;
            p.setPen(QColor(c / 3, c, c / 3));
            p.drawPoint(x, y);
        }
    }
}

const QPixmap& TextureManager::workerTex() const { return m_worker; }
const QPixmap& TextureManager::soldierTex() const { return m_soldier; }
const QPixmap& TextureManager::archerTex() const { return m_archer; }
const QPixmap& TextureManager::tankTex() const { return m_tank; }
const QPixmap& TextureManager::enemyTex() const { return m_enemy; }
const QPixmap& TextureManager::fastenemyTex() const { return m_fastenemy; }
const QPixmap& TextureManager::heavyenemyTex() const { return m_heavyenemy; }
const QPixmap& TextureManager::rangedenemyTex() const { return m_rangedenemy; }
const QPixmap& TextureManager::bossTex() const { return m_boss; }
const QPixmap& TextureManager::baseTex() const { return m_base; }
const QPixmap& TextureManager::goldmineTex() const { return m_goldmine; }
const QPixmap& TextureManager::backgroundTex() const { return m_background; }
