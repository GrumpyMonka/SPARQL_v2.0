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
    void slotCreateBasedBlock();
    void slotCreateCompositeBlock();
    void slotCreateSparqlBlock();
    void slotCurrentTabMode( int );
    void slotOnCreateBlock( DiagramItemSettings* );
    void slotOnClickedBlockExecutor();
    void slotOnDeleteItemOnScene();

private:
    void createMainForm();

private:
    Ui::MainWindow* ui;
    QGridLayout* main_layout;

    SMenuBar* menu_bar;
    SToolBar* tool_bar;
    SToolBox* tool_box;
    STabWidget* tab_widget;

    BlocksLibrary* library;
};
#endif // MAINWINDOW_H
