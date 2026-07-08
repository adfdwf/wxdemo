#pragma once

#include <QDateTime>
#include <QString>

// 消息类型：后续可以继续扩展语音、视频通话、位置等业务类型。
enum class MessageType {
    Text = 0,
    Image,
    File,
    System
};

// 消息状态：用于 UI 中展示发送中、已发送、已读或失败等状态。
enum class MessageStatus {
    Sending = 0,
    Sent,
    Read,
    Failed
};

struct Message {
    QString id;
    QString conversationId;
    QString senderId;
    QString senderName;
    QString avatarColor;
    QString content;
    MessageType type = MessageType::Text;
    QDateTime timestamp;
    MessageStatus status = MessageStatus::Sent;
    bool outgoing = false;

    bool canRecall(const QDateTime& now = QDateTime::currentDateTime()) const;
};

QString messageTypeToString(MessageType type);
MessageType messageTypeFromString(const QString& value);
QString messageStatusToString(MessageStatus status);
MessageStatus messageStatusFromString(const QString& value);
