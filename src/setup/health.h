// health.h

#pragma once
#include <chrono>

class Health {
public:
    Health(int max_hp);

    void TakeDamage(int damage);
    void Heal(int amount);
    void ApplyTimeDamage(int damagePerSecond, float timeElapsed);

    bool IsDead() const;

    int GetCurrentHealth() const;
    void SetMaxHealth(int new_max);
    int GetMaxHealth() const;

    // Public methods to access last damage time
    std::chrono::steady_clock::time_point GetLastDamageTime() const;
    void SetLastDamageTime(std::chrono::steady_clock::time_point time);

private:
    int max_health;
    int current_health;
    std::chrono::steady_clock::time_point lastDamageTime;
};
