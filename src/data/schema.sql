PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS games (
    id                      INTEGER PRIMARY KEY AUTOINCREMENT,
    title                   TEXT    NOT NULL,
    description             TEXT,
    cover_path              TEXT,
    steam_playtime_seconds  INTEGER NOT NULL DEFAULT 0,
    steam_synced_at         INTEGER
);

CREATE TABLE IF NOT EXISTS game_sources (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    game_id     INTEGER NOT NULL,
    platform    TEXT    NOT NULL,
    external_id TEXT,
    exe_path    TEXT,

    FOREIGN KEY (game_id) REFERENCES games(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS sessions (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    game_id     INTEGER NOT NULL,
    started_at  INTEGER NOT NULL,
    ended_at    INTEGER,

    FOREIGN KEY (game_id) REFERENCES games(id) ON DELETE CASCADE
);