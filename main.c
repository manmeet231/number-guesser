#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int FONT_SIZE_ABOVE = 48;
const int FONT_SIZE_BELOW = 24;
const int MAX_INPUT_LENGTH = 100;
const int FEEDBACK_DELAY_MS = 2000; // 2 seconds in milliseconds

// Function prototypes
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, SDL_Color color);
void compareGuess(int guess, int randomNumber, char** message);

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

    // Load a font for above text
    TTF_Font* font_above = TTF_OpenFont("assets/font.ttf", FONT_SIZE_ABOVE);
    if (font_above == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load a font for below text
    TTF_Font* font_below = TTF_OpenFont("assets/font.ttf", FONT_SIZE_BELOW);
    if (font_below == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(font_above);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(font_above);
        TTF_CloseFont(font_below);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Variables to store user input
    char inputText[MAX_INPUT_LENGTH + 1];
    memset(inputText, 0, sizeof(inputText)); // Initialize all elements to 0 (null terminator)
    SDL_StartTextInput();

    // Seed the random number generator
    srand(time(NULL));

    // Generate a random number between 1 and 100
    int randomNumber = rand() % 100 + 1;
    char* message = "Start";
    int userGuess = atoi(inputText);

    // Flag to track if the user has guessed correctly
    bool guessedCorrectly = false;

    // Event loop
    int quit = 0;
    SDL_Event e;

    while (!quit && !guessedCorrectly) {
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
                    int guess = atoi(inputText);
                    inputText[0] = '\0'; // Clear input text after guess
                    compareGuess(guess, randomNumber, &message);
                    if (strcmp(message, "correct") == 0) {
                        guessedCorrectly = true;
                    }
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        SDL_Color textColor2 = { 0, 0, 0, 255 };
        renderText(renderer, message, 200, 400, font_above, textColor2);

        // Render text above the input box
        SDL_Color textColor = { 0, 0, 0, 255 };
        renderText(renderer, "Enter Your Number!", 200, 100, font_above, textColor);
        renderText(renderer, inputText, 200, 200, font_above, textColor);

        // Render text below the input box
        SDL_Color textColorBelow = { 0, 0, 0, 255 };
        const char* textBelow = "The range is from one to 100.";
        renderText(renderer, textBelow , 240, 250, font_below, textColorBelow);

        // Draw input box
        SDL_Rect inputBox = { 190, 190, 400, 60 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for the box
        SDL_RenderDrawRect(renderer, &inputBox);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_StopTextInput();
    TTF_CloseFont(font_below);
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

// Function to compare the user's guess with the random number
void compareGuess(int guess, int randomNumber, char** message) {
    if (guess <100 && guess >0){
        if (guess == randomNumber) {
        *message = "correct";
    } else if (guess < randomNumber) {
        *message = "too low";
    } else {
        *message = "too high";
    }
    }
    else{
        *message = "invalid";
    }
}