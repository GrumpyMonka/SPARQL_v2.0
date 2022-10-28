#include "basedblockwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include <QVariant>
#include <exception>

BasedBlockWindow::BasedBlockWindow( const BasedMode& mode, QWidget* parent )
    : QWidget( parent )
{
    setMode( mode );
    createWidget();
}

void BasedBlockWindow::setMode( const BasedMode& mode )
{
    mode_ = mode;
}

void BasedBlockWindow::setBlockOnRework( DiagramItemBased* item )
{
    if ( ReworkMode == mode_ )
        emit error( tr( "BasedBlockWindow, this not rework mode!" ) );
    item_on_rework = item;
    setSettings( item->getSetting() );
}

void BasedBlockWindow::setSettings( BasedBlockSettings* settings )
{
    settings_ = settings;
    setSettingOnWindow();
}

void BasedBlockWindow::createWidget()
{
    QGridLayout* gridLayout = new QGridLayout( this );

    QLabel* label_for_label = new QLabel( tr( "Text label:" ), this );
    QLabel* label_for_script = new QLabel( tr( "Script:" ), this );
    QLabel* label_for_name_blocks = new QLabel( tr( "Name Block:" ), this );
    QLabel* label_for_line_edit = new QLabel( tr( "Text lineEdit:" ), this );
    label_pixmap = new QLabel( tr( "NULL" ), this );

    QPushButton* button_info = new QPushButton( tr( "Info" ), this );
    QPushButton* button_save = new QPushButton( tr( "Save" ), this );
    QPushButton* button_save_as = new QPushButton( tr( "Save as" ), this );
    QPushButton* button_open = new QPushButton( tr( "Open" ), this );
    QPushButton* button_test_script = new QPushButton( tr( "Test Script" ), this );
    QPushButton* button_set_image = new QPushButton( tr( "Set Image" ), this );
    QPushButton* button_delete_image = new QPushButton( tr( "Delete Image" ), this );
    connect( button_info, SIGNAL( clicked() ),
        this, SLOT( slotInfo() ) );
    connect( button_save, SIGNAL( clicked() ),
        this, SLOT( slotSave() ) );
    connect( button_open, SIGNAL( clicked() ),
        this, SLOT( slotOpen() ) );
    connect( button_test_script, SIGNAL( clicked() ),
        this, SLOT( slotTestScript() ) );
    connect( button_set_image, SIGNAL( clicked() ),
        this, SLOT( slotSetImage() ) );
    connect( button_delete_image, SIGNAL( clicked() ),
        this, SLOT( slotDeleteImage() ) );
    connect( button_save_as, SIGNAL( clicked() ),
        this, SLOT( slotSaveAs() ) );

    line_name = new QLineEdit( this );
    line_line_edit = new QLineEdit( this );
    line_label = new QLineEdit( this );

    check_line_edit = new QCheckBox( tr( "LineEdit" ), this );
    check_label = new QCheckBox( tr( "Label" ), this );

    text_edit = new QTextEdit( this );
    text_edit->setTabStopWidth( 30 );

    gridLayout->addWidget( label_for_label, 4, 2, 1, 1 );
    gridLayout->addWidget( label_for_script, 6, 1, 1, 1 );
    gridLayout->addWidget( label_for_name_blocks, 1, 1, 1, 1 );
    gridLayout->addWidget( label_for_line_edit, 4, 4, 1, 1 );
    gridLayout->addWidget( label_pixmap, 0, 4, 3, 1 );

    gridLayout->addWidget( button_info, 6, 3, 1, 1 );
    gridLayout->addWidget( button_save_as, 8, 4, 1, 1 );
    gridLayout->addWidget( button_save, 8, 3, 1, 1 );
    gridLayout->addWidget( button_open, 8, 2, 1, 1 );
    gridLayout->addWidget( button_test_script, 6, 4, 1, 1 );
    gridLayout->addWidget( button_set_image, 1, 3, 1, 1 );
    gridLayout->addWidget( button_delete_image, 2, 3, 1, 1 );

    gridLayout->addWidget( line_name, 2, 1, 1, 2 );
    gridLayout->addWidget( line_line_edit, 5, 3, 1, 2 );
    gridLayout->addWidget( line_label, 5, 1, 1, 2 );

    gridLayout->addWidget( check_line_edit, 4, 3, 1, 1 );
    gridLayout->addWidget( check_label, 4, 1, 1, 1 );
    gridLayout->addWidget( text_edit, 7, 1, 1, 4 );
}

