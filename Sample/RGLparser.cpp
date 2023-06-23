#include "RGLparser.h"
#include <set>
#include <QtCore>
#include <iostream>
using namespace std;

RGLParser::RGLParser()
{
}

QString RGLParser::parse()
{
    cerr << "Given text is " << '\n';
    cerr << RGL.toStdString() << '\n';
    RGL = RGL.trimmed();
    set<QChar> terminals;
    QStringList lines = RGL.split("\n");
    QString automata;
    for (QString line : lines) {
        if (line.length() < 6 || line.length() > 7) return RGL; //S -> aS
        if (line.mid(1, 4) != " -> ") return RGL;
        if (line[0] < 'A' || line[0] > 'Z') return RGL;
        if (line.length() == 6) {
            if (line[5] == '3') {
                terminals.insert(line[0]);
            } else if (line[5].isLower()) {
                automata += line.left(5) += "F ";
                automata += line[5];
                automata += '\n';
            }
            else return RGL;
            continue;
        }
        if (line[5] < 'a' || line[5] > 'z') return RGL;
        if (line[6] < 'A' || line[6] > 'Z') return RGL;
        automata += line.left(5);
        automata += line[6];
        automata += ' ';
        automata += line[5];
        automata += '\n';
    }
    cerr << "Yo wtf is this bitch" << '\n';
    cerr << "Automata = " << automata.toStdString() << '\n';
    return automata;
}
