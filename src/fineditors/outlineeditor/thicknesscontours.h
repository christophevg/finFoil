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

#ifndef THICKNESSCONTOURS_H
#define THICKNESSCONTOURS_H

#include <QGraphicsObject>
//#include <QMutex>
#include <QThreadPool>
#include "patheditorfwd/patheditorfwd.h"

using namespace patheditor;

namespace fineditors
{
    class ThicknessContours : public QGraphicsObject
    {
        Q_OBJECT
    public:
        explicit ThicknessContours(QGraphicsItem *parent = 0);

        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        virtual QRectF boundingRect() const;

        virtual ~ThicknessContours() {}

    signals:

    public slots:
        void onOutlineChange(EditablePath *sender);
        void onProfileChange(EditablePath *sender);
        void onThicknessChange(EditablePath *sender);

    private:
//        QMutex _calcLock;
        QThreadPool _tPool;

        QScopedPointer<QPainterPath> _outline;
        QScopedPointer<QPainterPath> _profile;
        QScopedPointer<QPainterPath> _thickness;

        QList<qreal> _contourThicknesses;
        QList<QSharedPointer<QPainterPath> > _contours;

        void calcContours();
        bool profilesSet();
    };
}

#endif // THICKNESSCONTOURS_H
