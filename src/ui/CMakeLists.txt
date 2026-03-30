qt_add_library(echogl_ui STATIC
    MainWindow.cpp
    MainWindow.h
    widgets/GameCard.cpp
    widgets/GameCard.h
    views/DetailsView.cpp
    views/DetailsView.h
    dialogs/SettingsDialog.cpp
    dialogs/SettingsDialog.h
)

qt_add_resources(echogl_ui "assets"
    PREFIX "/assets"
    FILES
        ../../assets/style.qss
        ../../assets/icons/play.svg
)

qt_add_resources(echogl_ui "sql"
    PREFIX "/sql"
    FILES
        ../data/schema.sql
)

target_include_directories(echogl_ui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(echogl_ui
    PUBLIC  echogl_core
            Qt6::Widgets
)