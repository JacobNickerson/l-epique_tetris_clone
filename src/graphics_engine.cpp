#include "graphics_engine.hpp"
#include <iostream>

bool GraphicsEngine::initialize() {
    // Load font for all text in the game
    if (!game_font.loadFromFile("fonts/cetris-font.ttf")) {
        std::cout << "Game font failed to load!" << std::endl;
        return false;
    }

    // Initialize text for title
    title_text.setFont(game_font);
    title_text.setString("Cetris!");
    title_text.setCharacterSize(200);
    title_color = sf::Color::White;
    title_color.a = 0;
    title_text.setFillColor(title_color);

    // Centering the title text
    sf::FloatRect textRect = title_text.getLocalBounds();
    title_text.setOrigin(textRect.left + textRect.width/2.0f,
                textRect.top  + textRect.height/2.0f);
    title_text.setPosition(sf::Vector2f(window_width/2.0f,window_height/2.0f - 252));  // the weird offset is for an animation where the title "slides" into place

    // Initializing press to start text
    press_to_start_text = title_text;
    press_to_start_text.setString("Press enter to start");
    press_to_start_text.setCharacterSize(50);
    press_to_start_text.setFillColor(sf::Color::White);
    sf::FloatRect start_rect = press_to_start_text.getLocalBounds();
    press_to_start_text.setOrigin(start_rect.left + start_rect.width/2.0f,
                start_rect.top  + start_rect.height/2.0f);
    press_to_start_text.setPosition(sf::Vector2f(window_width/2.0f,window_height/2.0f));

    // Initialize score text for score window
    score_text.setString("0");
    score_text.setFont(game_font);
    score_text.setFillColor(sf::Color::White);
    score_text.setCharacterSize(50);
    
    float score_textX = 744.0 + ((320 - score_text.getGlobalBounds().width)/2);  // scorebox is 320 px wide
    float score_textY = 109.0 + ((192 - score_text.getGlobalBounds().height)/2); // scorebox is 109 px tall

    score_text.setPosition(sf::Vector2f(score_textX, score_textY));

    // Initialize text for level window
    level_text.setFont(game_font);
    level_text.setPosition(sf::Vector2f(score_text.getPosition().x, score_text.getPosition().y+(12*32))); // position is set relative to score_text, call its init first
    level_text.setString("1");                                                                            // 12 * 32 corresponds to twelve 32 pixel squares down
    level_text.setFillColor(sf::Color::White);
    level_text.setCharacterSize(50);
    
    // Initializing end screen text
    end_screen_text.setFont(game_font);
    end_screen_text.setString("Game Over!");
    end_screen_text.setCharacterSize(100);
    end_screen_text.setFillColor(sf::Color::White);

    // Centering the end screen text
    sf::FloatRect end_text_rect = end_screen_text.getLocalBounds();
    end_screen_text.setOrigin(end_text_rect.left + end_text_rect.width/2.0f,
                end_text_rect.top  + end_text_rect.height/2.0f);
    end_screen_text.setPosition(sf::Vector2f(window_width/2.0f,window_height/2.0f-200));

    // Initializing end score text
    end_score_text = end_screen_text;
    end_score_text.setString("You Scored: 0 Points");
    end_score_text.setCharacterSize(50);

    // Centering the end score text
    sf::FloatRect end_rect = end_score_text.getLocalBounds();
    end_score_text.setOrigin(end_rect.left + end_rect.width/2.0f,
                end_rect.top  + end_rect.height/2.0f);
    end_score_text.setPosition(sf::Vector2f(window_width/2.0f,window_height/2.0f - 50.0f));
    
    // Initializing end prompt text
    end_prompt_text = end_screen_text;
    end_prompt_text.setString("Press ESC to play again");
    end_prompt_text.setCharacterSize(50);

    // Centering the end prompt text
    sf::FloatRect end_prompt_rect = end_prompt_text.getLocalBounds();
    end_prompt_text.setOrigin(end_prompt_rect.left + end_prompt_rect.width/2.0f,
                end_prompt_rect.top  + end_prompt_rect.height/2.0f);
    end_prompt_text.setPosition(sf::Vector2f(window_width/2.0f,window_height/2.0f + 100.0f));

    // Initialize text for pause
    pause_text.setFont(game_font);
    pause_text.setString("PAUSED");
    pause_text.setCharacterSize(200);
    pause_text.setFillColor(sf::Color::White);
    pause_text.setOutlineThickness(2);
    pause_text.setOutlineColor(sf::Color::Black);

    // Centering the pause text
    sf::FloatRect pause_rect = pause_text.getLocalBounds();
    pause_text.setOrigin(pause_rect.left + pause_rect.width/2.0f,
                pause_rect.top  + pause_rect.height/2.0f);
    pause_text.setPosition(sf::Vector2f(window_width/2.0f,window_height/2.0f-200));  // the weird offset is for an animation where the title "slides" into place

    // Initializing pause prompt text
    pause_prompt_text = pause_text;
    pause_prompt_text.setString("Press ESCAPE to resume");
    pause_prompt_text.setCharacterSize(50);
    pause_prompt_text.setFillColor(sf::Color::White);
    pause_prompt_text.setOutlineThickness(2);
    pause_prompt_text.setOutlineColor(sf::Color::Black);
    sf::FloatRect pause_prompt_rect = pause_prompt_text.getLocalBounds();
    pause_prompt_text.setOrigin(pause_prompt_rect.left + pause_prompt_rect.width/2.0f,
                pause_prompt_rect.top  + pause_prompt_rect.height/2.0f);
    pause_prompt_text.setPosition(sf::Vector2f(window_width/2.0f,window_height/2.0f));

    // Load texture for all block sprites
    if (!block_texture.loadFromFile("images/block.png")) {
        std::cout << "Block sprite failed to load!" << std::endl;
        return false;
    }

    // Initialize sprites for the play field matrix
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            sprite_board_matrix[i][j]->setTexture(block_texture);
            sprite_board_matrix[i][j]->setScale(sf::Vector2f(0.5, 0.5));
            sprite_board_matrix[i][j]->setPosition(sf::Vector2f(200.0, 45.0));
            sprite_board_matrix[i][j]->move(sf::Vector2f(32.0*j, 32.0*i));
        }
    }

    // Initialize sprites for next tetromino matrix
    for (int i = 0; i < next_tetromino_matrix.size(); i++) {
        for (int j = 0; j < next_tetromino_matrix[0].size(); j++) {
            next_tetromino_matrix[i][j]->setTexture(block_texture);
            next_tetromino_matrix[i][j]->setScale(sf::Vector2f(0.5, 0.5));
            next_tetromino_matrix[i][j]->setPosition(sf::Vector2f(744.0+(32*3), 301.0+(32*1))); // offset by block size * number of blocks, ie 32 px * 3 blocks
            next_tetromino_matrix[i][j]->move(sf::Vector2f(32.0*j, 32.0*i));
        }
    }

    // Load texture for windows
    if (!box_texture.loadFromFile("images/scorebox.png")) {
        std::cout << "Box sprite failed to load!" << std::endl;
        return false;
    }


    // Loading background textures
    if (!menu_background_texture.loadFromFile("./images/menubackground.png")) {
        std::cout << "Menu background failed to load" << std::endl;
        return false;
    }
    if (!play_background_texture.loadFromFile("./images/playbackground.png")) {
        std::cout << "Play background failed to load" << std::endl;
        return false;
    }

    // Initializing background sprites
    menu_background.setTexture(menu_background_texture);
    play_background.setTexture(play_background_texture);
    
    return true;
}

