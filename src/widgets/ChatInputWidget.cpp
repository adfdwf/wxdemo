#include "widgets/ChatInputWidget.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMenu>
#include <QProcess>
#include <QPushButton>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidgetAction>

MessageEditor::MessageEditor(QWidget* parent)
    : QTextEdit(parent)
{
    setObjectName("MessageEditor");
    setPlaceholderText("输入消息，Enter 发送，Shift+Enter 换行");
    setAcceptRichText(false);
}

void MessageEditor::keyPressEvent(QKeyEvent* event)
{
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        && !(event->modifiers() & Qt::ShiftModifier)) {
        emit submitRequested();
        event->accept();
        return;
    }
    QTextEdit::keyPressEvent(event);
}

ChatInputWidget::ChatInputWidget(QWidget* parent)
    : QFrame(parent)
{
    setObjectName("InputPanel");
    setMinimumHeight(174);
    setMaximumHeight(230);

    auto* emojiButton = createToolButton("☺", "表情");
    auto* fileButton = createToolButton("📎", "发送文件");
    auto* screenshotButton = createToolButton("✂", "截图");
    auto* historyButton = createToolButton("🕘", "聊天记录");

    auto* toolLayout = new QHBoxLayout;
    toolLayout->setContentsMargins(14, 8, 14, 0);
    toolLayout->setSpacing(8);
    toolLayout->addWidget(emojiButton);
    toolLayout->addWidget(fileButton);
    toolLayout->addWidget(screenshotButton);
    toolLayout->addWidget(historyButton);
    toolLayout->addStretch();

    m_editor = new MessageEditor(this);
    m_sendButton = new QPushButton("发送", this);
    m_sendButton->setObjectName("SendButton");
    m_sendButton->setCursor(Qt::PointingHandCursor);
    m_sendButton->setEnabled(false);

    auto* bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(14, 2, 18, 12);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_sendButton);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addLayout(toolLayout);
    layout->addWidget(m_editor, 1);
    layout->addLayout(bottomLayout);

    connect(emojiButton, &QPushButton::clicked, this, &ChatInputWidget::openEmojiPopup);
    connect(fileButton, &QPushButton::clicked, this, [this]() {
        const QString file = QFileDialog::getOpenFileName(this, "选择要发送的文件");
        if (!file.isEmpty()) {
            emit sendFileRequested(file);
        }
    });
    connect(screenshotButton, &QPushButton::clicked, this, [this]() {
        emit screenshotRequested();
        QProcess::startDetached("explorer.exe", QStringList() << "ms-screenclip:");
    });
    connect(historyButton, &QPushButton::clicked, this, &ChatInputWidget::historyRequested);
    connect(m_editor, &QTextEdit::textChanged, this, &ChatInputWidget::updateSendState);
    connect(m_editor, &MessageEditor::submitRequested, this, &ChatInputWidget::sendCurrentText);
    connect(m_sendButton, &QPushButton::clicked, this, &ChatInputWidget::sendCurrentText);
}

QPushButton* ChatInputWidget::createToolButton(const QString& text, const QString& tooltip)
{
    auto* button = new QPushButton(text, this);
    button->setObjectName("ToolButton");
    button->setToolTip(tooltip);
    button->setFixedSize(32, 32);
    button->setCursor(Qt::PointingHandCursor);
    button->setFocusPolicy(Qt::NoFocus);
    return button;
}

void ChatInputWidget::openEmojiPopup()
{
    auto* menu = new QMenu(this);
    auto* container = new QWidget(menu);
    auto* grid = new QGridLayout(container);
    grid->setContentsMargins(8, 8, 8, 8);
    grid->setSpacing(4);

    const QStringList emojis = {"😀", "😂", "😍", "👍", "🙏", "🎉", "😎", "🤔", "😭", "😴", "🔥", "❤️"};
    for (int i = 0; i < emojis.size(); ++i) {
        auto* button = new QPushButton(emojis.at(i), container);
        button->setFixedSize(34, 34);
        button->setCursor(Qt::PointingHandCursor);
        grid->addWidget(button, i / 6, i % 6);
        connect(button, &QPushButton::clicked, this, [this, menu, emoji = emojis.at(i)]() {
            m_editor->insertPlainText(emoji);
            menu->close();
        });
    }
    auto* action = new QWidgetAction(menu);
    action->setDefaultWidget(container);
    menu->addAction(action);
    menu->exec(mapToGlobal(QPoint(12, -container->sizeHint().height())));
}

void ChatInputWidget::updateSendState()
{
    m_sendButton->setEnabled(!m_editor->toPlainText().trimmed().isEmpty());
}

void ChatInputWidget::sendCurrentText()
{
    const QString text = m_editor->toPlainText();
    if (text.trimmed().isEmpty()) {
        return;
    }
    emit sendTextRequested(text);
    m_editor->clear();
    updateSendState();
}
