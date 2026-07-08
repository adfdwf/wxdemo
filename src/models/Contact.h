#pragma once

#include <QString>

struct Contact {
    QString id;
    QString name;
    QString pinyin;
    QString groupName;
    QString avatarColor;
    QString status;
    bool isGroup = false;
};
