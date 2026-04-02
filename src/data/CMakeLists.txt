add_library(echogl_data STATIC
    DBManager.cpp
    DBManager.hpp
    models/Game.hpp
    models/Platform.hpp
    models/Session.hpp
)

target_include_directories(echogl_data
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(echogl_data
    PUBLIC sqlite3
    PRIVATE Qt6::Core
)