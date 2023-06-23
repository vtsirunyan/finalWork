#ifndef RLGPARSER_H
#define RLGPARSER_H
#include <QString>
#include <unordered_set>

class RLGParser
{
public:
    RLGParser();
    void setRLG(QString text) {RLG = text;};
    void setALPHA(QString alpha) {inputAlpha = alpha;};
    QString parse();
    QString nextDummy();
private:
    QString RLG;
    QString inputAlpha;
    std::unordered_set<char> alphabet;
};

#endif // RLGPARSER_H
