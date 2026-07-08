#pragma once

#include <QFrame>
#include <QPoint>

class QLabel;
class QPushButton;

class TitleBar : public QFrame
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget* parent = nullptr);
    void setTitle(const QString& title);

signals:
    void minimizeRequested();
    void maximizeRestoreRequested();
    void closeRequested();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    QLabel* m_titleLabel = nullptr;
    bool m_dragging = false;
    QPoint m_dragStartGlobal;
    QPoint m_windowStartPos;
};
