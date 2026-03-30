#pragma once
#include <stdexcept>

class DatabaseException : public std::runtime_error {
public:
    explicit DatabaseException(const std::string& message, int sqliteCode)
        : std::runtime_error(message)
        , m_sqliteCode(sqliteCode)
    {}

    int sqliteCode() const { return m_sqliteCode; }

private:
    int m_sqliteCode;
};