void BasedBlockWindow::slotInfo()
{
}

void BasedBlockWindow::slotSave()
{
    setWindowToSetting();
}

void BasedBlockWindow::slotSaveAs()
{
    if ( nullptr == settings_ )
        emit error( tr( "BasedBlockWindow settings_ = nullptr" ) );
    setWindowToSetting();
    QString file_name = QFileDialog::getSaveFileName( this, "Save as", QDir::currentPath(), tr( "JSON (*.json);;All files (*)" ) );
    QStringList list = file_name.split( "." );
    if ( list.size() == 1 || list.back() != "json" )
    {
        file_name += ".json";
    }

    QFile file( file_name );
    if ( file.open( QIODevice::WriteOnly ) )
    {
        QJsonDocument json;
        json.setObject( settings_->getJsonFromSetting() );
        file.write( json.toJson() );
        QMessageBox::about( this, tr( "Based Block" ), tr( "Block is saved!" ) );
    }
    else
    {
        QMessageBox::about( this, tr( "Based Block" ), tr( "Failed save block!" ) );
    }
    file.close();
}

void BasedBlockWindow::slotOpen()
{
    QString file_name = QFileDialog::getOpenFileName( this, "Choose File", QDir::currentPath(), tr( "JSON (*.json);;All files (*)" ) );
    QFile file( file_name );
    if ( file.open( QIODevice::ReadOnly ) )
    {
        settings_->setSettingFromString( file.readAll() );
        slotSetName( getName( file_name ) );
        setSettingOnWindow();
        QMessageBox::about( this, tr( "Based Block" ), tr( "Block is open!" ) );
    }
    else
    {
        QMessageBox::about( this, tr( "Based Block" ), tr( "Failed open block!" ) );
    }
    file.close();
}

void BasedBlockWindow::slotTestScript()
{
}

void BasedBlockWindow::slotSetImage()
{
}

void BasedBlockWindow::slotDeleteImage()
{
}

void BasedBlockWindow::slotSetName( const QString& name )
{
    emit changeNameWindow( name );
}

void BasedBlockWindow::setSettingOnWindow()
{
    if ( nullptr == settings_ )
        emit error( tr( "BasedBlockWindow settings_ = nullptr" ) );
    line_name->setText( settings_->block_name );
    line_label->setText( settings_->label_text );
    line_line_edit->setText( settings_->line_edit_text );

    check_label->setChecked( settings_->label );
    check_line_edit->setChecked( settings_->line_edit );

    text_edit->setText( settings_->script );

    if ( settings_->pixmap.isNull() )
    {
        settings_->pixmap = settings_->image().scaled( 70, 70 );
    }
    label_pixmap->setPixmap( settings_->pixmap.scaled( 70, 70 ) );
}

void BasedBlockWindow::setWindowToSetting()
{
    if ( nullptr == settings_ )
        emit error( tr( "BasedBlockWindow settings_ = nullptr" ) );
    settings_->block_name = line_name->text();
    settings_->label = check_label->isChecked();
    settings_->line_edit = check_line_edit->isChecked();
    settings_->label_text = line_label->text();
    settings_->line_edit_text = line_line_edit->text();
    settings_->script = text_edit->toPlainText();

    if ( ReworkMode == mode_ && item_on_rework != nullptr )
    {
        item_on_rework->setSetting( settings_ );
    }
}

QString BasedBlockWindow::getName( const QString& path )
{
    QStringList lst = path.split( "/" );
    return lst.at( lst.size() - 1 );
}
