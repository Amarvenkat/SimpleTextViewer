
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

using namespace Qt::StringLiterals;

MainWindow::MainWindow()
    : textViewer(new TextEdit)
    , assistant(new Assistant)
{
    textViewer->setContents(QLibraryInfo::path(QLibraryInfo::ExamplesPath)
                            + "/assistant/simpletextviewer/documentation/intro.html"_L1);


    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("By default it search for 'callgrind' or simply press the Search button to highlight 'callgrind' ");

    searchButton = new QPushButton(tr("Search"));


    connect(searchButton, &QPushButton::clicked, this, [this]() {
        QString term = searchBar->text().trimmed();
        if (!term.isEmpty()) {
            textViewer->highlightText(term);
        } else {
            textViewer->highlightText("callgrind");
        }
    });


    QWidget *centralWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *searchRow = new QHBoxLayout;

    searchRow->addWidget(searchBar);
    searchRow->addWidget(searchButton);

    mainLayout->addLayout(searchRow);
    mainLayout->addWidget(textViewer);

    this->setCentralWidget(centralWidget);

    createActions();
    createMenus();

    setWindowTitle(tr("Simple Text Viewer"));
    resize(750, 400);

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
