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
    PRIVATE
        ${CMAKE_SOURCE_DIR}/third_party/sqlite3/src
)

target_link_libraries(echogl_data
    PUBLIC sqlite3
    PRIVATE Qt6::Core
)