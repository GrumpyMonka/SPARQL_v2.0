#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTabWidget>

#include <smenubar.h>
#include <stoolbar.h>
#include <stoolbox.h>
#include <stabwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

private:
    void CreateMainForm();

private:
    Ui::MainWindow *ui;
    QGridLayout* mainLayout;

    SMenuBar* menuBar;
    SToolBar* toolBar;
    SToolBox* toolBox;
    STabWidget* tabWidget;
};
#endif // MAINWINDOW_H
