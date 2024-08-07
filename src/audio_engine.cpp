#include "audio_engine.hpp"

// AudioEngineBuffers
bool AudioEngineBuffers::initialize() {
    if (!buffer_place.loadFromFile("./sounds/place_tetromino.ogg")) return false;
    if (!buffer_harddrop.loadFromFile("./sounds/hard_drop.ogg")) return false;
    if (!buffer_menu_transition.loadFromFile("./sounds/menu_transition.ogg")) return false;
    if (!buffer_collision.loadFromFile("./sounds/collision.ogg")) return false;
    if (!buffer_one_line_clear.loadFromFile("./sounds/one_line_clear.ogg")) return false;
    if (!buffer_two_line_clear.loadFromFile("./sounds/two_line_clear.ogg")) return false;
    if (!buffer_three_line_clear.loadFromFile("./sounds/three_line_clear.ogg")) return false;
    if (!buffer_four_line_clear.loadFromFile("./sounds/four_line_clear.ogg")) return false;
    if (!buffer_end_to_start_transition.loadFromFile("./sounds/end_to_start_transition.ogg")) return false;
    if (!buffer_loss.loadFromFile("./sounds/loss.ogg")) return false;
    return true;
}

// AudioEngine
    bool AudioEngine::initialize() {
        if (!buffers.initialize()) return false;
        place.setBuffer(buffers.buffer_place);
        harddrop.setBuffer(buffers.buffer_harddrop);
        menu_transition.setBuffer(buffers.buffer_menu_transition);
        collision.setBuffer(buffers.buffer_collision);
        one_line_clear.setBuffer(buffers.buffer_one_line_clear);
        two_line_clear.setBuffer(buffers.buffer_two_line_clear);
        three_line_clear.setBuffer(buffers.buffer_three_line_clear);
        four_line_clear.setBuffer(buffers.buffer_four_line_clear);
        end_to_start_transition.setBuffer(buffers.buffer_end_to_start_transition);
        loss.setBuffer(buffers.buffer_loss);
        if (!play_music.openFromFile("music/game_music.ogg")) return false;
        return true;
    }

    void AudioEngine::playPlace() {
        place.play();
    }

    void AudioEngine::playHardDrop() {
        harddrop.play();
    }

    void AudioEngine::playMenuTransition() {
        menu_transition.play();
    }

    void AudioEngine::playCollision() {
        collision.play();
    }

    void AudioEngine::playOneLineClear() {
        one_line_clear.play();
    }

    void AudioEngine::playTwoLineClear() {
        two_line_clear.play();
    }

    void AudioEngine::playThreeLineClear() {
        three_line_clear.play();
    }

    void AudioEngine::playFourLineClear() {
        four_line_clear.play();
    }

    void AudioEngine::playMusic() {
        play_music.setLoop(true);
        play_music.play();
    }

    void AudioEngine::pauseMusic() {
        play_music.pause();
    }

    void AudioEngine::stopMusic() {
        play_music.stop();
    }

    void AudioEngine::playEndToStartTransition() {
        end_to_start_transition.play();
    }
    
    void AudioEngine::playLossSound() {
        loss.play();
    }