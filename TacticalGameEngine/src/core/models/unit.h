#pragma once

#include "../utils/id-generator.h"

#include <string>

namespace Engine::Core::Models {

    class Unit {
    private:
        std::string name_;
        std::string type_;
        std::string id_;
    public:
        Unit(std::string name, std::string type) : name_(std::move(name)), type_(type), id_(std::move(Utils::generateId())) {}//ДОБАВИТЬ MOVE ДЛЯ СТРОК
        ~Unit() = default;
        std::string getName() const { return name_; }
        std::string getType() const { return type_; }
        std::string getId() const { return id_; }
    };
}