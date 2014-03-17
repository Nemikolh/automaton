/*
 * CellularAutomaton.hpp
 *
 *  Created on: 4 mars 2014
 *      Author: nemikolh
 */

#ifndef CELLULARAUTOMATON_HPP_
#define CELLULARAUTOMATON_HPP_


namespace nkh {

namespace priv {

struct CellularAutomaton;

}  /* namespace priv */

class CellularAutomaton
{
public:

    struct CellColor
    {
        CellColor()
            : r(255)
            , g(255)
            , b(255)
        {}

        CellColor(int p_value)
            : r(p_value)
            , g(p_value)
            , b(p_value)
        {}

        CellColor(int p_r, int p_g, int p_b)
            : r(p_r)
            , g(p_g)
            , b(p_b)
        {}

        int r;
        int g;
        int b;
    };

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    CellularAutomaton();

    ~CellularAutomaton();

    void test() const;

    void load_lua(const char* p_src);

    void set_grid(int p_height, int p_width);

    void run_next_step();

    void call_init_cell();

    void call_set_cell(int p_row, int p_column);

    CellColor state(int p_row, int p_column) const;

    int width() const;

    int height() const;

private:
    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    priv::CellularAutomaton* m_impl;
};

} /* namespace nkh */

#endif /* CELLULARAUTOMATON_HPP_ */
