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