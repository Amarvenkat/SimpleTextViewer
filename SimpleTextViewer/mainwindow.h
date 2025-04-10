#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include "textedit.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class Assistant;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void about();
    void showDocumentation();
    void updateWindowTitle(const QString &fileName);
    void updateFunctionCostView(const QMap<QString, int> &costs);

private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *assistantAct;
    QAction *openAct;
    QAction *clearAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    TextEdit *textViewer;
    Assistant *assistant;

    QLineEdit *searchBar;
    QPushButton *searchButton;

    QTextBrowser *functionCostView;
};

#endif // MAINWINDOW_H
