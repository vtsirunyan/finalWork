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
#include <QMenuBar>
#include <QTabWidget>
#include <QFileDialog>
#include <QDir>
#include <QSplitter>

#include "graphvizwrapper.h"
#include "nfatodfa.h"
#include "automata.h"
#include "RLGparser.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          m_file(nullptr)
{
  setup();
  wrapper = new GraphvizWrapper(this);
  connect(_scene, SIGNAL(nodeContextMenu(QGVNode *)), SLOT(nodeContextMenu(QGVNode *)));
  connect(_scene, SIGNAL(nodeDoubleClick(QGVNode *)), SLOT(nodeDoubleClick(QGVNode *)));
  connect(_scene, SIGNAL(edgeContextMenu(QGVEdge *)), SLOT(edgeContextMenu(QGVEdge *)));
  setupInitialGraph();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setup()
{

  // setup menu
  setWindowTitle("Finite automaton visualisator");
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  QAction *saveAction = new QAction(tr("&Save"), this);
  saveAction->setShortcuts(QKeySequence::Save);
  fileMenu->addAction(saveAction);
  QAction *saveAsAction = new QAction(tr("Save &As..."), this);
  saveAsAction->setShortcuts(QKeySequence::SaveAs);
  fileMenu->addAction(saveAsAction);
  QAction *loadAction = new QAction(tr("&Open"), this);
  loadAction->setShortcuts(QKeySequence::Open);
  fileMenu->addAction(loadAction);

  connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
  connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
  connect(loadAction, &QAction::triggered, this, &MainWindow::loadFile);
  //
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  _scene = new QGVScene("DEMO", centralWidget);
  graphicsView = new QGraphicsViewEc(centralWidget);
  graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
  graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
  graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
  graphicsView->setScene(_scene);
  QGridLayout *layout = new QGridLayout(centralWidget);
  centralWidget->setLayout(layout);
  layout->addWidget(graphicsView, 0, 0, 4, 6);
  m_mainTabWidget = new QTabWidget(centralWidget);
  layout->addWidget(m_mainTabWidget, 0, 6, 1, 1);

  QWidget *firstTab = new QWidget(m_mainTabWidget);
  QVBoxLayout *firstTabLayout = new QVBoxLayout(firstTab);
  QWidget *alphabetHolder = new QWidget(centralWidget);
  QVBoxLayout *alphLayout = new QVBoxLayout(alphabetHolder);
  alphabetHolder->setLayout(alphLayout);
  m_main_alphabet = new QLineEdit(centralWidget);
  QLabel *alphapbetLabel = new QLabel("Alphabet", alphabetHolder);
  alphLayout->addWidget(alphapbetLabel);
  alphLayout->addWidget(m_main_alphabet);
  m_edit = new QTextEdit(centralWidget);
  connect(m_edit, SIGNAL(textChanged()), this, SLOT(parseGraph()));
  connect(m_main_alphabet, SIGNAL(textChanged(const QString &)), this, SLOT(parseGraph()));
  firstTabLayout->addWidget(alphabetHolder);
  firstTabLayout->addWidget(m_edit);
  m_mainTabWidget->addTab(firstTab, "Graph");

  QWidget *secondTab = new QWidget(m_mainTabWidget);
  m_mainTabWidget->addTab(secondTab, "RLG");
  QVBoxLayout *secondTabLayout = new QVBoxLayout(secondTab);
  secondTab->setLayout(secondTabLayout);
  QWidget *secondAlphabetHolder = new QWidget(centralWidget);
  QVBoxLayout *secondAlphLayout = new QVBoxLayout(secondAlphabetHolder);
  alphabetHolder->setLayout(alphLayout);
  m_rlg_alphabet = new QLineEdit(centralWidget);
  QLabel *secondAlphapbetLabel = new QLabel("Alphabet", secondAlphabetHolder);
  secondAlphLayout->addWidget(secondAlphapbetLabel);
  secondAlphLayout->addWidget(m_rlg_alphabet);
  m_RLGEdit = new QTextEdit(centralWidget);
  connect(m_RLGEdit, SIGNAL(textChanged()), this, SLOT(parseRLG()));
  connect(m_rlg_alphabet, SIGNAL(textChanged(const QString &)), this, SLOT(parseRLG()));
  secondTabLayout->addWidget(secondAlphabetHolder);
  secondTabLayout->addWidget(m_RLGEdit);

  //  QWidget* errorBox = new QWidget(centralWidget);
  QLabel *errorBoxLabel = new QLabel("ERROR LOG", this);
  //  secondAlphLayout->addWidget(secondAlphapbetLabel);

  QWidget *thirdTab = new QWidget(m_mainTabWidget);
  layout->addWidget(thirdTab, 1, 6, 1, 1);
  // m_mainTabWidget->addTab(thirdTab, "ERROR LOG");
  QVBoxLayout *thirdTabLayout = new QVBoxLayout(thirdTab);
  thirdTab->setLayout(thirdTabLayout);
  thirdTabLayout->addWidget(errorBoxLabel);
  m_ERROR_LOG = new QTextEdit(centralWidget);
  thirdTabLayout->addWidget(m_ERROR_LOG);

  QWidget *forthTab = new QWidget(m_mainTabWidget);
  m_mainTabWidget->addTab(forthTab, "Deduction");
  QVBoxLayout *forthTabLayout = new QVBoxLayout(forthTab);
  forthTab->setLayout(forthTabLayout);
  QWidget *forthAlphabetHolder = new QWidget(centralWidget);
  QVBoxLayout *forthAlphLayout = new QVBoxLayout(forthAlphabetHolder);
  alphabetHolder->setLayout(alphLayout);
  m_deduction_word = new QLineEdit(centralWidget);
  QLabel *forthAlphapbetLabel = new QLabel("Word", forthAlphabetHolder);
  forthAlphLayout->addWidget(forthAlphapbetLabel);
  forthAlphLayout->addWidget(m_deduction_word);
  m_Deduction = new QTextEdit(centralWidget);
  forthTabLayout->addWidget(forthAlphabetHolder);
  forthTabLayout->addWidget(m_Deduction);
  connect(m_deduction_word, SIGNAL(textChanged(const QString &)), this, SLOT(getDeduction()));

  resize(1280, 720);
  // TODO make it work
  dummyButton1 = new QPushButton("Convert to DFA", this);
  connect(dummyButton1, &QPushButton::clicked, this, &MainWindow::dummySlot1);
  dummyButton3 = new QPushButton("Export to...", this);
  connect(dummyButton3, &QPushButton::clicked, this, &MainWindow::dummySlot3);

  sidebarLayout = new QVBoxLayout();
  sidebarLayout->addWidget(dummyButton1);
  sidebarLayout->addWidget(dummyButton3);

  QWidget *sidebar = new QWidget(this);
  sidebar->setLayout(sidebarLayout);
  QDockWidget *rightDockWidget = new QDockWidget("Right Sidebar", this);
  rightDockWidget->setWidget(sidebar);
  addDockWidget(Qt::RightDockWidgetArea, rightDockWidget);
}

void MainWindow::drawGraph()
{

  // Configure scene attributes
  _scene->setGraphAttribute("label", "DEMO");

  _scene->setGraphAttribute("splines", "ortho");
  _scene->setGraphAttribute("rankdir", "LR");
  _scene->setGraphAttribute("nodesep", "0.4");

  _scene->setNodeAttribute("shape", "box");
  _scene->setNodeAttribute("style", "filled");
  _scene->setNodeAttribute("fillcolor", "white");
  _scene->setEdgeAttribute("minlen", "3");

  // Add some nodes
  QGVNode *node1 = _scene->addNode("B", 45);
  QGVNode *node2 = _scene->addNode("SERVER0", 34);

  // Add some edges
  _scene->addEdge(node1, node2, "TTL")->setAttribute("color", "red");
  _scene->addEdge(node1, node2, "SERIAL");

  // Layout scene
  _scene->applyLayout();
}

void MainWindow::FitAndChangeShapeToRect()
{
}

void MainWindow::setupInitialGraph()
{
  wrapper->addNode("q1");
  wrapper->addNode("q2");
  wrapper->addNode("q3");
  wrapper->addNode("q4");
  wrapper->addNode("q5");
  wrapper->addNode("q6");

  // Add Edges Like This
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

  // Add Attributes Like This
  wrapper->addGraphAttribute("rankdir", "LR");
  wrapper->addGraphAttribute("background", "red");
  wrapper->addNodeAttribute("style", "filled");
  wrapper->addNodeAttribute("fillcolor", "white");
  wrapper->addNodeAttribute("height", "0.1");
  wrapper->addNodeAttribute("width", "0.1");
  wrapper->addNodeAttribute("margin", "0.1");
  wrapper->addNodeAttribute("shape", "circle");

  wrapper->addNodeAttribute("overlap", "scalexy");
  wrapper->addNodeAttribute("style", "filled");
  wrapper->addNodeAttribute("color", "green");
  _scene->setEdgeAttribute("minlen", "3");
  _scene->applyLayout();
  FitAndChangeShapeToRect();
}

void MainWindow::nodeContextMenu(QGVNode *node)
{
  // Context menu exemple
  QMenu menu(node->label());

  menu.addSeparator();
  menu.addAction(tr("toggle final"));
  menu.addAction(tr("toggle start"));

  QAction *action = menu.exec(QCursor::pos());
  if (action == 0)
    return;
  qDebug() << action->text();
  if (action->text() == "toggle final")
  {
    node->setIs_final(!node->getIs_final());
  }
  else if (action->text() == "toggle start")
  {
    node->setIs_initial(!node->getIs_initial());
  }
  _scene->applyLayout();
}

void MainWindow::nodeDoubleClick(QGVNode *node)
{
  QMessageBox::information(this, tr("Node double clicked"), tr("Node %1").arg(node->label()));
}

void MainWindow::edgeContextMenu(QGVEdge *edge)
{
  // Context menu exemple
  QMenu menu(edge->label());

  menu.addSeparator();
  menu.addAction(tr("Delete"));
  menu.addAction(tr("Options"));

  QAction *action = menu.exec(QCursor::pos());
  if (action == 0)
    return;
  qDebug() << action->text();
  if (action->text() == "Delete")
  {
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
  if (m_edit->toPlainText().trimmed().isEmpty())
  {
    return;
  }
  NFA nfa(m_edit->toPlainText());
  printNFA(nfa);
  DFA dfa = convertNFAtoDFA(nfa);
  printDFA(dfa);
  m_edit->setText(toText(dfa));
}

void MainWindow::dummySlot2()
{
  qDebug() << m_edit->toPlainText();
  RLGParser parser;
  parser.setRLG(m_RLGEdit->toPlainText());
  m_edit->setText(parser.parse());
}

void MainWindow::dummySlot3()
{
  QDialog messageBox(this);
  messageBox.setWindowTitle("Create Edge");

  QWidget *content = new QWidget(&messageBox);
  QVBoxLayout *contentLayout = new QVBoxLayout(content);

  QComboBox *fromComboBox = new QComboBox(content);
  QComboBox *toComboBox = new QComboBox(content);
  for (auto *item : _scene->getNodes())
  {
    fromComboBox->addItem(item->label(), QVariant::fromValue((void *)item));
    toComboBox->addItem(item->label(), QVariant::fromValue((void *)item));
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

  if (messageBox.exec() == QDialog::Accepted)
  {
    QGVNode *fromItem = static_cast<QGVNode *>(fromComboBox->currentData().value<void *>());
    QGVNode *toItem = static_cast<QGVNode *>(toComboBox->currentData().value<void *>());
    wrapper->addEdge(fromItem->label(), toItem->label(), "12");
    _scene->applyLayout();
  }
}

void MainWindow::parseGraph()
{
  if(m_mainTabWidget->currentIndex() != 0) {
    qDebug() << m_mainTabWidget->tabBar()->tabText(m_mainTabWidget->currentIndex());
    return;
  }

  _scene->reset();
  wrapper->reset();
  m_ERROR_LOG->clear();
  QSet<QChar> alphabet;
  QRegularExpression exp(" +");
  qDebug() << m_main_alphabet->text() << " 111 " <<  m_main_alphabet ->displayText();
  QStringList chars;
  if (!m_main_alphabet->text().isEmpty()) chars = m_main_alphabet->text().trimmed().split(exp);
  for(QString c : chars) {
    if(c.size() != 1) {
      m_ERROR_LOG->setText(m_ERROR_LOG->toPlainText() + "Invalid alphabet letter \"" + c + "\". Please use single characters for alphabet.\n");
    } else {
      alphabet.insert(c[0]);
    }
  }
  QTextEdit* edit = m_edit;
  QStringList lines = edit->toPlainText().trimmed().split("\n");
  if (edit->toPlainText().trimmed().isEmpty()) return;
  qDebug() << "Lines: " << lines << '\n';
  QString startingState = lines[0].trimmed();
  if (startingState.contains(' ')) {
    m_ERROR_LOG->setText(m_ERROR_LOG->toPlainText() + "Starting state without whitespaces should be specified on the first line.\n");
    return;
  }
  if (startingState.length() == 1 && alphabet.contains(startingState[0])) {
    m_ERROR_LOG->setText(m_ERROR_LOG->toPlainText() + "Can't use a symbol from alphabet for a starting state.\n");
    return;
  }

  QSet<QString> nodeNames;

  qDebug() << "Starting state: " << startingState << '\n';
  if (lines.size() > 1) {
    bool usedStartingState = false;
    QString line2 = lines[1].trimmed();
    QStringList line2Tokens;
    if (line2 != "") {
      line2Tokens = line2.split(exp);
    }
    qDebug() << line2;
    QSet<QString> usedTerminals;
    QSet<QString> terminals(line2Tokens.begin(), line2Tokens.end());
    qDebug() << "Terminals initially is: " << terminals << '\n';

    for (int i = 2; i < lines.size(); ++i) {
      QString line = lines[i];
      QStringList words = line.split(exp);
      if(words.size() == 4 && words[3] != "" && words[1] == "->") {
        QString from = words[0];
        QString to = words[2];
        QString value = words[3].trimmed();
        bool isValidLine = true;
        for (QChar ccc : value) {
          if(ccc != 'e' && alphabet.find(ccc) == alphabet.end()) {
            m_ERROR_LOG->setText(m_ERROR_LOG->toPlainText() + "Invalid line " + QString::number(i) + "  \"" + ccc + "\" is not from the alphabet.\n");
            isValidLine = false;
            break;
          }
        }
        if (!isValidLine) continue;
        if(nodeNames.find(from) == nodeNames.end()) {
          nodeNames.insert(from);
          if (terminals.contains(from)) {
              terminals.remove(from);
              usedTerminals.insert(from);
          }
          wrapper->addNode(from);
          if (from == startingState) usedStartingState = true;
        }
        if(nodeNames.find(to) == nodeNames.end()) {
          nodeNames.insert(to);
          if (terminals.contains(to)) {
              terminals.remove(to);
              usedTerminals.insert(to);
          }
          wrapper->addNode(to);
          if (to == startingState) usedStartingState = true;
        }
        wrapper->addEdge(from, to, value);
      } else {
          m_ERROR_LOG->setText(m_ERROR_LOG->toPlainText() + "Invalid line " + QString::number(i) + "  \"" + line + "\" is not in format \'From -> To Char\'.\n");
      }
    }
    if (!usedStartingState) terminals.insert(startingState);
    if (!terminals.empty()) {
      qDebug() << "line2Tokens is: " << line2Tokens << " Terminals is: " << terminals << '\n';
      QString message = m_ERROR_LOG->toPlainText() + "Some states are declared but not used: ";
      for (QString state : terminals) {
        message += state;
        message += ' ';
      }
      message += ".\n";
      m_ERROR_LOG->setText(message);
    }
    for (auto xasdfag : usedTerminals) {
      _scene->setTerminal(xasdfag, true);
    }
  } else if (lines.size() == 1) {

    if (!nodeNames.contains(startingState)) {
      nodeNames.insert(startingState);
      wrapper->addNode(startingState);
    }
  }
  _scene->setInitial(startingState, true);
  _scene->applyLayout();
}

void MainWindow::parseRLG()
{
  RLGParser parser;
  parser.setRLG(m_RLGEdit->toPlainText());
  parser.setALPHA(m_rlg_alphabet->text());
  QString parsedRLG = parser.parse();
  if (parsedRLG.size() > 7 && parsedRLG.left(7) == "[ERROR]") {
      m_ERROR_LOG->setText(parsedRLG);
  } else {
    m_ERROR_LOG->setText("");
    m_edit->setText(parsedRLG);
    m_main_alphabet->setText(m_rlg_alphabet->text());
  }
}

void MainWindow::getDeduction()
{
  Automata automata(m_edit->toPlainText(), m_main_alphabet->text());
  qDebug() << "tf yo = " << '\n';
  QString token = m_deduction_word->text().trimmed();
  qDebug() << "Token = " << token << '\n';
  m_Deduction->setText(automata.findDeduction(token));
//  qDebug() << "WTF is it crashing" << '\n';
}

void MainWindow::saveFile()
{
  if (m_file != nullptr)
  {
    if (m_file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream out(m_file);
      if (m_rlg)
      {
        out << m_rlg_alphabet->text() << "\n"
            << m_RLGEdit->toPlainText();
      }
      else
      {
        out << m_main_alphabet->text() << "\n"
            << m_edit->toPlainText();
      }
      m_file->close();
    }
  }
  else
  {
    saveAsFile();
  }
}

void MainWindow::saveAsFile()
{
  QString filter = "PNG Files (*.png);;RightLinearGrammar Files (*.rlg);;TransitionGraph Files (*.tg)";
  QString fileName = QFileDialog::getSaveFileName(this, "Save as", QDir::homePath(), filter);
  if (!fileName.isEmpty())
  {
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix().toLower();
    if (suffix == "rlg" || suffix == "tg")
    {
      if (m_file == nullptr)
      {
        m_file = new QFile(fileName);
        m_rlg = suffix == "rlg";
      }
      if (m_file->open(QIODevice::WriteOnly | QIODevice::Text))
      {
        QTextStream out(m_file);
        if (suffix == "rlg")
        {
          out << m_rlg_alphabet->text() << "\n"
              << m_RLGEdit->toPlainText();
        }
        else
        {
          out << m_main_alphabet->text() << "\n"
              << m_edit->toPlainText();
        }
        m_file->close();
      }
      else
      {
        // Handle error here
        qDebug() << "Could not open file for writing";
      }
    }
    qDebug() << "Chosen file:" << fileName;
  }
}
void MainWindow::loadFile()
{
  QString filter = "RLG Files (*.rlg);;TG Files (*.tg)";
  QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), filter);

  if (!fileName.isEmpty())
  {
    if(m_file) {
      delete m_file;
    }

    m_file = new QFile(fileName);
    QString suffix = QFileInfo(fileName).suffix().toLower();
    if (m_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QTextStream in(m_file);
      QString firstLine = in.readLine();
      QString rest = in.readAll();
      m_file->close();

      // Now you can use firstLine and rest variables for your needs
      if (suffix == "rlg") {
        m_rlg = true;
        m_RLGEdit->setText(rest);
        m_rlg_alphabet->setText(firstLine);
      } else {
        m_rlg = false;
        m_edit->setText(rest);
        m_main_alphabet->setText(firstLine);
      }

      qDebug() << "First line:" << firstLine;
      qDebug() << "Rest of the file:" << rest;
    }
    else
    {
      // Handle error here
      qDebug() << "Could not open file for reading";
    }

    qDebug() << "Chosen file:" << fileName;
  }
}
