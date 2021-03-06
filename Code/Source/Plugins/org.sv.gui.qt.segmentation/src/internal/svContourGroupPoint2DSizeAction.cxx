/* Copyright (c) Stanford University, The Regents of the University of
 *               California, and others.
 *
 * All Rights Reserved.
 *
 * See Copyright-SimVascular.txt for additional details.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "svContourGroupPoint2DSizeAction.h"

#include "svContourGroup.h"

#include <mitkNodePredicateDataType.h>

#include <QInputDialog>

svContourGroupPoint2DSizeAction::svContourGroupPoint2DSizeAction()
    : m_Functionality(NULL)
{
}

svContourGroupPoint2DSizeAction::~svContourGroupPoint2DSizeAction()
{
}

void svContourGroupPoint2DSizeAction::Run(const QList<mitk::DataNode::Pointer> &selectedNodes)
{
    mitk::DataNode::Pointer selectedNode = selectedNodes[0];

    mitk::NodePredicateDataType::Pointer isGroup = mitk::NodePredicateDataType::New("svContourGroup");

    float initialValue=8.0f;
    if(isGroup->CheckNode(selectedNode))
        selectedNode->GetFloatProperty ("point.displaysize", initialValue);

    bool ok;
    float size=QInputDialog::getDouble(NULL, "Point 2D Display Size", "Size:", initialValue, 0.1, 100.0, 1, &ok);

    if(!ok)
        return;

    mitk::DataNode::Pointer segFolderNode=NULL;
    if(selectedNode.IsNotNull())
    {
        mitk::DataStorage::SetOfObjects::ConstPointer rs = m_DataStorage->GetSources(selectedNode);
        if(rs->size()>0){
            segFolderNode=rs->GetElement(0);
            if(segFolderNode.IsNotNull())
                segFolderNode->SetFloatProperty("point.displaysize",size);
        }
    }

    for(int i=0;i<selectedNodes.size();i++)
    {
        mitk::DataNode::Pointer node = selectedNodes[i];
        if(!isGroup->CheckNode(node))
            continue;

        node->SetFloatProperty("point.displaysize",size);
        svContourGroup* group=dynamic_cast<svContourGroup*>(node->GetData());
        if(group)
        {
            group->SetProp("point 2D display size",QString::number(size).toStdString());
            group->SetDataModified();
        }
    }

}

void svContourGroupPoint2DSizeAction::SetDataStorage(mitk::DataStorage *dataStorage)
{
    m_DataStorage = dataStorage;
}

void svContourGroupPoint2DSizeAction::SetFunctionality(berry::QtViewPart *functionality)
{
    m_Functionality=functionality;
}

