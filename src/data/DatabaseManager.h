#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QVector>

#include "models/Conversation.h"
#include "models/Contact.h"
#include "models/Message.h"

// DatabaseManager 负责本地 SQLite 持久化，避免 UI 代码直接拼 SQL。
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();

    bool initialize(const QString& databasePath);
    QVector<Conversation> conversations(const QString& keyword = QString()) const;
    QVector<Contact> contacts(const QString& keyword = QString()) const;
    QVector<Message> messagesForConversation(const QString& conversationId,
                                             const QString& keyword = QString()) const;

    bool addMessage(const Message& message);
    bool setConversationPinned(const QString& conversationId, bool pinned);
    bool setConversationUnread(const QString& conversationId, int unreadCount);
    bool hideConversation(const QString& conversationId, bool hidden);
    bool deleteConversationMessages(const QString& conversationId);

private:
    bool createSchema();
    bool seedIfEmpty();
    bool execSql(const QString& sql) const;

    QSqlDatabase m_db;
};
