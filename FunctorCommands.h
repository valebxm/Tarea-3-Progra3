#pragma once
#include <list>
#include <string>
#include <stdexcept>
#include <iostream>
#include "Entity.h"

class HealCommand {
private:
    Entity& entity_;
    int     execCount_;
    double  totalHealed_;

public:
    explicit HealCommand(Entity& entity)
        : entity_(entity), execCount_(0), totalHealed_(0.0) {}

    void operator()(const std::list<std::string>& args) {
        if (args.size() != 1)
            throw std::invalid_argument("heal requires exactly 1 argument: amount");

        double amount = std::stod(args.front());
        if (amount <= 0)
            throw std::invalid_argument("heal amount must be positive");

        std::string before = entity_.statusString();
        entity_.heal(amount);
        ++execCount_;
        totalHealed_ += amount;

        std::cout << "[heal] " << before << " -> " << entity_.statusString() << std::endl;
        std::cout << "[heal] Total healed so far: " << totalHealed_
                  << " (executions: " << execCount_ << ")" << std::endl;
    }

    int    getExecCount()  const { return execCount_; }
    double getTotalHealed() const { return totalHealed_; }
};

class EnergyCommand {
private:
    Entity& entity_;
    int     usageLimit_;
    int     usageCount_;

public:
    EnergyCommand(Entity& entity, int limit)
        : entity_(entity), usageLimit_(limit), usageCount_(0) {}

    void operator()(const std::list<std::string>& args) {
        if (usageCount_ >= usageLimit_) {
            throw std::runtime_error("energy command has reached its usage limit of " +
                                     std::to_string(usageLimit_));
        }
        if (args.size() != 1)
            throw std::invalid_argument("energy requires exactly 1 argument: amount");

        double amount = std::stod(args.front());
        std::string before = entity_.statusString();
        entity_.addEnergy(amount);
        ++usageCount_;

        std::cout << "[energy] " << before << " -> " << entity_.statusString() << std::endl;
        std::cout << "[energy] Uses remaining: " << (usageLimit_ - usageCount_) << std::endl;
    }
};

class RenameCommand {
private:
    Entity&                  entity_;
    std::list<std::string>   nameLog_;

public:
    explicit RenameCommand(Entity& entity) : entity_(entity) {}

    void operator()(const std::list<std::string>& args) {
        if (args.size() != 1)
            throw std::invalid_argument("rename requires exactly 1 argument: new_name");

        nameLog_.push_back(entity_.getName());
        entity_.setName(args.front());

        std::cout << "[rename] Entity renamed to: " << entity_.getName() << std::endl;
        std::cout << "[rename] Name history: ";
        for (const std::string& n : nameLog_) std::cout << n << " -> ";
        std::cout << entity_.getName() << std::endl;
    }
};
