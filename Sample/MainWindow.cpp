/***************************************************************
QGVCore Sample
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
#include "MainWindow.h"
#include "moc_MainWindow.cpp"
#include "QGraphicsViewEc.h"
#include "QGVScene.h"
#include "QGVNode.h"
#include "QGVEdge.h"
#include "QGVSubGraph.h"
#include <QMessageBox>

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDockWidget>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDebug>
#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QLabel>

#include "graphvizwrapper.h"
#include "nfatodfaconvertor.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  setup();
  wrapper = new GraphvizWrapper(this);
  connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
  connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
  connect(_scene, SIGNAL(edgeContextMenu(QGVEdge*)), SLOT(edgeContextMenu(QGVEdge*)));
  setupInitialGraph();
}

MainWindow::~MainWindow()
{
}



void MainWindow::setup()
{
  QWidget* centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  _scene = new QGVScene("DEMO", centralWidget);
  graphicsView = new QGraphicsViewEc(centralWidget);
  graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
  graphicsView->setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
  graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
  graphicsView->setScene(_scene);
  QGridLayout* layout = new QGridLayout(centralWidget);
  centralWidget->setLayout(layout);
  layout->addWidget(graphicsView, 0, 0, 4, 2);

  QWidget* alphabetHolder = new QWidget(centralWidget);
  QVBoxLayout* alphLayout = new QVBoxLayout(alphabetHolder);
  alphabetHolder->setLayout(alphLayout);
  QLineEdit* alphabet = new QLineEdit(centralWidget);
  QLabel* alphapbetLabel = new QLabel("Alphabet", alphabetHolder);
  alphLayout->addWidget(alphapbetLabel);
  alphLayout->addWidget(alphabet);
  m_edit = new QTextEdit(centralWidget);
  connect(m_edit, SIGNAL(textChanged()), this, SLOT(parseGraph()));
  m_edit->setAutoFormatting(QTextEdit::AutoBulletList);
  layout->addWidget(alphabetHolder, 0, 2, 1, 2);
  layout->addWidget(m_edit, 2, 2, 1, 2);
  resize(1280, 720);
  addItemButton = new QPushButton("Add Item", this);
  connect(addItemButton, &QPushButton::clicked, this, &MainWindow::addItem);

  dummyButton1 = new QPushButton("Convert to DFA", this);
  connect(dummyButton1, &QPushButton::clicked, this, &MainWindow::dummySlot1);

  dummyButton2 = new QPushButton("Dummy 2", this);
  connect(dummyButton2, &QPushButton::clicked, this, &MainWindow::dummySlot2);

  dummyButton3 = new QPushButton("Dummy 3", this);
  connect(dummyButton3, &QPushButton::clicked, this, &MainWindow::dummySlot3);

  sidebarLayout = new QVBoxLayout();
  sidebarLayout->addWidget(addItemButton);
  sidebarLayout->addWidget(dummyButton1);
  sidebarLayout->addWidget(dummyButton2);
  sidebarLayout->addWidget(dummyButton3);

  QWidget *sidebar = new QWidget(this);
  sidebar->setLayout(sidebarLayout);
  QDockWidget *rightDockWidget = new QDockWidget("Right Sidebar", this);
  rightDockWidget->setWidget(sidebar);
  addDockWidget(Qt::RightDockWidgetArea, rightDockWidget);
}

void MainWindow::drawGraph()
{
    /*
    _scene->loadLayout("digraph test{node [style=filled,fillcolor=white];N1 -> N2;N2 -> N3;N3 -> N4;N4 -> N1;}");
    connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
    connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
    ui->graphicsView->setScene(_scene);
    return;
    */

    //Configure scene attributes
    _scene->setGraphAttribute("label", "DEMO");

    _scene->setGraphAttribute("splines", "ortho");
    _scene->setGraphAttribute("rankdir", "LR");
    //_scene->setGraphAttribute("concentrate", "true"); //Error !
    _scene->setGraphAttribute("nodesep", "0.4");

    _scene->setNodeAttribute("shape", "box");
    _scene->setNodeAttribute("style", "filled");
    _scene->setNodeAttribute("fillcolor", "white");
    //_scene->setNodeAttribute("height", "1.2");
    _scene->setEdgeAttribute("minlen", "3");
    //_scene->setEdgeAttribute("dir", "both");

    //Add some nodes
    QGVNode *node1 = _scene->addNode("B",45);
    //node1->setIcon(QImage(":/icons/Gnome-System-Run-64.png"));
    QGVNode *node2 = _scene->addNode("SERVER0" , 34);
    //node2->setIcon(QImage(":/icons/Gnome-Network-Transmit-64.png"));
   /* QGVNode *node3 = _scene->addNode("SERVER1");
    node3->setIcon(QImage(":/icons/Gnome-Network-Transmit-64.png"));
    QGVNode *node4 = _scene->addNode("USER");
    node4->setIcon(QImage(":/icons/Gnome-Stock-Person-64.png"));
    QGVNode *node5 = _scene->addNode("SWITCH");
    node5->setIcon(QImage(":/icons/Gnome-Network-Server-64.png"));
*/
    //Add some edges
    _scene->addEdge(node1, node2, "TTL")->setAttribute("color", "red");
    _scene->addEdge(node1, node2, "SERIAL");
    /*_scene->addEdge(node1, node3, "RAZ")->setAttribute("color", "blue");
    _scene->addEdge(node2, node3, "SECU");

    _scene->addEdge(node2, node4, "STATUS")->setAttribute("color", "red");

    _scene->addEdge(node4, node3, "ACK")->setAttribute("color", "red");

    _scene->addEdge(node4, node2, "TBIT");
    _scene->addEdge(node4, node2, "ETH");
    _scene->addEdge(node4, node2, "RS232");

    _scene->addEdge(node4, node5, "ETH1");
    _scene->addEdge(node2, node5, "ETH2");

    QGVSubGraph *sgraph = _scene->addSubGraph("SUB1");
    sgraph->setAttribute("label", "OFFICE");

    QGVNode *snode1 = sgraph->addNode("PC0152");
    QGVNode *snode2 = sgraph->addNode("PC0153");

    _scene->addEdge(snode1, snode2, "RT7");

    _scene->addEdge(node3, snode1, "GB8");
    _scene->addEdge(node3, snode2, "TS9");


    QGVSubGraph *ssgraph = sgraph->addSubGraph("SUB2");
    ssgraph->setAttribute("label", "DESK");
    _scene->addEdge(snode1, ssgraph->addNode("PC0155"), "S10");
*/
    //Layout scene
    _scene->applyLayout();

    //Fit in view
    graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);

}

