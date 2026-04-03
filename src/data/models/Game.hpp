#pragma once
#include <optional>
#include <string>

struct Game {
    std::optional<int> id;
    std::string title;
    std::optional<std::string> description;
    std::optional<std::string> cover_path;
    int steam_playtime_seconds;
    std::optional<int> steam_synced_at;
};