sf::Sprite GraphicsEngine::getBoardSprite(int row, int col) {
    return *sprite_board_matrix[row][col];
}

sf::Sprite GraphicsEngine::getScoreBoxSprite() {
    return score_box;
}

sf::Text GraphicsEngine::getScoreText() {
    return score_text;
}

sf::Text GraphicsEngine::getLevelText() {
    return level_text;
}

void GraphicsEngine::setLevelText(int level) {
    level_text.setString(std::to_string(level+1));
}

sf::Sprite GraphicsEngine::getNextTetrominoBox() {
    return next_tetromino_box;
}

sf::Sprite GraphicsEngine::getNextSprite(int i, int j) {
    return *next_tetromino_matrix[i][j];
}

void GraphicsEngine::setScoreText(int score) {
    score_text.setString(std::to_string(score));
    float score_textX = 744.0 + ((320 - score_text.getGlobalBounds().width)/2);  // scorebox is 320 px wide
    float score_textY = 109.0 + ((192 - score_text.getGlobalBounds().height)/2); // scorebox is 109 px tall
    score_text.setPosition(sf::Vector2f(score_textX, score_textY));
}

int GraphicsEngine::getWidth() {
    return sprite_board_matrix[0].size();
}

int GraphicsEngine::getHeight() {
    return sprite_board_matrix.size();
}

