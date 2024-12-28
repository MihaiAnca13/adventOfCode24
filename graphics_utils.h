//
// Created by mihai on 17-12-24.
//

#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>


class GridDrawer {
public:
    GridDrawer(int windowWidth, int windowHeight, int cellSize)
        : windowWidth(windowWidth), windowHeight(windowHeight), cellSize(cellSize) {
        square.setSize(sf::Vector2f(cellSize - 2, cellSize - 2)); // Padding for grid lines
        // set opacity
        square.setFillColor(sf::Color(255, 255, 255, 20));
        backgroundColor = sf::Color(50, 50, 50); // Dark grey background
    }

    // Thread-safe method to update the matrix
    void updateMatrix(const std::vector<std::vector<int>>& newMatrix) {
        std::lock_guard<std::mutex> lock(matrixMutex);
        matrix = newMatrix;
    }

    void updateMatrix(const std::vector<std::vector<std::string>>& newMatrix) {
        std::lock_guard<std::mutex> lock(matrixMutex);
        // initialize new empty matrix
        matrix = std::vector<std::vector<int>>(newMatrix.size(), std::vector<int>(newMatrix[0].size(), 0));
        for (int i = 0; i < newMatrix.size(); i++) {
            for (int j = 0; j < newMatrix[i].size(); j++) {
                if (newMatrix[i][j] == "#") {
                    matrix[i][j] = 50;
                } else if (newMatrix[i][j] == ".") {
                    matrix[i][j] = 0;
                } else if (newMatrix[i][j] == "[") {
                    matrix[i][j] = 3;
                } else if (newMatrix[i][j] == "]") {
                    matrix[i][j] = 7;
                } else if (newMatrix[i][j] == "@") {
                    matrix[i][j] = -1;
                }
            }
        }
    }

    // Main drawing loop (runs in its own thread)
    void run() {
        window.create(sf::VideoMode(windowWidth, windowHeight), "Grid Drawer");
        window.setFramerateLimit(60);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear(backgroundColor);

            // Draw the grid
            {
                std::lock_guard<std::mutex> lock(matrixMutex); // Protect matrix access
                for (size_t i = 0; i < matrix.size(); ++i) {
                    for (size_t j = 0; j < matrix[i].size(); ++j) {
                        if (matrix[i][j] != 0) {
                            square.setPosition(j * cellSize, i * cellSize);
                            if (matrix[i][j] > 0) {
                                square.setFillColor(sf::Color(255, 255, 255, std::min(20 * matrix[i][j], 255)));
                            } else {
                                square.setFillColor(sf::Color(sf::Color::Red));
                            }
                            window.draw(square);
                        }
                    }
                }
            }

            window.display();
        }
    }

    bool isOpen() const {
        return window.isOpen();
    }

private:
    int windowWidth, windowHeight, cellSize;
    sf::RenderWindow window;
    sf::RectangleShape square;
    sf::Color backgroundColor;

    std::vector<std::vector<int>> matrix;
    std::mutex matrixMutex; // Mutex for thread safety
};


#endif //GRAPHICS_UTILS_H
