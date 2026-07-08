#pragma once

#include <QWidget>

#include "models/Message.h"

class QLabel;

class ChatMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatMessageWidget(const Message& message, QWidget* parent = nullptr);

signals:
    void copyRequested(const QString& text);
    void forwardRequested(const Message& message);
    void favoriteRequested(const Message& message);
    void deleteRequested(const Message& message);
    void recallRequested(const Message& message);
    void multiSelectRequested(const Message& message);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    Message m_message;
};
