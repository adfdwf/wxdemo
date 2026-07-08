#include "widgets/ConversationItemWidget.h"

#include "widgets/AvatarWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

ConversationItemWidget::ConversationItemWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(72);

    m_avatar = new AvatarWidget(this);
    m_title = new QLabel(this);
    m_preview = new QLabel(this);
    m_time = new QLabel(this);
    m_unread = new QLabel(this);

    m_title->setStyleSheet("font-size:14px;font-weight:600;color:#111;");
    m_preview->setStyleSheet("font-size:12px;color:#8a8a8a;");
    m_time->setStyleSheet("font-size:11px;color:#a0a0a0;");
    m_unread->setObjectName("BadgeLabel");
    m_unread->setAlignment(Qt::AlignCenter);
    m_preview->setTextFormat(Qt::PlainText);
    m_preview->setMaximumWidth(165);

    auto* textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(7);
    textLayout->addWidget(m_title);
    textLayout->addWidget(m_preview);

    auto* rightLayout = new QVBoxLayout;
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(8);
    rightLayout->addWidget(m_time, 0, Qt::AlignRight);
    rightLayout->addWidget(m_unread, 0, Qt::AlignRight);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 10, 10, 10);
    layout->setSpacing(10);
    layout->addWidget(m_avatar);
    layout->addLayout(textLayout, 1);
    layout->addLayout(rightLayout);
}

void ConversationItemWidget::setConversation(const Conversation& conversation)
{
    m_conversationId = conversation.id;
    m_avatar->setName(conversation.title);
    m_avatar->setColor(conversation.avatarColor);
    m_title->setText(conversation.pinned ? "📌 " + conversation.title : conversation.title);
    m_preview->setText(conversation.lastMessage);
    m_time->setText(conversation.lastTime.time().toString("HH:mm"));
    m_unread->setVisible(conversation.unreadCount > 0);
    m_unread->setText(conversation.unreadCount > 99 ? "99+" : QString::number(conversation.unreadCount));
    m_unread->adjustSize();
}

QString ConversationItemWidget::conversationId() const
{
    return m_conversationId;
}
