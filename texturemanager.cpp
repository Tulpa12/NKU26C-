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
        // Fallback: use source-relative path (when running from build dir)
        m_basePath = QApplication::applicationDirPath() + "/../../images/";
    }

    generateDefaults();

    // Try loading default pack
    if (QDir(m_basePath + "default").exists())
        loadPack("default");
}

QStringList TextureManager::availablePacks() const
{
    QStringList packs;
    QDir dir(m_basePath);
    if (!dir.exists()) return packs;
    for (const auto& entry : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
        packs.append(entry.fileName());
    return packs;
}

QString TextureManager::currentPack() const { return m_currentPack; }

void TextureManager::loadPack(const QString& packFolder)
{
    QString path = m_basePath + packFolder + "/";
    m_currentPack = packFolder;

    auto loadImg = [&](const QString& name, QPixmap& target) {
        QString file = path + name;
        if (QFile::exists(file)) {
            target = QPixmap(file);
        }
        // else keep default
    };

    loadImg("worker.png", m_worker);
    loadImg("soldier.png", m_soldier);
    loadImg("enemy.png", m_enemy);
    loadImg("boss.png", m_boss);
    loadImg("base.png", m_base);
    loadImg("goldmine.png", m_goldmine);
    loadImg("background.png", m_background);
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
const QPixmap& TextureManager::enemyTex() const { return m_enemy; }
const QPixmap& TextureManager::bossTex() const { return m_boss; }
const QPixmap& TextureManager::baseTex() const { return m_base; }
const QPixmap& TextureManager::goldmineTex() const { return m_goldmine; }
const QPixmap& TextureManager::backgroundTex() const { return m_background; }
