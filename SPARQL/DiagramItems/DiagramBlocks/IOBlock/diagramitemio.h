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

    IOBlockSettings* getSettings() override;
    QString getText();
    void setText( const QString& text );
    QString getScript() override { return script; }
    void setString( QString& str ) { script = str; }

private:
    QLineEdit* line_edit;
    QGraphicsProxyWidget* proxy_line_edit;
    IOBlockSettings::TypeIO type_block;
    QString block_name;
    QString script;
};

#endif // DIAGRAMITEMIO_H
