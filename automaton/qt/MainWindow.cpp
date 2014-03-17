#include "MainWindow.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QQuickItem>
#include <QQuickTextDocument>
#include <QDebug>

#include "CellModel.h"

namespace nkh {

namespace priv {

class MainWindow
{
    friend class nkh::MainWindow;
};

}  /* namespace priv */


MainWindow::MainWindow(QWindow *parent) :
    QQuickView(parent),
    d(new priv::MainWindow),
    m_model(new CellModel(this))
{
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    setResizeMode(QQuickView::SizeRootObjectToView);
}

MainWindow::~MainWindow()
{
    delete d;
}

void MainWindow::setMainQmlFile(const QString &file)
{
    auto ctx = this->rootContext();
    ctx->setContextProperty("automaton", m_model);
    setSource(QUrl(file));
}

void MainWindow::addImportPath(const QString &path)
{
    engine()->addImportPath(path);//priv::MainWindow::adjustPath(path));
}

void MainWindow::showExpanded()
{
    show();
}



} /* namespace nkh */
