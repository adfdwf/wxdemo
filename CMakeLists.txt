cmake_minimum_required(VERSION 3.21)

project(wxdemo
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
qt_standard_project_setup()

qt_add_executable(wxdemo MANUAL_FINALIZATION WIN32
    src/main.cpp

    src/models/Message.h
    src/models/Message.cpp
    src/models/Conversation.h
    src/models/Contact.h

    src/data/DatabaseManager.h
    src/data/DatabaseManager.cpp

    src/viewmodels/ChatViewModel.h
    src/viewmodels/ChatViewModel.cpp

    src/widgets/AvatarWidget.h
    src/widgets/AvatarWidget.cpp
    src/widgets/TitleBar.h
    src/widgets/TitleBar.cpp
    src/widgets/NavigationBar.h
    src/widgets/NavigationBar.cpp
    src/widgets/ConversationItemWidget.h
    src/widgets/ConversationItemWidget.cpp
    src/widgets/ChatMessageWidget.h
    src/widgets/ChatMessageWidget.cpp
    src/widgets/ChatInputWidget.h
    src/widgets/ChatInputWidget.cpp
    src/widgets/ProfileDialog.h
    src/widgets/ProfileDialog.cpp
    src/widgets/SettingsDialog.h
    src/widgets/SettingsDialog.cpp
    src/widgets/ChatWindow.h
    src/widgets/ChatWindow.cpp

    resources/resources.qrc
)

target_include_directories(wxdemo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(wxdemo PRIVATE
    Qt6::Widgets
    Qt6::Sql
)

if(MSVC)
    target_compile_options(wxdemo PRIVATE /utf-8 /W4 /permissive-)
else()
    target_compile_options(wxdemo PRIVATE -Wall -Wextra)
endif()

set_target_properties(wxdemo PROPERTIES
    OUTPUT_NAME wxdemo
)

qt_finalize_executable(wxdemo)
