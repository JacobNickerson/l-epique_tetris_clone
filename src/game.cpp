#include "block.hpp"
#include "game.hpp"
#include "tetromino_types.hpp"
#include <array>
#include <iostream>

void Game::run() {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Cetris");

    // Initializing backgrounds
    sf::Texture menu_background_texture;
    if (!menu_background_texture.loadFromFile("./images/menubackground.png")) {
        std::cout << "Menu background failed to load" << std::endl;
        return;
    }
    sf::Sprite menu_background(menu_background_texture);
    sf::Texture play_background_texture;
    if (!play_background_texture.loadFromFile("./images/playbackground.png")) {
        std::cout << "Play background failed to load" << std::endl;
        return;
    }
    sf::Sprite play_background(play_background_texture);

    // Initializing title with our specified font
    if (!game_font.loadFromFile("./fonts/tetris-font.ttf")) {
        std::cout << "Font failed to load" << std::endl;
        return;
    }
    sf::Text title;
    title.setFont(game_font);
    title.setString("Cetris!");
    title.setCharacterSize(200);
    sf::Color title_color = sf::Color::White;
    title_color.a = 0;
    title.setFillColor(title_color);

    // center start screen text
    sf::FloatRect textRect = title.getLocalBounds();
    title.setOrigin(textRect.left + textRect.width/2.0f,
                textRect.top  + textRect.height/2.0f);
    title.setPosition(sf::Vector2f(1600/2.0f,900/2.0f - 252));  // the weird offset is for an animation where the title "slides" into place

    sf::Text press_to_start_message = title;
    press_to_start_message.setString("Press any key to start");
    press_to_start_message.setCharacterSize(50);
    press_to_start_message.setFillColor(sf::Color::White);
    sf::FloatRect start_rect = press_to_start_message.getLocalBounds();
    press_to_start_message.setOrigin(start_rect.left + start_rect.width/2.0f,
                start_rect.top  + start_rect.height/2.0f);
    press_to_start_message.setPosition(sf::Vector2f(1600/2.0f,900/2.0f));

    // Initializing endscreen with our specified font
    sf::Text end_text;
    end_text.setFont(game_font);
    end_text.setString("Game Over!");
    end_text.setCharacterSize(100);
    end_text.setFillColor(sf::Color::White);

    // center end screen text
    sf::FloatRect end_text_rect = end_text.getLocalBounds();
    end_text.setOrigin(end_text_rect.left + end_text_rect.width/2.0f,
                end_text_rect.top  + end_text_rect.height/2.0f);
    end_text.setPosition(sf::Vector2f(1600/2.0f,900/2.0f-200));

    sf::Text end_score = end_text;
    end_score.setString("You Scored: 0 Points");
    end_score.setCharacterSize(50);
    sf::FloatRect end_rect = end_score.getLocalBounds();
    end_score.setOrigin(end_rect.left + end_rect.width/2.0f,
                end_rect.top  + end_rect.height/2.0f);
    end_score.setPosition(sf::Vector2f(1600/2.0f,900/2.0f - 50.0f));
    
    sf::Text end_prompt = end_text;
    end_prompt.setString("Press ESC to play again");
    end_prompt.setCharacterSize(50);
    sf::FloatRect end_prompt_rect = end_prompt.getLocalBounds();
    end_prompt.setOrigin(end_prompt_rect.left + end_prompt_rect.width/2.0f,
                end_prompt_rect.top  + end_prompt_rect.height/2.0f);
    end_prompt.setPosition(sf::Vector2f(1600/2.0f,900/2.0f + 100.0f));

    // Initializing our sprite texture
    sf::Texture block_texture;
    if (!block_texture.loadFromFile("images/block.png")) {
        std::cout << "BLOCK TEXTURE FAILED TO LOAD" << std::endl;
        return;
    }
    
    // Initializing our score box texture
    sf::Texture score_box_texture;
    if (!score_box_texture.loadFromFile("images/scorebox.png")) {
        std::cout << "SCOREBOX TEXTURE FAILED TO LOAD" << std::endl;
        return;
    }    

    // Initializing our sprite board
    game_sprite_board.initializeSpriteMatrix(block_texture);

    // Initializing our scoreboard
    game_sprite_board.initializeScoreBox(score_box_texture, game_font);

    // Initializing our next tetromino window
    game_sprite_board.initializeNextTetrominoBox(score_box_texture);

    // Initializing our next tetromino block matrix
    game_sprite_board.initializeNextTetrominoMatrix(block_texture);

    // Initializing our level window
    game_sprite_board.initializeLevelText(game_font);

    while (window.isOpen()) {
        while (window.isOpen() && game_state == GameState::Title) {
            titleScreen(window, title, press_to_start_message, menu_background);
        }

        while (window.isOpen() && game_state == GameState::GameRunning) {
            playGame(window, play_background);
        }

        while (window.isOpen() && game_state == GameState::GameOver) {
            endScreen(window, end_text, end_score, end_prompt, menu_background);
            reset();
        }
    }
}

