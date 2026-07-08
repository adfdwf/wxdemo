QT += widgets sql

CONFIG += c++17
TEMPLATE = app
TARGET = wxdemo

DEFINES += QT_DEPRECATED_WARNINGS

win32-msvc*: QMAKE_CXXFLAGS += /utf-8
win32-g++: QMAKE_CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/main.cpp \
    src/models/Message.cpp \
    src/data/DatabaseManager.cpp \
    src/viewmodels/ChatViewModel.cpp \
    src/widgets/AvatarWidget.cpp \
    src/widgets/TitleBar.cpp \
    src/widgets/NavigationBar.cpp \
    src/widgets/ConversationItemWidget.cpp \
    src/widgets/ChatMessageWidget.cpp \
    src/widgets/ChatInputWidget.cpp \
    src/widgets/ProfileDialog.cpp \
    src/widgets/SettingsDialog.cpp \
    src/widgets/ChatWindow.cpp

HEADERS += \
    src/models/Message.h \
    src/models/Conversation.h \
    src/models/Contact.h \
    src/data/DatabaseManager.h \
    src/viewmodels/ChatViewModel.h \
    src/widgets/AvatarWidget.h \
    src/widgets/TitleBar.h \
    src/widgets/NavigationBar.h \
    src/widgets/ConversationItemWidget.h \
    src/widgets/ChatMessageWidget.h \
    src/widgets/ChatInputWidget.h \
    src/widgets/ProfileDialog.h \
    src/widgets/SettingsDialog.h \
    src/widgets/ChatWindow.h

RESOURCES += resources/resources.qrc
