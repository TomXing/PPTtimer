#ifndef DIALOG_H
#define DIALOG_H

#include <QColor>
#include <QDialog>
#include <QLabel>
#include <QTimer>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    //倒计时显示方式
    enum DISPLAY_WAYS { SECONDS, MINUTES };
    explicit Dialog(QWidget *parent, QColor b, QColor t, double minutes);
    ~Dialog();

    void setDispWays(DISPLAY_WAYS way);

private:
    Ui::Dialog *ui;
    QWidget *m_parent;
    QLabel *m_text;

    QTimer *m_timer;
    int m_timeSecond;
    int m_timeSecondInit;
    int m_wayOfDisplay;

    bool m_bDragging;          // 是否正在拖动
    QPointF m_poStartPosition; // 拖动开始前的鼠标位置
    QPointF m_poFramePosition; // 窗体的原始位置

    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void onTimeout();
    void displayTime();
};

#endif // DIALOG_H
