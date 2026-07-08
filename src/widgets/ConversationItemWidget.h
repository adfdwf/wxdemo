#pragma once

#include <QWidget>

#include "models/Conversation.h"

class AvatarWidget;
class QLabel;

class ConversationItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConversationItemWidget(QWidget* parent = nullptr);
    void setConversation(const Conversation& conversation);
    QString conversationId() const;

private:
    AvatarWidget* m_avatar = nullptr;
    QLabel* m_title = nullptr;
    QLabel* m_preview = nullptr;
    QLabel* m_time = nullptr;
    QLabel* m_unread = nullptr;
    QString m_conversationId;
};
