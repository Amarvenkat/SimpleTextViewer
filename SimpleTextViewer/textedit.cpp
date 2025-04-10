#include "textedit.h"

#include <QFile>
#include <QFileInfo>
#include <QTextDocument>
#include <QTextCursor>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QMap>
#include <QDebug>

#include "callgrindhighlighter.h"
TextEdit::TextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    //setReadOnly(true);
    CallgrindHighlighter *highlighter = new CallgrindHighlighter(this->document());

    //highlighter->finalize();
}

void TextEdit::setContents(const QString &fileName)
{
    const QFileInfo fi(fileName);
    srcUrl = QUrl::fromLocalFile(fi.absoluteFilePath());
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        const QString data(QString::fromUtf8(file.readAll()));

        if (fileName.contains("callgrind")) {


            highlightCallgrind(data);


        } else if (fileName.endsWith(".html")) {

            setHtml(data);


        } else {

            setPlainText(data);
        }
    }
    emit fileNameChanged(fileName);
}

QVariant TextEdit::loadResource(int type, const QUrl &name)
{
    if (type == QTextDocument::ImageResource) {
        QFile file(srcUrl.resolved(name).toLocalFile());
        if (file.open(QIODevice::ReadOnly))
            return file.readAll();
    }
    return QTextEdit::loadResource(type, name);
}
void TextEdit::highlightText(const QString &searchstring)
{

    if (searchstring.trimmed().isEmpty()){
        return;
    }
    QTextCursor documentcursor(this->document());


    QTextCharFormat highlightAlltext;
    highlightAlltext.setBackground(Qt::yellow);


    documentcursor.beginEditBlock();


    QTextCursor clear(this->document());
    QTextCharFormat resetit;
    resetit.setBackground(Qt::transparent);

    clear.select(QTextCursor::Document);
    clear.setCharFormat(resetit);


    QTextCursor searchcursor(this->document());
    while (true) {
        searchcursor = this->document()->find(searchstring, searchcursor);
        if (searchcursor.isNull()) {
            break;
        }

        searchcursor.mergeCharFormat(highlightAlltext);
    }

    documentcursor.endEditBlock();
}


void TextEdit::highlightCallgrind(const QString &callgrindData)
{

    setPlainText(callgrindData);
    computeCost(callgrindData);
    //new CallgrindHighlighter(this->document());
}


void TextEdit::computeCost(const QString &callgrindData)
{


    QMap<QString, int> functionCost;

    QTextStream readfullText(const_cast<QString *>(&callgrindData));
    QString line;
    QString currentFunction;
    int currentCost = 0;

    //qDebug() << "Inside Function:"  ;

    while (readfullText.readLineInto(&line)) {



        line = line.trimmed();

        //qDebug() << "Line  "<< line;

        if (line.startsWith("fn=")) {

            if (!currentFunction.isEmpty()) {
                functionCost[currentFunction] += currentCost;
                currentCost = 0;
            }

            //qDebug() << "Line----"<< line;

            currentFunction = line.mid(3).trimmed();

            //qDebug() << "CurrentFucntion"<<currentFunction;

        }
        else if (QRegularExpression(R"(^\d+\s+\d+$)").match(line).hasMatch()) {

            QStringList parts = line.split(' ');


            if (parts.size() == 2) {
                bool ok;

                int cost = parts[1].toInt(&ok);

                if (ok) {


                    currentCost += cost;
                }
            }

            // qDebug() << "currentCost  from num  num"<<currentCost;


        }
        else if (line.isEmpty()) {

            if (!currentFunction.isEmpty()) {


                functionCost[currentFunction] += currentCost;
                currentFunction.clear();

                currentCost = 0;
            }
        }

        // qDebug() << "currentCost from empty------<<currentCost;
    }


    if (!currentFunction.isEmpty()) {

        functionCost[currentFunction] += currentCost;

        // qDebug() << "currentCost-----from "<< functionCost[currentFunction];
    }


    qDebug() << "Function Costs:";


    for (auto it = functionCost.begin(); it != functionCost.end(); it++) {


        qDebug() << it.key() << ":" << it.value();
    }


    highlightFunctionsByCost(functionCost);
}

void TextEdit::highlightFunctionsByCost(const QMap<QString, int> &functionCost)
{
    if (functionCost.isEmpty()) return;

    ////////////////////////////////////////////////////////////////////////


    int minCost = std::numeric_limits<int>::max();
    int maxCost = std::numeric_limits<int>::min();

    for (auto cost : functionCost.values()) {
        if (cost < minCost){
            minCost = cost;

        }
        if (cost > maxCost) {
            maxCost = cost;
        }
    }

    qDebug() << "Max Function Cost:" << maxCost << ", Min Function Cost:" << minCost;


    ////////////////////////////////////////////////////////////////////////

    QTextCursor cursor(this->document());
    cursor.beginEditBlock();



    QTextCursor clear(this->document());
    QTextCharFormat resetFormat;
    resetFormat.setBackground(Qt::transparent);
    clear.select(QTextCursor::Document);
    clear.setCharFormat(resetFormat);


    for (auto it = functionCost.begin(); it != functionCost.end(); it++) {
        QString functionName = it.key();
        int cost = it.value();


        QRegularExpression regex(QStringLiteral(R"((^|\n)fn=%1\b)").arg(QRegularExpression::escape(functionName)));

        QRegularExpressionMatchIterator matchName = regex.globalMatch(this->toPlainText());

        /////////while match
        while (matchName.hasNext()) {

            QRegularExpressionMatch match = matchName.next();

            int start = match.capturedStart();
            int length = match.capturedLength();



            QTextCursor highlightCursor(this->document());


            highlightCursor.setPosition(start);
            highlightCursor.setPosition(start + length, QTextCursor::KeepAnchor);


            int redIntensity = 50 + 205 * (cost - minCost) / qMax(1, maxCost - minCost);
            QColor color(redIntensity, 0, 0);


            QTextCharFormat formatFuncname;

            formatFuncname.setBackground(color);
            formatFuncname.setForeground(Qt::white);
            formatFuncname.setFontWeight(QFont::Bold);
            highlightCursor.setCharFormat(formatFuncname);
        }


    }


    cursor.endEditBlock();
}
