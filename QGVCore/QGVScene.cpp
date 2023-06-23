/***************************************************************
QGVCore
Copyright (c) 2014, Bergont Nicolas, All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.
***************************************************************/
#include "QGVScene.h"
// The following include allows the automoc to detect, that it must moc this class
#include "moc_QGVScene.cpp"
#include <QDebug>

#include <QGVNode.h>
#include <QGVEdge.h>
#include <QGVSubGraph.h>

#include <QGVCore.h>
#include <QGVGraphPrivate.h>
#include <QGVGvcPrivate.h>
#include <QGVEdgePrivate.h>
#include <QGVNodePrivate.h>
#include <iostream>
#include <QDebug>

QGVScene::QGVScene(const QString &name, QObject *parent) : QGraphicsScene(parent)
{
    _context = new QGVGvcPrivate(gvContext());
    _graph = new QGVGraphPrivate(agopen(name.toLocal8Bit().data(), Agdirected, NULL));
    //setGraphAttribute("fontname", QFont().family());
}

QGVScene::~QGVScene()
{
    gvFreeLayout(_context->context(), _graph->graph());
    agclose(_graph->graph());
    gvFreeContext(_context->context());
    delete _graph;
    delete _context;
}

void QGVScene::setGraphAttribute(const QString &name, const QString &value)
{
    agattr(_graph->graph(), AGRAPH, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVScene::setNodeAttribute(const QString &name, const QString &value)
{
    agattr(_graph->graph(), AGNODE, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVScene::setEdgeAttribute(const QString &name, const QString &value)
{
  agattr(_graph->graph(), AGEDGE, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVScene::getGraphAttribute(const QString &name)
{
  auto sym = agattr(_graph->graph(), AGRAPH, name.toLocal8Bit().data(), 0);
  if (sym)
  printf("The default value for %s is %s.\n", name.toLocal8Bit().data(), sym->defval);
}

void QGVScene::getNodeAttribute(const QString &name)
{
  auto sym = agattr(_graph->graph(), AGNODE, name.toLocal8Bit().data(), 0);
  if (sym)
  printf("The default value for %s is %s.\n", name.toLocal8Bit().data(), sym->defval);

  qDebug() << "Dumping all";
  sym = 0; /* to get the first one */
  while (sym = agnxtattr(_graph->graph(),AGNODE,sym)) {
    qDebug() << "Name is " << sym->name << " val is " << sym->defval;
  }
}

void QGVScene::getEdgeAttribute(const QString &name)
{
  auto sym = agattr(_graph->graph(), AGEDGE, name.toLocal8Bit().data(), 0);
  if (sym)
    printf("The default value for %s is %s.\n", name.toLocal8Bit().data(), sym->defval);
}

void QGVScene::reset()
{
 for (auto edge : _edges) {
     deleteEdge(edge);
 }
 for (auto node : _nodes) {
     deleteNode(node);
 }
 _edges.clear();
 _nodes.clear();
 }

QGVNode *QGVScene::addNode(const QString &label , int frequency)
{
    Agnode_t *node = agnode(_graph->graph(), NULL, TRUE);
    if(node == NULL)
    {
        qWarning()<<"Invalid node :"<<label;
        return 0;
    }
    QGVNode *item = new QGVNode(new QGVNodePrivate(node, _graph->graph()), this);
    item->setLabel(label);
    item->alable = label;
    item->frequency = frequency;
    addItem(item);
    _nodes.append(item);
    return item;
}

QGVEdge *QGVScene::addEdge(QGVNode *source, QGVNode *target, const QString &label)
{
    Agedge_t* edge = agedge(_graph->graph(), source->_node->node(), target->_node->node(), NULL, TRUE);
    if(edge == NULL)
    {
        qWarning()<<"Invalid egde :"<<label;
        return 0;
    }

    QGVEdge *item = new QGVEdge(new QGVEdgePrivate(edge), this);
    item->setFrom(source);
    item->setTo(target);
    item->setLabel(label);
    addItem(item);
    _edges.append(item);
    return item;
}

QGVSubGraph *QGVScene::addSubGraph(const QString &name, bool cluster)
{
    Agraph_t* sgraph;
    if(cluster)
        sgraph = agsubg(_graph->graph(), ("cluster_" + name).toLocal8Bit().data(), TRUE);
    else
        sgraph = agsubg(_graph->graph(), name.toLocal8Bit().data(), TRUE);

    if(sgraph == NULL)
    {
        qWarning()<<"Invalid subGraph :"<<name;
        return 0;
    }

    QGVSubGraph *item = new QGVSubGraph(new QGVGraphPrivate(sgraph), this);
    addItem(item);
    _subGraphs.append(item);
    return item;
}

void QGVScene::setInitial(QString nodeName, bool val)
{
  for(auto node : _nodes) {
    if(node->label() == nodeName) {
        node->setIs_initial(val);
    }
  }
}

void QGVScene::setTerminal(QString nodeName, bool val)
{
  for(auto node : _nodes) {
    if(node->label() == nodeName) {
        node->setIs_final(val);
    }
  }
}

void QGVScene::deleteNode(QGVNode* node)
{
    QList<QGVNode *>::iterator it = std::find(_nodes.begin(), _nodes.end(), node);
    if(it == _nodes.end())
    {
        std::cout << "Error, node not part of Scene" << std::endl;
        return;
    }
    QVector<QGVEdge*> toBeDeleted;
    for(QGVEdge* edge : _edges) {
      if(edge->getFrom() == *it || edge->getTo() == *it) {
        //qDebug() << "Removed edge from" << edge->getFrom()->label() << " to " << edge->getTo()->label();
        toBeDeleted.push_back(edge);
      }
    }
    for(auto edge : toBeDeleted) {
      deleteEdge(edge);
    }
//    qDebug() << "we are left with following edges";
//    for(QGVEdge* edge : _edges) {
//        qDebug() << edge->getFrom()->label() << " to " << edge->getTo()->label() << " value " << edge->label();
//    }
    qDebug() << "delNode ret " << agdelnode(node->_node->graph(), node->_node->node());
    _nodes.erase(it);
    delete node;
    applyLayout();
}

void QGVScene::deleteEdge(QGVEdge* edge)
{
    qDebug() << "delEdge ret " << agdeledge(_graph->graph(), edge->_edge->edge());
    QList<QGVEdge *>::iterator it = std::find(_edges.begin(), _edges.end(), edge);
    if(it == _edges.end())
    {
        qDebug() << "Error, QGVEdge not part of Scene";
        return;
    }
    _edges.erase(it);
    delete edge;
}

void QGVScene::deleteSubGraph(QGVSubGraph *subgraph)
{
    std::cout << "Removing sug " << subgraph->_sgraph->graph() << std::endl;
    std::cout << "delSubg ret " << agclose(subgraph->_sgraph->graph()) << std::endl;
    QList<QGVSubGraph *>::iterator it = std::find(_subGraphs.begin(), _subGraphs.end(), subgraph);
    if(it == _subGraphs.end())
    {
        std::cout << "Error, QGVSubGraph not part of Scene" << std::endl;
        return;
    }
    _subGraphs.erase(it);

    delete subgraph;
}

void QGVScene::setRootNode(QGVNode *node)
{
    Q_ASSERT(_nodes.contains(node));
    char root[] = "root";
    agset(_graph->graph(), root, node->label().toLocal8Bit().data());
}

void QGVScene::loadLayout(const QString &text)
{
    _graph->setGraph(QGVCore::agmemread2(text.toLocal8Bit().constData()));

    if(gvLayout(_context->context(), _graph->graph(), "dot") != 0)
    {
        qCritical()<<"Layout render error"<<agerrors()<<QString::fromLocal8Bit(aglasterr());
        return;
    }

    //Debug output
    //gvRenderFilename(_context->context(), _graph->graph(), "png", "debug.png");

    //Read nodes and edges
    for (Agnode_t* node = agfstnode(_graph->graph()); node != NULL; node = agnxtnode(_graph->graph(), node))
    {
        QGVNode *inode = new QGVNode(new QGVNodePrivate(node, _graph->graph()), this);
        inode->updateLayout();
        addItem(inode);
        for (Agedge_t* edge = agfstout(_graph->graph(), node); edge != NULL; edge = agnxtout(_graph->graph(), edge))
        {
            QGVEdge *iedge = new QGVEdge(new QGVEdgePrivate(edge), this);
            iedge->updateLayout();
            addItem(iedge);
        }

    }
    update();
}

void QGVScene::applyLayout()
{
    if(gvLayout(_context->context(), _graph->graph(), "dot") != 0)
    {
        /*
         * If crash here:
         * - Check that the dlls are in the execution directory
         * - Check that the "configN" file is in the execution directory!
         */
        qCritical()<<"Layout render error"<<agerrors()<<QString::fromLocal8Bit(aglasterr());
        return;
    }
_graph->graph();
    //Debug output
    gvRenderFilename(_context->context(), _graph->graph(), "canon", "debug.dot");
    gvRenderFilename(_context->context(), _graph->graph(), "png", "debug.png");

    //Update items layout
    foreach(QGVNode* node, _nodes)
        node->updateLayout();

    foreach(QGVEdge* edge, _edges)
        edge->updateLayout();

    foreach(QGVSubGraph* sgraph, _subGraphs)
        sgraph->updateLayout();

    //Graph label
    textlabel_t *xlabel = GD_label(_graph->graph());
    if(xlabel)
    {
        QGraphicsTextItem *item = addText(xlabel->text);
        item->setPos(QGVCore::centerToOrigin(QGVCore::toPoint(xlabel->pos, QGVCore::graphHeight(_graph->graph())), xlabel->dimen.x, -4));
    }

gvFreeLayout(_context->context(), _graph->graph());
    update();
}

void QGVScene::clear()
{
    gvFreeLayout(_context->context(), _graph->graph());
    _nodes.clear();
    _edges.clear();
    _subGraphs.clear();
    QGraphicsScene::clear();
}

#include <QGraphicsSceneContextMenuEvent>
void QGVScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    QGraphicsItem *item = itemAt(contextMenuEvent->scenePos(), QTransform());
    if(item)
    {
        item->setSelected(true);
        if(item->type() == QGVNode::Type)
            emit nodeContextMenu(qgraphicsitem_cast<QGVNode*>(item));
        else if(item->type() == QGVEdge::Type)
            emit edgeContextMenu(qgraphicsitem_cast<QGVEdge*>(item));
        else if(item->type() == QGVSubGraph::Type)
            emit subGraphContextMenu(qgraphicsitem_cast<QGVSubGraph*>(item));
        else
            emit graphContextMenuEvent();
    }
    QGraphicsScene::contextMenuEvent(contextMenuEvent);
}

void QGVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
    if(item)
    {
        if(item->type() == QGVNode::Type)
            emit nodeDoubleClick(qgraphicsitem_cast<QGVNode*>(item));
        else if(item->type() == QGVEdge::Type)
            emit edgeDoubleClick(qgraphicsitem_cast<QGVEdge*>(item));
        else if(item->type() == QGVSubGraph::Type)
            emit subGraphDoubleClick(qgraphicsitem_cast<QGVSubGraph*>(item));
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

#include <QVarLengthArray>
#include <QPainter>
void QGVScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    const int gridSize = 25;

    const qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    const qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->setRenderHint(QPainter::Antialiasing, false);

    painter->setPen(QColor(Qt::lightGray).lighter(110));
    painter->drawLines(lines.data(), lines.size());
    painter->setPen(Qt::black);
    //painter->drawRect(sceneRect());
}
