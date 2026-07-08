#pragma once

#include <QObject>
#include <QVector>

#include "data/DatabaseManager.h"

// ChatViewModel 聚合业务状态，UI 只订阅信号并发起意图，降低耦合度。
class ChatViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ChatViewModel(QObject* parent = nullptr);

    bool initialize(const QString& databasePath);
    QVector<Conversation> conversations(const QString& keyword = QString()) const;
    QVector<Contact> contacts(const QString& keyword = QString()) const;
    QVector<Message> messages(const QString& conversationId, const QString& keyword = QString()) const;
    Conversation conversationById(const QString& conversationId) const;

    QString currentConversationId() const;
    void setCurrentConversationId(const QString& conversationId);

public slots:
    void sendText(const QString& text);
    void sendFile(const QString& filePath);
    void togglePinned(const QString& conversationId);
    void markUnread(const QString& conversationId);
    void hideConversation(const QString& conversationId);
    void clearConversation(const QString& conversationId);
    void simulateIncomingReply(const QString& conversationId);

signals:
    void conversationsChanged();
    void messagesChanged(const QString& conversationId);
    void currentConversationChanged(const Conversation& conversation);

private:
    Message buildOutgoingMessage(const QString& content, MessageType type) const;

    DatabaseManager m_database;
    QString m_currentConversationId;
};
