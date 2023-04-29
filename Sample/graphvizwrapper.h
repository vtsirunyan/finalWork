#ifndef GRAPHVIZWRAPPER_H
#define GRAPHVIZWRAPPER_H

#include "MainWindow.h"
#include <map>
#include <QGVNode.h>

namespace Ui {
class MainWindow;
}

class GraphvizWrapper
{

public:
    GraphvizWrapper(MainWindow* window): mw(window)
    {
    }
    void addNode(QString lable , int frequency = 0)
    {
        QGVNode* newNode = mw->_scene->addNode(lable , frequency);
        nodeTable.insert(std::pair<QString,QGVNode*>(lable,newNode));
    }
    void addEdge(QString node1 , QString node2 , int weight)
    {
        mw->_scene->addEdge(nodeTable[node1],nodeTable[node2],QString::number(weight));
    }
    void addEdge(QString node1 , QString node2 , QString label)
    {
        mw->_scene->addEdge(nodeTable[node1],nodeTable[node2], label);
    }
    void addGraphAttribute(QString name , QString value)
    {
        mw->_scene->setGraphAttribute(name, value);
    }
    void addNodeAttribute(QString name , QString value)
    {
        mw->_scene->setNodeAttribute(name, value);
    }
    void draw()
    {
        mw->_scene->applyLayout();
        mw->FitAndChangeShapeToRect();
    }
    void removeNode(QGVNode* node) {
      nodeTable.erase(node->label());
      mw->_scene->deleteNode(node);
    }
    void removeEdge(QGVEdge* edge) {
      mw->_scene->deleteEdge(edge);
    }
    void reset() {
      nodeTable.clear();
    }
private:
    MainWindow* mw;
    std::map<QString,QGVNode*> nodeTable;

};

#endif // GRAPHVIZWRAPPER_H
