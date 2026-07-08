#include "viewmodels/ChatViewModel.h"

#include <QFileInfo>
#include <QRandomGenerator>
#include <QTimer>
#include <QUuid>

ChatViewModel::ChatViewModel(QObject* parent)
    : QObject(parent)
{
}

bool ChatViewModel::initialize(const QString& databasePath)
{
    if (!m_database.initialize(databasePath)) {
        return false;
    }
    const auto list = conversations();
    if (!list.isEmpty()) {
        setCurrentConversationId(list.first().id);
    }
    return true;
}

QVector<Conversation> ChatViewModel::conversations(const QString& keyword) const
{
    return m_database.conversations(keyword);
}

QVector<Contact> ChatViewModel::contacts(const QString& keyword) const
{
    return m_database.contacts(keyword);
}

QVector<Message> ChatViewModel::messages(const QString& conversationId, const QString& keyword) const
{
    return m_database.messagesForConversation(conversationId, keyword);
}

Conversation ChatViewModel::conversationById(const QString& conversationId) const
{
    for (const Conversation& c : conversations()) {
        if (c.id == conversationId) {
            return c;
        }
    }
    return Conversation{};
}

QString ChatViewModel::currentConversationId() const
{
    return m_currentConversationId;
}

void ChatViewModel::setCurrentConversationId(const QString& conversationId)
{
    if (conversationId.isEmpty()) {
        return;
    }
    m_currentConversationId = conversationId;
    m_database.setConversationUnread(conversationId, 0);
    emit conversationsChanged();
    emit currentConversationChanged(conversationById(conversationId));
    emit messagesChanged(conversationId);
}

void ChatViewModel::sendText(const QString& text)
{
    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty() || m_currentConversationId.isEmpty()) {
        return;
    }

    const Message message = buildOutgoingMessage(trimmed, MessageType::Text);
    if (m_database.addMessage(message)) {
        emit conversationsChanged();
        emit messagesChanged(m_currentConversationId);
        QTimer::singleShot(900, this, [this, id = m_currentConversationId]() { simulateIncomingReply(id); });
    }
}

void ChatViewModel::sendFile(const QString& filePath)
{
    if (filePath.isEmpty() || m_currentConversationId.isEmpty()) {
        return;
    }

    QFileInfo info(filePath);
    const QString suffix = info.suffix().toLower();
    const MessageType type = QStringList({"png", "jpg", "jpeg", "gif", "bmp"}).contains(suffix)
                                 ? MessageType::Image
                                 : MessageType::File;
    const Message message = buildOutgoingMessage(QStringLiteral("[文件] %1").arg(info.fileName()), type);
    if (m_database.addMessage(message)) {
        emit conversationsChanged();
        emit messagesChanged(m_currentConversationId);
    }
}

void ChatViewModel::togglePinned(const QString& conversationId)
{
    const Conversation c = conversationById(conversationId);
    if (m_database.setConversationPinned(conversationId, !c.pinned)) {
        emit conversationsChanged();
    }
}

void ChatViewModel::markUnread(const QString& conversationId)
{
    if (m_database.setConversationUnread(conversationId, 1)) {
        emit conversationsChanged();
    }
}

void ChatViewModel::hideConversation(const QString& conversationId)
{
    if (m_database.hideConversation(conversationId, true)) {
        emit conversationsChanged();
    }
}

void ChatViewModel::clearConversation(const QString& conversationId)
{
    if (m_database.deleteConversationMessages(conversationId)) {
        emit conversationsChanged();
        emit messagesChanged(conversationId);
    }
}

void ChatViewModel::simulateIncomingReply(const QString& conversationId)
{
    if (conversationId.isEmpty()) {
        return;
    }

    const QStringList replies = {
        "好的，我看到了。",
        "这个方案挺顺滑的，继续推进吧。",
        "晚点我把补充材料发你。",
        "收到，辛苦了！"
    };
    Message reply;
    reply.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    reply.conversationId = conversationId;
    reply.senderId = "robot";
    reply.senderName = conversationById(conversationId).title;
    reply.avatarColor = "#5b8def";
    reply.content = replies.at(QRandomGenerator::global()->bounded(replies.size()));
    reply.type = MessageType::Text;
    reply.timestamp = QDateTime::currentDateTime();
    reply.status = MessageStatus::Sent;
    reply.outgoing = false;

    if (m_database.addMessage(reply)) {
        emit conversationsChanged();
        emit messagesChanged(conversationId);
    }
}

Message ChatViewModel::buildOutgoingMessage(const QString& content, MessageType type) const
{
    Message message;
    message.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    message.conversationId = m_currentConversationId;
    message.senderId = "me";
    message.senderName = "我";
    message.avatarColor = "#07c160";
    message.content = content;
    message.type = type;
    message.timestamp = QDateTime::currentDateTime();
    message.status = MessageStatus::Sent;
    message.outgoing = true;
    return message;
}
