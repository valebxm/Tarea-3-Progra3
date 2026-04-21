#include <iostream>
#include <list>
#include <string>
#include "Entity.h"
#include "CommandCenter.h"
#include "FreeCommands.h"
#include "FunctorCommands.h"

static void section(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

int main() {
    std::ios::sync_with_stdio(true);

    section("1. Creating Entity and CommandCenter");

    Entity        hero("Arthas", 100.0, 60.0, 1);
    CommandCenter center;

    hero.printStatus();

    section("2. Registering commands");


    center.registerCommand("move",
        [&hero](const std::list<std::string>& args) {
            cmdMove(hero, args);
        });

    center.registerCommand("damage",
        [&hero](const std::list<std::string>& args) {
            cmdDamage(hero, args);
        });

    center.registerCommand("levelup",
        [&hero](const std::list<std::string>& args) {
            cmdLevelUp(hero, args);
        });



    center.registerCommand("quickheal",
        [&hero](const std::list<std::string>& args) {
            if (args.size() != 1)
                throw std::invalid_argument("quickheal requires 1 argument: amount");
            double amount = std::stod(args.front());
            if (amount <= 0) throw std::invalid_argument("amount must be positive");
            std::string before = hero.statusString();
            hero.heal(amount);
            std::cout << "[quickheal] " << before << " -> " << hero.statusString() << std::endl;
        });

    center.registerCommand("reset",
        [&hero](const std::list<std::string>& args) {
            if (!args.empty()) throw std::invalid_argument("reset takes no arguments");
            std::string before = hero.statusString();
            hero.reset();
            std::cout << "[reset] " << before << " -> " << hero.statusString() << std::endl;
        });

    center.registerCommand("status",
        [&hero](const std::list<std::string>& args) {
            if (!args.empty()) throw std::invalid_argument("status takes no arguments");
            hero.printStatus();
        });

    center.registerCommand("setpos",
        [&hero](const std::list<std::string>& args) {
            if (args.size() != 2) throw std::invalid_argument("setpos requires x y");
            std::list<std::string>::const_iterator it = args.begin();
            double x = std::stod(*it); ++it;
            double y = std::stod(*it);
            std::string before = hero.statusString();
            hero.setPosition(x, y);
            std::cout << "[setpos] " << before << " -> " << hero.statusString() << std::endl;
        });


    HealCommand   healCmd(hero);
    EnergyCommand energyCmd(hero, 3);
    RenameCommand renameCmd(hero);

    center.registerCommand("heal",   healCmd);
    center.registerCommand("energy", energyCmd);
    center.registerCommand("rename", renameCmd);

    section("3. Executing valid commands");

    center.execute("move",    {"10", "20"});
    center.execute("move",    {"-5", "3"});
    center.execute("damage",  {"30.5"});
    center.execute("levelup", {});
    center.execute("levelup", {});
    center.execute("levelup", {});

    center.execute("quickheal", {"15"});
    center.execute("setpos",    {"0", "0"});
    center.execute("status",    {});
    center.execute("reset",     {});
    center.execute("status",    {});

    center.execute("heal",   {"25"});
    center.execute("heal",   {"10"});
    center.execute("heal",   {"5"});

    center.execute("energy", {"20"});
    center.execute("energy", {"10"});
    center.execute("energy", {"5"});

    center.execute("rename", {"Lich King"});
    center.execute("rename", {"Arthas"});
    center.execute("rename", {"Hero"});

    section("4. Executing invalid / error commands");

    center.execute("fly", {});

    center.execute("move",   {"10"});
    center.execute("heal",   {});
    center.execute("damage", {"10", "extra"});

    center.execute("damage", {"abc"});

    center.execute("energy", {"99"});

    section("5. Dynamic command removal");

    center.removeCommand("quickheal");
    center.execute("quickheal", {"10"});

    center.removeCommand("nonexistent");

    section("6. Macro commands");

    center.registerMacro("full_restore", {
        {"reset",  {}},
        {"status", {}}
    });

    center.registerMacro("battle_round", {
        {"damage", {"20"}},
        {"heal",   {"5"}},
        {"status", {}}
    });

    center.registerMacro("explore", {
        {"move",   {"15", "5"}},
        {"move",   {"0",  "10"}},
        {"setpos", {"100", "100"}},
        {"status", {}}
    });

    center.registerMacro("broken_macro", {
        {"heal",      {"10"}},
        {"quickheal", {"5"}},
        {"status",    {}}
    });

    std::cout << std::endl;
    center.executeMacro("full_restore");
    std::cout << std::endl;
    center.executeMacro("battle_round");
    std::cout << std::endl;
    center.executeMacro("explore");
    std::cout << std::endl;
    center.executeMacro("broken_macro");
    std::cout << std::endl;
    center.executeMacro("nonexistent_macro");

    section("7. Final state and execution history");

    hero.printStatus();
    center.printHistory();

    return 0;
}
