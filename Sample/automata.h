#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <QtCore>
using namespace std;

class Automata
{
public:
    QString startState;
    unordered_set<QChar> inputSymbols;
    unordered_set<QString> finalStates;
    unordered_map<QString, unordered_map<QString, vector<QString>>> transitionFn;

    Automata(QString &startState, unordered_set<QChar> &inputSymbols, unordered_map<QString, unordered_map<QString, vector<QString>>> &transitionFn, unordered_set<QString> &finalStates);
    Automata(QString text, QString alphabet);

    vector<pair<QString, QString>> checkToken(QString token);
    QString findDeduction(QString token);
    void DFS(QString currentNode, vector<pair<QString, QString>> &path, QString token, int pos, vector<pair<QString, QString>> &ans);
};

#endif // AUTOMATA_H
