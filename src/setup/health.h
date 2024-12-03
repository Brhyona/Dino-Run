#pragma once
#include <chrono>
#include <functional>

class Health {
public:
    using GameOverCallback = std::function<void()>;

    Health(int max_hp);

    void TakeDamage(int damage);
    void Heal(int amount);
    void ApplyTimeDamage(int damagePerSecond, float timeElapsed);

    bool IsDead() const;

    int GetCurrentHealth() const;
    void SetMaxHealth(int new_max);
    int GetMaxHealth() const;

    std::chrono::steady_clock::time_point GetLastDamageTime() const;
    void SetLastDamageTime(std::chrono::steady_clock::time_point time);

    void SetGameOverCallback(GameOverCallback callback);

private:
    int max_health;
    int current_health;
    std::chrono::steady_clock::time_point lastDamageTime;
    GameOverCallback gameOverCallback;
};
