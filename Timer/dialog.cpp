/**
 * TODO 
 * 1. 添加不透明度
 * https://blog.csdn.net/wenxingchen/article/details/128233022?spm=1035.2023.3001.6557&utm_medium=distribute.pc_relevant_bbs_down_v2.none-task-blog-2~default~ESQUERY~Rate-3-128233022-bbs-390155423.264^v3^pc_relevant_bbs_down_v2_default&depth_1-utm_source=distribute.pc_relevant_bbs_down_v2.none-task-blog-2~default~ESQUERY~Rate-3-128233022-bbs-390155423.264^v3^pc_relevant_bbs_down_v2_default
 * 2. 实现窗口可拖动[ok]
 * https://blog.csdn.net/qq_41883523/article/details/108417674?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-108417674-blog-115511785.235%5Ev38%5Epc_relevant_default_base&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-108417674-blog-115511785.235%5Ev38%5Epc_relevant_default_base&utm_relevant_index=5
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */
#include "dialog.h"
#include <QAction>
#include <QCursor>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPalette>
#include "ui_dialog.h"
Dialog::Dialog(QWidget *parent, QColor b, QColor t, double minutes)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , m_parent(parent)
    , m_timeSecond(minutes * 60)
    , m_timeSecondInit(minutes * 60)
    , m_wayOfDisplay(0)
{
    ui->setupUi(this);

    setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint); //置顶不显示标题栏
    m_text = ui->label_time;
    QPalette palette;
    palette.setColor(QPalette::WindowText, t);
    m_text->setPalette(palette);

    palette.setColor(QPalette::Window, b);
    this->setPalette(palette);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Dialog::onTimeout);
    m_timer->start(1000); //1秒中断一次
}

Dialog::~Dialog()
{
    delete ui;
    delete m_timer;
}

void Dialog::setDispWays(DISPLAY_WAYS index)
{
    m_wayOfDisplay = index;
}

void Dialog::onTimeout()
{
    if (m_timeSecond > 0) {
        QString message;
        switch (m_wayOfDisplay) {
        case SECONDS:
            message = QString::number(m_timeSecond);
            break;
        case MINUTES: {
            int sec = m_timeSecond % 60;
            int min = m_timeSecond / 60;
            message = QString("%1:%2").arg(min).arg(sec, 2, 10, QLatin1Char('0'));
        } break;
        default:
            break;
        }
        m_text->setText(message);
        m_timeSecond--;
    } else {
        m_timer->stop();
        m_text->setText("时间到");
        QFont font;
        font.setPixelSize(12); /// TODO: 这里应该动态设置大小，是原来字体的1.5倍。
        m_text->setFont(font);
    }
}

void Dialog::displayTime()
{
    if (m_timeSecond > 0) {
        m_text->setText(QString::number(m_timeSecond));
        m_timeSecond--;
    } else {
        m_timer->stop();
        m_text->setText("时间到");
        QFont font;
        font.setPixelSize(12); /// TODO: 这里应该动态设置大小，是原来字体的1.5倍。
        m_text->setFont(font);
    }
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QAction reStart(this);
        reStart.setText("重新开始");
        QAction stop(this);
        if (m_timer->isActive()) {
            stop.setText("暂停");
        } else {
            stop.setText("开始");
        }

        QMenu menu_ways(this);
        menu_ways.setTitle("显示方式");
        QAction seconds(this);
        seconds.setText("秒钟");
        QAction minutes(this);
        minutes.setText("分钟");
        menu_ways.addAction(&seconds);
        menu_ways.addAction(&minutes);

        QAction close(this);
        close.setText("关闭");

        QMenu menu(this);
        menu.addAction(&reStart);
        menu.addAction(&stop);
        menu.addMenu(&menu_ways);
        menu.addAction(&close);

        QAction *act = menu.exec(QCursor::pos());
        if (act == &reStart) {
            m_timeSecond = m_timeSecondInit;
        } else if (act == &close) {
            m_parent->show();
            this->close();
        } else if (act == &stop) {
            if (m_timer->isActive()) {
                m_timer->stop();
            } else {
                m_timer->start();
            }
        } else if (act == &seconds) {
            m_wayOfDisplay = SECONDS;
        } else if (act == &minutes) {
            m_wayOfDisplay = MINUTES;
        }
    }
    if (event->button() == Qt::LeftButton) {
        {
            QRect rect = this->rect(); //rect是鼠标实现可拖动的区域
            rect.setBottom(rect.top() + 220);
            if (rect.contains(event->pos())) {
                m_bDragging = true;
                m_poStartPosition = event->globalPosition();
                m_poFramePosition = frameGeometry().topLeft();
            }
        }
    }

    QDialog::mousePressEvent(event);
}

void Dialog::closeEvent(QCloseEvent *event)
{
    m_parent->show();
    QDialog::closeEvent(event);
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (m_bDragging) {
            // delta 相对偏移量,
            QPointF delta = event->globalPosition() - m_poStartPosition;
            // 新位置：窗体原始位置  + 偏移量
            QPointF pos = m_poFramePosition + delta;
            move(pos.x(), pos.y());
        }
    }
    QWidget::mouseMoveEvent(event);
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_bDragging = false;
    QWidget::mouseReleaseEvent(event);
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    //阻塞按键的功能
}
