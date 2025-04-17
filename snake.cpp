// #include <SFML/Graphics.hpp>
// #include <deque>
// #include <random>
// #include <cmath>
// #include <iostream>
// #include <vector>

// // Constants
// const int CELL_SIZE = 20;
// const int GRID_WIDTH = 25;
// const int GRID_HEIGHT = 25;
// const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;
// const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;
// const float SNAKE_SPEED = 0.1f; // Move every 0.1 seconds
// const float WAVE_FREQUENCY = 0.3f; // Controls the amplitude of the wave
// const float WAVE_SPEED = 0.1f; // Controls how quickly the wave moves
// const float LEVEL_UP_TIME = 0.5f; // Time in seconds for level-up message to stay visible (0.5 sec)

// enum Direction { UP, DOWN, LEFT, RIGHT };

// class SnakeGame {
// private:
//     sf::RenderWindow window;
//     sf::RectangleShape snakeBody;  // Old Snake Body (Rectangular)
//     sf::CircleShape snakeBodyWavy; // New Snake Body (Circular, Wavy)
//     sf::RectangleShape foodShape;
//     std::deque<sf::Vector2i> snake;
//     sf::Vector2i food;
//     Direction dir;
//     bool gameOver;
//     bool gameStarted;
//     bool levelUp;
//     float timer;
//     float levelUpTimer;  // Timer to control how long the level-up message stays visible
//     int score;
//     int foodEaten;

//     // SFML objects for displaying the score and level-up message
//     sf::Font font;
//     sf::Text scoreText;
//     sf::Text gameOverText;
//     sf::Text levelUpText;

//     std::vector<sf::Color> foodColors = {
//         sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow,
//         sf::Color::Magenta, sf::Color::Cyan, sf::Color(255, 165, 0), sf::Color(255, 105, 180)
//     };

//     std::vector<sf::Color> snakeColors = {
//         sf::Color::Green, sf::Color::Yellow, sf::Color::Red, sf::Color::Blue,
//         sf::Color::Magenta, sf::Color::Cyan, sf::Color(255, 165, 0), sf::Color(255, 105, 180)
//     };

//     std::vector<sf::Color> alternatingColors = {
//         sf::Color::Red, sf::Color::Yellow, sf::Color::Green, sf::Color::Blue,
//         sf::Color::Magenta, sf::Color::Cyan, sf::Color::White
//     };

// public:
//     // Constructor
//     SnakeGame()
//         : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Snake Game"),
//           dir(RIGHT), gameOver(false), gameStarted(false), levelUp(false), timer(0), score(0), foodEaten(0),
//           levelUpTimer(0), scoreText(font, "Score: 0", 24), gameOverText(font, "Game Over!", 30), levelUpText(font, "Level Up Noobre!", 30) {

//         // Load the font
//         if (!font.openFromFile("arial.ttf")) {
//             std::cerr << "Failed to load font!" << std::endl;
//         }

//         scoreText.setPosition(sf::Vector2f(10.f, 10.f));
//         gameOverText.setPosition(sf::Vector2f(WINDOW_WIDTH / 4.f, WINDOW_HEIGHT / 2.f));
//         levelUpText.setPosition(sf::Vector2f(WINDOW_WIDTH / 4.f, WINDOW_HEIGHT / 2.f));

//         // Initialize snake body as rectangle (old snake)
//         snakeBody.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
//         snakeBody.setFillColor(sf::Color::Green);
        
//         // Initialize snake body as circle (new snake, wavy type)
//         snakeBodyWavy.setRadius(CELL_SIZE / 2 - 2);
//         snakeBodyWavy.setFillColor(sf::Color::Green);

//         foodShape.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
//         foodShape.setFillColor(randomFoodColor());  // Initial random food color