bool Game::spawnTetromino(Tetromino* tetromino) {
    tetromino->reset();
    tetromino->movePivot(2,6, game_board);
    if (!tetromino->constructTetromino(game_board)) return false;
    tetromino->activate();
    game_sprite_board.colorTetromino(tetromino);
    return true;
}

bool Game::spawnTetromino(Tetromino* tetromino, int row, int col) {
    tetromino->movePivot(row, col, game_board);
    if (!tetromino->constructTetromino(game_board)) return false;
    tetromino->activate();
    game_sprite_board.colorTetromino(tetromino);
    return true;
}

void Game::titleScreen(sf::RenderWindow& window, sf::Text& title, sf::Text& press_to_start_message, sf::Sprite& title_background) {
    sf::Clock render_clock;

    while (window.isOpen() && game_state == GameState::Title) {
        sf::Event event;

        // pollEvent pops any new events off the event stack and breaks when empty
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { 
                window.close(); 
            }
            if (event.type == sf::Event::KeyPressed) {
                game_state = GameState::GameRunning;
            }
        }
        int time_elapsed = render_clock.getElapsedTime().asMilliseconds();
        if (time_elapsed <= 1020) {
            sf::Color title_color = title.getFillColor();
            title_color.a = render_clock.getElapsedTime().asMilliseconds()/4;
            title.setFillColor(title_color);
            title.move(sf::Vector2f(0, 0.1));
        }

        window.clear();
        window.draw(title_background);
        if (time_elapsed <= 2000) {
            window.draw(title);
        } else if (time_elapsed % 1000 < 500) {
            window.draw(title);
        }
        if (time_elapsed >= 2400) {
            window.draw(press_to_start_message);
        }
        window.display();
    }
}

