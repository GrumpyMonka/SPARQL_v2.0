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
        IOBlockSettings* settings = new IOBlockSettings() );

    IOBlockSettings* getSettings() override;
    QString getText();
    void setText( const QString& text );

private:
    QLineEdit* line_edit;
    QGraphicsProxyWidget* proxy_line_edit;
    IOBlockSettings::TypeIO type_block;
    QString block_name;
};

#endif // DIAGRAMITEMIO_H
