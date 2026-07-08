#pragma once

#include <QWidget>

#include "models/Conversation.h"
#include "widgets/NavigationBar.h"
#include "viewmodels/ChatViewModel.h"

class ChatInputWidget;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QScrollArea;
class QStackedWidget;
class QVBoxLayout;

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget* parent = nullptr);

private:
    QWidget* buildMiddlePane();
    QWidget* buildConversationPage();
    QWidget* buildContactPage();
    QWidget* buildFavoritePage();
    QWidget* buildChatPane();
    QWidget* buildInfoPanel();

    void loadStyleSheet();
    void reloadConversations();
    void reloadContacts();
    void reloadMessages(const QString& conversationId);
    void setConversationHeader(const Conversation& conversation);
    void showConversationMenu(const QPoint& pos);
    void showQuickAddMenu();
    void showHistoryDialog();
    void updateUnreadBadge();
    void smoothScrollToBottom();

    ChatViewModel m_viewModel;
    NavigationBar* m_navigation = nullptr;
    QStackedWidget* m_middleStack = nullptr;
    QLineEdit* m_searchEdit = nullptr;
    QListWidget* m_conversationList = nullptr;
    QListWidget* m_contactList = nullptr;
    QListWidget* m_favoriteList = nullptr;
    QLabel* m_chatTitle = nullptr;
    QWidget* m_messageViewport = nullptr;
    QVBoxLayout* m_messageLayout = nullptr;
    QScrollArea* m_messageArea = nullptr;
    ChatInputWidget* m_input = nullptr;
    QWidget* m_infoPanel = nullptr;
};
