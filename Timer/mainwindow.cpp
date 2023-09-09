#include "mainwindow.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QPalette>
#include "./ui_mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_color_background(QColor(210, 71, 38))
    , m_color_text(QColor(255, 255, 255))
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    { // bind ui
        m_about = ui->actionAbout;
        m_language = ui->actionLanguage;
        m_exit = ui->actionExit;

        m_pColor_background = ui->pushButton_background;
        m_pColor_text = ui->pushButton_textcolor;
        m_LineTime = ui->time;
        m_pDefault = ui->pushButton_default;
        m_pWindows = ui->pushButton_openwindows;
        m_comboBoxDispWays = ui->comboBox_dispWays;
    }
    setBackGroundColor(m_pColor_background, m_color_background);
    setBackGroundColor(m_pColor_text, m_color_text);

    connect(m_about, &QAction::triggered, this, &MainWindow::eventActionAbout);
    connect(m_language, &QAction::triggered, this, &MainWindow::eventActionLanguage);
    connect(m_exit, &QAction::triggered, this, &MainWindow::eventActionExit);

    connect(m_pColor_background, &QPushButton::clicked, this, &MainWindow::eventBackground);
    connect(m_pColor_text, &QPushButton::clicked, this, &MainWindow::eventTextColor);
    connect(m_pDefault, &QPushButton::clicked, this, &MainWindow::eventDefault);
    connect(m_pWindows, &QPushButton::clicked, this, &MainWindow::eventOpenWindows);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::eventActionAbout()
{
    //QMessageBox::information(this, "About", "Powered By TomKing at SWJTU");
    QMessageBox::information(
        this,
        "关于",
        "<H1>PPT倒计时软件</H1>"
        "<p>开发这个小软件的契机是出于一次偶然：在讲PPT的时候，我演讲的时间超过了规定时间。"
        "导致后面有几页PPT没有讲，虽然影响不大，但是我觉的这样不好！</p>"
        "<p>于是，开发了这款小软件,希望你能够喜欢！</p>"
        "<p>唐名,西南交通大学 2023年9月8日</p>"
        "<p>Copyright, All Rights Reserved</p>",
        "确认");
}

void MainWindow::eventActionLanguage()
{
    QMessageBox::information(this, "提示", "对不起，该功能还没开发", "确认");
}

void MainWindow::eventActionExit()
{
    this->close();
}

void MainWindow::eventBackground()
{
    setColor(m_color_background);
    setBackGroundColor(m_pColor_background, m_color_background);
    this->statusBar()->showMessage("选择颜色：" + m_color_background.name(), 3000);
}

void MainWindow::eventTextColor()
{
    setColor(m_color_text);
    setBackGroundColor(m_pColor_text, m_color_text);
    this->statusBar()->showMessage("选择颜色：" + m_color_text.name(), 3000);
}

void MainWindow::eventDefault()
{
    m_color_background = QColor(210, 71, 38);
    setBackGroundColor(m_pColor_background, m_color_background);
    m_color_text = QColor(255, 255, 255);
    setBackGroundColor(m_pColor_text, m_color_text);

    m_LineTime->setText("10");
    this->statusBar()->showMessage("已设置为默认配置", 3000);
}

void MainWindow::eventOpenWindows()
{
    bool isOk;
    double minutes = m_LineTime->text().toDouble(&isOk);
    if (isOk) {
        if (minutes <= 0) {
            QMessageBox::information(this, "关于", "输入的倒计时时间必须大于0", "确认");
        } else {
            Dialog *dialog = new Dialog(this, m_color_background, m_color_text, minutes);
            dialog->show();
            switch (m_comboBoxDispWays->currentIndex()) {
            case 0: //秒钟
                dialog->setDispWays(Dialog::SECONDS);
                break;
            case 1: //分钟
                dialog->setDispWays(Dialog::MINUTES);
                break;
            default:
                break;
            };
            this->hide();
        }
    } else {
        QMessageBox::critical(this, "无效参数", "请有效倒计时长", "确认");
    }
}

void MainWindow::setBackGroundColor(QPushButton *button, const QColor &color)
{
    QPalette palette = button->palette();
    palette.setColor(QPalette::Button, color);
    button->setPalette(palette);
    button->setAutoFillBackground(true);
    button->setFlat(true);
}

/**
 * @brief 设置颜色
 * @return 
 */
void MainWindow::setColor(QColor &color)
{
    QColorDialog windows(this);
    connect(&windows, &QColorDialog::colorSelected, this, [&](const QColor &newColor) {
        color = newColor;
    });
    windows.exec();
}
