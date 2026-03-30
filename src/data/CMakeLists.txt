add_library(echogl_data STATIC
    DBManager.cpp
    DBManager.h
    models/Game.h
    models/Platform.h
    models/Session.h
)

target_include_directories(echogl_data
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(echogl_data
    PUBLIC sqlite3
    PRIVATE Qt6::Core
)