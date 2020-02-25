#pragma once

enum class Color
{
    RED,
    BLUE,
    ORANGE,
    WHITE,
    GREEN,
    YELLOW,
    ERROR
};

enum class Face
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

enum class Direction
{
    CLOCKWISE,
    ANTI_CLOCKWISE
};

enum class Mode
{
    SIMULATION,
    ROBOT
};

enum class Command
{
    ERROR,
    MOVE_UP,
    MOVE_FRONT,
    MOVE_DOWN,
    MOVE_BACK,
    MOVE_RIGHT,
    MOVE_LEFT,
    DO_U_ROTATION,
    DO_UA_ROTATION,
    DO_F_ROTATION,
    DO_FA_ROTATION,
    DO_D_ROTATION,
    DO_DA_ROTATION,
    DO_B_ROTATION,
    DO_BA_ROTATION,
    DO_L_ROTATION,
    DO_LA_ROTATION,
    DO_R_ROTATION,
    DO_RA_ROTATION,
};
