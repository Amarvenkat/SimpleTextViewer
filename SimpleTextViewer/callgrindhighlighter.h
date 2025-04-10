#ifndef CALLGRINDHIGHLIGHTER_H
#define CALLGRINDHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QMap>

class CallgrindHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit CallgrindHighlighter(QTextDocument *parent = nullptr);
    void finalize(const QString &text);

protected:
    void highlightBlock(const QString &text) override;

private:
    QTextCharFormat headerlineIndoc;
    QTextCharFormat highCostText;

    QString currentFunction;
    QMap<QString, int> functionCosts;
    QMap<QString, int> totalcost;
    QString lastFunction;
    QMap<int, QTextCharFormat> pendingFunctionFormats;
    QMap<int, QString> lineToFunction;
    int maxObservedCost = 1;
    QString parseFunctionName(const QString &text);
    int parseCost(const QString &text);
    QString parseCalledFunction(const QString &text);
    int parseCallCount(const QString &text);
    int parseCallCost(const QString &text);

    void logFunctionCosts(const QString &text);
};

#endif
