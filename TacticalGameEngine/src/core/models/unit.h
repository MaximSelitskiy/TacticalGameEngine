#pragma once

#include "../utils/id-generator.h"
#include "map.h"
#include "item.h"

#include <string>
#include <optional>
#include <memory>
#include <vector>
namespace Engine::Core::Models {

    class Unit {
    private:
        std::string name_;
        std::string type_;
        std::string id_;
        std::optional<Vec2> position_;
        std::vector<std::unique_ptr<Item>> inventory_;
    public:
        Unit(std::string name, std::string type) :
            name_(std::move(name)),
            type_(type),
            id_(std::move(Utils::generateId())),
            position_(std::nullopt){}//ДОБАВИТЬ MOVE ДЛЯ СТРОК
        ~Unit() = default;
        std::string getName() const { return name_; }
        std::string getType() const { return type_; }
        std::string getId() const { return id_; }
        void setId(std::string id) { id_ = std::move(id); }
        //POSITION
        std::optional<Vec2> getPosition() const { return position_; }
        void setPosition(short x,short y) {
            position_.reset();
            position_ = std::make_optional<Vec2>(x, y);
        }
        bool isPlaced() const {
            return position_.has_value();
        }
        void removeFromMap() {
            position_ = std::nullopt;
        }
        void addItem(std::unique_ptr<Item> item) {
            inventory_.push_back(std::move(item));
        }
        const std::vector<std::unique_ptr<Item>>& getInventory() const { return inventory_; }
    };
}