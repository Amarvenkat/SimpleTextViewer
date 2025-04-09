#ifndef CALLGRINDHIGHLIGHTER_H
#define CALLGRINDHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

class CallgrindHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit CallgrindHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    QTextCharFormat headerlineIndoc;  // Blue bold for header lines
    QTextCharFormat highCostText;     // Red bold for high-cost fn blocks
    // QTextCharFormat creatorText;   // (Uncomment if needed)
};

#endif // CALLGRINDHIGHLIGHTER_H
