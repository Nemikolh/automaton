/*
 * CellularAutomaton.cpp
 *
 *  Created on: 4 mars 2014
 *      Author: nemikolh
 */

#include "CellularAutomaton.hpp"

#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <cassert>

#include <lua5.2/lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/std_shared_ptr_converter.hpp>
#include <luabind/iterator_policy.hpp>


namespace nkh {

namespace priv {

static int pcall_handler(lua_State*)
{
    return 1;
}

static void dostring(lua_State* state, char const* str)
{
    lua_pushcclosure(state, &pcall_handler, 0);

    if (luaL_loadbuffer(state, str, std::strlen(str), str))
    {
        std::string err(lua_tostring(state, -1));
        lua_pop(state, 2);
        throw err;
    }

    if (lua_pcall(state, 0, 0, -2))
    {
        std::string err(lua_tostring(state, -1));
        lua_pop(state, 2);
        throw err;
    }

    lua_pop(state, 1);
}

struct CellColor
{
    CellColor()
        : r(0)
        , g(0)
        , b(0)
    {}

    float r;
    float g;
    float b;
};

struct Cell : std::enable_shared_from_this<Cell>
{
    Cell()
        : m_name("Cell n° ")
        , m_state()
        , m_neighbors()
    {
        m_name += std::to_string(++test);
    }

    void add_neighbor(const std::shared_ptr<Cell>& p_other)
    {
        m_neighbors.push_back(p_other);
    }

    void clear_neighbors()
    {
        m_neighbors.clear();
    }

    std::string         m_name;
    CellColor           m_state;
    CellColor           m_old_state;
    std::vector<std::shared_ptr<Cell>> m_neighbors;
    static int test;
};

int Cell::test = 0;

struct CellularAutomaton
{
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    CellularAutomaton()
        : m_lua_state(nullptr)
        , m_width(0)
        , m_height(0)
        , m_cells()
    {
        m_lua_state = luaL_newstate();
    }

    ~CellularAutomaton()
    {
        lua_close(m_lua_state);
    }

    void register_to_lua()
    {
        luaL_openlibs(m_lua_state);

        using namespace luabind;
        open(m_lua_state);

        module(m_lua_state)
        [
             class_<CellColor>("Color")
                 .def(constructor<>())
                 .def_readwrite("r", &CellColor::r)
                 .def_readwrite("g", &CellColor::g)
                 .def_readwrite("b", &CellColor::b),
             class_<Cell, std::shared_ptr<Cell>>("Cell")
                 .def(constructor<>())
                 .def_readonly("name", &Cell::m_name)
                 .def_readwrite("state", &Cell::m_state)
                 .def_readwrite("old_state", &Cell::m_old_state)
                 .def_readwrite("neighbors", &Cell::m_neighbors, return_stl_iterator)
                 .def("add_neighbor", &Cell::add_neighbor)
        ];

    }

    void test() const
    {
        try
        {
            dostring(m_lua_state, "print(4)");
            dostring(m_lua_state, "c = Cell()");
            dostring(m_lua_state, "a = Cell()");
            dostring(m_lua_state, "a.state.r = 10 a.state.b = 0.5");
            dostring(m_lua_state, "print(\" c = \" .. c.name .. \" a = \".. a.name)");
            dostring(m_lua_state, "c:add_neighbor(a)");
            dostring(m_lua_state, "for n in c.neighbors do print(n.name .. \" state: \" .. n.state.r .. \", \" .. n.state.g .. \", \" .. n.state.b) end");
        }
        catch(const std::string& err)
        {
            std::cout << err << std::endl;
        }
    }

    void load_lua(const char* p_src) const
    {
        dostring(m_lua_state, p_src);
    }

    void set_grid(unsigned int p_height, unsigned int p_width)
    {
        m_height = p_height;
        m_width = p_width;
        assert(m_height > 0 && m_width > 0);
        m_cells.clear();
        m_cells.reserve(p_height * p_width);

        apply([this](int, int) {
            m_cells.emplace_back(std::make_shared<Cell>());
        });
    }

    /**
     * @brief Set all the cell to have the moore neighborhood.
     * @param p_order is the order of the moore neighborhood.
     */
    void set_moore_neighborhood(int p_order)
    {
        clean_neighbors();
        apply([this, p_order](int i, int j){
            for(int k = i - p_order; k <= i + p_order; ++k)
                for(int l = j - p_order; l <= j + p_order; ++l)
                {
                    if(is_index_valid(k, l) && (k != i) && (l != j))
                        cell(i, j)->add_neighbor(cell(k,l));
                }
        });
    }

    void set_von_neumann_neighborhood()
    {
        clean_neighbors();
        apply([this](int i, int j) {
            if(i > 0)           cell(i,j)->add_neighbor(cell(i-1, j));
            if(i < m_height-1)  cell(i,j)->add_neighbor(cell(i+1, j));
            if(j > 0)           cell(i,j)->add_neighbor(cell(i, j-1));
            if(j < m_width-1)   cell(i,j)->add_neighbor(cell(i, j+1));
        });
    }

    void clean_neighbors()
    {
        apply([this](int i, int j) {
            cell(i,j)->clear_neighbors();
        });
    }

private:

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    template<typename F>
    void apply(F&& func)
    {
        for(int i = 0 ; i < m_height; ++i)
            for(int j = 0; j < m_width; ++j)
            {
                func(i, j);
            }
    }

    bool is_index_valid(int i, int j)
    {
        return (i > 0) && (j > 0) && (i < m_height - 1) && (j < m_width - 1);
    }

    std::shared_ptr<Cell> & cell(int i, int j)
    {
        return m_cells[i * m_width +  j];
    }

    const std::shared_ptr<Cell> & cells(int i, int j) const
    {
        return m_cells[i * m_width + j];
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    lua_State* m_lua_state;
    int m_width;
    int m_height;
    std::vector<std::shared_ptr<Cell>> m_cells;
};

}  /* namespace priv */

CellularAutomaton::CellularAutomaton()
    : m_impl(new priv::CellularAutomaton)
{
    m_impl->register_to_lua();
}

CellularAutomaton::~CellularAutomaton()
{
    delete m_impl;
}

void CellularAutomaton::test() const
{
    m_impl->test();
}

void CellularAutomaton::load_lua(const char* p_src)
{
    m_impl->load_lua(p_src);
}

void CellularAutomaton::set_grid(unsigned int p_height, unsigned int p_width)
{
    m_impl->set_grid(p_height, p_width);
    m_impl->set_von_neumann_neighborhood();
}

} /* namespace nkh */
