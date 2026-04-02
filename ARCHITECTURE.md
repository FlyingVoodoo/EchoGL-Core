# EchoGL Architecture

EchoGL is a performant, extensible game launcher that aggregates games 
from multiple sources (Steam, EGS, manual) with minimal resource usage.
Built with C++20, Qt 6, SQLite3, and Python 3.

---

## Core Principles

- **Layered architecture** — each layer has a single responsibility and 
  depends only on layers below it
- **No circular dependencies** — data layer knows nothing about UI, 
  UI knows nothing about SQL
- **C++ owns data, Python owns intelligence** — all database access goes 
  through C++, Python receives data as parameters and returns results

---

## Architecture Layers
```
┌─────────────────────────────────────────┐
│              UI Layer (Qt 6)            │  ← shows data, sends commands
├─────────────────────────────────────────┤
│         Logic Layer (GameEngine)        │  ← orchestrates everything
├──────────────────┬──────────────────────┤
│  Provider Layer  │   Scripting Layer    │  ← Steam/EGS  |  Python ML
├──────────────────┴──────────────────────┤
│            Data Layer (SQLite)          │  ← single source of truth
└─────────────────────────────────────────┘
```

Dependency rule: arrows go **downward only**. 
UI depends on Logic. Logic depends on Data. 
Data depends on nothing inside this project.

---

## C++ Layers

### Data Layer — `src/data/`
The only layer that speaks SQL. Nothing else in the project touches 
the database directly.

Key classes:
- `DBManager` — opens the database, runs migrations, provides CRUD methods
- `models/Game.h` — game data structure
- `DatabaseException.h` — typed exception for database errors

Rule: if you are writing a SQL query outside of `src/data/`, 
something is wrong.

### Provider Layer — `src/providers/`
Responsible for discovering installed games from external sources.

Key classes:
- `ILibraryProvider` — abstract interface, every provider implements this
- `SteamProvider` — parses Steam's `libraryfolders.vdf`
- `EGSProvider` — reads Epic Games Store JSON manifests

To add a new store: inherit from `ILibraryProvider`, implement 
`scanGames()`, register in `GameEngine`. Nothing else changes.

### Logic Layer — `src/core/`
The brain of the application. Manages game launches, session tracking, 
and coordinates between all other layers.

Key classes:
- `GameEngine` — launches games, tracks sessions, calls Python modules
- `ProcessWatcher` — uses WinAPI to detect when a game process starts/stops
- `ScriptingBridge` — the only place where C++ calls Python (via pybind11)

### UI Layer — `src/ui/`
Displays data and sends user commands to `GameEngine`. 
Contains no business logic.

Key components:
- `MainWindow` — game grid dashboard
- `GameCard` — single game card widget
- `DetailsView` — game details, playtime, play button
- `SettingsDialog` — store paths, user city for weather

---

## Python Layer — `scripts/` (Lisa's zone)

Python modules are called exclusively through `ScriptingBridge`. 
They **never** access the database directly — all data arrives 
as parameters from C++.

### Data flow
```
GameEngine → ScriptingBridge → Python module(data) → returns recommendations
```

C++ passes data as Python dicts/lists. Python computes and returns results. 
Python has zero knowledge of SQLite or file paths.

### Modules

#### `backlog_digger/`
Surfaces unplayed or rarely played games at the right moment.

Inputs (received from C++):
- full game list with playtime per game
- recent session history (last 30 days)
- current time and day of week

Logic:
- score each unplayed game based on: time since purchase, 
  genre match with recent sessions, estimated game length vs 
  available time
- return top N recommendations with score and reason string

Files:
- `recommender.py` — main scoring logic
- `metrics.py` — individual metric calculations

#### `weather_recommender/`
Adjusts recommendations based on weather and time context.

Inputs (received from C++):
- user city (from Settings)
- current game list with genres

