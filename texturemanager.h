#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QDir>

class TextureManager {
public:
    static TextureManager& instance();

    void loadPack(const QString& packFolder);
    QStringList availablePacks() const;
    QString currentPack() const;

    const QPixmap& workerTex() const;
    const QPixmap& soldierTex() const;
    const QPixmap& enemyTex() const;
    const QPixmap& bossTex() const;
    const QPixmap& baseTex() const;
    const QPixmap& goldmineTex() const;
    const QPixmap& backgroundTex() const;

private:
    TextureManager();
    QPixmap loadOrGenerate(const QString& filename, const std::function<QPixmap()>& generator);
    void generateDefaults();

    QString m_basePath;
    QString m_currentPack;

    QPixmap m_worker;
    QPixmap m_soldier;
    QPixmap m_enemy;
    QPixmap m_boss;
    QPixmap m_base;
    QPixmap m_goldmine;
    QPixmap m_background;
};

#endif
