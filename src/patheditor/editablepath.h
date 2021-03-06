/****************************************************************************

 Copyright (c) 2013, Hans Robeers
 All rights reserved.

 BSD 2-Clause License

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

****************************************************************************/

#ifndef EDITABLEPATH_H
#define EDITABLEPATH_H

#include "patheditorfwd/patheditorfwd.h"

#include <QGraphicsObject>

namespace patheditor
{
    /**
     * @brief A compound path, build by PathItems, that can be edited by dragging control points
     */
    class EditablePath : public QGraphicsObject
    {
        Q_OBJECT
    public:
        explicit EditablePath(Path* path, QGraphicsItem * parent = 0);

        // Implementing QGraphicsItem
        virtual QRectF boundingRect() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget);

        Path* path();

        QPointF pointAtPercent(qreal t);

        bool released();

        void setEditable(bool editable);
        bool editable();

        virtual ~EditablePath() {}

    signals:
        void pathChanged(EditablePath *sender);
        void pathReleased(EditablePath *sender);

    private slots:
        void onAppend(patheditor::PathItem *pathItem);
        void onPointDrag(PathPoint *sender);
        void onPointRelease(PathPoint *sender);

    private:
        bool _editable = true;
        bool _firstPaint = true;
        bool _released = true;
        Path* _path = 0;
        const PathSettings* _settings = 0;

        void connectPoints(PathItem *pathItem);
        void emitPathChanged();
        void emitPathReleased();
    };
}

#endif // EDITABLEPATH_H
