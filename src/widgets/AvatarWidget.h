#pragma once

#include <QWidget>

class AvatarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AvatarWidget(QWidget* parent = nullptr);

    void setName(const QString& name);
    void setColor(const QString& color);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString m_name = "我";
    QString m_color = "#07c160";
};
