#pragma once

#include "keys.hpp"
#include "location_access.hpp"
#include "debug.hpp"

#include <string>
#include <list>

#include "../code/src/entrance.h"

#define ENTRANCE_SHUFFLE_SUCCESS 0
#define ENTRANCE_SHUFFLE_FAILURE 1

extern std::list<EntranceOverride> entranceOverrides;

enum class EntranceType {
    None,
    OwlDrop,
    Spawn,
    WarpSong,
    Dungeon,
    GanonDungeon,
    DungeonReverse,
    Interior,
    InteriorReverse,
    SpecialInterior,
    GrottoGrave,
    GrottoGraveReverse,
    Overworld,
    Extra,
    Mixed,
    All,
};

class Entrance {
public:

    Entrance(AreaKey connectedRegion_, std::vector<ConditionFn> conditions_met_)
        : connectedRegion(connectedRegion_) {
        conditions_met.resize(2);
        for (size_t i = 0; i < conditions_met_.size(); i++) {
            conditions_met[i] = conditions_met_[i];
        }
    }

    // Resets the glitchless condition for the entrance
    void SetCondition(ConditionFn newCondition) {
        conditions_met[0] = newCondition;
    }

    bool GetConditionsMet() const {
        if (Settings::Logic.Is(LOGIC_NONE) || Settings::Logic.Is(LOGIC_VANILLA)) {
            return true;
        } else if (Settings::Logic.Is(LOGIC_GLITCHLESS)) {
            return conditions_met[0]();
        } else if (Settings::Logic.Is(LOGIC_GLITCHED)) {
            if (conditions_met[0]()) {
                return true;
            } else if (conditions_met[1] != NULL) {
                return conditions_met[1]();
            }
        }
        return false;
    }

    std::string to_string() const {
        return AreaTable(parentRegion)->regionName + " -> " + AreaTable(connectedRegion)->regionName;
    }

    void SetName(std::string name_ = "") {
        if (name_ == "") {
          name = AreaTable(parentRegion)->regionName + " -> " + AreaTable(connectedRegion)->regionName;
        } else {
          name = std::move(name_);
        }

    }

    std::string GetName() const {
        return name;
    }

    void printAgeTimeAccess() {
      CitraPrint("Name: ");
      CitraPrint(name);
      auto message = "Child Day:   " + std::to_string(CheckConditionAtAgeTime(Logic::IsChild, Logic::AtDay))   + "\t"
                     "Child Night: " + std::to_string(CheckConditionAtAgeTime(Logic::IsChild, Logic::AtNight)) + "\t"
                     "Adult Day:   " + std::to_string(CheckConditionAtAgeTime(Logic::IsAdult, Logic::AtDay))   + "\t"
                     "Adult Night: " + std::to_string(CheckConditionAtAgeTime(Logic::IsAdult, Logic::AtNight));
      CitraPrint(message);
    }

    bool ConditionsMet(bool allAgeTimes = false) const {

        Area* parent = AreaTable(parentRegion);
        int conditionsMet = 0;

        if (allAgeTimes && !parent->AllAccess()) {
            return false;
        }

        //check all possible day/night condition combinations
        conditionsMet = (parent->childDay   && CheckConditionAtAgeTime(Logic::IsChild, Logic::AtDay, allAgeTimes))   +
                        (parent->childNight && CheckConditionAtAgeTime(Logic::IsChild, Logic::AtNight, allAgeTimes)) +
                        (parent->adultDay   && CheckConditionAtAgeTime(Logic::IsAdult, Logic::AtDay, allAgeTimes))   +
                        (parent->adultNight && CheckConditionAtAgeTime(Logic::IsAdult, Logic::AtNight, allAgeTimes));

        return conditionsMet && (!allAgeTimes || conditionsMet == 4);
    }

    AreaKey GetAreaKey() const {
        return connectedRegion;
    }

