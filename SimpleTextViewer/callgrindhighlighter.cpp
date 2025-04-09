#include "callgrindhighlighter.h"
#include <QRegularExpression>

CallgrindHighlighter::CallgrindHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{


    headerlineIndoc.setForeground(Qt::blue);
    headerlineIndoc.setFontWeight(QFont::Bold);

    //creatorText.setForeground(Qt::green);
    //creatorText.setFontItalic(true);
    highCostText.setForeground(Qt::red);
    highCostText.setFontWeight(QFont::Bold);

}

void CallgrindHighlighter::highlightBlock(const QString &text)
{
    setCurrentBlockState(0);
    int state = previousBlockState();

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

    //QRegularExpression fnRegex(R"(^\s*fn=(\(\d+\)\s.*$|\w+)\s*\d+\s+\d{3,})");
    //r"^fn=(?:\(\d+\)\s.*|\w+)\r?\n\d+\s+\d{3,}$"

    QRegularExpression fnRegex(R"(^\s*fn=(\(\d+\)\s.*$|\w+))");

    QRegularExpressionMatch fnMatch = fnRegex.match(text);
    if (fnMatch.hasMatch()) {
        setCurrentBlockState(1);
        //setFormat(fnMatch.capturedStart(), fnMatch.capturedLength(), highCostText);
    }


    // Match cost lines only after fn=()
    if (state == 1) {
        QRegularExpression costLineRegex(R"(^\s*\d+\s+\d{3,})");
        QRegularExpressionMatch costMatch = costLineRegex.match(text);

        if (costMatch.hasMatch() && !text.contains('=')) {
            setFormat(fnMatch.capturedStart(), fnMatch.capturedLength(), highCostText); // Highlight fn= in red
            setFormat(costMatch.capturedStart(), costMatch.capturedLength(), highCostText); // Highlight cost line in red
        } else {
            // If there's no valid cost line, don't highlight fn= anymore
            setFormat(fnMatch.capturedStart(), fnMatch.capturedLength(), Qt::black); // Reset fn= color to black (normal)
        }
    }
}
