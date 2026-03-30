add_library(echogl_providers STATIC
    ILibraryProvider.hpp
    SteamProvider.cpp
    SteamProvider.hpp
    EGSProvider.cpp
    EGSProvider.hpp
)

target_include_directories(echogl_providers
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(echogl_providers
    PUBLIC  echogl_data
    PRIVATE Qt6::Core
)