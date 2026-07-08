#include "widgets/SettingsDialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("设置");
    setFixedSize(520, 430);

    auto* tabs = new QTabWidget(this);

    auto makeTab = [](const QString& title, const QStringList& options) {
        auto* page = new QWidget;
        auto* layout = new QVBoxLayout(page);
        layout->setContentsMargins(18, 18, 18, 18);
        auto* label = new QLabel(title, page);
        label->setObjectName("SectionTitle");
        layout->addWidget(label);
        for (const QString& option : options) {
            auto* check = new QCheckBox(option, page);
            check->setChecked(true);
            layout->addWidget(check);
        }
        layout->addStretch();
        return page;
    };

    tabs->addTab(makeTab("通用设置", {"开机自动启动", "适配高 DPI 缩放", "保留最近聊天记录"}), "通用");
    tabs->addTab(makeTab("消息通知", {"接收新消息通知", "播放提示音", "显示消息详情"}), "通知");
    tabs->addTab(makeTab("隐私安全", {"加密本地数据库", "开启登录保护", "允许通过手机号搜索到我"}), "隐私");

    auto* about = new QWidget(this);
    auto* aboutLayout = new QVBoxLayout(about);
    aboutLayout->setContentsMargins(18, 18, 18, 18);
    auto* aboutText = new QLabel("WeChat Desktop Demo\nQt Widgets + MVVM + SQLite\n用于桌面端即时通讯界面原型。", about);
    aboutText->setWordWrap(true);
    aboutLayout->addWidget(aboutText);
    aboutLayout->addStretch();
    tabs->addTab(about, "关于");

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    buttons->button(QDialogButtonBox::Ok)->setText("完成");
    buttons->button(QDialogButtonBox::Ok)->setObjectName("PrimaryButton");

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(tabs);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
}
