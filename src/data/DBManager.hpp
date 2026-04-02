#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite3.h>
#include "SQLiteHandles.hpp"
#include "models/Game.hpp"

class DBManager {
private:
    SQLiteHandle m_db;
    void execute(const std::string& sql);

public:
    explicit DBManager(const std::string& dbPath, const std::string& schema);
    ~DBManager();

    void addGame(const Game& game);
    std::optional<Game> getGameById(int id);
    std::vector<Game> getAllGames();
    void updateGameMetadata(int id, const std::string& description, 
                            const std::string& coverPath);

    std::optional<int> startSession(int gameId);
    void endSession(int sessionId);
};