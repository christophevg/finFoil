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

#include "outlineeditor.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QGraphicsScene>
#include "patheditor/patheditorwidget.h"
#include "foil.h"
#include "editablepath.h"
#include "thicknesscontours.h"

using namespace foileditors;
using namespace foillogic;

OutlineEditor::OutlineEditor(Foil *foil, QWidget *parent) :
    QWidget(parent)
{    
    _foilCalculator.reset(new FoilCalculator(foil));


    //
    // PathEditors
    //
    _topPathEditor = new patheditor::PathEditorWidget();
    _topPathEditor->enableFeature(Features::HorizontalAxis);
    _botPathEditor = new patheditor::PathEditorWidget();
    _botPathEditor->enableFeature(Features::HorizontalAxis);

    ThicknessContours *topContours = new ThicknessContours(_foilCalculator.data(), Side::Top);
    ThicknessContours *botContours = new ThicknessContours(_foilCalculator.data(), Side::Bottom);

    EditablePath* nonEditableOutline = new EditablePath(foil->outline());
    nonEditableOutline->setEditable(false);
    _topPathEditor->addGraphicsItem(topContours);
    _topPathEditor->addPath(new EditablePath(foil->outline()));
    _botPathEditor->addGraphicsItem(botContours);
    _botPathEditor->addPath(nonEditableOutline);


    //
    // Layout
    //
    QGroupBox* topGb = new QGroupBox(tr("Outside"));
    QVBoxLayout* topGbLayout = new QVBoxLayout();
    QGroupBox* botGb = new QGroupBox(tr("Inside"));
    QVBoxLayout* botGbLayout = new QVBoxLayout();

    topGbLayout->addWidget(_topPathEditor);
    topGb->setLayout(topGbLayout);

    botGbLayout->addWidget(_botPathEditor);
    botGb->setLayout(botGbLayout);

    _mainLayout = new QVBoxLayout();
    _mainLayout->addWidget(topGb);
    _mainLayout->addWidget(botGb);
    this->setLayout(_mainLayout);
}

FoilCalculator *OutlineEditor::foilCalculator()
{
    return _foilCalculator.data();
}

void OutlineEditor::setGridUnitSize(qreal pxPerUnit)
{
    _topPathEditor->setGridUnitSize(pxPerUnit);
    _botPathEditor->setGridUnitSize(pxPerUnit);
}
