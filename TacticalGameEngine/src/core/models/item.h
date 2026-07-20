#pragma once

#include "../utils/id-generator.h"

#include <string>

namespace Engine::Core::Models
{

    class Item
    {
    private:
        std::string name_;
        std::string type_;
        std::string id_;

    public:
        Item(std::string name, std::string type) : name_(std::move(name)),
                                                   type_(std::move(type)),
                                                   id_(std::move(Utils::generateId())) {} // ДОБАВИТЬ MOVE ДЛЯ СТРОК

        ~Item() = default;

        Item(const Item &) = delete;

        Item &operator=(const Item &) = delete;

        Item(Item &&) noexcept = default;

        Item &operator=(Item &&) noexcept = default;
        std::string getName() const { return name_; }
        std::string getType() const { return type_; }
        std::string getId() const { return id_; }
        void setId(std::string id) { id_ = std::move(id); }
        void CreateConfigFile(std::string file_path);
        void CreateConfigFile(std::string file_path, std::string port = "80");
    };
}