#include "widgets/ChatWindow.h"

#include "widgets/ChatInputWidget.h"
#include "widgets/ChatMessageWidget.h"
#include "widgets/ConversationItemWidget.h"
#include "widgets/ProfileDialog.h"
#include "widgets/SettingsDialog.h"
#include "widgets/TitleBar.h"

#include <QApplication>
#include <QAbstractAnimation>
#include <QAbstractItemView>
#include <QCoreApplication>
#include <QCursor>
#include <QFile>
#include <QFrame>
#include <QHBoxLayout>
#include <QIODevice>
#include <QLabel>
#include <QLayoutItem>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QEasingCurve>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QTimer>
#include <QVBoxLayout>

ChatWindow::ChatWindow(QWidget* parent)
    : QWidget(parent)
{
    setObjectName("RootWindow");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setMinimumSize(980, 680);
    resize(1180, 760);
    loadStyleSheet();

    auto* titleBar = new TitleBar(this);
    titleBar->setTitle("微信 - Qt 高仿桌面端");

    m_navigation = new NavigationBar(this);
    m_middleStack = new QStackedWidget(this);
    m_middleStack->addWidget(buildConversationPage());
    m_middleStack->addWidget(buildContactPage());
    m_middleStack->addWidget(buildFavoritePage());

    auto* middlePane = new QFrame(this);
    middlePane->setObjectName("MiddlePane");
    middlePane->setMinimumWidth(250);
    middlePane->setMaximumWidth(330);
    auto* middleLayout = new QVBoxLayout(middlePane);
    middleLayout->setContentsMargins(0, 0, 0, 0);
    middleLayout->addWidget(m_middleStack);

    auto* content = new QHBoxLayout;
    content->setContentsMargins(0, 0, 0, 0);
    content->setSpacing(0);
    content->addWidget(m_navigation);
    content->addWidget(middlePane);
    content->addWidget(buildChatPane(), 1);

    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayout->addWidget(titleBar);
    rootLayout->addLayout(content, 1);

    const QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/wechat_demo.db";
    m_viewModel.initialize(dbPath);

    connect(titleBar, &TitleBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(titleBar, &TitleBar::maximizeRestoreRequested, this, [this]() {
        isMaximized() ? showNormal() : showMaximized();
    });
    connect(titleBar, &TitleBar::closeRequested, this, &QWidget::close);
    connect(m_navigation, &NavigationBar::profileRequested, this, [this]() { ProfileDialog(this).exec(); });
    connect(m_navigation, &NavigationBar::settingsRequested, this, [this]() { SettingsDialog(this).exec(); });
    connect(m_navigation, &NavigationBar::pageRequested, this, [this](NavigationPage page) {
        m_middleStack->setCurrentIndex(static_cast<int>(page));
        if (page == NavigationPage::Contacts) {
            reloadContacts();
        }
    });
    connect(m_searchEdit, &QLineEdit::textChanged, this, [this]() {
        reloadConversations();
        reloadContacts();
    });
    connect(&m_viewModel, &ChatViewModel::conversationsChanged, this, &ChatWindow::reloadConversations);
    connect(&m_viewModel, &ChatViewModel::messagesChanged, this, &ChatWindow::reloadMessages);
    connect(&m_viewModel, &ChatViewModel::currentConversationChanged, this, &ChatWindow::setConversationHeader);
    connect(m_input, &ChatInputWidget::sendTextRequested, &m_viewModel, &ChatViewModel::sendText);
    connect(m_input, &ChatInputWidget::sendFileRequested, &m_viewModel, &ChatViewModel::sendFile);
    connect(m_input, &ChatInputWidget::historyRequested, this, &ChatWindow::showHistoryDialog);
    connect(m_input, &ChatInputWidget::screenshotRequested, this, [this]() {
        QMessageBox::information(this, "截图", "已调用 Windows 截图入口；截图后可粘贴到输入框。");
    });

    reloadConversations();
    reloadContacts();
    setConversationHeader(m_viewModel.conversationById(m_viewModel.currentConversationId()));
    reloadMessages(m_viewModel.currentConversationId());
}

QWidget* ChatWindow::buildConversationPage()
{
    auto* page = new QWidget(this);
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(10, 12, 10, 10);
    layout->setSpacing(10);

    auto* top = new QHBoxLayout;
    m_searchEdit = new QLineEdit(page);
    m_searchEdit->setObjectName("SearchEdit");
    m_searchEdit->setPlaceholderText("搜索");
    auto* addButton = new QPushButton("+", page);
    addButton->setObjectName("RoundButton");
    addButton->setFixedSize(34, 34);
    addButton->setCursor(Qt::PointingHandCursor);
    top->addWidget(m_searchEdit, 1);
    top->addWidget(addButton);

    m_conversationList = new QListWidget(page);
    m_conversationList->setObjectName("ConversationList");
    m_conversationList->setContextMenuPolicy(Qt::CustomContextMenu);
    m_conversationList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_conversationList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout->addLayout(top);
    layout->addWidget(m_conversationList, 1);

    connect(addButton, &QPushButton::clicked, this, &ChatWindow::showQuickAddMenu);
    connect(m_conversationList, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        m_viewModel.setCurrentConversationId(item->data(Qt::UserRole).toString());
    });
    connect(m_conversationList, &QListWidget::customContextMenuRequested, this, &ChatWindow::showConversationMenu);
    return page;
}

