#include <mainwindow.h>

#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleF;
    styleF.setFileName( ":/Sources/style/project_style.qss" );
    styleF.open( QFile::ReadOnly );
    QString qssStr = styleF.readAll();
    qApp->setStyleSheet( qssStr );

    MainWindow w;
    w.show();

    return a.exec();
}
