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

    GridPos getUp() { return GridPos(x, y + 1); }
    GridPos getDown() { return GridPos(x, y - 1); }
    GridPos getLeft() { return GridPos(x - 1, y); }
    GridPos getRight() { return GridPos(x + 1, y); }

    bool operator == (const GridPos& p) {
        return x == p.x && y == p.y;
    }

};

struct GridStyling {
    int cellSize = 50;
    int cellSpacing = 5;
    int gridOffsetX;
    int gridOffsetY;
    int color = 0xFFFFFF;
    GridStyling(int size, int sp, int c) :
        cellSize(size),
        cellSpacing(sp),
        color(c) {}

    void setOffset(int gridWidth, int gridHeight) {
        gridOffsetX = (renderState.width - gridWidth * cellSize) / 2;
        gridOffsetY = (renderState.height - gridHeight * cellSize) / 2;
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
            newPos = snake.getHead().getUp();
            break;
        }
        case DOWN: {
            newPos = snake.getHead().getDown();
            break;
        }
        case LEFT: {
            newPos = snake.getHead().getLeft();
            break;
        }
        case RIGHT: {
            newPos = snake.getHead().getRight();
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
            GridPos cell = snake.arr[(snake.tailIndex + i) % snake.arrSize];
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
            GridPos cell = snake.arr[(snake.tailIndex + i) % snake.arrSize];
            drawRect(cell.x * style.cellSize + style.gridOffsetX, cell.y * style.cellSize + style.gridOffsetY, style.cellSize - style.cellSpacing, style.cellSize - style.cellSpacing, 0xFFFFE0);
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
    drawRectOutline(style.gridOffsetX - style.cellSpacing,
        style.gridOffsetY - style.cellSpacing,
        (width - 1) * style.cellSize + (width + 1) * style.cellSpacing,
        (height - 1) * style.cellSize + (height + 1) * style.cellSpacing,
        1, 0xFFFFE0);
    
    //Draw fruit
    drawRect(fruit.x * style.cellSize + style.gridOffsetX,
        fruit.y * style.cellSize + style.gridOffsetY,
        style.cellSize - style.cellSpacing,
        style.cellSize - style.cellSpacing, 0xFFDD20);
}
