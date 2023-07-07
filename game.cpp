#define isDown(b) input->buttons[b].isDown
#define isPressed(b) input->buttons[b].isDown && input->buttons[b].changed
#define released(b) !input->buttons[b].isDown && input->buttons[b].changed

int posx = 0;
int posy = 0;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct GridPos {
    int x;
    int y;
    GridPos() : x(0), y(0) {}
    GridPos(int _x, int _y) : x(_x), y(_y) {}

    GridPos get_up() { return GridPos(x, y + 1); }
    GridPos get_down() { return GridPos(x, y - 1); }
    GridPos get_left() { return GridPos(x - 1, y); }
    GridPos get_right() { return GridPos(x + 1, y); }

};

struct GridStyling {
    int cell_size = 50;
    int cell_spacing = 5;
    int grid_offset_x;
    int grid_offset_y;
    int color = 0xFFFFFF;
    GridStyling(int size, int sp, int c) :
        cell_size(size),
        cell_spacing(sp),
        color(c) {}

    void setOffset(int window_w, int window_h) {
        grid_offset_x = (render_state.width - window_w * cell_size) / 2;
        grid_offset_y = (render_state.height - window_h * cell_size) / 2;
    }
};

struct Snake {
    Queue<GridPos> snake;
    Direction dir;
    
    Snake(GridPos startPos) {
        snake.push(startPos);
    }
    void move(bool eat) {
        
        switch (dir) {
            case UP: {
                snake.push(snake.getHead().get_up());
                break;
            }
            case DOWN: {
                snake.push(snake.getHead().get_down());
                break;
            }
            case LEFT: {
                snake.push(snake.getHead().get_left());
                break;
            }
            case RIGHT: {
                snake.push(snake.getHead().get_right());
                break;
            }
        }
        checkCollide();
    }

    void changeDirection(Direction newDir) {
        dir = newDir;
    }
    void checkCollide() {}

    void render(GridStyling style) {
        for (int i = 0; i < snake.length; i++) {
            GridPos cell = snake.arr[snake.tail_index+i];
            draw_rect_in_pixels(cell.x * style.cell_size + style.grid_offset_x, cell.y * style.cell_size + style.grid_offset_y, style.cell_size - style.cell_spacing, style.cell_size - style.cell_spacing, 0xFFFFE0);
        }
    }
};

struct GameGrid {
    int width;
    int height;
    Snake snake;
    GridPos fruit;
    GridStyling style;

    // Sizing / Appearance


    GameGrid() = default;
    GameGrid(int w, int h) :width(w), height(h), snake(GridPos(w/2,h/2)), style(50, 5, 0xFFFFE0) {
        style.setOffset(w, h);
    }
    
    void render() {
        /*
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
            
                draw_rect_in_pixels(j*cell_size + grid_offset_x, i *cell_size + grid_offset_y, cell_size - cell_spacing, cell_size - cell_spacing, );
            }
        }
        */
        snake.render(style);
    }
}; 

GameGrid* grid;
void setup_game() {
    grid = new GameGrid(10, 10);
}

internal void simulate_game(Input* input) {
    render_background(0x1A0f1F);
    grid->render();
    if (isDown(BUTTON_UP)) {
        posy += 1;
    }
    if (isDown(BUTTON_DOWN)) {
        posy -= 1;
    }
    if (isDown(BUTTON_LEFT)) {
        posx -= 1;
    }
    if (isDown(BUTTON_RIGHT)) {
        posx += 1;
    }
}