void GraphicsEngine::colorTetromino(Tetromino* tetromino) {
    for (Block* block : tetromino->getBlocks()) {
        sprite_board_matrix[block->getRow()][block->getColu()]->setColor(tetromino->getColor());
    }
}

void GraphicsEngine::reset() {
    setScoreText(0);
    setLevelText(0);
}

void GraphicsEngine::colorNextTetromino(Tetromino* tetromino) {
    std::array<std::pair<int, int>, 4> rotation_offsets = tetromino->getRotationPositions();
    for (int i = 0; i < 4; i++) {
        next_tetromino_matrix[2+rotation_offsets[i].first][1+rotation_offsets[i].second]->setColor(tetromino->getColor());
    }
}

void GraphicsEngine::titleSlideAnimation(sf::RenderWindow& window, sf::Clock& render_clock) {
    int time_elapsed = 0;
    while (time_elapsed <= 1020) {
        time_elapsed = render_clock.getElapsedTime().asMilliseconds();
        title_color = title_text.getFillColor();
        title_color.a = time_elapsed/4;
        title_text.setFillColor(title_color);
        title_text.move(sf::Vector2f(0, 0.1));
        window.clear();
        window.draw(menu_background);
        window.draw(title_text);
        window.display();
    }
    title_text.setFillColor(sf::Color::White);
}

void GraphicsEngine::titleToPlayAnimation(sf::RenderWindow& window, sf::Clock& render_clock) {
    sf::CircleShape transition_circle(1.0f);
    transition_circle.setFillColor(sf::Color::Black);
    transition_circle.setPosition(window_width / 2.f, window_height / 2.f);
    transition_circle.setOrigin(transition_circle.getRadius(), transition_circle.getRadius());
    sf::Clock circle_clock;

    while (transition_circle.getRadius() <= 1000) {
        transition_circle.setRadius(circle_clock.getElapsedTime().asMilliseconds());
        transition_circle.setOrigin(transition_circle.getRadius(), transition_circle.getRadius());
        window.clear();
        window.draw(menu_background);
        window.draw(title_text);
        int time_elapsed = render_clock.getElapsedTime().asMilliseconds();
        if (time_elapsed >= 2400 && time_elapsed % 1000 < 500) {
            window.draw(press_to_start_text);
        }
        window.draw(transition_circle);
        window.display();
    }
}
void GraphicsEngine::titleToPlayAnimation2(sf::RenderWindow& window, Board& game_board, Board& next_tet_board) {
    // rendering transition
    sf::CircleShape transition_circle(1000.0f);
    transition_circle.setFillColor(sf::Color::Black);
    transition_circle.setPosition(window_width / 2.f, window_height / 2.f);
    transition_circle.setOrigin(transition_circle.getRadius(), transition_circle.getRadius());
    sf::Clock circle_clock;

    while (transition_circle.getRadius() >= 0) {
        transition_circle.setRadius(1000.0f - circle_clock.getElapsedTime().asMilliseconds());
        transition_circle.setOrigin(transition_circle.getRadius(), transition_circle.getRadius());
        window.clear();
        window.draw(play_background);
        for (int i = 2; i < getHeight()-1; i++) {
            for (int j = 1; j < getWidth()-1; j++) {
                if (game_board.getBlock(i, j)->isActive()) {
                    window.draw(getBoardSprite(i,j));
                }
            }
        }

        for (int i = 0; i < 4; i++) { // i got lazy and didn't make another getter
            for (int j = 0; j < 4; j++) {  // its a 4 length square box every time lol
                if (next_tet_board.getBlock(i, j)->isActive()) {
                    window.draw(getNextSprite(i, j));
                }
            }
        }

        window.draw(score_text);
        window.draw(level_text);
        window.draw(transition_circle);
        window.display();
    }
}

