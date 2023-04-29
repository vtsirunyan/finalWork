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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QGVScene.h"

class GraphvizWrapper;
class QGraphicsViewEc;
class QPushButton;
class QVBoxLayout;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void drawGraph();
    void setup();
    void FitAndChangeShapeToRect();
    void setupInitialGraph();
private slots:
  void nodeContextMenu(QGVNode* node);
  void nodeDoubleClick(QGVNode* node);
  void edgeContextMenu(QGVEdge* node);
  void addItem();
  void dummySlot1();
  void dummySlot2();
  void dummySlot3();
  void parseGraph();
public:
    QGVScene *_scene;
    GraphvizWrapper* wrapper;
    QGraphicsViewEc* graphicsView;
    QVBoxLayout* sidebarLayout;
    QPushButton* addItemButton;
    QPushButton* dummyButton1;
    QPushButton* dummyButton2;
    QPushButton* dummyButton3;
    QTextEdit* m_edit;
};

#endif // MAINWINDOW_H
