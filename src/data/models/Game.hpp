#pragma once
#include <string>
#include <optional>

struct Game {
    std::optional<int> id;
    std::string        title;
    std::string        description;
    std::string        cover_path;
    int                steam_playtime_seconds;
    std::optional<int> steam_synced_at;
};