#pragma once
#include <list>
#include <string>
#include <stdexcept>
#include <iostream>
#include "Entity.h"

void cmdMove(Entity& entity, const std::list<std::string>& args) {
    if (args.size() != 2)
        throw std::invalid_argument("move requires exactly 2 arguments: dx dy");

    std::list<std::string>::const_iterator it = args.begin();
    double dx = std::stod(*it); ++it;
    double dy = std::stod(*it);

    std::string before = entity.statusString();
    entity.move(dx, dy);
    std::cout << "[move] " << before << " -> " << entity.statusString() << std::endl;
}

void cmdDamage(Entity& entity, const std::list<std::string>& args) {
    if (args.size() != 1)
        throw std::invalid_argument("damage requires exactly 1 argument: amount");

    double amount = std::stod(args.front());
    if (amount < 0)
        throw std::invalid_argument("damage amount must be non-negative");

    std::string before = entity.statusString();
    entity.damage(amount);
    std::cout << "[damage] " << before << " -> " << entity.statusString() << std::endl;

    if (!entity.isAlive())
        std::cout << "[damage] " << entity.getName() << " has been defeated!" << std::endl;
}

void cmdLevelUp(Entity& entity, const std::list<std::string>& args) {
    if (!args.empty())
        throw std::invalid_argument("levelup takes no arguments");

    std::string before = entity.statusString();
    entity.levelUp();
    std::cout << "[levelup] " << before << " -> " << entity.statusString() << std::endl;
}
