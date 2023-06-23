#ifndef RGLPARSER_H
#define RGLPARSER_H
#include <QString>

class RGLParser
{
public:
    RGLParser();
    void setRGL(QString text) {RGL = text;};
    QString parse();
private:
    QString RGL;
};

#endif // RGLPARSER_H
