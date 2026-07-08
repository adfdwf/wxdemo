#pragma once

#include <QFrame>

class QLabel;
class QPushButton;

enum class NavigationPage {
    Chats = 0,
    Contacts,
    Favorites
};

class NavigationBar : public QFrame
{
    Q_OBJECT
public:
    explicit NavigationBar(QWidget* parent = nullptr);
    void setUnreadCount(int count);

signals:
    void profileRequested();
    void pageRequested(NavigationPage page);
    void settingsRequested();

private:
    QPushButton* makeButton(const QString& text, const QString& tooltip);
    void setActive(QPushButton* button);

    QPushButton* m_chatButton = nullptr;
    QPushButton* m_contactButton = nullptr;
    QPushButton* m_favoriteButton = nullptr;
    QLabel* m_badge = nullptr;
};
