#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QUrl>
#include <QMap>

class TextEdit : public QTextEdit
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = nullptr);
    void setContents(const QString &fileName);
    void highlightCallgrind(const QString &callgrindData);

    void computeCost(const QString &callgrindData);
    void highlightFunctionsByCost(const QMap<QString, int> &functionCost);

signals:
    void fileNameChanged(const QString &fileName);

public slots:
    void highlightText(const QString &searchstring);

private:
    QVariant loadResource(int type, const QUrl &name) override;
    QUrl srcUrl;
};

#endif
