#include "src/setup/health.h"

Health::Health(int max_hp) : max_health(max_hp), current_health(max_hp) {
    lastDamageTime = std::chrono::steady_clock::now();
}

void Health::TakeDamage(int damage) {
    current_health -= damage;
    if (current_health < 0) {
        current_health = 0;
    }
}

void Health::Heal(int amount) {
    current_health += amount;
    if (current_health > max_health) {
        current_health = max_health;
    }
}

void Health::ApplyTimeDamage(int damagePerSecond, float timeElapsed) {
    int damage = static_cast<int>(damagePerSecond * timeElapsed);
    TakeDamage(damage);
}

bool Health::IsDead() const {
    return current_health <= 0;
}

int Health::GetCurrentHealth() const {
    return current_health;
}

void Health::SetMaxHealth(int new_max) {
    max_health = new_max;
}

int Health::GetMaxHealth() const {
    return max_health;
}

std::chrono::steady_clock::time_point Health::GetLastDamageTime() const {
    return lastDamageTime;
}

void Health::SetLastDamageTime(std::chrono::steady_clock::time_point time) {
    lastDamageTime = time;
}