QWidget* ChatWindow::buildContactPage()
{
    auto* page = new QWidget(this);
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(10, 12, 10, 10);
    auto* title = new QLabel("通讯录", page);
    title->setObjectName("SectionTitle");
    m_contactList = new QListWidget(page);
    m_contactList->setObjectName("ContactList");
    m_contactList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    layout->addWidget(title);
    layout->addWidget(m_contactList, 1);
    return page;
}

QWidget* ChatWindow::buildFavoritePage()
{
    auto* page = new QWidget(this);
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(10, 12, 10, 10);
    auto* title = new QLabel("收藏", page);
    title->setObjectName("SectionTitle");
    m_favoriteList = new QListWidget(page);
    m_favoriteList->setObjectName("FavoriteList");
    const QStringList favorites = {
        "文本：桌面端三栏式布局规范",
        "图片：聊天气泡视觉参考",
        "链接：https://weixin.qq.com/",
        "文件：Qt_Widget_MVVM_Design.pdf"
    };
    for (const QString& item : favorites) {
        m_favoriteList->addItem(item);
    }
    layout->addWidget(title);
    layout->addWidget(m_favoriteList, 1);
    return page;
}

QWidget* ChatWindow::buildChatPane()
{
    auto* pane = new QWidget(this);
    auto* paneLayout = new QHBoxLayout(pane);
    paneLayout->setContentsMargins(0, 0, 0, 0);
    paneLayout->setSpacing(0);

    auto* main = new QWidget(pane);
    auto* layout = new QVBoxLayout(main);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* header = new QFrame(main);
    header->setObjectName("ChatHeader");
    header->setFixedHeight(64);
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(22, 0, 16, 0);
    m_chatTitle = new QLabel("未选择会话", header);
    m_chatTitle->setObjectName("ChatTitle");
    auto* infoButton = new QPushButton("⋯", header);
    infoButton->setObjectName("HeaderButton");
    infoButton->setFixedSize(36, 36);
    infoButton->setCursor(Qt::PointingHandCursor);
    headerLayout->addWidget(m_chatTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(infoButton);

    m_messageArea = new QScrollArea(main);
    m_messageArea->setObjectName("MessageArea");
    m_messageArea->setWidgetResizable(true);
    m_messageArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScroller::grabGesture(m_messageArea->viewport(), QScroller::TouchGesture);

    m_messageViewport = new QWidget(m_messageArea);
    m_messageViewport->setObjectName("MessageViewport");
    m_messageLayout = new QVBoxLayout(m_messageViewport);
    m_messageLayout->setContentsMargins(0, 16, 0, 16);
    m_messageLayout->setSpacing(2);
    m_messageLayout->addStretch();
    m_messageArea->setWidget(m_messageViewport);

    m_input = new ChatInputWidget(main);
    layout->addWidget(header);
    layout->addWidget(m_messageArea, 1);
    layout->addWidget(m_input);

    m_infoPanel = buildInfoPanel();
    m_infoPanel->hide();
    paneLayout->addWidget(main, 1);
    paneLayout->addWidget(m_infoPanel);

    connect(infoButton, &QPushButton::clicked, this, [this]() {
        m_infoPanel->setVisible(!m_infoPanel->isVisible());
    });
    return pane;
}

QWidget* ChatWindow::buildInfoPanel()
{
    auto* panel = new QFrame(this);
    panel->setObjectName("InfoPanel");
    panel->setFixedWidth(280);
    auto* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(14);

    auto* title = new QLabel("聊天信息", panel);
    title->setObjectName("ChatTitle");
    layout->addWidget(title);
    layout->addWidget(new QLabel("群成员 / 好友资料", panel));
    layout->addWidget(new QLabel("查找聊天记录", panel));
    layout->addWidget(new QLabel("消息免打扰：已关闭", panel));
    auto* clear = new QPushButton("清空聊天记录", panel);
    clear->setObjectName("SecondaryButton");
    layout->addWidget(clear);
    layout->addStretch();

    connect(clear, &QPushButton::clicked, this, [this]() {
        m_viewModel.clearConversation(m_viewModel.currentConversationId());
    });
    return panel;
}

void ChatWindow::loadStyleSheet()
{
    QFile file(":/styles/wechat.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(file.readAll()));
    }
}

void ChatWindow::reloadConversations()
{
    const QString selected = m_viewModel.currentConversationId();
    m_conversationList->clear();
    for (const Conversation& c : m_viewModel.conversations(m_searchEdit->text())) {
        auto* item = new QListWidgetItem(m_conversationList);
        item->setSizeHint(QSize(260, 72));
        item->setData(Qt::UserRole, c.id);
        auto* widget = new ConversationItemWidget(m_conversationList);
        widget->setConversation(c);
        m_conversationList->addItem(item);
        m_conversationList->setItemWidget(item, widget);
        if (c.id == selected) {
            item->setSelected(true);
        }
    }
    updateUnreadBadge();
}

void ChatWindow::reloadContacts()
{
    if (!m_contactList) {
        return;
    }
    m_contactList->clear();
    QString currentGroup;
    for (const Contact& c : m_viewModel.contacts(m_searchEdit ? m_searchEdit->text() : QString())) {
        if (c.groupName != currentGroup) {
            currentGroup = c.groupName;
            auto* groupItem = new QListWidgetItem(currentGroup, m_contactList);
            groupItem->setFlags(Qt::NoItemFlags);
            groupItem->setForeground(QColor("#888888"));
        }
        auto* item = new QListWidgetItem(QString("%1    %2").arg(c.name, c.status), m_contactList);
        item->setSizeHint(QSize(240, 40));
    }
}

void ChatWindow::reloadMessages(const QString& conversationId)
{
    if (conversationId != m_viewModel.currentConversationId()) {
        return;
    }

    while (m_messageLayout->count() > 1) {
        QLayoutItem* child = m_messageLayout->takeAt(0);
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    for (const Message& message : m_viewModel.messages(conversationId)) {
        auto* widget = new ChatMessageWidget(message, m_messageViewport);
        connect(widget, &ChatMessageWidget::favoriteRequested, this, [this](const Message&) {
            QMessageBox::information(this, "收藏", "已加入收藏（演示）。");
        });
        connect(widget, &ChatMessageWidget::forwardRequested, this, [this](const Message&) {
            QMessageBox::information(this, "转发", "请选择要转发的联系人（演示）。");
        });
        connect(widget, &ChatMessageWidget::deleteRequested, this, [this](const Message&) {
            QMessageBox::information(this, "删除", "消息级删除接口已预留。");
        });
        connect(widget, &ChatMessageWidget::recallRequested, this, [this](const Message&) {
            QMessageBox::information(this, "撤回", "2 分钟内撤回逻辑已命中，后续可接入服务端撤回协议。");
        });
        m_messageLayout->insertWidget(m_messageLayout->count() - 1, widget);
    }
    smoothScrollToBottom();
}

void ChatWindow::setConversationHeader(const Conversation& conversation)
{
    if (!conversation.title.isEmpty()) {
        m_chatTitle->setText(conversation.title);
    }
}

void ChatWindow::showConversationMenu(const QPoint& pos)
{
    QListWidgetItem* item = m_conversationList->itemAt(pos);
    if (!item) {
        return;
    }
    const QString conversationId = item->data(Qt::UserRole).toString();
    QMenu menu(this);
    QAction* pin = menu.addAction("置顶聊天");
    QAction* unread = menu.addAction("标记为未读");
    QAction* remove = menu.addAction("删除该聊天");
    QAction* hide = menu.addAction("不显示");
    QAction* selected = menu.exec(m_conversationList->viewport()->mapToGlobal(pos));
    if (selected == pin) {
        m_viewModel.togglePinned(conversationId);
    } else if (selected == unread) {
        m_viewModel.markUnread(conversationId);
    } else if (selected == remove) {
        m_viewModel.clearConversation(conversationId);
    } else if (selected == hide) {
        m_viewModel.hideConversation(conversationId);
    }
}

void ChatWindow::showQuickAddMenu()
{
    QMenu menu(this);
    menu.addAction("发起群聊");
    menu.addAction("添加朋友");
    menu.addAction("扫一扫");
    menu.addAction("收付款");
    QAction* selected = menu.exec(QCursor::pos());
    if (selected) {
        QMessageBox::information(this, "快捷入口", selected->text() + " 功能入口已打开（演示）。");
    }
}

void ChatWindow::showHistoryDialog()
{
    const QString id = m_viewModel.currentConversationId();
    QString text;
    for (const Message& message : m_viewModel.messages(id, m_searchEdit->text())) {
        text += QString("[%1] %2：%3\n")
                    .arg(message.timestamp.toString("yyyy-MM-dd HH:mm"), message.senderName, message.content);
    }
    QMessageBox::information(this, "聊天记录", text.isEmpty() ? "暂无匹配记录。" : text.left(3500));
}

void ChatWindow::updateUnreadBadge()
{
    int count = 0;
    for (const Conversation& c : m_viewModel.conversations()) {
        count += c.unreadCount;
    }
    m_navigation->setUnreadCount(count);
}

void ChatWindow::smoothScrollToBottom()
{
    QTimer::singleShot(0, this, [this]() {
        QScrollBar* bar = m_messageArea->verticalScrollBar();
        auto* animation = new QPropertyAnimation(bar, "value", this);
        animation->setDuration(180);
        animation->setStartValue(bar->value());
        animation->setEndValue(bar->maximum());
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });
}