External API: [open-meteo.com](https://open-meteo.com) — 
no API key required, free and open source.

Logic:
- fetch current weather for user city
- rainy/cold → prefer long immersive games
- sunny/warm → prefer short sessions or casual games
- friday evening → prefer any genre, longer sessions expected

Files:
- `client.py` — HTTP requests to open-meteo
- `signals.py` — weather condition → recommendation modifier

#### `mood_detector/`
Infers user mood from recent session patterns.

Inputs (received from C++):
- session history for last 14 days (start time, duration, game id)

Logic:
- short fragmented sessions → user is tired, recommend casual/short games
- long focused sessions → user is engaged, recommend deep/complex games
- no sessions for 3+ days → returning player, recommend something familiar

Files:
- `detector.py` — mood classification
- `patterns.py` — session pattern definitions

#### `shared/`
Shared code across all modules.

- `models.py` — Python dataclasses mirroring C++ structs 
  (`Game`, `Session`). Keep in sync with `src/data/models/`.

---

## Database Schema

Three tables. See `src/data/schema.sql` for full definition.
```
games          — game metadata, cover path, Steam playtime offset
game_sources   — where each game came from (steam / egs / manual)
sessions       — every launch: start time, end time, linked to game
```

Key rule: `PRAGMA foreign_keys = ON` is always enabled. 
Deleting a game cascades to its sources and sessions automatically.

---

## Project Structure
```
EchoGL/
├── CMakeLists.txt              # root build file
├── vcpkg.json                  # C++ dependencies
├── ARCHITECTURE.md             # this file
├── CONTRIBUTING.md             # how to contribute
│
├── cmake/                      # CMake helpers
├── assets/                     # QSS styles, icons
├── docs/                       # diagrams, notes
│
├── scripts/                    # Python modules
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
│   ├── data/                   # Data Layer
│   │   ├── DBManager.h/.cpp
│   │   ├── DatabaseException.h
│   │   ├── SQLiteHandles.h
│   │   ├── schema.sql
│   │   └── models/
│   │       └── Game.h
│   ├── providers/              # Provider Layer
│   │   ├── ILibraryProvider.h
│   │   ├── SteamProvider.h/.cpp
│   │   └── EGSProvider.h/.cpp
│   ├── core/                   # Logic Layer
│   │   ├── GameEngine.h/.cpp
│   │   ├── ProcessWatcher.h/.cpp
│   │   └── ScriptingBridge.h/.cpp
│   └── ui/                     # UI Layer
│       ├── MainWindow.h/.cpp
│       ├── widgets/
│       │   └── GameCard.h/.cpp
│       ├── views/
│       │   └── DetailsView.h/.cpp
│       └── dialogs/
│           └── SettingsDialog.h/.cpp
│
├── tests/
│   ├── CMakeLists.txt
│   ├── data/
│   │   └── test_DBManager.cpp
│   └── core/
│       └── test_GameEngine.cpp
│
└── third_party/
    ├── sqlite3/
    └── pybind11/
```

---

## For New Contributors

### If you work on C++ layers
1. Never write SQL outside `src/data/`
2. Never access `m_db` outside `DBManager`
3. Every new provider inherits `ILibraryProvider`
4. UI never calls `DBManager` directly — always through `GameEngine`

### If you work on Python modules
1. Your entry point is always a function called from `ScriptingBridge`
2. You never open the database — data arrives as parameters
3. Return types must match what `ScriptingBridge` expects — 
   coordinate with the C++ side before changing signatures
4. Each module has its own `requirements.txt` if it needs dependencies
5. Write tests in `scripts/<module>/tests/` using `pytest`

### Setting up development environment
```bash
# C++ side
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Python side
cd scripts
pip install -r requirements.txt
pytest
```

---

## Roadmap Integration

| Stage | What gets built |
|-------|----------------|
| 1 — Foundation | DBManager, schema, models |
| 2 — Providers | Steam/EGS integration, ProcessWatcher |
| 3 — Python bridge | ScriptingBridge, pybind11 setup |
| 3 — ML modules | backlog_digger, weather, mood |
| 4 — UI | Qt dashboard, game cards, details |
| 5 — Backend | Friend activity, cloud sync |