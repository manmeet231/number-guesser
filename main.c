#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int FONT_SIZE = 48;
const int MAX_INPUT_LENGTH = 100;

// Function prototypes
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, SDL_Color color);

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load a font
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", FONT_SIZE);
    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Variables to store user input
    char inputText[MAX_INPUT_LENGTH + 1];
    memset(inputText, 0, sizeof(inputText)); // Initialize all elements to 0 (null terminator)
    SDL_StartTextInput();

    // Event loop
    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_TEXTINPUT) {
                // Input validation: limit input length
                if (strlen(inputText) < MAX_INPUT_LENGTH) {
                    strcat(inputText, e.text.text);
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
                    inputText[strlen(inputText) - 1] = '\0';
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    printf("User guessed: %s\n", inputText);
                    inputText[0] = '\0'; // Clear input text after guess
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Render text above the input box
        SDL_Color textColor = { 0, 0, 0, 255 };
        renderText(renderer, "Enter Your Number!", 200, 100, font, textColor);
        renderText(renderer, inputText, 200, 200, font, textColor);

        // Render text below the input box
        SDL_Color textColorBelow = { 0, 0, 0, 255 };
        renderText(renderer, "The range is from one to 100.", 200, 300, font, textColorBelow);

        // Draw input box
        SDL_Rect inputBox = { 190, 190, 400, 60 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for the box
        SDL_RenderDrawRect(renderer, &inputBox);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

// Function to render text
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, SDL_Color color) {
    // Check if the input string is empty or contains only whitespace characters
    if (text == NULL || strlen(text) == 0 || strlen(text) == strspn(text, " \t\n\r")) {
        return; // Return without rendering anything
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if (textSurface == NULL) {
        printf("Failed to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        printf("Failed to create texture from surface! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture); // Free the texture after use
}
