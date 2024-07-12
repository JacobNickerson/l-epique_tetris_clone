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

std::pair<int, int> Board::checkPlacement(std::array<Block*, 4> blocks, int& game_level, int& game_clears, int& score) {
    std::vector<int> rows;
    int bottom_row = -1;
    int rows_cleared = 0;
    for (Block* block : blocks) {
        if (std::find(rows.begin(), rows.end(), block->getRow()) == rows.end()) {
            rows.push_back(block->getRow());
        }
    }
    for (int row : rows) {
        if (rowIsFull(row)) {
            removeRow(row);
            rows_cleared++;
            bottom_row = std::max(row, bottom_row);
        }
    }
    int score_increase;
    int clears;
    switch (rows_cleared) {
        case 0:
            score_increase = 0;
            clears = 0;
            break;
        case 1:
            score_increase = 40*(1+game_level);
            clears = 1;
            break;
        case 2:
            score_increase = 100*(1+game_level);
            clears = 2;
            break;
        case 3:
            score_increase = 300*(1+game_level);
            clears = 3;
            break;
        case 4:
            score_increase = 1200*(1+game_level);
            clears = 4;
            break;
    }
    game_clears += clears;
    if (game_clears >= 10) {
        game_clears %= 10;
        game_level++;
    }
    score += score_increase;
    return {clears, bottom_row};
}

void Board::pullBlocksDown(int& row) {
    for (int r = row-1; r >= 2; r--) {
        for (int c = 2; c < BOARD_WIDTH-2; c++) {
            if (board_matrix[r][c]->isActive()) {
                board_matrix[r+1][c]->activate(board_matrix[r][c]->getColo());
                board_matrix[r][c]->deactivate();
            }
        }
    }
}

std::vector<std::vector<std::pair<Block*, sf::Color>>> Board::findConnectedChunks(int row) {
    std::vector<std::vector<std::pair<Block*, sf::Color>>> connected_chunks;
    std::vector<std::pair<Block*, sf::Color>> chunk;
    for (int r = 2; r < row; r++) {
        for (int c = 2; c < BOARD_WIDTH-2; c++) {
            if (board_matrix[r][c]->isActive()) {
                floodFillChunk(r, c, chunk);
                connected_chunks.push_back(chunk);
                chunk.clear();
            }
        }
    }
    return connected_chunks;
}

void Board::floodFillChunk(int row, int col, std::vector<std::pair<Block*, sf::Color>>& chunk) {
    if (!board_matrix[row][col]->isActive()) return;  // base case
    chunk.push_back({board_matrix[row][col], board_matrix[row][col]->getColo()});
    board_matrix[row][col]->deactivate();
    if (row-1 >= 2) floodFillChunk(row-1, col, chunk);
    if (row+1 < BOARD_HEIGHT) floodFillChunk(row+1, col, chunk);
    if (col-1 >= 2) floodFillChunk(row, col-1, chunk);
    if (col+1 < BOARD_WIDTH - 2) floodFillChunk(row, col+1, chunk);
}

int Board::findGravityPosition(std::vector<std::pair<Block*, sf::Color>>& chunk) {
    // find bottom block in each column
    std::unordered_map<int, Block*> bottom_coords; // this map stores {col, bottom block}
    for (std::pair<Block*, sf::Color> pair : chunk) {
        if (bottom_coords.find(pair.first->getColu()) == bottom_coords.end()) {
            bottom_coords[pair.first->getColu()] = pair.first;
        } else {
            if (pair.first->getRow() > bottom_coords[pair.first->getColu()]->getRow()) {
                bottom_coords[pair.first->getColu()] = pair.first;
            }
        }
    }

    // find minimum distance downwards to an active block
    int min_dist = 100;
    for (std::pair<int, Block*> pair : bottom_coords) {
        int dist = 0;
        while (!board_matrix[pair.second->getRow() + dist][pair.first]->isActive()) {
            dist++;
        }
        min_dist = std::min(dist, min_dist);
    }
    return min_dist-1;
}

void Board::activateGravityChunk(std::vector<std::pair<Block*, sf::Color>>& chunk, int gravity_dist) {
    for (std::pair<Block*, sf::Color> pair : chunk) {
        board_matrix[pair.first->getRow()+gravity_dist][pair.first->getColu()]->activate(pair.second);
    }
}

void Board::stickyGravity(int row) {
    std::vector<std::vector<std::pair<Block*, sf::Color>>> chunks = findConnectedChunks(row);
    int chunk_num = 0;
    for (auto chunk : chunks) {
        int gravity_dist = findGravityPosition(chunk);
        std::cout << gravity_dist << std::endl;
        activateGravityChunk(chunk, gravity_dist);
    }
}

void Board::setUpDebug() {
    reset();
    for (int j = 5; j < BOARD_WIDTH-2; j++) {
        board_matrix[BOARD_HEIGHT-3][j]->activate();
        board_matrix[BOARD_HEIGHT-5][j]->activate();
    }
    board_matrix[BOARD_HEIGHT-4][5]->activate();
    board_matrix[BOARD_HEIGHT-5][5]->activate();
    board_matrix[BOARD_HEIGHT-4][6]->activate();
    board_matrix[BOARD_HEIGHT-5][6]->activate();
    board_matrix[BOARD_HEIGHT-6][6]->activate();
    for (int j = 2; j < BOARD_WIDTH-2; j++) {
        board_matrix[BOARD_HEIGHT-7][j]->activate();
    }
    board_matrix[BOARD_HEIGHT-7][7]->deactivate();
    board_matrix[BOARD_HEIGHT-8][2]->activate();
    board_matrix[BOARD_HEIGHT-8][BOARD_WIDTH-3]->activate();
    board_matrix[BOARD_HEIGHT-9][BOARD_WIDTH-3]->activate();
    board_matrix[BOARD_HEIGHT-8][BOARD_WIDTH-4]->activate();
    board_matrix[BOARD_HEIGHT-9][BOARD_WIDTH-4]->activate();
}