#include "O_tetromino.hpp"
#include "tetromino.hpp"


bool O_Tetromino::constructTetromino(Board& board) {
    std::array<Block*, 4> new_blocks = blocks;
    expandPivot(board);
    for (size_t i = 0; i < blocks.size(); i++) {
        if (blocks[i]->isActive()) {
            blocks = new_blocks;
            return false;
        }
    }
    return true;
}

bool O_Tetromino::rotateRight(Board& board) {
    return true;
}

bool O_Tetromino::rotateLeft(Board& board) {
    return true;
}

void O_Tetromino::expandPivot(Board& board) {
    blocks[0] = board.getBlock(pivot->getRow(), pivot->getCol());
    blocks[1] = board.getBlock(pivot->getRow()-1, pivot->getCol());
    blocks[2] = board.getBlock(pivot->getRow()-1, pivot->getCol()+1);
    blocks[3] = board.getBlock(pivot->getRow(), pivot->getCol()+1);
}