void Game::playGame(sf::RenderWindow& window, sf::Sprite& play_background) {
    int game_level = 0; // game level progresses according to line clears and determines things like points per clear and drop speed
    int game_clears = 0; // amount of lines cleared, once it reaches 10 we increment game level and reduce it by 10
    // moving our tetromino to the correct spawn location and generating its blocks
    std::uniform_int_distribution<> distribution(0,6);  // setting up RNG to generate a number in range [0,6]
    Tetromino* tetropointer = tetrominos[distribution(RNG)];
    Tetromino* next_tetropointer = tetrominos[distribution(RNG)];
    spawnTetromino(tetropointer);
    next_tet_board.activate(next_tetropointer);
    game_sprite_board.colorNextTetromino(next_tetropointer);

    // resetting our clock to 0
    game_clock.restart();

    while (window.isOpen() && game_state == GameState::GameRunning) {
        if (game_clock.getElapsedTime().asMilliseconds() >= std::max(1000-(100*game_level), 50)) {  // tetromino moves down after a certain elapsed time
            game_clock.restart();
            if (!tetropointer->down(game_board)) {
                placeTetromino(tetropointer, next_tetropointer, game_level, game_clears, distribution(RNG));
                if (!spawnTetromino(tetropointer)) {
                    game_state = GameState::GameOver;
                    return;
                }
            }
            game_sprite_board.colorTetromino(tetropointer);
        }
        // User Inputs
        sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) { 
                    window.close(); 
                } else if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                        case sf::Keyboard::S: 
                            if (!tetropointer->down(game_board)) {
                                placeTetromino(tetropointer, next_tetropointer, game_level, game_clears, distribution(RNG));
                                if (!spawnTetromino(tetropointer)) {
                                    game_state = GameState::GameOver;
                                    return;
                                }
                                break;
                            }
                            game_sprite_board.colorTetromino(tetropointer);
                            break;
                        case sf::Keyboard::A:
                            tetropointer->left(game_board);
                            game_sprite_board.colorTetromino(tetropointer);
                            break;
                        case sf::Keyboard::D:
                            tetropointer->right(game_board);
                            game_sprite_board.colorTetromino(tetropointer);
                            break;
                        case sf::Keyboard::Q:
                            tetropointer->rotateLeft(game_board);
                            game_sprite_board.colorTetromino(tetropointer);
                            break;
                        case sf::Keyboard::E:
                            tetropointer->rotateRight(game_board);
                            game_sprite_board.colorTetromino(tetropointer);
                            break;
                        case sf::Keyboard::Space:
                            tetropointer->hardDrop(game_board);
                            game_sprite_board.colorTetromino(tetropointer);
                            placeTetromino(tetropointer, next_tetropointer, game_level, game_clears, distribution(RNG));
                            if (!spawnTetromino(tetropointer)) {
                                game_state = GameState::GameOver;
                                return;
                            }
                            break;
                    }
                }
            }

        // rendering
        window.clear();
        window.draw(play_background);
        for (int i = 2; i < game_sprite_board.getHeight()-1; i++) {
            for (int j = 1; j < game_sprite_board.getWidth()-1; j++) {
                if (game_board.getBlock(i, j)->isActive()) {
                    window.draw(game_sprite_board.getBoardSprite(i,j));
                }
            }
        }

        for (int i = 0; i < 4; i++) { // i got lazy and didn't make another getter
            for (int j = 0; j < 4; j++) {  // its a 4 length square box every time lol
                if (next_tet_board.getBlock(i, j)->isActive()) {
                    window.draw(game_sprite_board.getNextSprite(i, j));
                }
            }
        }

        window.draw(game_sprite_board.getScoreBoxSprite());
        window.draw(game_sprite_board.getScoreText());
        window.draw(game_sprite_board.getNextTetrominoBox());
        window.draw(game_sprite_board.getLevelText());
        window.display();
    }
}

void Game::reset() {
    game_board.reset();
    game_sprite_board.reset();
    next_tet_board.reset();
    game_state = GameState::Title;
    score = 0;
}

void Game::endScreen(sf::RenderWindow& window, sf::Text& end_text, sf::Text& end_score, sf::Text& end_prompt, sf::Sprite& end_background) {
    sf::Clock render_clock;
    end_score.setString("You Scored: " + std::to_string(score));
    while (window.isOpen() && game_state == GameState::GameOver) {
        sf::Event event;

        // pollEvent pops any new events off the event stack and breaks when empty
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { 
                window.close(); 
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                game_state = GameState::Title;
                
            }
        }
        
        window.clear();
        window.draw(end_background);
        if (render_clock.getElapsedTime().asMilliseconds() >= 1000) {
            window.draw(end_text);
        }

        if (render_clock.getElapsedTime().asMilliseconds() >= 2000) {
            window.draw(end_score);
        }

        if (render_clock.getElapsedTime().asMilliseconds() >= 3000) {
            window.draw(end_prompt);
        }
        window.display();
    }
}

void Game::placeTetromino(Tetromino*& tetropointer, Tetromino*& next_tetropointer, int& game_level, int& game_clears, int RNG_index) {
    score += game_board.checkPlacement(tetropointer->getBlocks(), game_level, game_clears);
    game_sprite_board.setScoreText(score);
    tetropointer = next_tetropointer;
    next_tet_board.reset();
    next_tetropointer = tetrominos[RNG_index];
    next_tet_board.activate(next_tetropointer);
    game_sprite_board.colorNextTetromino(next_tetropointer);
    game_sprite_board.setLevelText(game_level);
}