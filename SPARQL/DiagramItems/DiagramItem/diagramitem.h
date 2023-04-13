/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QApplication>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QLineEdit>
#include <QList>

class DiagramArrow;
#include <diagramitemsettings.h>

class DiagramItem : public QGraphicsPolygonItem
{
public:
    enum
    {
        DiagramArrowType = UserType,
        DiagramItemType = UserType + 2,
        BasedItemType = UserType + 4,
        CompositeItemType = UserType + 6,
        SparqlItemType = UserType + 8,
        AtomItemType = UserType + 10,
        IOItemType = UserType + 12
    };

    int type() const override { return DiagramItemType; }
    virtual DiagramItemSettings* getSettings();
    virtual QString getScript() { return {}; }
    virtual QString getInputData() { return {}; }
    virtual QString getName() { return {}; }

    explicit DiagramItem( QMenu* context_menu, QGraphicsItem* parent = 0 );
    ~DiagramItem();

    void setContextMenu( QMenu* );
    QMenu* getContextMenu();

    void removeArrow( DiagramArrow* arrow );
    void removeArrows();
    QPolygonF getPolygon() const { return my_polygon; }
    void setItemPolygon( QPolygonF& polygon );

    void addArrow( DiagramArrow* arrow );
    QVector<DiagramArrow*> getArrows();
    QVector<DiagramArrow*> getStartArrows();
    QVector<DiagramArrow*> getEndArrows();
    QPointF getStartPos();
    QPointF getEndPos();
    void setSupportAddItem( bool );
    bool getSupportAddItem();
    void setAllowLineToChild( bool );
    bool getAllowLineToChild();
    void setAllowResize( bool );
    bool getAllowResize();

    QVector<QPair<DiagramItem*, DiagramItem*>> getDependecies();
    void setDependecies( QVector<QPair<DiagramItem*, DiagramItem*>> );
    void addDependecies( DiagramItem*, DiagramItem* );
    void clearDependecies();
    void setColorArrowToItem( const QColor&, DiagramItem* );
    void setColorArrows( const QColor& );

    static bool CheckItemOnDiagramItem( const qint64 code );
    static DiagramItem* FactoryDiagramItem( QMenu* context_menu,
        DiagramItemSettings* settings, QGraphicsItem* parent = 0 );

    void contextMenuShow( QGraphicsSceneMouseEvent* mouseEvent );

public slots:
    void setOutputText( const QString& output_text );
    void mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;

protected:
    QVariant itemChange( GraphicsItemChange change, const QVariant& value ) override;

protected:
    QMenu* my_context_menu;
    QVector<DiagramArrow*> arrows;
    QPolygonF my_polygon;
    bool support_add_item;
    bool allow_line_to_child;
    bool allow_resize = false;
    QVector<QPair<DiagramItem*, DiagramItem*>> dependencies;
};

#endif // DIAGRAMITEM_H
