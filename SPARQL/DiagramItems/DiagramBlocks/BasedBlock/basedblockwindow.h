#ifndef BASEDBLOCKWINDOW_H
#define BASEDBLOCKWINDOW_H

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QWidget>

#include <diagramitembased.h>

class BasedBlockWindow : public QWidget
{
    Q_OBJECT
public:
    enum BasedMode
    {
        CreateMode = 0,
        ReworkMode = 1
    };

    enum
    {
        DiagramMode = 0
    };

    explicit BasedBlockWindow( const BasedMode& mode, QWidget* parent = nullptr );

    void createWidget();
    void setMode( const BasedMode& mode );
    void setBlockOnRework( DiagramItemBased* item );
    void setSettings( BasedBlockSettings* settings );

    QString getName( const QString& path );

private slots:
    void slotInfo();
    void slotSave();
    void slotSaveAs();
    void slotOpen();
    void slotTestScript();
    void slotSetImage();
    void slotDeleteImage();
    void slotSetName( const QString& name );

    void setSettingOnWindow();
    void setWindowToSetting();

signals:
    void changeNameWindow( QString );
    void error( QString );

private:
    BasedMode mode_;
    BasedBlockSettings* settings_ = nullptr;
    DiagramItemBased* item_on_rework;

    QLabel* label_pixmap;
    QLineEdit* line_name;
    QLineEdit* line_line_edit;
    QLineEdit* line_label;
    QCheckBox* check_line_edit;
    QCheckBox* check_label;
    QTextEdit* text_edit;
};

#endif // BASEDBLOCKWINDOW_H
