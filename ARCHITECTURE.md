# EchoGL — Architecture

Built with C++20, Qt 6, SQLite3, and Python 3.

---

## Layers

```
┌─────────────────────────────────────────┐
│              UI Layer (Qt 6)            │
├─────────────────────────────────────────┤
│         Logic Layer (GameEngine)        │
├──────────────────┬──────────────────────┤
│  Provider Layer  │   Scripting Layer    │
├──────────────────┴──────────────────────┤
│            Data Layer (SQLite)          │
└─────────────────────────────────────────┘
```

Dependencies go downward only. Data layer knows nothing about the rest.

---

## C++ Layers

### `src/data/` — Data Layer
The only place SQL lives. Nothing outside this directory touches the database.

- `DBManager` — database connection, CRUD operations
- `models/Game.hpp` — game struct
- `DatabaseException.hpp` — typed db errors
- `SQLiteHandles.hpp` — RAII wrappers for sqlite3 pointers

### `src/providers/` — Provider Layer
Game discovery from external sources.

- `ILibraryProvider` — interface every provider implements
- `SteamProvider` — reads `libraryfolders.vdf`
- `EGSProvider` — reads Epic JSON manifests
- `EAProvider` — reads EA App XML manifests

To add a new store: inherit `ILibraryProvider`, implement `scanGames()`.

### `src/core/` — Logic Layer
Coordinates everything. Manages launches, sessions, Python calls.

- `GameEngine` — launches games, records sessions
- `ProcessWatcher` — WinAPI process detection
- `ScriptingBridge` — the only place C++ calls Python (pybind11)

### `src/ui/` — UI Layer
Displays data, sends commands to `GameEngine`. No business logic here.

---

## Python Layer — `scripts/`

Called exclusively through `ScriptingBridge`.
No direct database access. No knowledge of file paths.

```
GameEngine → ScriptingBridge → Python module(data) → returns result
```

### `backlog_digger/`
Scores unplayed and rarely played games.

Inputs: game list with playtime, session history (30 days), current time.
Output: top N games with `{game_id, score, reason}`.

- `recommender.py` — scoring pipeline
- `metrics.py` — individual metrics

### `weather_recommender/`
Adjusts recommendations based on weather and time of day.

Inputs: user city, game list with genres.
API: [open-meteo.com](https://open-meteo.com) — no key required.

- `client.py` — weather fetching
- `signals.py` — condition → modifier mapping

### `mood_detector/`
Infers mood from session patterns.

Inputs: session history (14 days).
Output: mood context used by other modules.

- `detector.py` — classification
- `patterns.py` — pattern definitions

### `shared/`
- `models.py` — dataclasses mirroring `src/data/models/`

---

## Database

Three tables — see `src/data/schema.sql`.

```
games         — metadata, cover path, playtime offset
game_sources  — steam / egs / ea / manual per game
sessions      — start time, end time, game_id
```

`PRAGMA foreign_keys = ON` always. Deleting a game cascades to sources and sessions.

---

## Project Structure

```
EchoGL/
├── CMakeLists.txt
├── vcpkg.json
├── ARCHITECTURE.md
├── CONTRIBUTING.md
│
├── cmake/
├── assets/
├── docs/
│
├── scripts/
│   ├── backlog_digger/
│   │   ├── __init__.py
│   │   ├── recommender.py
│   │   └── metrics.py
│   ├── weather_recommender/
│   │   ├── __init__.py
│   │   ├── client.py
│   │   └── signals.py
│   ├── mood_detector/
│   │   ├── __init__.py
│   │   ├── detector.py
│   │   └── patterns.py
│   └── shared/
│       ├── __init__.py
│       └── models.py
│
├── src/
│   ├── main.cpp
│   ├── data/
│   │   ├── DBManager.hpp/.cpp
│   │   ├── DatabaseException.hpp
│   │   ├── SQLiteHandles.hpp
│   │   ├── schema.sql
│   │   └── models/
│   │       └── Game.hpp
│   ├── providers/
│   │   ├── ILibraryProvider.hpp
│   │   ├── SteamProvider.hpp/.cpp
│   │   ├── EGSProvider.hpp/.cpp
│   │   └── EAProvider.hpp/.cpp
│   ├── core/
│   │   ├── GameEngine.hpp/.cpp
│   │   ├── ProcessWatcher.hpp/.cpp
│   │   └── ScriptingBridge.hpp/.cpp
│   └── ui/
│
├── tests/
│   ├── CMakeLists.txt
│   ├── data/
│   └── core/
│
└── third_party/
    ├── sqlite3/
    └── pybind11/
```

---

## Contributing

See `CONTRIBUTING.md`.
