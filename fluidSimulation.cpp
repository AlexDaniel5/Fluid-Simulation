#include <iostream>
#include <SDL.h>
#include <cstdlib>
#include <ctime>

const int WIDTH = 800, HEIGHT = 600; // Window dimensions
const int CELL_SIZE = 10;

const int GRID_WIDTH = WIDTH / CELL_SIZE;
const int GRID_HEIGHT = HEIGHT / CELL_SIZE;

// Grid values: 0 = empty, 1 = white block, 2 = blue block
int grid[GRID_HEIGHT][GRID_WIDTH] = {0};
bool isDrawing = false; // Drawing state
SDL_Color drawColor = {255, 255, 255, 255}; // Default to white blocks
int frameCount = 0;

void drawBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderClear(renderer); // Clear the screen for animation
}

void drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Dark gray
    for (int x = 0; x < WIDTH; x += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, x, 0, x, HEIGHT); // Vertical lines
    }
    for (int y = 0; y < HEIGHT; y += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, 0, y, WIDTH, y); // Horizontal lines
    }
}

// Draw a block into the grid
void drawBlock(SDL_Renderer *renderer, int x, int y, int type) {
    SDL_Color color = (type == 1) ? SDL_Color{255, 255, 255, 255} : SDL_Color{0, 0, 255, 255}; // White or blue
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    SDL_RenderFillRect(renderer, &cell);
}

// Render the entire grid
void renderGrid(SDL_Renderer *renderer) {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (grid[y][x] != 0) {
                drawBlock(renderer, x, y, grid[y][x]);
            }
        }
    }
}

// Update the blue blocks to fall
void updateBlueBlocks() {
    // Seed the random number generator (call this once in your program's initialization code)
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int y = GRID_HEIGHT - 2; y >= 0; --y) { // Start from the second-last row
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (grid[y][x] == 2) { // If the current cell is a blue block
                if (grid[y + 1][x] == 0) { // Check if the cell below is empty
                    // Move block down
                    grid[y + 1][x] = 2;
                    grid[y][x] = 0;
                } else { // Cell below is filled
                    // Determine random direction: -1 (left), 1 (right)
                    int direction = (std::rand() % 2 == 0) ? -1 : 1;

                    // Check if the block can move left or right
                    int newX = x + direction;
                    if (newX >= 0 && newX < GRID_WIDTH && grid[y + 1][newX] == 0) {
                        // Move block diagonally left or right
                        grid[y + 1][newX] = 2;
                        grid[y][x] = 0;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Draw with Mouse", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL) {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            // Handle mouse events
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                isDrawing = true;
            } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                isDrawing = false;
            } else if (event.type == SDL_MOUSEMOTION && isDrawing) {
                int mouseX = event.motion.x / CELL_SIZE;
                int mouseY = event.motion.y / CELL_SIZE;
                if (mouseX >= 0 && mouseX < GRID_WIDTH && mouseY >= 0 && mouseY < GRID_HEIGHT) {
                    grid[mouseY][mouseX] = (drawColor.r == 255) ? 1 : 2; // White = 1, Blue = 2
                }
            }

            // Handle keyboard events
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                // Toggle between white and blue
                if (drawColor.r == 255) {
                    drawColor = {0, 0, 255, 255}; // Blue
                } else {
                    drawColor = {255, 255, 255, 255}; // White
                }
            }
        }

        // Update every 6 frames
        frameCount++;
        if (frameCount % 6 == 0) {
            updateBlueBlocks();
        }

        // Render the scene
        drawBackground(renderer);
        drawGrid(renderer);
        renderGrid(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Around 60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
