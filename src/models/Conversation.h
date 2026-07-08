#pragma once

#include <QDateTime>
#include <QString>

struct Conversation {
    QString id;
    QString title;
    QString avatarColor;
    QString lastMessage;
    QDateTime lastTime;
    int unreadCount = 0;
    bool pinned = false;
    bool muted = false;
    bool hidden = false;
    bool group = false;
};
