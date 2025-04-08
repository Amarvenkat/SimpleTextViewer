#include "callgrindhighlighter.h"
#include <QRegularExpression>

CallgrindHighlighter::CallgrindHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{


    headerlineIndoc.setForeground(Qt::blue);
    headerlineIndoc.setFontWeight(QFont::Bold);

    //creatorText.setForeground(Qt::green);
    //creatorText.setFontItalic(true);


}

void CallgrindHighlighter::highlightBlock(const QString &text)
{

    QRegularExpression regexForheaderlineIndoc(R"((version:|creator:|pid:|cmd:|part:|desc:|positions:|events:|summary:|totals:|))");

   // QRegularExpression creator(R"((^creator:\s*(.*)))");

    //^creator:\s*(.*)

    //QRegularExpression regexForheaderlineIndoc(R"((summary:|events:|calls=\d+))");


    for (const QRegularExpressionMatch &match : regexForheaderlineIndoc.globalMatch(text)){

        setFormat(match.capturedStart(), match.capturedLength(), headerlineIndoc);
}

//    for (const QRegularExpressionMatch &match : creator.globalMatch(text)){

//        setFormat(match.capturedStart(), match.capturedLength(), creatorText);
//}



}
