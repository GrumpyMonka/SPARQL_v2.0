#ifndef STOOLBOX_H
#define STOOLBOX_H

#include <QToolBox>
#include <QButtonGroup>
#include <QGridLayout>

class SToolBox : public QToolBox
{
    Q_OBJECT
public:
    explicit SToolBox( QWidget *parent = nullptr );

private:
    void createToolButtonGroup();

signals:

private:
    QButtonGroup* buttonGroup;
    QGridLayout* buttonLayout;
};

#endif // STOOLBOX_H
