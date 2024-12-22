#include <iostream>
#include <SDL.h>

const int WIDTH = 800, HEIGHT = 600; // Window dimensions
const int CELL_SIZE = 10;

bool isDrawing = false; // Flag to check if mouse is being dragged
SDL_Color drawColour = {255, 255, 255, 255}; // Default drawing colour

void drawBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderFillRect(renderer, NULL); // Fill the entire window with the background colour
}

void drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 20); // Light gray with transparency
    for (int x = 0; x < WIDTH; x += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, x, 0, x, HEIGHT);
    }
    for (int y = 0; y < HEIGHT; y += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, 0, y, WIDTH, y);
    }
}

// Draw filled cell
void draw(SDL_Renderer *renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, drawColour.r, drawColour.g, drawColour.b, drawColour.a);
    SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    SDL_RenderFillRect(renderer, &cell);
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

    // Enable alpha blending for transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Event windowEvent;

    // Draw the background layer
    drawBackground(renderer);
    // Draw grid
    drawGrid(renderer);
    while (true) {
        if (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT) break;

            // Check for mouse button press/release and movement
            if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                if (windowEvent.button.button == SDL_BUTTON_LEFT) {
                    isDrawing = true; // Start drawing on left click
                }
            } else if (windowEvent.type == SDL_MOUSEBUTTONUP) {
                if (windowEvent.button.button == SDL_BUTTON_LEFT) {
                    isDrawing = false; // Stop drawing on left click release
                }
            } else if (windowEvent.type == SDL_MOUSEMOTION) {
                if (isDrawing) {
                    int mouseX = windowEvent.motion.x / CELL_SIZE;
                    int mouseY = windowEvent.motion.y / CELL_SIZE; 
                    draw(renderer, mouseX, mouseY); // Draw at the mouse position
                }
            }

            // Check for spacebar to toggle drawing colour
            if (windowEvent.type == SDL_KEYDOWN) {
                if (windowEvent.key.keysym.sym == SDLK_SPACE) {
                    if (drawColour.r == 255) {
                        // Blue
                        drawColour = {150, 180, 255, 255};
                    } else {
                        // White
                        drawColour = {255, 255, 255, 255};
                    }
                }
            }
        }
        // Render to screen
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
