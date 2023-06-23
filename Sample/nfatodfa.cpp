#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <QtCore>
#include "nfatodfa.h"

using namespace std;

NFA::NFA(vector<QString> &states, vector<QChar> &inputSymbols, unordered_map<QString, unordered_map<QChar, vector<QString>>> &transitionFn, QString &startState, vector<QString> &finalStates)
{
    this->states = states;
    this->inputSymbols = inputSymbols;
    this->transitionFn = transitionFn;
    this->startState = startState;
    this->finalStates = finalStates;
}

NFA::NFA(QString text)
{
    QString startState;
    unordered_set<QString> states;
    unordered_set<QChar> inputSymbols;
    vector<QString> finalStates;
    unordered_map<QString, unordered_map<QChar, vector<QString>>> transitionFn;

    text = text.trimmed();
    QStringList lines = text.split("\n");
    startState = lines[0].trimmed();
    states.insert(startState);

    qDebug() << lines << '\n';
    for (QString state : lines[1].trimmed().split(" "))
    {
        finalStates.push_back(state);
        states.insert(state);
    }

    QString dummyState = "Z";
    for (int i = 2; i < lines.size(); ++i)
    {
        QString line = lines[i].trimmed();
        QStringList tokens = line.split(" -> ");
        QString from = tokens[0];
        QStringList rhs = tokens[1].split(" ");
        QString to = rhs[0];
        QString symbol = rhs[1];
        QString prevState = from;
        qDebug() << "Line = " << i << '\n';
        for (int j = 0; j < symbol.size(); ++j) {
            QString currentState = (j + 1 == symbol.size()) ? to : nextDummy(dummyState);
            states.insert(prevState);
            states.insert(currentState);
            inputSymbols.insert(symbol[j]);
            transitionFn[prevState][symbol[j]].push_back(currentState);
            qDebug() << "j = " << j << " prevState = " << prevState << " currentState = " << currentState << " symbol = " << symbol[j] << '\n';
            prevState = currentState;
        }
//        QString line = lines[i].trimmed();
//        qDebug() << "i = " << i << " line = " << line << '\n';
//        char from = line[0].toLatin1();
//        char to = line[5].toLatin1();
//        char symbol = line[7].toLatin1();

//        states.insert(from);
//        states.insert(to);
//        inputSymbols.insert(symbol);
//        transitionFn[from][symbol].push_back(to);
    }

    if (inputSymbols.count('e')) inputSymbols.erase('e');
    vector<QString> distinctStates(states.begin(), states.end());
    vector<QChar> distinctInputSymbols(inputSymbols.begin(), inputSymbols.end());
//    cout << "startState = " << startState << '\n';
//    cout << finalStates.size() << " finalStates = ";
//    for (char c : finalStates) {
//        cout << c << ' ';
//    }
//    cout << '\n';
//    cout << inputSymbols.size() << " inputSymbols = ";
//    for (char c : inputSymbols) {
//        cout << c << ' ';
//    }
//    cout << '\n';
//    cout << states.size() << " states = ";
//    for (char c : states) {
//        cout << c << ' ';
//    }
//    cout << '\n';
//    cout << "transitionFn = " << '\n';
//    for (auto [k, v] : transitionFn) {
//        cout << k << "=== \n";
//        for (auto [kk, vv] : v) {
//            cout << kk << ": ";
//            for (char c : vv) cout << c << ' ';
//            cout << '\n';
//        }
//    }
    this->states = distinctStates;
    this->inputSymbols = distinctInputSymbols;
    this->transitionFn = transitionFn;
    this->startState = startState;
    this->finalStates = finalStates;
//    NFA(distinctStates, distinctInputSymbols, transitionFn, startState, finalStates);
}

unordered_map<QString, unordered_set<QString>> findeClosure(NFA &nfa)
{
    unordered_map<QString, unordered_set<QString>> eClosure;
    for (QString c : nfa.states)
    {
        queue<QString> q;
        unordered_set<QString> visited;
        q.push(c);
        visited.insert(c);
        while (!q.empty())
        {
            QString srcState = q.front();
            q.pop();
            vector<QString> destStates = nfa.transitionFn[srcState]['e'];
            for (QString destState : destStates)
            {
                if (visited.find(destState) == visited.end())
                {
                    visited.insert(destState);
                    q.push(destState);
                }
            }
        }
        for (auto eReachableState : visited)
        {
            eClosure[c].insert(eReachableState);
        }
    }

    return eClosure;
}

void printeClosure(NFA &nfa, unordered_map<QString, unordered_set<QString>> &eClosure)
{
    cout << "Epsilon Closures are:";
    for (QString state : nfa.states)
    {
        cout << "\nState " << state.toStdString() << ": ";
        for (QString c : eClosure[state])
        {
            cout << c.toStdString() << " ";
        }
    }
}

void printMappings(unordered_map<int, unordered_set<QString>> &mappedStates)
{
    cout << "\n\nMapped states are:\n";
    cout << "DFA == NFA\n";
    int n = mappedStates.size();
    for (int i = 0; i < n; i++)
    {
        cout << i << " == ";
        for (auto c : mappedStates[i])
        {
            cout << c.toStdString() << ", ";
        }
        cout << "\b\b \n";
    }
}