void GraphicsEngine::endToTitleAnimation(sf::RenderWindow& window, sf::Clock& render_clock) {
    sf::RectangleShape transition_rect(sf::Vector2f(window_width,window_height));
    sf::Color transition_color(255,255,255,0);
    transition_rect.setFillColor(transition_color);
    for (int i = 0; i < 52; i++) {  // can't use color.a for comparison
        window.clear();
        window.draw(menu_background);
        if (render_clock.getElapsedTime().asMilliseconds() >= 1000) {
            window.draw(end_screen_text);
        }

        if (render_clock.getElapsedTime().asMilliseconds() >= 2000) {
            window.draw(end_score_text);
        }

        if (render_clock.getElapsedTime().asMilliseconds() >= 3000) {
            window.draw(end_prompt_text);
        }
        window.draw(transition_rect);
        window.display();
        transition_color.a += 5;
        transition_rect.setFillColor(transition_color);
    }
}

void GraphicsEngine::setEndScoreText(int& game_score) {
    end_score_text.setString("You Scored: " + std::to_string(game_score) + " Points");

    // Centering the end score text
    sf::FloatRect end_rect = end_score_text.getLocalBounds();
    end_score_text.setOrigin(end_rect.left + end_rect.width/2.0f,
                end_rect.top  + end_rect.height/2.0f);
    end_score_text.setPosition(sf::Vector2f(window_width/2.0f,window_height/2.0f - 50.0f));
}

void GraphicsEngine::lossAnimation(sf::RenderWindow& window, Board& game_board, Board& next_tet_board) {
    sf::Clock flash_clock;
    sf::Clock block_remove_clock;
    sf::RectangleShape screen_color_animation(sf::Vector2f(window_width,window_height));
    screen_color_animation.setFillColor(sf::Color::Black);
    bool color_change = false;
    int row = 0;
    while (row < BOARD_HEIGHT-1) {
        if (flash_clock.getElapsedTime().asMilliseconds() > 400) {
            if (block_remove_clock.getElapsedTime().asMilliseconds() > 150) {
                block_remove_clock.restart();
                game_board.removeRow(row);
                row++;
            }
        }
        window.clear();
        window.draw(play_background);
        for (int i = 2; i < getHeight()-1; i++) {
            for (int j = 1; j < getWidth()-1; j++) {
                if (game_board.getBlock(i, j)->isActive()) {
                    window.draw(getBoardSprite(i,j));
                }
            }
        }

        for (int i = 0; i < 4; i++) { // i got lazy and didn't make another getter
            for (int j = 0; j < 4; j++) {  // its a 4 length square box every time lol
                if (next_tet_board.getBlock(i, j)->isActive()) {
                    window.draw(getNextSprite(i, j));
                }
            }
        }

        window.draw(score_text);
        window.draw(level_text);
        if (flash_clock.getElapsedTime().asMilliseconds() <= 200) {
            if (flash_clock.getElapsedTime().asMilliseconds() > 100 && !color_change) {
                screen_color_animation.setFillColor(sf::Color::White);
                color_change = !color_change;
            }
            window.draw(screen_color_animation);
        }
        window.display();
    }
}

void GraphicsEngine::colorBlock(Block* block) {
    sprite_board_matrix[block->getRow()][block->getColu()]->setColor(block->getColo());
}


void GraphicsEngine::lineClearAnimation(sf::RenderWindow& window, Board& game_board, Board& next_tet_board, std::vector<int>& rows) {

    sf::Clock render_clock;
    bool black = false;
    for (int j = 2; j < getWidth()-2; j++) {
        for (int i : rows)
        sprite_board_matrix[i][j]->setColor(sf::Color::White);
    }

    while (render_clock.getElapsedTime().asMilliseconds() < 200) {
        window.clear();
        window.draw(play_background);

        if (render_clock.getElapsedTime().asMilliseconds() > 100 && black == false) {
            for (int j = 2; j < getWidth()-2; j++) {
                for (int i : rows)
                sprite_board_matrix[i][j]->setColor(sf::Color::Black);
            }
            black = !black;
        }
        for (int i = 2; i < getHeight()-1; i++) {
            for (int j = 1; j < getWidth()-1; j++) {
                if (game_board.getBlock(i, j)->isActive()) {
                    window.draw(getBoardSprite(i,j));
                }
            }
        }

        for (int i = 0; i < 4; i++) { // i got lazy and didn't make another getter
            for (int j = 0; j < 4; j++) {  // its a 4 length square box every time lol
                if (next_tet_board.getBlock(i, j)->isActive()) {
                    window.draw(getNextSprite(i, j));
                }
            }
        }

        window.draw(score_text);
        window.draw(level_text);
        window.display();
    }
}