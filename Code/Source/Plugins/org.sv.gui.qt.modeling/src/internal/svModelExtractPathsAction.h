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

#ifndef SVMODELEXTRACTPATHSACTION_H
#define SVMODELEXTRACTPATHSACTION_H

#include <org_sv_gui_qt_modeling_Export.h>
#include "svDataNodeOperationInterface.h"

#include <svmitkIContextMenuAction.h>
#include <mitkDataNode.h>

#include <QObject>
#include <QThread>

class SV_QT_MODELING svModelExtractPathsAction : public QObject, public svmitk::IContextMenuAction
{
    Q_OBJECT
    Q_INTERFACES(svmitk::IContextMenuAction)

public:
    svModelExtractPathsAction();
    ~svModelExtractPathsAction();

    // IContextMenuAction
    void Run(const QList<mitk::DataNode::Pointer> &selectedNodes) override;
    void SetDataStorage(mitk::DataStorage *dataStorage) override;
    void SetSmoothed(bool smoothed) override {}
    void SetDecimated(bool decimated) override {}
    void SetFunctionality(berry::QtViewPart *functionality) override {}

    // Custom functionality
    void SetSourceCapIds(std::vector<int> sourceCapIds);

public slots:
    void UpdateStatus();

private:
    svModelExtractPathsAction(const svModelExtractPathsAction &);
    svModelExtractPathsAction & operator=(const svModelExtractPathsAction &);

    mitk::DataStorage::Pointer m_DataStorage;

    mitk::DataNode::Pointer m_ProjFolderNode;

    svDataNodeOperationInterface* m_Interface;

    std::vector<int> m_SourceCapIds;

    class WorkThread : public QThread
    {
        //      Q_OBJECT
    public:
        WorkThread(mitk::DataStorage::Pointer dataStorage, mitk::DataNode::Pointer selectedNode,
                   std::vector<int> sourceCapIds);

        QString GetStatus(){return m_Status;}

        mitk::DataNode::Pointer GetPathFolderNode(){return m_PathFolderNode;}

        std::vector<mitk::DataNode::Pointer> GetPathNodes(){return m_PathNodes;}

        mitk::DataNode::Pointer GetCenterlinesModelNode() {return m_CenterlinesModelNode;}

        mitk::DataNode::Pointer GetMergedCenterlinesModelNode() {return m_MergedCenterlinesModelNode;}


        mitk::DataNode::Pointer GetSelectedNode(){return m_SelectedNode;}

    private:

        void run();

        mitk::DataNode::Pointer m_SelectedNode;

        mitk::DataStorage::Pointer mm_DataStorage;

        mitk::DataNode::Pointer m_PathFolderNode;

        QString m_Status;

        std::vector<mitk::DataNode::Pointer> m_PathNodes;

        mitk::DataNode::Pointer m_CenterlinesModelNode;

        mitk::DataNode::Pointer m_MergedCenterlinesModelNode;

        std::vector<int> mm_SourceCapIds;

    };

    WorkThread* m_Thread;

};

#endif
