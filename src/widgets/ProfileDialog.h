#pragma once

#include <QDialog>

class AvatarWidget;
class QComboBox;
class QLineEdit;

class ProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileDialog(QWidget* parent = nullptr);

private:
    AvatarWidget* m_avatar = nullptr;
    QLineEdit* m_nicknameEdit = nullptr;
    QComboBox* m_statusBox = nullptr;
};
