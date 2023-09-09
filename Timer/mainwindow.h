#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAction *m_about;
    QAction *m_language;
    QAction *m_exit;
    QPushButton *m_pColor_background;
    QPushButton *m_pColor_text;
    QLineEdit *m_LineTime;
    QPushButton *m_pDefault;
    QPushButton *m_pWindows;
    QComboBox *m_comboBoxDispWays;

    QColor m_color_background;
    QColor m_color_text;

    void eventActionAbout();
    void eventActionLanguage();
    void eventActionExit();

    void eventBackground();
    void eventTextColor();
    void eventDefault();
    void eventOpenWindows();

    void setBackGroundColor(QPushButton *button, const QColor &color);
    void setColor(QColor &color);
};
#endif // MAINWINDOW_H