//         // Initialize snake with 3 segments
//         snake.push_back({ GRID_WIDTH / 2, GRID_HEIGHT / 2 });
//         snake.push_back({ GRID_WIDTH / 2 - 1, GRID_HEIGHT / 2 });
//         snake.push_back({ GRID_WIDTH / 2 - 2, GRID_HEIGHT / 2 });

//         spawnFood();
//     }

//     void spawnFood() {
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<int> distX(0, GRID_WIDTH - 1);
//         std::uniform_int_distribution<int> distY(0, GRID_HEIGHT - 1);

//         food = { distX(gen), distY(gen) };
//         foodShape.setFillColor(randomFoodColor());  // Change food color on spawn
//     }

//     sf::Color randomFoodColor() {
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<int> dist(0, foodColors.size() - 1);
//         return foodColors[dist(gen)];
//     }

//     sf::Color getSnakeColor(int score) {
//         // Change snake color pattern based on score
//         if (score <= 100) {
//             return snakeColors[0];  // Solid snake color
//         } else if (score <= 200) {
//             int index = (score / 10) % alternatingColors.size();
//             return alternatingColors[index];
//         } else {
//             int index = (score / 20) % alternatingColors.size();
//             return alternatingColors[index];
//         }
//     }

//     void handleInput() {
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && dir != DOWN)
//             dir = UP;
//         else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && dir != UP)
//             dir = DOWN;
//         else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && dir != RIGHT)
//             dir = LEFT;
//         else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && dir != LEFT)
//             dir = RIGHT;

//         // Allow WASD controls
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && dir != DOWN)
//             dir = UP;
//         else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && dir != UP)
//             dir = DOWN;
//         else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && dir != RIGHT)
//             dir = LEFT;
//         else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && dir != LEFT)
//             dir = RIGHT;

//         // Start the game if any key is pressed
//         if (!gameStarted && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
//                              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
//                              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
//                              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
//                              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
//                              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
//                              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
//                              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))) {
//             gameStarted = true;
//         }
//     }

//     void update(float deltaTime) {
//         if (gameOver) return;
    
//         if (!gameStarted) return;
    
//         timer += deltaTime;
//         if (timer < SNAKE_SPEED) return;
    
//         timer = 0;
    
//         // Move the snake
//         sf::Vector2i newHead = snake.front();
//         if (dir == UP) newHead.y -= 1;
//         else if (dir == DOWN) newHead.y += 1;
//         else if (dir == LEFT) newHead.x -= 1;
//         else if (dir == RIGHT) newHead.x += 1;
    
//         // Make the snake wrap around walls (instead of dying)
//         if (newHead.x < 0) newHead.x = GRID_WIDTH - 1;
//         if (newHead.x >= GRID_WIDTH) newHead.x = 0;
//         if (newHead.y < 0) newHead.y = GRID_HEIGHT - 1;
//         if (newHead.y >= GRID_HEIGHT) newHead.y = 0;
    
//         // Check self-collision
//         for (const auto& segment : snake) {
//             if (segment == newHead) {
//                 gameOver = true;
//                 return;
//             }
//         }
    
//         // Check food collision
//         if (newHead == food) {
//             score += 10;
//             foodEaten++;
//             spawnFood();
//         } else {
//             snake.pop_back(); // Remove tail if no food eaten
//         }
    
//         snake.push_front(newHead);
//     }
    
//     void render() {
//         window.clear(sf::Color::Black); // Set background to black
    
//         // Draw food
//         foodShape.setPosition(sf::Vector2f(food.x * CELL_SIZE, food.y * CELL_SIZE));
//         window.draw(foodShape);
    
//         // Draw snake with alternating color pattern
//         for (size_t i = 0; i < snake.size(); ++i) {
//             float waveOffset = std::sin(i * WAVE_FREQUENCY) * 5;
//             sf::Vector2f position(snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE + waveOffset);
//             snakeBodyWavy.setPosition(position);
//             snakeBodyWavy.setFillColor(getSnakeColor(score));  // Change snake color based on score
//             window.draw(snakeBodyWavy);
//         }
    
