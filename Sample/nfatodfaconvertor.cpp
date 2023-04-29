#include "nfatodfaconvertor.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <set>
#include <QtCore>
#define MAX_NFA_STATES 10
#define MAX_ALPHABET_SIZE 10
using namespace std;
// Representation of an NFA state
class NFAstate
{
    public:
        int transitions[MAX_ALPHABET_SIZE][MAX_NFA_STATES];
        NFAstate()
        {
            for (int i = 0; i < MAX_ALPHABET_SIZE; i++)
                for (int j = 0; j < MAX_NFA_STATES; j++)
                    transitions[i][j] = -1;
        }
}*NFAstates;
// Representation of a DFA state
struct DFAstate
{
        bool finalState;
        bitset<MAX_NFA_STATES> constituentNFAstates;
        bitset<MAX_NFA_STATES> transitions[MAX_ALPHABET_SIZE];
        int symbolicTransitions[MAX_ALPHABET_SIZE];
};
set<int> NFA_finalStates;
vector<int> DFA_finalStates;
vector<DFAstate*> DFAstates;
queue<int> incompleteDFAstates;
int N, M; // N -> No. of stattes, M -> Size of input alphabet
// finds the epsilon closure of the NFA state "state" and stores it into "closure"
void epsilonClosure(int state, bitset<MAX_NFA_STATES> &closure)
{
    for (int i = 0; i < N && NFAstates[state].transitions[0][i] != -1; i++)
        if (closure[NFAstates[state].transitions[0][i]] == 0)
        {
            closure[NFAstates[state].transitions[0][i]] = 1;
            epsilonClosure(NFAstates[state].transitions[0][i], closure);
        }
}
// finds the epsilon closure of a set of NFA states "state" and stores it into "closure"
void epsilonClosure(bitset<MAX_NFA_STATES> state,
        bitset<MAX_NFA_STATES> &closure)
{
    for (int i = 0; i < N; i++)
        if (state[i] == 1)
            epsilonClosure(i, closure);
}
// returns a bitset representing the set of states the NFA could be in after moving
// from state X on input symbol A
void NFAmove(int X, int A, bitset<MAX_NFA_STATES> &Y)
{
    for (int i = 0; i < N && NFAstates[X].transitions[A][i] != -1; i++)
        Y[NFAstates[X].transitions[A][i]] = 1;
}
// returns a bitset representing the set of states the NFA could be in after moving
// from the set of states X on input symbol A
void NFAmove(bitset<MAX_NFA_STATES> X, int A, bitset<MAX_NFA_STATES> &Y)
{
    for (int i = 0; i < N; i++)
        if (X[i] == 1)
            NFAmove(i, A, Y);
}

NfaToDfaConvertor::NfaToDfaConvertor()
{
}

void NfaToDfaConvertor::parse()
{
std::set<int> states, alphabets;
  QStringList lines = nfaText.split("\n");
  QRegExp exp(" +");
  QSet<QString> nodeNames;
  for (QString line : lines) {
    QStringList words = line.split(exp);
    if(words.size() == 4 && words[3] != "" && words[1] == "->") {
      int from = words[0].toInt();
      int to = words[2].toInt();
      int value = words[3].trimmed().toInt();
      states.insert(from);
      states.insert(to);
      alphabets.insert(value);
    }
  }

  N = states.size();
  M = alphabets.size();
  NFAstates = new NFAstate[N];
  for (QString line : lines) {
    QStringList words = line.split(exp);
    if(words.size() == 4 && words[3] != "" && words[1] == "->") {
      int from = words[0].toInt();
      int to = words[2].toInt();
      int value = words[3].trimmed().toInt();
      for(int i = 0; i < MAX_NFA_STATES; ++i) {
        if(NFAstates[from].transitions[value][i] == -1) {
          NFAstates[from].transitions[value][i] = to;
          break;
        }
      }
    }
  }
  int i, j, D = 1;
     DFAstates.push_back(new DFAstate);
     DFAstates[0]->constituentNFAstates[0] = 1;
     epsilonClosure(0, DFAstates[0]->constituentNFAstates);
     for (j = 0; j < N; j++)
         if (DFAstates[0]->constituentNFAstates[j] == 1 && NFA_finalStates.find(
                 j) != NFA_finalStates.end())
         {
             DFAstates[0]->finalState = true;
             DFA_finalStates.push_back(0);
             break;
         }
     incompleteDFAstates.push(0);
     while (!incompleteDFAstates.empty())
     {
         int X = incompleteDFAstates.front();
         incompleteDFAstates.pop();
         for (i = 1; i <= M; i++)
         {
             NFAmove(DFAstates[X]->constituentNFAstates, i,
                     DFAstates[X]->transitions[i]);
             epsilonClosure(DFAstates[X]->transitions[i],
                     DFAstates[X]->transitions[i]);
             for (j = 0; j < D; j++)
                 if (DFAstates[X]->transitions[i]
                         == DFAstates[j]->constituentNFAstates)
                 {
                     DFAstates[X]->symbolicTransitions[i] = j;
                     break;
                 }
             if (j == D)
             {
                 DFAstates[X]->symbolicTransitions[i] = D;
                 DFAstates.push_back(new DFAstate);
                 DFAstates[D]->constituentNFAstates
                         = DFAstates[X]->transitions[i];
                 for (j = 0; j < N; j++)
                     if (DFAstates[D]->constituentNFAstates[j] == 1
                             && NFA_finalStates.find(j) != NFA_finalStates.end())
                     {
                         DFAstates[D]->finalState = true;
                         DFA_finalStates.push_back(D);
                         break;
                     }
                 incompleteDFAstates.push(D);
                 D++;
             }
         }
     }
  dfaString.clear();
  QTextStream stream(&dfaString);
  for (int i = 0; i < D; i++)
      {
          for (int j = 1; j <= M; j++)
              stream << i << " -> " << DFAstates[i]->symbolicTransitions[j]  << " " << j << "\n";
      }
}

QString NfaToDfaConvertor::getDfaTest()
{
  return dfaString;
}

