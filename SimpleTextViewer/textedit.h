
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QUrl>

class TextEdit : public QTextEdit
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = nullptr);
    void setContents(const QString &fileName);

signals:
    void fileNameChanged(const QString &fileName);

public slots:
    void highlightText(const QString &searchstring);

private:
    QVariant loadResource(int type, const QUrl &name) override;
    QUrl srcUrl;
};

#endif
