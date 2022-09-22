#pragma once

#define CLASS_NAME "MapEditor"

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr int COLOR_DEPTH = 32;

constexpr int DEFAULT_BLOCK_SIZE = 32;
constexpr int BLOCK_X = 100;
constexpr int BLOCK_Y = 100;

constexpr int TOTAL_ARROW = 100;
constexpr int TOTAL_SNOWBALL = 4;

constexpr int TOTAL_BLOCK_SPRITE = 110;
constexpr int TOTAL_BLOCK_SPRITE_X = 23;
constexpr int TOTAL_BLOCK_SPRITE_Y = 20;

constexpr int WORLDMAP_WIDTH = DEFAULT_BLOCK_SIZE * BLOCK_X;
constexpr int WORLDMAP_HEIGHT = DEFAULT_BLOCK_SIZE * BLOCK_Y;

constexpr int WORLDMAP_DATA_SIZE = WORLDMAP_WIDTH * WORLDMAP_HEIGHT * 3;

constexpr int SCREEN_MAX_WIDTH = BLOCK_X * DEFAULT_BLOCK_SIZE - SCREEN_WIDTH;
constexpr int SCREEN_MAX_HEIGHT = BLOCK_Y * DEFAULT_BLOCK_SIZE - SCREEN_HEIGHT;

constexpr int COLOR_KEY = 0;
