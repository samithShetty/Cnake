#include <cstdlib>
#include "game_utils.cpp"

#define isDown(b) input->buttons[b].isDown

global_variable GameGrid* grid;
global_variable GameState state;
int visibilityTimer = 1;

void setup_game() {
    grid = new GameGrid(10, 10);
    state = PLAYING;
}

void reset_game() {
    delete grid;
    visibilityTimer = 1;
    setup_game();
}

internal void simulate_game(Input* input) {
    switch (state) {
    case PLAYING: {
        if (isDown(BUTTON_UP)) {
            grid->snake->setDirection(UP);
        }
        else if (isDown(BUTTON_DOWN)) {
            grid->snake->setDirection(DOWN);
        }
        else if (isDown(BUTTON_LEFT)) {
            grid->snake->setDirection(LEFT);
        }
        else if (isDown(BUTTON_RIGHT)) {
            grid->snake->setDirection(RIGHT);
        }
        bool collided = grid->snake->move();
        if (collided) {
            state = GAME_OVER;
        }
        break;
    }

    case GAME_OVER: {
        visibilityTimer++;
        if (visibilityTimer > 20) {
            reset_game();
        }
        break;
    }
    }
}

internal void render_game() {
    renderBackground(0x1A0f1F);
    grid->render();
    if (visibilityTimer % 8 < 4) { // Produces blinking animation that plays on death
        grid->snake->render(grid->style);
    }

}