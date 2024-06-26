#include "board.hpp"
#include <iostream>


void Board::reset() {
    // deactivate the entire board
    for (int r = 0; r < BOARD_HEIGHT; r++) {
        for (int c = 0; c < BOARD_WIDTH; c++) {
            board_matrix[r][c]->deactivate();
        }
    }
    // activate side walls
    for (int r = 0; r < BOARD_HEIGHT; r++) {
        board_matrix[r][0]->activate(sf::Color(0,0,0));
        board_matrix[r][1]->activate(sf::Color(0,0,0));
        board_matrix[r][BOARD_WIDTH-1]->activate(sf::Color(0,0,0));
        board_matrix[r][BOARD_WIDTH-2]->activate(sf::Color(0,0,0));
    }
    // activate floor
    for (int c = 0; c < BOARD_WIDTH; c++) {
        board_matrix[BOARD_HEIGHT-1][c]->activate(sf::Color(0,0,0));
        board_matrix[BOARD_HEIGHT-2][c]->activate(sf::Color(0,0,0));
    }
}

Block* Board::getBlock(int row, int colu) {
    return board_matrix[row][colu];
}

void Board::removeRow(int row) {
    for (int i=2; i < BOARD_WIDTH-2; i++) { // starts and ends +/- 2 due to two block thick boundary walls
        board_matrix[row][i]->deactivate();
    }
}

bool Board::rowIsFull(int row) {
    for (int i = 2; i < BOARD_WIDTH-2; i++) {
        if (!board_matrix[row][i]->isActive()) {
            return false;
        }
    }
    return true;
}

int Board::checkPlacement(std::array<Block*, 4> blocks) {
    std::vector<int> rows;
    int rows_cleared = 0;
    for (Block* block : blocks) {
        if (std::find(rows.begin(), rows.end(), block->getRow()) == rows.end()) {
            rows.push_back(block->getRow());
        }
    }
    for (int row : rows) {
        if (rowIsFull(row)) {
            removeRow(row);
            pullBlocksDown(row);
            rows_cleared++;
        }
    }
    switch (rows_cleared) {
        case 0:
            return 0;
        case 1:
            return 100;
        case 2:
            return 300;
        case 3:
            return 500;
        case 4:
            return 800;
    }
    return 0;
}

void Board::pullBlocksDown(int& row) {
    for (int r = row-1; r >= 2; r--) {
        for (int c = 2; c < BOARD_WIDTH-2; c++) {
            if (board_matrix[r][c]->isActive()) {
                board_matrix[r][c]->deactivate();
                board_matrix[r+1][c]->activate(sf::Color(0,0,0));
            }
        }
    }
}