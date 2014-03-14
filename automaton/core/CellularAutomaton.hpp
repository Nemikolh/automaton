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


    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    CellularAutomaton();

    ~CellularAutomaton();

    void test() const;

    void load_lua(const char* p_src);

    void set_grid(unsigned int p_height, unsigned int p_width);

private:
    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    priv::CellularAutomaton* m_impl;
};

} /* namespace nkh */

#endif /* CELLULARAUTOMATON_HPP_ */
