#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QDir>

class TextureManager {
public:
    static TextureManager& instance();

    void loadSkinPack(const QString& packFolder);
    void loadBackgroundFile(const QString& filename);
    QStringList availableSkinPacks() const;
    QStringList availableBackgrounds() const;
    QString currentSkin() const;
    QString currentBackground() const;

    const QPixmap& workerTex() const;
    const QPixmap& soldierTex() const;
    const QPixmap& archerTex() const;
    const QPixmap& tankTex() const;
    const QPixmap& enemyTex() const;
    const QPixmap& fastenemyTex() const;
    const QPixmap& heavyenemyTex() const;
    const QPixmap& rangedenemyTex() const;
    const QPixmap& bossTex() const;
    const QPixmap& baseTex() const;
    const QPixmap& goldmineTex() const;
    const QPixmap& backgroundTex() const;

private:
    TextureManager();
    void generateDefaults();

    QString m_basePath;
    QString m_skinPath;
    QString m_bgPath;
    QString m_currentSkin;
    QString m_currentBg;

    QPixmap m_worker;
    QPixmap m_soldier;
    QPixmap m_archer;
    QPixmap m_tank;
    QPixmap m_enemy;
    QPixmap m_fastenemy;
    QPixmap m_heavyenemy;
    QPixmap m_rangedenemy;
    QPixmap m_boss;
    QPixmap m_base;
    QPixmap m_goldmine;
    QPixmap m_background;
};

#endif
