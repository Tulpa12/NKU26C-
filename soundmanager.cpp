#include "soundmanager.h"
#include <QApplication>
#include <QDir>
#include <QFile>

SoundManager& SoundManager::instance()
{
    static SoundManager mgr;
    return mgr;
}

SoundManager::SoundManager()
{
    m_soundPath = QApplication::applicationDirPath() + "/sounds/";
    if (!QDir(m_soundPath).exists()) {
        m_soundPath = QApplication::applicationDirPath() + "/../sounds/";
    }
    if (!QDir(m_soundPath).exists()) {
        m_soundPath = QApplication::applicationDirPath() + "/../../sounds/";
    }

    loadSound(m_spawn,         "spawn.wav");
    loadSound(m_soldierAttack, "soldier_attack.wav");
    loadSound(m_enemyAttack,   "enemy_attack.wav");
    loadSound(m_baseHit,       "base_hit.wav");
    loadSound(m_allyDeath,     "ally_death.wav");
    loadSound(m_enemyDeath,    "enemy_death.wav");
    loadSound(m_gold,          "gold.wav");
    loadSound(m_wave,          "wave.wav");
    loadSound(m_boss,          "boss.wav");
    loadSound(m_victory,       "victory.wav");
    loadSound(m_defeat,        "defeat.wav");

    // Start timers so first call plays immediately
    m_soldierAttackTimer.start();
    m_enemyAttackTimer.start();
    m_baseHitTimer.start();
    m_allyDeathTimer.start();
    m_enemyDeathTimer.start();
    m_goldTimer.start();

    // Pre-warm all sounds to avoid first-play stutter.
    // QSoundEffect decodes lazily — the first play() triggers audio device
    // init and WAV decoding on the main thread. Playing at zero volume
    // forces that work here (during construction) instead of mid-gameplay.
    QSoundEffect* all[] = {m_spawn, m_soldierAttack, m_enemyAttack, m_baseHit,
                           m_allyDeath, m_enemyDeath, m_gold, m_wave, m_boss,
                           m_victory, m_defeat};
    for (auto* e : all) {
        if (e) {
            qreal saved = e->volume();
            e->setVolume(0);
            e->play();
            QApplication::processEvents();
            e->stop();
            e->setVolume(saved);
        }
    }
}

void SoundManager::loadSound(QSoundEffect*& effect, const QString& filename)
{
    QString path = m_soundPath + filename;
    if (!QFile::exists(path)) return;
    effect = new QSoundEffect(this);
    effect->setSource(QUrl::fromLocalFile(path));
    effect->setVolume(0.5);
}

void SoundManager::setVolume(qreal vol)
{
    auto setVol = [vol](QSoundEffect* e) { if (e) e->setVolume(vol); };
    setVol(m_spawn); setVol(m_soldierAttack); setVol(m_enemyAttack);
    setVol(m_baseHit); setVol(m_allyDeath); setVol(m_enemyDeath);
    setVol(m_gold); setVol(m_wave); setVol(m_boss);
    setVol(m_victory); setVol(m_defeat);
}

void SoundManager::playWithCooldown(QSoundEffect* effect, QElapsedTimer& timer, int minIntervalMs)
{
    if (!m_enabled || !effect) return;
    if (timer.elapsed() < minIntervalMs) return;
    timer.restart();
    effect->play();
}

// --- Low-frequency: always play ---

void SoundManager::playSpawn()  { if (m_enabled && m_spawn)  m_spawn->play(); }
void SoundManager::playWave()   { if (m_enabled && m_wave)   m_wave->play(); }
void SoundManager::playBoss()   { if (m_enabled && m_boss)   m_boss->play(); }
void SoundManager::playVictory(){ if (m_enabled && m_victory) m_victory->play(); }
void SoundManager::playDefeat() { if (m_enabled && m_defeat)  m_defeat->play(); }

// --- High-frequency: cooldown-limited ---

void SoundManager::playSoldierAttack() { playWithCooldown(m_soldierAttack, m_soldierAttackTimer, 200); }
void SoundManager::playEnemyAttack()   { playWithCooldown(m_enemyAttack,   m_enemyAttackTimer,   200); }
void SoundManager::playBaseHit()       { playWithCooldown(m_baseHit,       m_baseHitTimer,       400); }
void SoundManager::playAllyDeath()     { playWithCooldown(m_allyDeath,     m_allyDeathTimer,     300); }
void SoundManager::playEnemyDeath()    { playWithCooldown(m_enemyDeath,    m_enemyDeathTimer,    300); }
void SoundManager::playGold()          { playWithCooldown(m_gold,          m_goldTimer,          250); }
