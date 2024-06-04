#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PADDLE_WIDTH = 10.f;
const float PADDLE_HEIGHT = 100.f;
const float BALL_RADIUS = 10.f;
const float OBSTACLE_WIDTH = 20.f;
const float OBSTACLE_HEIGHT = 150.f;

const float PLAYER_PADDLE_SPEED = 6.0f; // 패들 속도 증가
const float COMPUTER_PADDLE_SPEED = 5.0f; // 컴퓨터 패들 속도 증가
const float BALL_SPEED_X = 5.5f; // 공의 속도 증가
const float BALL_SPEED_Y = 5.5f; // 공의 속도 증가
const float OBSTACLE_SPEED_Y = 2.0f; // 장애물 속도 증가

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Tennis Game");
    window.setFramerateLimit(60);

    sf::RectangleShape playerPaddle(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    playerPaddle.setFillColor(sf::Color::Blue);
    playerPaddle.setPosition(10, (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2));

    sf::RectangleShape computerPaddle(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    computerPaddle.setFillColor(sf::Color::Red);
    computerPaddle.setPosition(WINDOW_WIDTH - 20, (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2));

    sf::CircleShape ball(BALL_RADIUS);
    ball.setFillColor(sf::Color::White);
    ball.setPosition((WINDOW_WIDTH / 2) - BALL_RADIUS, (WINDOW_HEIGHT / 2) - BALL_RADIUS);

    sf::RectangleShape obstacle(sf::Vector2f(OBSTACLE_WIDTH, OBSTACLE_HEIGHT));
    obstacle.setFillColor(sf::Color::Green);
    obstacle.setPosition((WINDOW_WIDTH / 2) - (OBSTACLE_WIDTH / 2), 0);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        return -1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition((WINDOW_WIDTH / 2) - 50, 10);

    int playerScore = 0, computerScore = 0;
    float ballVelocityX = BALL_SPEED_X;
    float ballVelocityY = BALL_SPEED_Y;
    float obstacleVelocityY = OBSTACLE_SPEED_Y;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // Player paddle control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && playerPaddle.getPosition().y > 0) {
            playerPaddle.move(0, -PLAYER_PADDLE_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && playerPaddle.getPosition().y + PADDLE_HEIGHT < WINDOW_HEIGHT) {
            playerPaddle.move(0, PLAYER_PADDLE_SPEED);
        }

        // Computer paddle AI
        if (computerPaddle.getPosition().y + (PADDLE_HEIGHT / 2) < ball.getPosition().y) {
            computerPaddle.move(0, COMPUTER_PADDLE_SPEED);
        }
        else if (computerPaddle.getPosition().y + (PADDLE_HEIGHT / 2) > ball.getPosition().y) {
            computerPaddle.move(0, -COMPUTER_PADDLE_SPEED);
        }

        // Ball movement
        ball.move(ballVelocityX, ballVelocityY);
        if (ball.getPosition().y <= 0 || ball.getPosition().y >= WINDOW_HEIGHT - BALL_RADIUS * 2) {
            ballVelocityY = -ballVelocityY;
        }
        if ((ball.getPosition().x <= playerPaddle.getPosition().x + PADDLE_WIDTH && ball.getPosition().y + BALL_RADIUS * 2 >= playerPaddle.getPosition().y && ball.getPosition().y <= playerPaddle.getPosition().y + PADDLE_HEIGHT)
            || (ball.getPosition().x + BALL_RADIUS * 2 >= computerPaddle.getPosition().x && ball.getPosition().y + BALL_RADIUS * 2 >= computerPaddle.getPosition().y && ball.getPosition().y <= computerPaddle.getPosition().y + PADDLE_HEIGHT)) {
            ballVelocityX = -ballVelocityX;
        }

        // Obstacle collision
        if (ball.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
            ballVelocityX = -ballVelocityX;
        }

        // Score calculation
        if (ball.getPosition().x <= 0) {
            computerScore++;
            ball.setPosition((WINDOW_WIDTH / 2) - BALL_RADIUS, (WINDOW_HEIGHT / 2) - BALL_RADIUS);
        }
        else if (ball.getPosition().x >= WINDOW_WIDTH - BALL_RADIUS * 2) {
            playerScore++;
            ball.setPosition((WINDOW_WIDTH / 2) - BALL_RADIUS, (WINDOW_HEIGHT / 2) - BALL_RADIUS);
        }

        // Update score display
        scoreText.setString(std::to_string(playerScore) + " : " + std::to_string(computerScore));

        // Check game end condition
        if (playerScore == 5 || computerScore == 5) {
            std::cout << (playerScore == 5 ? "Player wins!" : "Computer wins!") << std::endl;
            break; // Exit game loop
        }

        // Obstacle movement
        obstacle.move(0, obstacleVelocityY);
        if (obstacle.getPosition().y <= 0 || obstacle.getPosition().y >= WINDOW_HEIGHT - OBSTACLE_HEIGHT) {
            obstacleVelocityY = -obstacleVelocityY;
        }

        window.clear();
        window.draw(playerPaddle);
        window.draw(computerPaddle);
        window.draw(ball);
        window.draw(obstacle);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
