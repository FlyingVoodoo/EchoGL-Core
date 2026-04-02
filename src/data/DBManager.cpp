#include "DBManager.h"
#include "DatabaseException.h"

DBManager::DBManager(const std::string& dbPath, const std::string& schema) {
    sqlite3* raw_db = nullptr;
    int rc = sqlite3_open(dbPath.c_str(), &raw_db);
    m_db.reset(raw_db);

    if (rc != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(m_db.get()), rc);
    }

    execute("PRAGMA foreign_keys = ON;");
    execute(schema);
}

void DBManager::execute(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db.get(), sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string error(errMsg);
        sqlite3_free(errMsg);
        throw DatabaseException(error, rc);
    }
}

void DBManager::addGame(const Game& game) {
    sqlite3_stmt* raw_stmt = nullptr;
    int rc = sqlite3_prepare_v2(
        m_db.get(),
        "INSERT INTO games (title, description, cover_path, "
        "steam_playtime_seconds, steam_synced_at) "
        "VALUES (?, ?, ?, ?, ?)",
        -1, &raw_stmt, nullptr
    );
    StatementHandle stmt(raw_stmt);
    if (rc != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(m_db.get()), rc);
    }

    sqlite3_bind_text(stmt.get(), 1, game.title.c_str(), -1, SQLITE_TRANSIENT);

    if (game.description)
        sqlite3_bind_text(stmt.get(), 2, game.description->c_str(), -1, SQLITE_TRANSIENT);
    else
        sqlite3_bind_null(stmt.get(), 2);

    if (game.cover_path)
        sqlite3_bind_text(stmt.get(), 3, game.cover_path->c_str(), -1, SQLITE_TRANSIENT);
    else
        sqlite3_bind_null(stmt.get(), 3);

    sqlite3_bind_int(stmt.get(), 4, game.steam_playtime_seconds);

    if (game.steam_synced_at)
        sqlite3_bind_int(stmt.get(), 5, *game.steam_synced_at);
    else
        sqlite3_bind_null(stmt.get(), 5);

    if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
        throw DatabaseException(sqlite3_errmsg(m_db.get()), SQLITE_ERROR);
    }
}