#include "textedit.h"

#include <QFile>
#include <QFileInfo>
#include <QTextDocument>
#include <QTextCursor>
#include <QRegularExpression>
#include <QTextCharFormat>

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

        if (fileName.endsWith(".callgrind")) {
            highlightCallgrind(data);
        }
        if (fileName.endsWith(".html"))
            setHtml(data);
        else
            setPlainText(data);
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

    //new CallgrindHighlighter(this->document());
}
