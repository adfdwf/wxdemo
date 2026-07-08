#pragma once

#include <QFrame>
#include <QTextEdit>

class QPushButton;

class MessageEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit MessageEditor(QWidget* parent = nullptr);

signals:
    void submitRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

class ChatInputWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ChatInputWidget(QWidget* parent = nullptr);

signals:
    void sendTextRequested(const QString& text);
    void sendFileRequested(const QString& filePath);
    void historyRequested();
    void screenshotRequested();

private:
    QPushButton* createToolButton(const QString& text, const QString& tooltip);
    void openEmojiPopup();
    void updateSendState();
    void sendCurrentText();

    MessageEditor* m_editor = nullptr;
    QPushButton* m_sendButton = nullptr;
};
