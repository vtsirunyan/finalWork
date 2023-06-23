#ifndef NFATODFA_H
#define NFATODFA_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <QtCore>
using namespace std;

class NFA
{
public:
    vector<QString> states;
    vector<QChar> inputSymbols;
    unordered_map<QString, unordered_map<QChar, vector<QString>>> transitionFn;
    QString startState;
    vector<QString> finalStates;

    NFA(vector<QString> &states, vector<QChar> &inputSymbols, unordered_map<QString, unordered_map<QChar, vector<QString>>> &transitionFn, QString &startState, vector<QString> &finalStates);

    NFA(QString text);
};

class DFA
{
public:
    vector<int> states;
    vector<QChar> inputSymbols;
    unordered_map<int, unordered_set<QString>> mappedStates;
    unordered_map<int, unordered_map<QChar, int>> transitionFn;
    int startState;
    vector<int> finalStates;
};

unordered_map<QString, unordered_set<QString>> findeClosure(NFA &nfa);
void printeClosure(NFA &nfa, unordered_map<QString, unordered_set<QString>> &eClosure);
void printMappings(unordered_map<int, unordered_set<QString>> &mappedStates);
DFA convertNFAtoDFA(NFA &nfa);
void printNFA(NFA &nfa);
void printDFA(DFA &dfa);
QString toText(DFA &dfa);
QString getMapped(DFA &dfa, int state);
QString nextDummy(QString &dummyState);

#endif // NFATODFA_H
