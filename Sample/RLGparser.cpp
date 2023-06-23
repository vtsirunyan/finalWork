#include "RLGparser.h"
#include <set>
#include <QtCore>
#include <iostream>
using namespace std;

RLGParser::RLGParser()
{
}

QString RLGParser::parse()
{
    inputAlpha = inputAlpha.trimmed();
    qDebug() << inputAlpha.split(" ");
    QList<QString> symbs;
    if (!inputAlpha.isEmpty()) symbs = inputAlpha.split(" ");
    for (QString symbol : symbs) {
        if (symbol.length() != 1 || !symbol[0].isLower()) return "[ERROR]: Use single lowercase latin letters for input alphabet.";
        alphabet.insert(symbol[0].toLatin1());
    }
    cerr << "Given text is " << '\n';
    cerr << RLG.toStdString() << '\n';
    RLG = RLG.trimmed();
    set<QString> terminals;
    if (RLG.isEmpty()) return "";
    QStringList lines = RLG.split("\n");
    qDebug() << lines << '\n';
    QString startingState = lines[0].trimmed();
    if (startingState.length() != 1 || !startingState[0].isUpper()) {
        return "[ERROR]: Please specify the starting state using capital latin letter at the first line.";
    }
    QString automata;
    for (int i = 1; i < lines.size(); ++i) {
        QString line = lines[i].trimmed();
        if (line.length() < 6) {
            return "[ERROR]: Line " + QString::number(i + 1) + ": Please use this format for grammar rules A -> xB or A -> x where A and B are states and x is a word.";
        }
        if (line.mid(1, 4) != " -> ") {
            return "[ERROR]: Line " + QString::number(i + 1) + ": Please use this format for grammar rules A -> xB or A -> x where A and B are states and x is a word.";
        }
        if (line[0] < 'A' || line[0] > 'Z') {
            return "[ERROR]: Line " + QString::number(i + 1) + ": Please use single uppercase latin letters for states.";
        }
        QString rhs = line.right(line.size() - 5);
        bool foundUpper = false;
        for (QChar c : rhs) {
            if (!c.isUpper() && !alphabet.count(c.toLatin1())) {
                return "[ERROR]: Line " + QString::number(i + 1) + ": Used symbol '" + c + "' is not from the alphabet.";
            }
            if (c.isLower() && foundUpper) {
                return "[ERROR]: Line " + QString::number(i + 1) + ": Given grammar is not right linear.";
            }
            if (c.isUpper()) {
                if (foundUpper) {
                    return "[ERROR]: Line " + QString::number(i + 1) + ": Please use single uppercase latin letters for states.";
                }
                foundUpper = true;
            }
        }

        if (rhs == "e") {
            terminals.insert(line[0]);
            continue;
        }

        QString toState = "T";
        if (foundUpper) {
            toState = rhs.back();
            rhs.chop(1);
        } else {
            terminals.insert("T");
        }
        automata += line.left(5);
        automata += toState;
        automata += ' ';
        automata += rhs;
        automata += '\n';
//        if (!foundUpper) rhs += '$';
//        QString prevState = line[0];
//        for (int i = 0; i + 1 < rhs.size(); ++i) {
//            QChar c = rhs[i];
//            QString currentState;
//            if (i + 1 == rhs.size() - 1 && rhs.back() != '$') {
//                currentState = rhs.back();
//            } else {
//                currentState = nextDummy();
//            }
//            automata += prevState;
//            automata += " -> ";
//            automata += currentState;
//            automata += ' ';
//            automata += c;
//            automata += '\n';
//            prevState = currentState;
//        }
//        if (!foundUpper) {
//            terminals.insert(prevState);
//        }
    }

    cerr << "Automata = " << automata.toStdString() << '\n';
    QString states = startingState;
    states += '\n';
    for (QString tt : terminals) {
        states += tt;
        states += ' ';
    }
    states = states.trimmed();
    states += '\n';
    return states + automata;
}
