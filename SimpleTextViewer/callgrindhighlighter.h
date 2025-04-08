
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class CallgrindHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    CallgrindHighlighter(QTextDocument *parent = nullptr);

    void highlightBlock(const QString &text) ;

private:
    QTextCharFormat headerlineIndoc;
    //QTextCharFormat creatorText;

};
