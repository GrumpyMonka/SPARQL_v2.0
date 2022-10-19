#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>
#include <QTabWidget>
#include <QVector>

#include <blockslibrary.h>
#include <smenubar.h>
#include <stabwidget.h>
#include <stoolbar.h>
#include <stoolbox.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

public slots:
    void slotCreateNewProject();
    void slotCurrentTabMode( int );

private:
    void CreateMainForm();

private:
    Ui::MainWindow* ui;
    QGridLayout* mainLayout;

    SMenuBar* menuBar;
    SToolBar* toolBar;
    SToolBox* toolBox;
    STabWidget* tabWidget;

    BlocksLibrary* library;
};
#endif // MAINWINDOW_H
