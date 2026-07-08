#include "widgets/TitleBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QWidget>

namespace {
QPoint mouseGlobalPosition(QMouseEvent* event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return event->globalPosition().toPoint();
#else
    return event->globalPos();
#endif
}
}

TitleBar::TitleBar(QWidget* parent)
    : QFrame(parent)
{
    setObjectName("TitleBar");
    setFixedHeight(34);

    m_titleLabel = new QLabel("微信", this);
    m_titleLabel->setObjectName("WindowTitle");

    auto* minimizeButton = new QPushButton("—", this);
    auto* maximizeButton = new QPushButton("□", this);
    auto* closeButton = new QPushButton("×", this);
    for (QPushButton* button : {minimizeButton, maximizeButton, closeButton}) {
        button->setObjectName("WindowButton");
        button->setFixedSize(46, 34);
        button->setCursor(Qt::PointingHandCursor);
        button->setFocusPolicy(Qt::NoFocus);
    }
    closeButton->setObjectName("CloseButton");

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_titleLabel);
    layout->addStretch();
    layout->addWidget(minimizeButton);
    layout->addWidget(maximizeButton);
    layout->addWidget(closeButton);

    connect(minimizeButton, &QPushButton::clicked, this, &TitleBar::minimizeRequested);
    connect(maximizeButton, &QPushButton::clicked, this, &TitleBar::maximizeRestoreRequested);
    connect(closeButton, &QPushButton::clicked, this, &TitleBar::closeRequested);
}

void TitleBar::setTitle(const QString& title)
{
    m_titleLabel->setText(title);
}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && window()) {
        m_dragging = true;
        m_dragStartGlobal = mouseGlobalPosition(event);
        m_windowStartPos = window()->pos();
    }
    QFrame::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging && window() && !window()->isMaximized()) {
        window()->move(m_windowStartPos + mouseGlobalPosition(event) - m_dragStartGlobal);
    }
    QFrame::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
    m_dragging = false;
    QFrame::mouseReleaseEvent(event);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit maximizeRestoreRequested();
    }
    QFrame::mouseDoubleClickEvent(event);
}
