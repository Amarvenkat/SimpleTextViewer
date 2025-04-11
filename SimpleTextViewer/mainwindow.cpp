#include "assistant.h"
#include "findfiledialog.h"
#include "mainwindow.h"
#include "textedit.h"

#include <QAction>
#include <QApplication>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtCore/QLibraryInfo>
#include <QSplitter>
using namespace Qt::StringLiterals;

MainWindow::MainWindow()
    : textViewer(new TextEdit)
    , assistant(new Assistant)
    , functionCostView(new QTextBrowser)
{
    textViewer->setContents(QLibraryInfo::path(QLibraryInfo::ExamplesPath)
                            + "/assistant/simpletextviewer/documentation/intro.html"_L1);

    connect(textViewer, &TextEdit::functionCostsComputed, this, &MainWindow::updateFunctionCostView);

    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Type a text to search");

    searchButton = new QPushButton(tr("Search"));

    connect(searchButton, &QPushButton::clicked, this, [this]() {
        QString term = searchBar->text().trimmed();
        if (!term.isEmpty()) {
            textViewer->highlightText(term);
        } else {
            textViewer->highlightText("");
        }
    });

    QWidget *centralWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *searchRow = new QHBoxLayout;

    searchRow->addWidget(searchBar);
    searchRow->addWidget(searchButton);

    functionCostView->setReadOnly(true);
    functionCostView->setMinimumHeight(120);
    functionCostView->setStyleSheet("background: black; color: white;");


    mainLayout->addLayout(searchRow);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(textViewer);
    splitter->addWidget(functionCostView);
    splitter->setStretchFactor(0, 7);
    splitter->setStretchFactor(1, 3);
    splitter->setSizes({700, 300});

    mainLayout->addWidget(splitter);
    this->setCentralWidget(centralWidget);

    createActions();
    createMenus();

    setWindowTitle(tr("Simple Text Viewer"));
    resize(750, 500);

    connect(textViewer, &TextEdit::fileNameChanged, this, &MainWindow::updateWindowTitle);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    delete assistant;
}

void MainWindow::updateWindowTitle(const QString &fileName)
{
    setWindowTitle(tr("Simple Text Viewer - %1").arg(fileName));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Text Viewer"),
                       tr("This example demonstrates how to use\n"
                          "Qt Assistant as help system for your\n"
                          "own application."));
}

void MainWindow::showDocumentation()
{
    assistant->showDocumentation("index.html");
}

void MainWindow::open()
{
    FindFileDialog dialog(textViewer, assistant);
    dialog.exec();
}

void MainWindow::createActions()
{
    assistantAct = new QAction(tr("Help Contents"), this);
    assistantAct->setShortcut(QKeySequence::HelpContents);
    connect(assistantAct, &QAction::triggered, this, &MainWindow::showDocumentation);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    clearAct = new QAction(tr("&Clear"), this);
    clearAct->setShortcut(tr("Ctrl+C"));
    connect(clearAct, &QAction::triggered, textViewer, &QTextEdit::clear);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, &QAction::triggered, QApplication::aboutQt);
}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(clearAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(assistantAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::updateFunctionCostView(const QMap<QString, int> &costs)
{
    QStringList lines;
    QList<QString> keys = costs.keys();

    std::sort(keys.begin(), keys.end(), [&costs](const QString &a, const QString &b) {
        return costs[a] > costs[b];
    });

    for (const QString &fn : keys) {
        lines << QString("%1 : %2").arg(fn).arg(costs[fn]);
    }

    functionCostView->setPlainText(lines.join("\n"));
}
