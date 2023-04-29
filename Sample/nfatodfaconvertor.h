#ifndef NFATODFACONVERTOR_H
#define NFATODFACONVERTOR_H
#include <QString>

class NfaToDfaConvertor
{
public:
  NfaToDfaConvertor();
  void setNfa(QString text) {nfaText = text;};
  void parse();
  QString getDfaTest();
private:
  QString nfaText;
  QString dfaString;
};

#endif // NFATODFACONVERTOR_H
