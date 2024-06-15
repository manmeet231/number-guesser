
# Number Guesser

Welcome to the Number Guessing Game! This project is a simple yet engaging game where players try to guess a randomly generated number. The game is built using SDL2 (Simple DirectMedia Layer), a powerful library that provides low-level access to audio, keyboard, mouse, joystick, and graphics hardware.Welcome to the Number Guessing Game! This project is a simple yet engaging game where players try to guess a randomly generated number. The game is built using SDL2 (Simple DirectMedia Layer), a powerful library that provides low-level access to audio, keyboard, mouse, joystick, and graphics hardware.


## Features

- Interactive graphical interface using SDL2
- Random number generation
- User input handling
- Feedback on guesses (too high, too low, correct)
- Simple and intuitive gameplay


## Installation

To run this project on your local machine, follow these steps:

- Clone the repository:

```bash
  git clone https://github.com/manmeet231/number-guesser.git
  cd number-guessing-game
```
- Install SDL2

**Windows**: Follow the instructions on the [SDL2](https://wiki.libsdl.org/SDL2/FrontPage) wiki.

**macOS**: Install via Homebrew
```bash
   brew install sdl2
```
**Linux**: Install via your package manager, e.g., for Debian-based distributions
```bash
   sudo apt-get install libsdl2-dev
```
## Build the project:

Ensure you have a C compiler installed (like gcc).
Compile the source code:
```bash
    gcc -I src/include -L src/lib main.c -o number_guesser.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
```


## Usage/Examples

To start the game, run the compiled executable:

```bash
   .\number_guesser.exe
```

Follow the on-screen prompts to guess the number. The game will guide you with hints until you guess the correct number.
## Contributing

Contributions are welcome! If you'd like to contribute to this project, please follow these steps:

- Fork the repository 

- Create a new branch:
   ```bash
   git checkout -b feature-branch
   ```
- Make your changes

- Commit your changes
  ```bash
   git commit -m "Description of your changes"
   ```
- Push to the branch
  ```bash
   git push origin feature-branch
   ```
- Create a pull request

- Go to the repository on GitHub.
- Click on the "Pull Requests" tab.
- Click the "New Pull Request" button.
- Follow the instructions to submit the pull request.
## Acknowledgements

 - SDL2 for providing a fantastic library to build this game.

 - Inspiration from various number guessing games available online.
