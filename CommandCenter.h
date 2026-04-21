#pragma once
#include <functional>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <stdexcept>
#include "Entity.h"

using Command = std::function<void(const std::list<std::string>&)>;

class CommandCenter {
private:
    std::map<std::string, Command>  commands_;
    std::list<std::string>          history_;

    std::map<std::string, std::list<std::pair<std::string, std::list<std::string>>>> macros_;

    void addHistory(const std::string& entry) {
        history_.push_back(entry);
    }

public:

    void registerCommand(const std::string& name, Command cmd) {
        commands_[name] = cmd;
        std::cout << "[CommandCenter] Registered command: " << name << std::endl;
    }

    void removeCommand(const std::string& name) {
        std::map<std::string, Command>::iterator it = commands_.find(name);
        if (it == commands_.end()) {
            std::cout << "[CommandCenter] Cannot remove '" << name
                      << "': command not found." << std::endl;
            return;
        }
        commands_.erase(it);
        std::cout << "[CommandCenter] Removed command: " << name << std::endl;
    }


    void execute(const std::string& name, const std::list<std::string>& args) {
        std::map<std::string, Command>::iterator it = commands_.find(name);
        if (it == commands_.end()) {
            std::string errMsg = "[ERROR] Command '" + name + "' not found.";
            std::cout << errMsg << std::endl;
            addHistory(errMsg);
            return;
        }

        try {
            it->second(args);
            std::string argsStr;
            for (const std::string& a : args) argsStr += " " + a;
            addHistory("[OK] " + name + argsStr);
        } catch (const std::exception& e) {
            std::string errMsg = "[EXCEPTION] " + name + ": " + e.what();
            std::cout << errMsg << std::endl;
            addHistory(errMsg);
        }
    }


    void registerMacro(
        const std::string& name,
        const std::list<std::pair<std::string, std::list<std::string>>>& steps)
    {
        macros_[name] = steps;
        std::cout << "[CommandCenter] Registered macro: " << name << std::endl;
    }

    void executeMacro(const std::string& name) {
        std::map<std::string,
            std::list<std::pair<std::string, std::list<std::string>>>>::iterator mit =
            macros_.find(name);

        if (mit == macros_.end()) {
            std::cout << "[CommandCenter] Macro '" << name << "' not found." << std::endl;
            return;
        }

        std::cout << "[CommandCenter] Executing macro: " << name << std::endl;

        std::list<std::pair<std::string, std::list<std::string>>>::iterator sit =
            mit->second.begin();

        while (sit != mit->second.end()) {
            const std::string& cmdName  = sit->first;
            const std::list<std::string>& cmdArgs = sit->second;

            std::map<std::string, Command>::iterator cit = commands_.find(cmdName);
            if (cit == commands_.end()) {
                std::cout << "[Macro ERROR] Step '" << cmdName
                          << "' not found. Stopping macro." << std::endl;
                addHistory("[MACRO ERROR] " + name + " stopped at: " + cmdName);
                return;
            }

            execute(cmdName, cmdArgs);
            ++sit;
        }

        addHistory("[MACRO OK] " + name);
    }


    void printHistory() const {
        std::cout << "\n=== Execution History ===" << std::endl;
        std::list<std::string>::const_iterator it = history_.begin();
        int i = 1;
        while (it != history_.end()) {
            std::cout << "  " << i++ << ". " << *it << std::endl;
            ++it;
        }
        std::cout << "=========================" << std::endl;
    }

    const std::list<std::string>& getHistory() const { return history_; }
};
