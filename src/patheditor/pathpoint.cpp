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

#include "pathpoint.h"

#include <QGraphicsScene>
#include "pointhandle.h"
#include "pathsettings.h"
#include "exceptions.h"
#include "restrictor.h"

using namespace patheditor;

static QMap<QGraphicsScene*, PathPoint*> s_prevSelected;

PathPoint::PathPoint(qreal xpos, qreal ypos)
    : QPointF(xpos, ypos), _selected(false)
{
}

void PathPoint::setParent(QObject* /*unused*/)
{
    QString message("Cannot use setParent() on a PathPoint. Use smart pointers to manage PathPoint objects.");
    throw hrlib::ImplementationException(message, this);
}

void PathPoint::setRestrictedX(qreal xpos)
{
    setRestrictedPos(xpos, y());
}

void PathPoint::setRestrictedY(qreal ypos)
{
    setRestrictedPos(x(), ypos);
}

void PathPoint::setRestrictedPos(qreal xpos, qreal ypos)
{
    if (_restrictor)
        _restrictor->restrictCoordinate(&xpos, &ypos);

    qreal dx = xpos - this->rx();
    qreal dy = ypos - this->ry();

    setPos(xpos, ypos);

    foreach(std::weak_ptr<PathPoint> linkedPoint, _followingPoints)
    {
        std::shared_ptr<PathPoint> strongPnt = linkedPoint.lock();
        if (strongPnt)
        {
            qreal rx(strongPnt->rx() + dx);
            qreal ry(strongPnt->ry() + dy);
            strongPnt->setPos(rx, ry);
        }
    }
}

std::shared_ptr<Restrictor> PathPoint::restrictor()
{
    return _restrictor;
}

void PathPoint::setRestrictor(std::shared_ptr<Restrictor> restrictor)
{
    _restrictor = restrictor;
    setRestrictedPos(this->rx(), this->ry());
}

void PathPoint::createPointHandle(QGraphicsItem *parent, const PathSettings *settings)
{
    if (!settings) settings = PathSettings::Default();
    createPointHandleImpl(parent, settings);
}

PathPoint *PathPoint::toFollowPoint()
{
    return _toFollowPoint;
}

void PathPoint::addFollowingPoint(std::shared_ptr<PathPoint> point)
{
    point->_toFollowPoint = this;
    _followingPoints.append(point);
}

bool PathPoint::visible()
{
    if (_toFollowPoint)
    {
        return _toFollowPoint->selected();
    }
    else
    {
        return true;
    }
}

void PathPoint::select()
{
    QGraphicsScene* scene = this->_pointHandle->scene();

    if (!_toFollowPoint)
        select(this, scene);
    else
        select(_toFollowPoint, scene);
}

bool PathPoint::selected() const
{
    return _selected;
}

PointHandle *PathPoint::handle()
{
    return _pointHandle;
}

void PathPoint::replaceCurrentPointHandle(PointHandle *pointHandle)
{
    if (_pointHandle)
    {
        if (_pointHandle->scene())
            _pointHandle->scene()->removeItem(_pointHandle);
        delete _pointHandle;
    }

    _pointHandle = pointHandle;
}

void PathPoint::createPointHandleImpl(QGraphicsItem *parent, const PathSettings *settings)
{
    PointHandle *newPointHandle = new PointHandle(this, settings->handleSize(), settings->pointBrush(), parent);
    newPointHandle->setZValue(1);
    replaceCurrentPointHandle(newPointHandle);
}

void PathPoint::setPos(qreal xpos, qreal ypos)
{
    this->setX(xpos);
    this->setY(ypos);

    if (_pointHandle)
    {
        _pointHandle->setCenter(xpos, ypos);
    }
}

void PathPoint::select(PathPoint *point, QGraphicsScene *scene)
{
    if (s_prevSelected.contains(scene))
        s_prevSelected[scene]->_selected = false;

    s_prevSelected[scene] = point;
    point->_selected = true;
}

void PathPoint::onPointDrag(QGraphicsSceneMouseEvent* /*unused*/)
{
    emit pointDrag(this);
}

void PathPoint::onPointRelease(QGraphicsSceneMouseEvent* /*unused*/)
{
    emit pointRelease(this);
}
