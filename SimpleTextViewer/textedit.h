#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QMap>

class TextEdit : public QTextEdit
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = nullptr);
    void setContents(const QString &fileName);
    QVariant loadResource(int type, const QUrl &name) override;
    void highlightText(const QString &searchstring);

signals:
    void fileNameChanged(const QString &fileName);
    void functionCostsComputed(const QMap<QString, int> &costs);

private:
    QUrl srcUrl;

    void highlightCallgrind(const QString &callgrindData);
    void computeCost(const QString &callgrindData);
    void highlightFunctionsByCost(const QMap<QString, int> &functionCost);
};

#endif // TEXTEDIT_H