void MainWindow::FitAndChangeShapeToRect()
{
  //graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::setupInitialGraph()
{
  wrapper->addNode("q1");
  wrapper->addNode("q2");
  wrapper->addNode("q3");
  wrapper->addNode("q4");
  wrapper->addNode("q5");
  wrapper->addNode("q6");

  //Add Edges Like This
  wrapper->addEdge("q1", "q2", 1);
  wrapper->addEdge("q1", "q4", 0);
  wrapper->addEdge("q2", "q1", "1");
  wrapper->addEdge("q2", "q3", "0");
  wrapper->addEdge("q3", "q3", 1);
  wrapper->addEdge("q3", "q6", 0);
  wrapper->addEdge("q4", "q4", 1);
  wrapper->addEdge("q4", "q5", 0);
  wrapper->addEdge("q5", "q4", 1);
  wrapper->addEdge("q5", "q6", 0);

  //Add Attributes Like This
  wrapper->addGraphAttribute("rankdir", "LR");
  wrapper->addGraphAttribute("background", "red");
  wrapper->addNodeAttribute("style", "filled");
  wrapper->addNodeAttribute("fillcolor", "white");
  wrapper->addNodeAttribute("height", "0.1");
  wrapper->addNodeAttribute("width", "0.1");
  wrapper->addNodeAttribute("margin", "0.1");
  wrapper->addNodeAttribute("shape", "circle");

  wrapper->addNodeAttribute("overlap", "scalexy");
  //wrapper->addNodeAttribute("shape", "box");
  wrapper->addNodeAttribute("style", "filled");
  wrapper->addNodeAttribute("color", "green");
  _scene->setEdgeAttribute("minlen", "3");
  //_scene->getNodeAttribute("width");
  //_scene->getNodeAttribute("height");
  _scene->applyLayout();
  FitAndChangeShapeToRect();
}

void MainWindow::nodeContextMenu(QGVNode *node)
{
    //Context menu exemple
    QMenu menu(node->label());

    menu.addSeparator();
    menu.addAction(tr("Delete"));
    menu.addAction(tr("Options"));

    QAction *action = menu.exec(QCursor::pos());
    if(action == 0)
        return;
    qDebug() << action->text();
    if(action->text() == "Delete") {
      wrapper->removeNode(node);
    }
}

void MainWindow::nodeDoubleClick(QGVNode *node)
{
  QMessageBox::information(this, tr("Node double clicked"), tr("Node %1").arg(node->label()));
}

void MainWindow::edgeContextMenu(QGVEdge *edge)
{
  //Context menu exemple
  QMenu menu(edge->label());

  menu.addSeparator();
  menu.addAction(tr("Delete"));
  menu.addAction(tr("Options"));

  QAction *action = menu.exec(QCursor::pos());
  if(action == 0)
      return;
  qDebug() << action->text();
  if(action->text() == "Delete") {
    wrapper->removeEdge(edge);
  }
}

void MainWindow::addItem()
{
 wrapper->addNode(QString::number(_scene->getNodes().size()), 0);
 _scene->applyLayout();
}

void MainWindow::dummySlot1()
{
  qDebug() << m_edit->toPlainText();
  NfaToDfaConvertor conv;
  conv.setNfa(m_edit->toPlainText());
  conv.parse();
  m_edit->setText(conv.getDfaTest());
}

void MainWindow::dummySlot2()
{
  _scene->applyLayout();
}

void MainWindow::dummySlot3() {
    QDialog messageBox(this);
    messageBox.setWindowTitle("Create Edge");

    QWidget *content = new QWidget(&messageBox);
    QVBoxLayout *contentLayout = new QVBoxLayout(content);

    QComboBox *fromComboBox = new QComboBox(content);
    QComboBox *toComboBox = new QComboBox(content);
    for (auto *item : _scene->getNodes()) {
        fromComboBox->addItem(item->label(), QVariant::fromValue((void*)item));
        toComboBox->addItem(item->label(), QVariant::fromValue((void*)item));
    }

    contentLayout->addWidget(fromComboBox);
    contentLayout->addWidget(toComboBox);
    content->setLayout(contentLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &messageBox);
    connect(buttonBox, &QDialogButtonBox::accepted, &messageBox, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &messageBox, &QDialog::reject);
    QHBoxLayout lout;
    messageBox.setLayout(&lout);
    messageBox.layout()->addWidget(content);
    messageBox.layout()->addWidget(buttonBox);

    if (messageBox.exec() == QDialog::Accepted) {
        QGVNode *fromItem = static_cast<QGVNode*>(fromComboBox->currentData().value<void*>());
        QGVNode *toItem = static_cast<QGVNode*>(toComboBox->currentData().value<void*>());
        wrapper->addEdge(fromItem->label(), toItem->label(), "12");
        _scene->applyLayout();
      }
}

void MainWindow::parseGraph()
{
  QTextEdit* edit = m_edit;
  QStringList lines = edit->toPlainText().split("\n");
  QRegExp exp(" +");
  QSet<QString> nodeNames;
  _scene->reset();
  wrapper->reset();
  for (QString line : lines) {
    QStringList words = line.split(exp);
    qDebug () << words;
    qDebug() << words.size();
    qDebug () << "words.size() == 4" << (words.size() == 4);
    if(words.size() == 4) {
        qDebug() << "words[3] != \"\"" << (words[3] != "");
        qDebug() << "words[1] == \"->\"" << (words[1] == "->");

      }
    if(words.size() == 4 && words[3] != "" && words[1] == "->") {
      QString from = words[0];
      QString to = words[2];
      QString value = words[3].trimmed();
      if(nodeNames.find(from) == nodeNames.end()) {
        nodeNames.insert(from);
        wrapper->addNode(from);
      }
      if(nodeNames.find(to) == nodeNames.end()) {
        nodeNames.insert(to);
        wrapper->addNode(to);
      }
      wrapper->addEdge(from, to, value);
    } else {
        qDebug() << "invalid line";
    }
  }
  _scene->applyLayout();
}
