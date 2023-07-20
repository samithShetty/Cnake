#include <cstdlib>
#include "game_utils.cpp"

#define isDown(b) input->buttons[b].isDown
#define isPressed(b) input->buttons[b].isDown && input->buttons[b].changed
#define released(b) !input->buttons[b].isDown && input->buttons[b].changed

global_variable GameGrid* grid;
global_variable GameState state;
int visibility_timer = 1;

void setup_game() {
    grid = new GameGrid(10, 10);
    state = PLAYING;
}

void reset_game() {
    delete grid;
    visibility_timer = 1;
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
        visibility_timer++;
        if (visibility_timer > 20) {
            reset_game();
        }
        break;
    }
    }
}

internal void render_game() {
    render_background(0x1A0f1F);
    //draw_rect_in_pixels()
    grid->render();
    if (visibility_timer % 8 < 4) { // Produces timed flicker that plays on game over
        grid->snake->render(grid->style);
    }

}