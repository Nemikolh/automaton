#include "CellModel.h"
#include "../core/CellularAutomaton.hpp"
#include <QColor>
#include <iostream>
#include <QTimer>
#include <QDebug>
#include <QQuickItem>
#include <QQuickTextDocument>
#include "LuaHighlighter.h"

namespace nkh {

CellModel::CellModel(QObject *parent)
    : QAbstractListModel(parent)
    , last_lua_error()
    , m_automaton(new CellularAutomaton)
    , m_timer(new QTimer(this))
    , m_refresh_data(new QTimer(this))
{
    connect(m_timer, SIGNAL(timeout()), this, SLOT(run_next_step()));
    connect(m_refresh_data, SIGNAL(timeout()), this, SLOT(refresh_data()));
    m_refresh_data->setInterval(60);
}

CellModel::~CellModel()
{
    delete m_automaton;
}

void CellModel::addSyntaxHighlightingSupport(QQuickItem * p_element)
{
    QVariant doc = p_element->property("textDocument");
    if (doc.canConvert<QQuickTextDocument*>()) {
        QQuickTextDocument *qqdoc = doc.value<QQuickTextDocument*>();
        if (qqdoc)
            new LuaHighlighter(qqdoc->textDocument());
    }
}

QHash<int, QByteArray> CellModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(CellRoles::StateRole)] = "cell_state";
    return roles;
}

int CellModel::rowCount(const QModelIndex &) const
{
    return m_automaton->height() * this->m_automaton->width();
}

int CellModel::rows() const
{
    return m_automaton->height();
}

int CellModel::columns() const
{
    return m_automaton->width();
}

//bool CellModel::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//    if(role == static_cast<int>(CellRoles::StateRole))
//    {
//        auto column = index.row() % this->columns();
//        auto row = index.row() / this->columns();
//        //m_automaton->
//        this->dataChanged(index, index);
//        return true;
//    }
//    return false;
//}

QVariant CellModel::data(const QModelIndex &index, int role) const
{
    if(role == static_cast<int>(CellRoles::StateRole))
    {
        auto column = index.row() % this->columns();
        auto row = index.row() / this->columns();
        auto color = m_automaton->state(row, column);
        return QColor(color.r, color.g, color.b);
    }

    return QVariant();
}

void CellModel::set_grid(int p_height, int p_width)
{
    auto size = p_height * p_width;
    this->beginRemoveRows(QModelIndex{}, 0, m_automaton->height() * m_automaton->width() - 1);
    this->endRemoveRows();
    this->beginInsertRows(QModelIndex{}, 0, size - 1);
    m_automaton->set_grid(p_height, p_width);
    this->endInsertRows();
}

void CellModel::setTimeInterval(int p_interval)
{
    m_timer->setInterval(p_interval);
}

void CellModel::pause()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
        m_refresh_data->stop();
        last_lua_error += "<i>Script Paused.</i><br/>";
        emit luaConsoleUpdated();
    }
}

void CellModel::setLuaError(const QString & p_err)
{
    m_timer->stop();
    m_refresh_data->stop();
    QString err(p_err);
    last_lua_error += " <span style=\" color:#aa0000;\">"
            + err.replace(QRegExp("<(.*)>"), "\\1") + " </span><br/>";
    emit luaConsoleUpdated();
}

QString CellModel::lastError() const
{
    return last_lua_error;
}

void CellModel::call_lua_init_cell(const QString & p_src)
{
    try
    {
        m_automaton->load_lua(p_src.toStdString().c_str());
        m_automaton->call_init_cell();
        //this->beginResetModel();
        //this->endResetModel();
        emit dataChanged(index(0,0), index(m_automaton->width() * m_automaton->height(), 1), {static_cast<int>(CellRoles::StateRole)});
    }
    catch(const std::string& err)
    {
        setLuaError(QString::fromStdString(err));
    }
}

void CellModel::setCellState(const QString & p_src, int p_index)
{
    try
    {
        auto column = p_index % this->columns();
        auto row = p_index / this->columns();
        m_automaton->load_lua(p_src.toStdString().c_str());
        m_automaton->call_set_cell(row, column);
        //this->beginResetModel();
        //this->endResetModel();
        emit dataChanged(index(0,0), index(m_automaton->width() * m_automaton->height(), 1), {static_cast<int>(CellRoles::StateRole)});
    }
    catch(const std::string& err)
    {
        setLuaError(QString::fromStdString(err));
    }
}

void CellModel::run_lua_code(const QString & p_src)
{
    try
    {
        last_lua_error += "<i>Script Started...</i><br/>";
        emit luaConsoleUpdated();
        m_automaton->load_lua(p_src.toStdString().c_str());
        m_timer->start();
        m_refresh_data->start();
    }
    catch(const std::string& err)
    {
        setLuaError(QString::fromStdString(err));
    }
}

void CellModel::run_next_step()
{
    try
    {
        m_automaton->run_next_step();
    }
    catch(const std::string& err)
    {
        setLuaError(QString::fromStdString(err));
    }
}

void CellModel::refresh_data()
{
    emit dataChanged(index(0,0), index(m_automaton->width() * m_automaton->height(), 1), {static_cast<int>(CellRoles::StateRole)});
}

Qt::ItemFlags CellModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

} /* namespace nkh */
