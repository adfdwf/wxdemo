#include "widgets/ProfileDialog.h"

#include "widgets/AvatarWidget.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

ProfileDialog::ProfileDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("个人资料");
    setFixedSize(360, 330);

    m_avatar = new AvatarWidget(this);
    m_avatar->setFixedSize(72, 72);
    m_avatar->setName("我");
    m_avatar->setColor("#07c160");

    auto* changeAvatar = new QPushButton("修改头像", this);
    changeAvatar->setObjectName("SecondaryButton");

    m_nicknameEdit = new QLineEdit("微信用户", this);
    m_statusBox = new QComboBox(this);
    m_statusBox->addItems({"在线", "忙碌", "请勿打扰", "离开"});

    auto* form = new QFormLayout;
    form->setLabelAlignment(Qt::AlignRight);
    form->addRow("昵称", m_nicknameEdit);
    form->addRow("状态", m_statusBox);

    auto* tips = new QLabel("资料修改保存在当前演示会话中，可继续接入真实用户系统。", this);
    tips->setWordWrap(true);
    tips->setStyleSheet("color:#8a8a8a;");

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText("保存");
    buttons->button(QDialogButtonBox::Ok)->setObjectName("PrimaryButton");
    buttons->button(QDialogButtonBox::Cancel)->setText("取消");
    buttons->button(QDialogButtonBox::Cancel)->setObjectName("SecondaryButton");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 24, 26, 20);
    layout->setSpacing(14);
    layout->addWidget(m_avatar, 0, Qt::AlignHCenter);
    layout->addWidget(changeAvatar, 0, Qt::AlignHCenter);
    layout->addLayout(form);
    layout->addWidget(tips);
    layout->addStretch();
    layout->addWidget(buttons);

    connect(changeAvatar, &QPushButton::clicked, this, [this]() {
        // 演示环境中用颜色替代真实图片上传，便于无资源文件直接运行。
        m_avatar->setColor("#5b8def");
    });
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
