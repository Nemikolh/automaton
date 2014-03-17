#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtQuick/QQuickView>

namespace nkh {

class CellModel;

namespace priv {

class MainWindow;

}

class MainWindow : public QQuickView
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWindow *parent = 0);
    virtual ~MainWindow();

    void setMainQmlFile(const QString &file);
    void addImportPath(const QString &path);

    void showExpanded();

private:
    priv::MainWindow *d;
    CellModel* m_model;
};

}

#endif // MAINWINDOW_H