DFA convertNFAtoDFA(NFA &nfa)
{
    unordered_map<QString, unordered_set<QString>> eClosure = findeClosure(nfa);
    printeClosure(nfa, eClosure);

    DFA dfa;
    dfa.inputSymbols = nfa.inputSymbols;
    dfa.startState = 0;
    int noOfDFAStates = 1;
    queue<int> q;
    q.push(0);
    dfa.mappedStates[0] = eClosure[nfa.startState];

    while (!q.empty())
    {
        int dfaState = q.front();
        q.pop();
        for (QChar symbol : nfa.inputSymbols)
        {
            unordered_set<QString> reach;
            unordered_set<QString> nfaStates = dfa.mappedStates[dfaState];
            for (QString srcState : nfaStates)
            {
                vector<QString> destStates = nfa.transitionFn[srcState][symbol];
                for (QString destState : destStates)
                {
                    unordered_set<QString> s = eClosure[destState];
                    reach.insert(s.begin(), s.end());
                }
            }

            int found = -1;
            for (int i = 0; i < noOfDFAStates; i++)
            {
                if (dfa.mappedStates[i] == reach)
                {
                    found = i;
                    break;
                }
            }
            if (found != -1)
            {
                dfa.transitionFn[dfaState][symbol] = found;
            }
            else
            {
                dfa.transitionFn[dfaState][symbol] = noOfDFAStates;
                dfa.mappedStates[noOfDFAStates] = reach;
                q.push(noOfDFAStates);
                noOfDFAStates++;
            }
        }
    }

    printMappings(dfa.mappedStates);
    for (int i = 0; i < noOfDFAStates; i++)
    {
        dfa.states.push_back(i);
    }

    auto &states1 = nfa.finalStates;
    for (int i = 0; i < noOfDFAStates; i++)
    {
        auto &states2 = dfa.mappedStates[i];
        for (auto c1 : states1)
        {
            if (states2.find(c1) != states2.end())
            {
                dfa.finalStates.push_back(i);
                break;
            }
        }
    }

    return dfa;
}

void printNFA(NFA &nfa)
{
    int width = 6;
    cout << "Given NFA:\n";
    cout << "States: ";
    for (QString c : nfa.states)
    {
        cout << c.toStdString() << ", ";
    }

    cout << " \nInput symbols: ";
    for (QString c : nfa.inputSymbols)
    {
        cout << c.toStdString() << ", ";
    }

    cout << "\b\b \nTransition Function:\n";
    cout << setw(width) << "";
    for (QChar c : nfa.inputSymbols)
    {
        cout << setw(width) << c.toLatin1();
    }
    cout << setw(width) << "e" << endl;
    for (QString srcState : nfa.states)
    {
        cout << setw(width) << srcState.toStdString();
        for (QChar symbol : nfa.inputSymbols)
        {
            vector<QString> destStates = nfa.transitionFn[srcState][symbol];
            string str = "";
            for (QString state : destStates)
            {
                str += state.toStdString();
            }
            cout << setw(width) << str;
        }
        vector<QString> destStates = nfa.transitionFn[srcState]['e'];
        string str = "";
        for (QString state : destStates)
        {
            str += state.toStdString();
        }
        cout << setw(width) << str << endl;
    }

    cout << "Start State: " << nfa.startState.toStdString();
    cout << "\nFinal States: ";
    for (QString c : nfa.finalStates)
    {
        cout << c.toStdString() << ", ";
    }
    cout << "\b\b \n\n";
}

void printDFA(DFA &dfa)
{
    int width = 6;
    cout << "\n\nThe converted DFA is:\n";
    cout << "States: ";
    for (int i : dfa.states)
    {
        cout << i << ", ";
    }

    cout << "\b\b \nInput symbols: ";
    for (QChar c : dfa.inputSymbols)
    {
        cout << c.toLatin1() << ", ";
    }

    cout << "\b\b \nTransition Function:\n";
    cout << setw(width) << "";
    for (QChar c : dfa.inputSymbols)
    {
        cout << setw(width) << c.toLatin1();
    }
    for (int srcState : dfa.states)
    {
        cout << endl
             << setw(width) << srcState;
        for (QChar symbol : dfa.inputSymbols)
        {
            int destState = dfa.transitionFn[srcState][symbol];
            cout << setw(width) << destState;
        }
    }

    cout << "\nStart State: " << dfa.startState;
    cout << "\nFinal States: ";
    for (int c : dfa.finalStates)
    {
        cout << c << ", ";
    }
    cout << "\b\b \n\n";
}

QString toText(DFA &dfa)
{
    QString dfaText;
    dfaText += getMapped(dfa, dfa.startState);
    dfaText += '\n';
    for (int state : dfa.finalStates) {
        dfaText += getMapped(dfa, state);
        dfaText += ' ';
    }
    dfaText += '\n';
    for (int srcState : dfa.states)
    {
        for (QChar symbol : dfa.inputSymbols)
        {
            int destState = dfa.transitionFn[srcState][symbol];
            dfaText += getMapped(dfa, srcState);
            dfaText += " -> ";
            dfaText += getMapped(dfa, destState);
            dfaText += ' ';
            dfaText += symbol;
            dfaText += '\n';
        }
    }
    return dfaText;
}

QString getMapped(DFA &dfa, int state) {
    QString stateLabel = "{";
    bool notFirst = false;
    for (QString nfaState : dfa.mappedStates[state]) {
        if (notFirst) {
            stateLabel += ',';
        }
        notFirst = true;
        stateLabel += nfaState;
    }
    stateLabel += '}';
    return stateLabel;
}

QString nextDummy(QString &dummyState)
{
    bool found = false;
    for (int i = dummyState.size() - 1; ~i && !found; --i) {
        if (dummyState[i] != 'Z') {
            found = true;
            dummyState[i] = char(dummyState[i].toLatin1() + 1);
            for (int j = i + 1; j < dummyState.size(); ++j) {
                dummyState[j] = 'A';
            }
        }
    }
    if (!found) {
        dummyState += 'A';
        for (QChar &c : dummyState) c = 'A';
    }
    return dummyState;
}
