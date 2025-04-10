#include "callgrindhighlighter.h"
#include <QRegularExpression>
#include <QDebug>

CallgrindHighlighter::CallgrindHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    headerlineIndoc.setForeground(Qt::blue);
    headerlineIndoc.setFontWeight(QFont::Bold);

    highCostText.setForeground(Qt::red);
    highCostText.setFontWeight(QFont::Bold);
}

void CallgrindHighlighter::highlightBlock(const QString &text)
{
    setCurrentBlockState(0);
    int state = previousBlockState();

    QRegularExpression regexForheaderlineIndoc(R"((version:|creator:|pid:|cmd:|part:|desc:|positions:|events:|summary:|totals:))");

    for (const QRegularExpressionMatch &match : regexForheaderlineIndoc.globalMatch(text)) {
        setFormat(match.capturedStart(), match.capturedLength(), headerlineIndoc);
    }
    int blockNumber = currentBlock().blockNumber();

    QRegularExpression fnRegex(R"(^\s*fn=(\(\d+\)\s.*$|\w+))");
    QRegularExpressionMatch fnMatch = fnRegex.match(text);
    if (fnMatch.hasMatch()) {
        setCurrentBlockState(1);
        // setFormat(fnMatch.capturedStart(), fnMatch.capturedLength(), highCostText);
        if (!currentFunction.isEmpty()) {
            logFunctionCosts(text);
        }

        //setFormat(fnMatch.capturedStart(), fnMatch.capturedLength(), highCostText);

        currentFunction = parseFunctionName(text);
        functionCosts[currentFunction] = 0;

        lineToFunction[blockNumber] = currentFunction;


    }

    ///////////////////////////////////////////////////////////////////////////
    QRegularExpression cfnRegex(R"(^\s*cfn=(\(\d+\)\s.*$|\w+))");
    QRegularExpressionMatch cfnMatch = cfnRegex.match(text);
    if (cfnMatch.hasMatch()) {
        setCurrentBlockState(1);

    }


    QRegularExpression callRegex(R"(^\s*calls=\d+\s+\d+)");
    QRegularExpressionMatch callMatch = callRegex.match(text);
    if (callMatch.hasMatch()) {
        setCurrentBlockState(1);
    }
    ///////////////////////////////////////////////////////////////////////////
    if (state == 1) {
        QRegularExpression costLineRegex(R"(^\s*\d+\s+\d+)");
        QRegularExpressionMatch match = costLineRegex.match(text);

        if (match.hasMatch()) {

                // qDebug() << "INSIDE---------------------:" ;
            int cost = parseCost(text);
            functionCosts[currentFunction] += cost;






            if (functionCosts[currentFunction] > maxObservedCost){
                maxObservedCost = functionCosts[currentFunction];
            }

            QTextCharFormat gradientFormat;


            int intensity = static_cast<int>(255.0 * (1.0 - qMin(1.0, functionCosts[currentFunction] / static_cast<double>(maxObservedCost))));
            gradientFormat.setBackground(QColor(255, intensity, intensity));

            setFormat(match.capturedStart(), match.capturedLength(), gradientFormat);

        }


        /*      QRegularExpression cfnRegex(R"(^\s*cfn=\w+)");
        QRegularExpressionMatch cfnMatch = cfnRegex.match(text);
        if (cfnMatch.hasMatch()) {
            QString calledFunction = parseCalledFunction(text);
            QRegularExpression callRegex(R"(^\s*calls=\d+\s+\d+)");
            QRegularExpressionMatch callMatch = callRegex.match(text);


            QRegularExpression costLineRegex(R"(^\s*(\d+)\s+(\d+))");
            QRegularExpressionMatch costLineMatch = costLineRegex.match(text);
            if (costLineMatch.hasMatch()) {

                int additionalCost = costLineMatch.captured(2).toInt();
                callCosts[calledFunction] += additionalCost;
            }
        }
*/
    }


    //qDebug() << "Function:" << currentFunction << "Total Cost:" << functionCosts[currentFunction];

    // qDebug() << "Function:" << funcname << "Total Cost:" << funcval;
}

QString CallgrindHighlighter::parseFunctionName(const QString &text)
{
    QRegularExpression fnNameRegex(R"(^\s*fn=(\(\d+\)\s.*$|\w+))");
    QRegularExpressionMatch match = fnNameRegex.match(text);
    if (match.hasMatch()) {
        return match.captured(1);
    }
    return QString();
}

int CallgrindHighlighter::parseCost(const QString &text)
{
    QRegularExpression costRegex(R"(\b(\d+)\s+(\d+))");
    QRegularExpressionMatch match = costRegex.match(text);
    if (match.hasMatch()) {
        return match.captured(2).toInt();
    }
    return 0;
}

QString CallgrindHighlighter::parseCalledFunction(const QString &text)
{
    QRegularExpression cfnRegex(R"(^\s*cfn=(\(\d+\)\s.*$|\w+))");
    QRegularExpressionMatch match = cfnRegex.match(text);
    if (match.hasMatch()) {
        return match.captured(1);
    }
    return QString();
}




void CallgrindHighlighter::logFunctionCosts(const QString &text)
{
    if (!currentFunction.isEmpty()) {
        /* int totalCost = functionCosts[currentFunction];
        qDebug() << "Function:" << currentFunction
                 << "Total Cost:" << totalCost;*/
    }
}
