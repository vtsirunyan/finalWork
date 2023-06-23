#include "automata.h"
#include <QtCore>
#include <iostream>

using namespace std;

Automata::Automata(QString &startState, unordered_set<QChar> &inputSymbols, unordered_map<QString, unordered_map<QString, vector<QString>>> &transitionFn, unordered_set<QString> &finalStates) {
    this->startState = startState;
    this->inputSymbols = inputSymbols;
    this->transitionFn = transitionFn;
    this->finalStates = finalStates;
}

Automata::Automata(QString text, QString alphabet) {
    text = text.trimmed();
    QStringList lines = text.split('\n');
    QString startState = lines[0].trimmed();

    unordered_set<QChar> inputSymbols;
    alphabet = alphabet.trimmed();
    for (QString symbol : alphabet.split(" ")) {
        if (symbol.length() != 1) cout << "ERROR! use single characters for input alphabet." << '\n';
        inputSymbols.insert(symbol[0]);
    }

    text = text.trimmed();
    unordered_map<QString, unordered_map<QString, vector<QString>>> transitionFn;
    for (int i = 2; i < lines.size(); ++i) {
        QString line = lines[i].trimmed();
        QStringList tokens = line.split(" -> ");
        if (tokens.size() != 2) cout << "ERROR! graph declaration should look like A -> B c" << '\n';
        QString from = tokens[0];
        QStringList rhs = tokens[1].split(" ");
        if (rhs.size() != 2) cout << "ERROR! graph declaration should look like A -> B c" << '\n';
//        if (rhs[1].length() != 1) cout << "ERROR! use single characters for input alphabet." << '\n';
        QString to = rhs[0];
        QString symbol = rhs[1];
        for (QChar c : symbol) {
            if (!inputSymbols.count(c)) cout << "ERROR! Given symbol is not from the alphabet." << '\n';
        }
        transitionFn[from][symbol].push_back(to);
    }

    unordered_set<QString> finalStates;
    for (QString finalState : lines[1].trimmed().split(" ")) {
        finalStates.insert(finalState);
    }

    this->startState = startState;
    this->inputSymbols = inputSymbols;
    this->transitionFn = transitionFn;
    this->finalStates = finalStates;
    qDebug() << "Strange shit" << '\n';
}

void Automata::DFS(QString currentNode, vector<pair<QString, QString>> &path, QString token, int pos, vector<pair<QString, QString>> &ans) {
    qDebug() << "Now in DFS(" << currentNode << ", " << path << ", " << token << ", " << pos << ", " << ans << " and finalStates.size() = " << finalStates.size() << '\n';

    for (auto zzz : finalStates) qDebug() << zzz << '\n';

    if (pos == token.length()) {
        qDebug() << "Inside if" << '\n';

        if (finalStates.count(currentNode)) {
            ans = path;
        }
    }

    /*
     S
     S -> abcS
     S -> kd
     S -> aaaA
     A -> e
     */
    for (auto [symbol, nextStates] : transitionFn[currentNode]) {
        qDebug() << "symbol = " << symbol << " nextStates = " << nextStates << '\n';
        if (symbol != "e" && (symbol.length() > token.length() - pos || token.mid(pos, symbol.length()) != symbol)) continue;
        qDebug() << " idk why this happens" << '\n';
        for (QString nextNode : transitionFn[currentNode][symbol]) {
            path.emplace_back(symbol, nextNode);
            int symbolsRead = symbol == "e" ? 0 : symbol.length();
            DFS(nextNode, path, token, pos + symbolsRead, ans);
            if (!ans.empty()) return;
            path.pop_back();
        }
        qDebug() << "Exiting for this symbol" << '\n';
    }
    qDebug() << "Exiting DFS" << '\n';
}

/*
 *
S
A D
S -> A abc
A -> B e
A -> C e
B -> B b
C -> C c
B -> D k
C -> D k
 */

vector<pair<QString, QString>> Automata::checkToken(QString token) {
    qDebug() << "Now in checkToken(" << token.toStdString() << ")" << '\n';
    vector<pair<QString, QString>> ans, path;
    qDebug() << "Calling DFS(" << startState << ", " << path << ", " << token << ", 0 " << ans << '\n';
    DFS(startState, path, token, 0, ans);
    return ans;
}

QString Automata::findDeduction(QString token) {
    auto path = checkToken(token);
    qDebug() << "path = " << path << '\n';
    if (path.empty()) {
        qDebug() << "It's here now" << '\n';
        return "There is no deduction for a given word.";
    }
    QString deduction;
    QString prevState = startState;
    for (auto [symbol, state] : path) {
        deduction += prevState;
        deduction += " -> ";
        deduction += state;
        deduction += ' ';
        deduction += symbol;
        deduction += '\n';
        prevState = state;
    }
    return deduction;
}
