#pragma once
#include <string>
#include <iostream>

class Entity {
private:
    std::string name_;
    double health_;
    double maxHealth_;
    double energy_;
    int level_;
    double x_, y_;

public:
    Entity(const std::string& name, double health = 100.0, double energy = 50.0, int level = 1)
        : name_(name), health_(health), maxHealth_(health), energy_(energy),
          level_(level), x_(0.0), y_(0.0) {}

    // Getters
    std::string getName()   const { return name_; }
    double      getHealth() const { return health_; }
    double      getMaxHealth() const { return maxHealth_; }
    double      getEnergy() const { return energy_; }
    int         getLevel()  const { return level_; }
    double      getX()      const { return x_; }
    double      getY()      const { return y_; }

    void move(double dx, double dy) { x_ += dx; y_ += dy; }
    void setPosition(double x, double y) { x_ = x; y_ = y; }

    void heal(double amount) {
        health_ = std::min(health_ + amount, maxHealth_);
    }

    void damage(double amount) {
        health_ = std::max(health_ - amount, 0.0);
    }

    void setEnergy(double e) { energy_ = e; }
    void addEnergy(double e) { energy_ = std::max(0.0, energy_ + e); }

    void levelUp() { ++level_; maxHealth_ += 10; }
    void setName(const std::string& n) { name_ = n; }

    void reset() {
        health_ = maxHealth_;
        energy_ = 50.0;
        x_ = 0.0; y_ = 0.0;
    }

    bool isAlive() const { return health_ > 0.0; }

    std::string statusString() const {
        return "[" + name_ + "] HP:" + std::to_string(health_) + "/" +
               std::to_string(maxHealth_) + " EN:" + std::to_string(energy_) +
               " LVL:" + std::to_string(level_) +
               " POS:(" + std::to_string(x_) + "," + std::to_string(y_) + ")";
    }

    void printStatus() const {
        std::cout << "=== STATUS ===" << std::endl;
        std::cout << "  Name   : " << name_ << std::endl;
        std::cout << "  Health : " << health_ << " / " << maxHealth_ << std::endl;
        std::cout << "  Energy : " << energy_ << std::endl;
        std::cout << "  Level  : " << level_ << std::endl;
        std::cout << "  Pos    : (" << x_ << ", " << y_ << ")" << std::endl;
        std::cout << "==============" << std::endl;
    }
};
