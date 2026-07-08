#include "models/Message.h"

bool Message::canRecall(const QDateTime& now) const
{
    // 微信撤回规则近似：仅允许撤回自己 2 分钟内发送的消息。
    return outgoing && timestamp.secsTo(now) <= 120;
}

QString messageTypeToString(MessageType type)
{
    switch (type) {
    case MessageType::Image: return "image";
    case MessageType::File: return "file";
    case MessageType::System: return "system";
    case MessageType::Text:
    default: return "text";
    }
}

MessageType messageTypeFromString(const QString& value)
{
    if (value == "image") return MessageType::Image;
    if (value == "file") return MessageType::File;
    if (value == "system") return MessageType::System;
    return MessageType::Text;
}

QString messageStatusToString(MessageStatus status)
{
    switch (status) {
    case MessageStatus::Sending: return "sending";
    case MessageStatus::Read: return "read";
    case MessageStatus::Failed: return "failed";
    case MessageStatus::Sent:
    default: return "sent";
    }
}

MessageStatus messageStatusFromString(const QString& value)
{
    if (value == "sending") return MessageStatus::Sending;
    if (value == "read") return MessageStatus::Read;
    if (value == "failed") return MessageStatus::Failed;
    return MessageStatus::Sent;
}
