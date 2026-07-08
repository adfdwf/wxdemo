#include "widgets/ChatMessageWidget.h"

#include "widgets/AvatarWidget.h"

#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QSizePolicy>
#include <QVBoxLayout>

ChatMessageWidget::ChatMessageWidget(const Message& message, QWidget* parent)
    : QWidget(parent)
    , m_message(message)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    auto* avatar = new AvatarWidget(this);
    avatar->setName(message.senderName);
    avatar->setColor(message.avatarColor);

    auto* meta = new QLabel(QString("%1  %2").arg(message.senderName, message.timestamp.toString("HH:mm")), this);
    meta->setObjectName("MessageMeta");

    auto* bubble = new QFrame(this);
    bubble->setObjectName(message.outgoing ? "OutgoingBubble" : "IncomingBubble");
    bubble->setMaximumWidth(520);

    auto* text = new QLabel(bubble);
    text->setObjectName("MessageText");
    text->setTextInteractionFlags(Qt::TextSelectableByMouse);
    text->setWordWrap(true);
    text->setText(message.content.toHtmlEscaped().replace("\n", "<br/>"));

    auto* bubbleLayout = new QVBoxLayout(bubble);
    bubbleLayout->setContentsMargins(12, 9, 12, 9);
    bubbleLayout->addWidget(text);

    auto* contentLayout = new QVBoxLayout;
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(4);
    contentLayout->addWidget(meta, 0, message.outgoing ? Qt::AlignRight : Qt::AlignLeft);
    contentLayout->addWidget(bubble, 0, message.outgoing ? Qt::AlignRight : Qt::AlignLeft);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(18, 8, 18, 8);
    layout->setSpacing(10);
    if (message.outgoing) {
        layout->addStretch();
        layout->addLayout(contentLayout);
        layout->addWidget(avatar, 0, Qt::AlignTop);
    } else {
        layout->addWidget(avatar, 0, Qt::AlignTop);
        layout->addLayout(contentLayout);
        layout->addStretch();
    }
}

void ChatMessageWidget::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    QAction* copyAction = menu.addAction("复制");
    QAction* forwardAction = menu.addAction("转发");
    QAction* favoriteAction = menu.addAction("收藏");
    QAction* deleteAction = menu.addAction("删除");
    QAction* recallAction = nullptr;
    if (m_message.canRecall()) {
        recallAction = menu.addAction("撤回");
    }
    menu.addSeparator();
    QAction* multiAction = menu.addAction("多选");

    QAction* selected = menu.exec(event->globalPos());
    if (!selected) {
        return;
    }
    if (selected == copyAction) {
        QApplication::clipboard()->setText(m_message.content);
        emit copyRequested(m_message.content);
    } else if (selected == forwardAction) {
        emit forwardRequested(m_message);
    } else if (selected == favoriteAction) {
        emit favoriteRequested(m_message);
    } else if (selected == deleteAction) {
        emit deleteRequested(m_message);
    } else if (selected == recallAction) {
        emit recallRequested(m_message);
    } else if (selected == multiAction) {
        emit multiSelectRequested(m_message);
    }
}
