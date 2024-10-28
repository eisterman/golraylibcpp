#pragma once

#include <cstdint>
#include <ranges>
#include <span>
#include <vector>
#include <algorithm>


class GameOfLife {
public:
    template <std::ranges::input_range R1, std::ranges::input_range R2>
    requires std::same_as<std::ranges::range_value_t<R1>, uint8_t> and std::same_as<std::ranges::range_value_t<R2>, uint8_t>
    GameOfLife(const size_t width, const size_t height, const R1 && birth, const R2 && stay):
        _birth{birth | std::ranges::to<std::vector>()}, _stay{stay | std::ranges::to<std::vector>()},
        _width{width}, _height{height}, _data(width*height, 0) {}
    uint8_t & operator[](const size_t x, const size_t y) { return _data.at(x + y * _width); }
    void step() {
        const auto old = std::vector(_data);
        for (size_t j = 0; j < _height; ++j) {
            for (size_t i = 0; i < _width; ++i) {
                uint8_t count = 0;
                auto d = std::array{-1,0,1};
                for (auto [dx,dy] : std::views::cartesian_product(d,d)) {
                    if (dx == 0 and dy == 0) continue;
                    const size_t x = (i + dx) % _width;
                    const size_t y = (j + dy) % _height;
                    if (old[x+y*_width] != 0) ++count;
                }
                _data[i+j*_width] = [&] {
                    switch (old[i+j*_width]) {
                        case 0: return std::ranges::contains(_birth, count) ? 1 : 0;
                        case 1: return std::ranges::contains(_stay, count) ? 1 : 0;
                        default: throw std::runtime_error("Unreachable!");
                    }
                }();
            }
        }
    }

private:
    std::vector<uint8_t> _birth;
    std::vector<uint8_t> _stay;
    size_t _width;
    size_t _height;
    std::vector<uint8_t> _data;
};