//         // Draw score
//         scoreText.setString("Score: " + std::to_string(score));
//         window.draw(scoreText);
    
//         // Draw Level Up message if applicable
//         if (levelUp && !gameOver) {
//             window.draw(levelUpText);
//         }
    
//         // Draw Game Over text
//         if (gameOver && !levelUp) {
//             window.draw(gameOverText);
//         }
    
//         window.display();
//     }
    
//     void restartGame() {
//         gameOver = false;
//         gameStarted = false;
//         score = 0;
//         foodEaten = 0;
//         dir = RIGHT;
//         timer = 0;
//         snake.clear();

//         // Initialize snake with 3 segments
//         snake.push_back({ GRID_WIDTH / 2, GRID_HEIGHT / 2 });
//         snake.push_back({ GRID_WIDTH / 2 - 1, GRID_HEIGHT / 2 });
//         snake.push_back({ GRID_WIDTH / 2 - 2, GRID_HEIGHT / 2 });

//         spawnFood();
//     }

//     void run() {
//         sf::Clock clock;

//         while (window.isOpen()) {
//             float deltaTime = clock.restart().asSeconds();

//             std::optional<sf::Event> event = window.pollEvent();
//             while (event.has_value()) {
//                 if (event->is<sf::Event::Closed>()) {
//                     window.close();
//                 }
//                 event = window.pollEvent();
//             }

//             handleInput();
//             update(deltaTime);
//             render();
//         }
//     }
// };

// int main() {
//     SnakeGame game;
//     game.run();
//     return 0;
// }
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>  // Include SFML Audio
#include <deque>
#include <random>
#include <cmath>
#include <iostream>
#include <vector>

// Constants
const int CELL_SIZE = 20;
const int GRID_WIDTH = 25;
const int GRID_HEIGHT = 25;
const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;
const float SNAKE_SPEED = 0.1f; // Move every 0.1 seconds
const float WAVE_FREQUENCY = 0.3f;
const float WAVE_SPEED = 0.1f;
const float LEVEL_UP_TIME = 0.5f; // Time in seconds for level-up message to stay visible (0.5 sec)

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
private:
    sf::RenderWindow window;
    sf::RectangleShape foodShape;
    sf::CircleShape snakeBodyWavy;
    std::deque<sf::Vector2i> snake;
    sf::Vector2i food;
    Direction dir;
    bool gameOver;
    bool gameStarted;
    bool levelUp;
    float timer;
    float levelUpTimer;
    int score;
    int foodEaten;

    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text levelUpText;

    sf::SoundBuffer moveBuffer;
    sf::Sound moveSound{moveBuffer};
    sf::SoundBuffer eatBuffer;
    sf::Sound eatSound{eatBuffer};

    std::vector<sf::Color> foodColors = {
        sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow,
        sf::Color::Magenta, sf::Color::Cyan, sf::Color(255, 165, 0), sf::Color(255, 105, 180)
    };

    std::vector<sf::Color> snakeColors = {
        sf::Color::Green, sf::Color::Yellow, sf::Color::Red, sf::Color::Blue,
        sf::Color::Magenta, sf::Color::Cyan, sf::Color(255, 165, 0), sf::Color(255, 105, 180)
    };

    std::vector<sf::Color> alternatingColors = {
        sf::Color::Red, sf::Color::Yellow, sf::Color::Green, sf::Color::Blue,
        sf::Color::Magenta, sf::Color::Cyan, sf::Color::White
    };

