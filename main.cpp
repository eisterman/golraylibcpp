#include <print>
#include <raylib.h>
#include <cstdint>
#include <random>
#include "GameOfLife.hpp"

constexpr int32_t SCREEN_WIDTH = 800;
constexpr int32_t SCREEN_HEIGHT = 450;
constexpr float RECT_W = 2.0;
constexpr float RECT_H = 2.0;
constexpr size_t MAX_RECS_X = SCREEN_WIDTH/RECT_W;
constexpr size_t MAX_RECS_Y = SCREEN_HEIGHT/RECT_H;
constexpr float PLAY_TIME_IN_FRAMES = 240.0; // At 60 FPS = 4 seconds


class BoolRand {
public:
    BoolRand() : generator(std::random_device{}()) { }

    explicit BoolRand(const uint32_t seed) : generator(seed) { }

    bool gen() {
        return distribution(generator);
    }
private:
    std::mt19937 generator;
    std::bernoulli_distribution distribution = std::bernoulli_distribution(0.5);
};

int main() {
    auto brnd = BoolRand(42);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life in C++");
    std::array<uint8_t,1> birth = {3};
    std::array<uint8_t,2> stay = {2,3};
    auto gol = GameOfLife(MAX_RECS_X, MAX_RECS_Y, std::span(birth), std::span(stay));

    for (auto y = 0; y < MAX_RECS_Y; ++y)
        for (auto x = 0; x < MAX_RECS_X; ++x)
            gol[x,y] = brnd.gen();

    // gol[10, 6] = 1;
    // gol[11, 6] = 1;
    // gol[12, 6] = 1;
    // gol[12, 5] = 1;
    // gol[11, 4] = 1;

    size_t frames_counter = 0;
    SetTargetFPS(60);
    // Main Game Loop
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            // When animation has finished, press space to restar
            frames_counter = 0;
            for (auto y = 0; y < MAX_RECS_Y; ++y)
                for (auto x = 0; x < MAX_RECS_X; ++x)
                    gol[x,y] = brnd.gen();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Draw GOL
        for (auto y = 0; y < MAX_RECS_Y; ++y) {
            for (auto x = 0; x < MAX_RECS_X; ++x) {
                DrawRectangleRec(Rectangle{ RECT_W*static_cast<float>(x), RECT_H*static_cast<float>(y), RECT_W, RECT_H},
                    gol[x,y] > 0 ? BLACK : WHITE);
            }
        }
        // Draw Gen Number
        DrawText(std::format("{}", frames_counter).c_str(), 10, 10, 42, ColorAlpha(GRAY, 0.7));
        EndDrawing();

        gol.step();
        ++frames_counter;
    }

    return 0;
}
