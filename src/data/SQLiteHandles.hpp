#pragma once
#include <memory>
#include <sqlite3.h>

struct SQLiteDeleter {
    void operator()(sqlite3* db) const { sqlite3_close(db); }
};

struct StatementDeleter {
    void operator()(sqlite3_stmt* stmt) const { sqlite3_finalize(stmt); }
};

using SQLiteHandle    = std::unique_ptr<sqlite3,      SQLiteDeleter>;
using StatementHandle = std::unique_ptr<sqlite3_stmt, StatementDeleter>;