    //set the logic to be a specific age and time of day and see if the condition still holds
    bool CheckConditionAtAgeTime(bool& age, bool& time, bool passAnyway = false) const {

        Logic::IsChild = false;
        Logic::IsAdult = false;
        Logic::AtDay   = false;
        Logic::AtNight = false;

        time = true;
        age = true;

        Logic::UpdateHelpers();
        return GetConditionsMet() && (connectedRegion != NONE || passAnyway);
    }

    AreaKey GetConnectedRegionKey() const {
        return connectedRegion;
    }

    Area* GetConnectedRegion() const {
        return AreaTable(connectedRegion);
    }

    void SetParentRegion(AreaKey newParent) {
        parentRegion = newParent;
    }

    AreaKey GetParentRegionKey() const {
        return parentRegion;
    }

    Area* GetParentRegion() const {
        return AreaTable(parentRegion);
    }

    void SetNewEntrance(AreaKey newRegion) {
        connectedRegion = newRegion;
    }

    void SetAsShuffled() {
        shuffled = true;
    }

    bool IsShuffled() const {
        return shuffled;
    }

    bool IsAddedToPool() const {
        return addedToPool;
    }

    void AddToPool() {
        addedToPool = true;
    }

    void RemoveFromPool() {
        addedToPool = false;
    }

    void SetAsPrimary() {
        primary = true;
    }

    bool IsPrimary() const {
        return primary;
    }

    s16 GetIndex() const {
        return index;
    }

    void SetIndex(s16 newIndex) {
        index = newIndex;
    }

    s16 GetBlueWarp() const {
        return blueWarp;
    }

    void SetBlueWarp(s16 newBlueWarp) {
        blueWarp = newBlueWarp;
    }

    Entrance* GetAssumed() const {
        return assumed;
    }

    void SetReplacement(Entrance* newReplacement) {
        replacement = newReplacement;
    }

    Entrance* GetReplacement() const {
        return replacement;
    }

    EntranceType GetType() const {
        return type;
    }

    void SetType(EntranceType newType) {
        type = newType;
    }

    Entrance* GetReverse() const {
        return reverse;
    }

    void Connect(AreaKey newConnectedRegion) {
        connectedRegion = newConnectedRegion;
        AreaTable(newConnectedRegion)->entrances.push_front(this);
    }

    AreaKey Disconnect() {
        AreaTable(connectedRegion)->entrances.remove_if([this](const auto entrance){return this == entrance;});
        AreaKey previouslyConnected = connectedRegion;
        connectedRegion = NONE;
        return previouslyConnected;
    }

    void BindTwoWay(Entrance* otherEntrance) {
        reverse = otherEntrance;
        otherEntrance->reverse = this;
    }

    Entrance* GetNewTarget() {
        AreaTable(ROOT)->AddExit(ROOT, connectedRegion, []{return true;});
        Entrance* targetEntrance = AreaTable(ROOT)->GetExit(connectedRegion);
        targetEntrance->SetReplacement(this);
        targetEntrance->SetName(GetParentRegion()->regionName + " -> " + GetConnectedRegion()->regionName);
        return targetEntrance;
    }

    Entrance* AssumeReachable() {
        if (assumed == nullptr) {
            assumed = GetNewTarget();
            Disconnect();
        }
        return assumed;
    }

private:
    AreaKey parentRegion;
    AreaKey connectedRegion;
    std::vector<ConditionFn> conditions_met;

    //Entrance Randomizer stuff
    EntranceType type = EntranceType::None;
    Entrance* target = nullptr;
    Entrance* reverse = nullptr;
    Entrance* assumed = nullptr;
    Entrance* replacement = nullptr;
    s16 index = 0xFFFF;
    s16 blueWarp = 0;
    bool shuffled = false;
    bool primary = false;
    bool addedToPool = false;
    std::string name = "";
};

int  ShuffleAllEntrances();
void CreateEntranceOverrides();
EntranceTrackingData* GetEntranceTrackingData();

extern std::vector<std::list<Entrance*>> playthroughEntrances;
extern bool noRandomEntrances;
