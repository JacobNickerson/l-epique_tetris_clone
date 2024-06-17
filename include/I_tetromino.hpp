#ifndef I_TETROMINO_HPP
#define I_TETROMINO_HPP

#include "tetromino.hpp"

class I_Tetromino : public Tetromino {
    private:
        std::array<Block*, 4> blocks;
        Block* pivot;
        int rotation; 
        Color colo;
        static const std::array<std::array<std::pair<int, int>, 4>, 4> rotation_positions;
    
};

#endif