public:
    // Constructor
    SnakeGame()
        : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Snake Game"),
          scoreText(font, "", 24),        // FIXED: Initialize text elements with font
          gameOverText(font, "Game Over!", 30),
          levelUpText(font, "Level Up!", 30),
          dir(RIGHT), gameOver(false), gameStarted(false), levelUp(false), timer(0), score(0), foodEaten(0),
          levelUpTimer(0) {

        if (!font.openFromFile("arial.ttf")) {  // Load font
            std::cerr << "Failed to load font!" << std::endl;
        }

        if (!moveBuffer.loadFromFile("move.wav")) {
            std::cerr << "Failed to load move.wav!" << std::endl;
        }
        moveSound.setBuffer(moveBuffer);

        if (!eatBuffer.loadFromFile("eat_food.wav")) {
            std::cerr << "Failed to load eat_food.wav!" << std::endl;
        }
        eatSound.setBuffer(eatBuffer);

        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(10.f, 10.f));

        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(sf::Vector2f(WINDOW_WIDTH / 4.f, WINDOW_HEIGHT / 2.f));

        levelUpText.setFillColor(sf::Color::Green);
        levelUpText.setPosition(sf::Vector2f(WINDOW_WIDTH / 4.f, WINDOW_HEIGHT / 2.f));

        snakeBodyWavy.setRadius(CELL_SIZE / 2 - 2);
        snakeBodyWavy.setFillColor(sf::Color::Green);

        foodShape.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
        foodShape.setFillColor(randomFoodColor());

        snake.push_back({ GRID_WIDTH / 2, GRID_HEIGHT / 2 });
        snake.push_back({ GRID_WIDTH / 2 - 1, GRID_HEIGHT / 2 });
        snake.push_back({ GRID_WIDTH / 2 - 2, GRID_HEIGHT / 2 });

        spawnFood();
    }

    void spawnFood() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distX(0, GRID_WIDTH - 1);
        std::uniform_int_distribution<int> distY(0, GRID_HEIGHT - 1);

        food = { distX(gen), distY(gen) };
        foodShape.setFillColor(randomFoodColor());
    }

    sf::Color randomFoodColor() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, foodColors.size() - 1);
        return foodColors[dist(gen)];
    }

    void handleInput() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && dir != DOWN)
            dir = UP;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && dir != UP)
            dir = DOWN;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && dir != RIGHT)
            dir = LEFT;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && dir != LEFT)
            dir = RIGHT;
    }

    void update(float deltaTime) {
        if (gameOver || !gameStarted) return;

        timer += deltaTime;
        if (timer < SNAKE_SPEED) return;
        timer = 0;

        sf::Vector2i newHead = snake.front();
        if (dir == UP) newHead.y -= 1;
        else if (dir == DOWN) newHead.y += 1;
        else if (dir == LEFT) newHead.x -= 1;
        else if (dir == RIGHT) newHead.x += 1;

        moveSound.play();

        for (const auto& segment : snake) {
            if (segment == newHead) {
                gameOver = true;
                return;
            }
        }

        if (newHead == food) {
            score += 10;
            foodEaten++;
            spawnFood();
            eatSound.play();
        } else {
            snake.pop_back();
        }

        snake.push_front(newHead);
    }

    void render() {
        window.clear(sf::Color::Black);

        foodShape.setPosition(sf::Vector2f(food.x * CELL_SIZE, food.y * CELL_SIZE));
        window.draw(foodShape);

        for (size_t i = 0; i < snake.size(); ++i) {
            float waveOffset = std::sin(i * WAVE_FREQUENCY) * 5;
            sf::Vector2f position(snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE + waveOffset);
            snakeBodyWavy.setPosition(position);
            window.draw(snakeBodyWavy);
        }

        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        if (gameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }
    void run() {
        sf::Clock clock;  // Correctly declare the clock instance
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
    
            // Poll events using std::optional
            while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
                if (eventOpt) {
                    const sf::Event& event = *eventOpt;  // Dereference the optional to access the event
    
                    if (event.is<sf::Event::Closed>()) {  // Using the modern event checking method
                        window.close();
                    }
                }
            }
    
            handleInput();
            update(deltaTime);
            render();
        }
    }
};    

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
