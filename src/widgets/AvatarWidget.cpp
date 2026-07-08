#include "widgets/AvatarWidget.h"

#include <QPainter>
#include <QPainterPath>

AvatarWidget::AvatarWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(42, 42);
}

void AvatarWidget::setName(const QString& name)
{
    m_name = name.isEmpty() ? "?" : name;
    update();
}

void AvatarWidget::setColor(const QString& color)
{
    m_color = color.isEmpty() ? "#8a8f98" : color;
    update();
}

QSize AvatarWidget::sizeHint() const
{
    return QSize(42, 42);
}

void AvatarWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;
    path.addRoundedRect(rect().adjusted(1, 1, -1, -1), 8, 8);
    painter.fillPath(path, QColor(m_color));

    painter.setPen(Qt::white);
    QFont f = painter.font();
    f.setBold(true);
    f.setPointSize(12);
    painter.setFont(f);

    painter.drawText(rect(), Qt::AlignCenter, m_name.left(1).toUpper());
}
