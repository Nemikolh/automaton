#ifndef CELLMODEL_H
#define CELLMODEL_H

#include <QAbstractListModel>

class QTimer;
class QQuickItem;

namespace nkh {

class CellularAutomaton;

class CellModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString last_lua_error READ lastError WRITE setLuaError NOTIFY luaConsoleUpdated)
public:
    explicit CellModel(QObject *parent = 0);
    ~CellModel();

    enum class CellRoles {
        StateRole = Qt::UserRole + 1
    };
    
    //bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void set_grid(int p_height, int p_width);
    Q_INVOKABLE void run_lua_code(const QString&);
    Q_INVOKABLE int rows() const;
    Q_INVOKABLE int columns() const;
    Q_INVOKABLE void pause();
    Q_INVOKABLE void setTimeInterval(int);
    Q_INVOKABLE void call_lua_init_cell(const QString&);
    Q_INVOKABLE void setCellState(const QString & p_src, int p_index);
    Q_INVOKABLE void addSyntaxHighlightingSupport(QQuickItem*);

    void setLuaError(const QString&);

    QString lastError() const;

private slots:
    void run_next_step();
    void refresh_data();

signals:
    void luaConsoleUpdated();

private:

    QString last_lua_error;
    CellularAutomaton* m_automaton;
    QTimer* m_timer;
    QTimer* m_refresh_data;
};

} /* namespace nkh */

#endif // CELLMODEL_H
