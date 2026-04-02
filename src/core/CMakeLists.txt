add_library(echogl_core STATIC
    GameEngine.cpp
    GameEngine.hpp
    ProcessWatcher.cpp
    ProcessWatcher.hpp
    ScriptingBridge.cpp
    ScriptingBridge.hpp
)

target_include_directories(echogl_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(echogl_core
    PUBLIC  echogl_data
            echogl_providers
    PRIVATE pybind11::embed
            Qt6::Core
)

if(WIN32)
    target_link_libraries(echogl_core PRIVATE
        psapi
        kernel32
    )
endif()