#pragma once
#include <sqlite3.h>

#include <optional>
#include <string>
#include <vector>

#include "SQLiteHandles.hpp"
#include "models/Game.hpp"

class DBManager {
   private:
    SQLiteHandle m_db;
    void execute(const std::string& sql);

    void bindOptionalText(sqlite3_stmt* stmt, int index, const std::optional<std::string>& value);

   public:
    explicit DBManager(const std::string& dbPath, const std::string& schema);

    void addGame(const Game& game);
    std::optional<Game> getGameById(int id);
    std::vector<Game> getAllGames();
    void updateGameMetadata(int id, const std::string& description, const std::string& coverPath);

    std::optional<int> startSession(int gameId);
    void endSession(int sessionId);
};