#ifndef BASEDBLOCKWINDOW_H
#define BASEDBLOCKWINDOW_H

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

#include <diagramitembased.h>
#include <swidget.h>

class BasedBlockWindow : public SWidget
{
    Q_OBJECT
public:
    enum BasedMode
    {
        CreateMode = 0,
        ReworkMode = 1
    };
    SWidgetTypes typeSWidget() override { return BasedBlockWindowType; }
    int modeDiagramBlocks() override { return BlocksLibrary::ModeBlocks::None; }

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

    BasedBlockSettings* getSettings();

signals:
    void changeNameWindow( QString );
    void error( QString );

private:
    BasedMode mode_;
    DiagramItemBased* item_on_rework;

    QPixmap pixmap;
    QLabel* label_pixmap;
    QLineEdit* line_name;
    QLineEdit* line_line_edit;
    QLineEdit* line_label;
    QCheckBox* check_line_edit;
    QCheckBox* check_label;
    QTextEdit* text_edit;
};

#endif // BASEDBLOCKWINDOW_H
