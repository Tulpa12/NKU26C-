#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QSoundEffect>
#include <QElapsedTimer>
#include <QString>
#include <QObject>

class SoundManager : public QObject {
    Q_OBJECT

public:
    static SoundManager& instance();

    void setEnabled(bool on) { m_enabled = on; }
    bool isEnabled() const { return m_enabled; }
    void setVolume(qreal vol);

    void playSpawn();
    void playSoldierAttack();
    void playEnemyAttack();
    void playBaseHit();
    void playAllyDeath();
    void playEnemyDeath();
    void playGold();
    void playWave();
    void playBoss();
    void playVictory();
    void playDefeat();

private:
    SoundManager();
    void loadSound(QSoundEffect*& effect, const QString& filename);
    void playWithCooldown(QSoundEffect* effect, QElapsedTimer& timer, int minIntervalMs);

    QSoundEffect* m_spawn = nullptr;
    QSoundEffect* m_soldierAttack = nullptr;
    QSoundEffect* m_enemyAttack = nullptr;
    QSoundEffect* m_baseHit = nullptr;
    QSoundEffect* m_allyDeath = nullptr;
    QSoundEffect* m_enemyDeath = nullptr;
    QSoundEffect* m_gold = nullptr;
    QSoundEffect* m_wave = nullptr;
    QSoundEffect* m_boss = nullptr;
    QSoundEffect* m_victory = nullptr;
    QSoundEffect* m_defeat = nullptr;

    QElapsedTimer m_soldierAttackTimer;
    QElapsedTimer m_enemyAttackTimer;
    QElapsedTimer m_baseHitTimer;
    QElapsedTimer m_allyDeathTimer;
    QElapsedTimer m_enemyDeathTimer;
    QElapsedTimer m_goldTimer;

    QString m_soundPath;
    bool m_enabled = true;
};

#endif
