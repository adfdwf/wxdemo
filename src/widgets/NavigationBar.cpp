#include "widgets/NavigationBar.h"

#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

NavigationBar::NavigationBar(QWidget* parent)
    : QFrame(parent)
{
    setObjectName("NavigationBar");
    setFixedWidth(64);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 14, 10, 14);
    layout->setSpacing(12);

    auto* avatarButton = makeButton("我", "个人资料");
    avatarButton->setObjectName("AvatarButton");
    layout->addWidget(avatarButton);
    layout->addSpacing(18);

    m_chatButton = makeButton("💬", "聊天");
    m_contactButton = makeButton("👥", "通讯录");
    m_favoriteButton = makeButton("☆", "收藏");

    auto* chatWrap = new QWidget(this);
    chatWrap->setFixedSize(44, 44);
    auto* wrapLayout = new QVBoxLayout(chatWrap);
    wrapLayout->setContentsMargins(0, 0, 0, 0);
    wrapLayout->addWidget(m_chatButton);
    m_badge = new QLabel(chatWrap);
    m_badge->setObjectName("BadgeLabel");
    m_badge->setAlignment(Qt::AlignCenter);
    m_badge->move(27, 0);
    m_badge->hide();

    layout->addWidget(chatWrap);
    layout->addWidget(m_contactButton);
    layout->addWidget(m_favoriteButton);
    layout->addStretch();

    auto* settingsButton = makeButton("⚙", "设置");
    layout->addWidget(settingsButton);

    connect(avatarButton, &QPushButton::clicked, this, &NavigationBar::profileRequested);
    connect(m_chatButton, &QPushButton::clicked, this, [this]() {
        setActive(m_chatButton);
        emit pageRequested(NavigationPage::Chats);
    });
    connect(m_contactButton, &QPushButton::clicked, this, [this]() {
        setActive(m_contactButton);
        emit pageRequested(NavigationPage::Contacts);
    });
    connect(m_favoriteButton, &QPushButton::clicked, this, [this]() {
        setActive(m_favoriteButton);
        emit pageRequested(NavigationPage::Favorites);
    });
    connect(settingsButton, &QPushButton::clicked, this, &NavigationBar::settingsRequested);
    setActive(m_chatButton);
}

void NavigationBar::setUnreadCount(int count)
{
    m_badge->setVisible(count > 0);
    m_badge->setText(count > 99 ? "99+" : QString::number(count));
    m_badge->adjustSize();
}

QPushButton* NavigationBar::makeButton(const QString& text, const QString& tooltip)
{
    auto* button = new QPushButton(text, this);
    button->setObjectName("NavButton");
    button->setToolTip(tooltip);
    button->setFixedSize(44, 44);
    button->setCursor(Qt::PointingHandCursor);
    button->setFocusPolicy(Qt::NoFocus);
    return button;
}

void NavigationBar::setActive(QPushButton* button)
{
    for (QPushButton* item : {m_chatButton, m_contactButton, m_favoriteButton}) {
        if (!item) {
            continue;
        }
        item->setProperty("active", item == button);
        item->style()->unpolish(item);
        item->style()->polish(item);
    }
}
