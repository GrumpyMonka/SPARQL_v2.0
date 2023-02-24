#ifndef DIAGRAMITEMIO_H
#define DIAGRAMITEMIO_H

#include <QGraphicsProxyWidget>
#include <QLineEdit>

#include "diagramitem.h"
#include "ioblocksettings.h"

class DiagramItemIO : public DiagramItem
{
public:
    int type() const override { return IOItemType; }

    DiagramItemIO( QMenu* context_menu, QGraphicsItem* parent = nullptr,
        IOBlockSettings* settings = nullptr );

    void setText( const QString& text );
    QString getScript() override { return script; }
    void setString( const QString& str ) { script = str; }
    void setTypeBlock( IOBlockSettings::TypeIO type ) { type_block = type; }
    void setName( const QString& name );

    IOBlockSettings* getSettings() override;
    QString getText();
    QString getName() override;

private:
    QString block_name;
    QLineEdit* line_edit = nullptr;
    QGraphicsProxyWidget* proxy_line_edit;
    IOBlockSettings::TypeIO type_block;
    QString script;
};

#endif // DIAGRAMITEMIO_H
