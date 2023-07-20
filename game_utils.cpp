enum GameState {
    PLAYING = 0,
    PAUSED = 1,
    GAME_OVER = 2
};

enum Direction {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3,
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

    bool operator == (const GridPos& p) {
        return x == p.x && y == p.y;
    }

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

    void setOffset(int grid_w, int grid_h) {
        grid_offset_x = (render_state.width - grid_w * cell_size) / 2;
        grid_offset_y = (render_state.height - grid_h * cell_size) / 2;
    }
};

struct Snake;

struct GameGrid {
    int width;
    int height;
    Snake* snake;
    GridPos fruit;
    GridStyling style;
    GameGrid(int w, int h);
    ~GameGrid();
    void placeFruit();
    void render();
};

struct Snake {
    GameGrid* grid;
    Queue<GridPos> snake;
    Direction dir = LEFT;

    Snake(GameGrid* gg, GridPos startPos) : grid(gg) {
        snake.push(startPos);
    }

    bool move() {
        GridPos newPos;
        switch (dir) {
        case UP: {
            newPos = snake.getHead().get_up();
            break;
        }
        case DOWN: {
            newPos = snake.getHead().get_down();
            break;
        }
        case LEFT: {
            newPos = snake.getHead().get_left();
            break;
        }
        case RIGHT: {
            newPos = snake.getHead().get_right();
            break;
        }
        }

        // If new position is out of bounds, wrap around grid
        if (newPos.x < 0) {
            newPos.x = grid->width - 1;
        }
        else if (newPos.x >= grid->width) {
            newPos.x = 0;
        }

        if (newPos.y < 0) {
            newPos.y = grid->height - 1;
        }
        else if (newPos.y >= grid->height) {
            newPos.y = 0;
        }

        if (checkCollide(newPos)) {
            return true;
        }

        snake.push(newPos);

        // Check if player ate fruit this turn
        if (checkEat(newPos)) {
            grid->placeFruit();
        }
        else {
            snake.pop();
        }
        return false;
    }

    void setDirection(Direction newDir) {
        // Prevent Snake from reversing direction into self
        if ((newDir + dir) % 2) {
            dir = newDir;
        }
    }

    bool checkCollide(GridPos pos) {
        for (int i = 0; i < snake.length; i++) {
            GridPos cell = snake.arr[(snake.tail_index + i) % snake.arrSize];
            if (cell == pos) {
                return true;
            }
        }
        return false;
    }

    bool checkEat(GridPos pos) {
        return pos == grid->fruit;
    }

    void render(GridStyling style) {
        for (int i = 0; i < snake.length; i++) {
            GridPos cell = snake.arr[(snake.tail_index + i) % snake.arrSize];
            draw_rect_in_pixels(cell.x * style.cell_size + style.grid_offset_x, cell.y * style.cell_size + style.grid_offset_y, style.cell_size - style.cell_spacing, style.cell_size - style.cell_spacing, 0xFFFFE0);
        }
    }
};

GameGrid::GameGrid(int w, int h) :width(w), height(h), style(50, 5, 0xFFFFE0) {
    style.setOffset(w, h);
    this->snake = new Snake(this, GridPos(w / 2, h / 2));
    placeFruit();
}

GameGrid::~GameGrid() {
    delete this->snake;
}

void GameGrid::placeFruit() {
    fruit.x = std::rand() % width;
    fruit.y = std::rand() % height;
    if (this->snake->checkCollide(fruit)) {
        placeFruit();
    }
}

void GameGrid::render() {
    // Draw Outline Around Grid
    draw_rect_outline_in_pixels(style.grid_offset_x - style.cell_spacing,
        style.grid_offset_y - style.cell_spacing,
        (width - 1) * style.cell_size + (width + 1) * style.cell_spacing,
        (height - 1) * style.cell_size + (height + 1) * style.cell_spacing,
        1, 0xFFFFE0);

    draw_rect_in_pixels(fruit.x * style.cell_size + style.grid_offset_x,
        fruit.y * style.cell_size + style.grid_offset_y,
        style.cell_size - style.cell_spacing,
        style.cell_size - style.cell_spacing, 0xFFDD20);
}
