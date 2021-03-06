/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for handling the different distance and position representations in the engine.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef COORD_H
#define COORD_H

#include "always.h"
#include "facing.h"
#include "target.h"
#include "gametypes.h"
#include "trect.h"

#define MAP_MAX_WIDTH 128
#define MAP_MAX_HEIGHT 128
#define MAPTD_MAX_WIDTH 64
#define MAPTD_MAX_HEIGHT 64
#define MAP_MAX_AREA MAP_MAX_WIDTH * MAP_MAX_HEIGHT

#define CELL_PIXELS 24
#define CELL_LEPTONS 256
#define CELL_MIN 0
#define CELL_MAX 128
#define CELL_MAX_X 128

#define COORD_MIN 0
#define COORD_MAX (CELL_LEPTONS * CELL_MAX)

extern const coord_t AdjacentCoord[FACING_COUNT];
extern const cell_t AdjacentCell[FACING_COUNT];

inline lepton_t Coord_Lepton_X(coord_t coord)
{
    return coord & 0x0000FFFF;
}

inline lepton_t Coord_Lepton_Y(coord_t coord)
{
    return (coord & 0xFFFF0000) >> 16;
}

inline lepton_t Coord_Sub_Cell_X(coord_t coord)
{
    return coord & 0xFF;
}

inline lepton_t Coord_Sub_Cell_Y(coord_t coord)
{
    return (coord >> 16) & 0xFF;
}

inline coord_t Coord_From_Lepton_XY(lepton_t x, lepton_t y)
{
    return (x & 0xFFFF) | (y << 16);
}

inline coord_t Coord_Add(coord_t coord1, coord_t coord2)
{
    lepton_t lx = Coord_Lepton_X(coord1) + Coord_Lepton_X(coord2);
    lepton_t ly = Coord_Lepton_Y(coord1) + Coord_Lepton_Y(coord2);

    return Coord_From_Lepton_XY(lx, ly);
}

inline coord_t Coord_Subtract(coord_t coord1, coord_t coord2)
{
    lepton_t lx = Coord_Lepton_X(coord1) - Coord_Lepton_X(coord2);
    lepton_t ly = Coord_Lepton_Y(coord1) - Coord_Lepton_Y(coord2);

    return Coord_From_Lepton_XY(lx, ly);
}

/**
 * Returns coordinates that are centered in the cell
 * operation - coord[0] = 128; coord[2] = 128;
 *
 */
inline coord_t Coord_Centered(coord_t coord)
{
    return (coord & 0xFF00FF00) | 0x00800080;
}

/**
 * Returns coordinates that are at the top left of the cell
 * operation - coord[0] = 0; coord[2] = 0;
 *
 */
inline coord_t Coord_Top_Left(coord_t coord)
{
    return coord & 0xFF00FF00;
}

/**
 * Returns coordinate that only contains subcells
 * operation - coord[1] = 0; coord[3] = 0;
 *
 */
inline coord_t Coord_Sub_Cell(coord_t coord)
{
    return coord & 0x00FF00FF;
}

/**
 * Fetch an adjacent coordinate from the specified direction.
 */
inline coord_t Coord_Get_Adjacent(coord_t coord, FacingType facing)
{
    return Coord_Centered(Coord_Add(coord, AdjacentCoord[(unsigned)facing % FACING_COUNT]));
}

/**
 * The map cell 'x' position of the coordinate.
 */
inline uint8_t Coord_Cell_X(coord_t coord)
{
    return (coord & 0x00007F00) >> 8;
}

/**
 * The map cell 'y' position of the coordinate.
 */
inline uint8_t Coord_Cell_Y(coord_t coord)
{
    return (coord & 0x7F000000) >> 24;
}

/**
 * Is this coordinate a negative? Used for sanity checks.
 */
inline BOOL Coord_Is_Negative(coord_t coord)
{
    return (coord & 0x80008000) != 0;
}

/**
 * Cell position value from x and y values.
 */
inline cell_t Cell_From_XY(uint8_t x, uint8_t y)
{
    return (((y % MAP_MAX_WIDTH) * MAP_MAX_WIDTH) + (x % MAP_MAX_WIDTH));
}

/*
 * Convert a coordinate value to a cell position.
 */
inline cell_t Coord_To_Cell(coord_t coord)
{
    return Cell_From_XY(Coord_Cell_X(coord), Coord_Cell_Y(coord));
}

inline uint8_t Cell_Get_X(cell_t cellnum)
{
    return (unsigned)cellnum % MAP_MAX_WIDTH;
}

inline uint8_t Cell_Get_Y(cell_t cellnum)
{
    return (unsigned)cellnum / MAP_MAX_HEIGHT;
}

/**
 * Makes a coord out of a cell
 * returned coordinate is in center of the cell
 *
 */
inline coord_t Cell_To_Coord(cell_t cellnum)
{
    return 0x00800080 | (Cell_Get_X(cellnum) << 8) | (Cell_Get_Y(cellnum) << 24);
}

/**
 * Makes a coord out of a cell
 * returned coordinate is in top left of the cell
 * 
 */
inline coord_t Cell_To_Coord_Top_Left(cell_t cellnum)
{
    return (Cell_Get_X(cellnum) << 8) | (Cell_Get_Y(cellnum) << 24);
}

inline lepton_t Coord_Cell_To_Lepton(int cellcoord)
{
    return cellcoord * 256;
}

inline lepton_t Pixel_To_Lepton(int pixel)
{
    return ((pixel << 8) + 12) / 24;
}

inline int Lepton_To_Pixel(lepton_t lepton)
{
    return (24 * lepton + 128) >> 8;
}

inline lepton_t Lepton_Round_To_Pixel(lepton_t lepton)
{
    return Pixel_To_Lepton(Lepton_To_Pixel(lepton));
}

inline coord_t Coord_From_Pixel_XY(int x, int y)
{
    return Coord_From_Lepton_XY(Pixel_To_Lepton(x), Pixel_To_Lepton(y));
}

inline coord_t Round_Coord_To_Pixel(coord_t coord)
{
    return Coord_From_Pixel_XY(Lepton_To_Pixel(Coord_Lepton_X(coord)), Lepton_To_Pixel(Coord_Lepton_Y(coord)));
}

inline uint8_t Lepton_To_Cell_Coord(lepton_t lepton)
{
    return ((uint16_t)(lepton + 128) >> 8) & 0xFF;
}

inline uint8_t Lepton_To_Cell_Coord_Chop(lepton_t lepton)
{
    return ((uint16_t)lepton >> 8) & 0xFF;
}

inline uint8_t Lepton_Sub_Cell(lepton_t lepton)
{
    return lepton & 0xFF;
}

inline BOOL Valid_Cell(cell_t cellnum)
{
    return cellnum != 0;
}

inline cell_t Cell_Get_Adjacent(cell_t cellnum, FacingType facing)
{
    return AdjacentCell[facing] + cellnum;
}

inline DirType Cell_Direction8(cell_t cell1, cell_t cell2)
{
    return Desired_Facing8(Cell_Get_X(cell1), Cell_Get_Y(cell1), Cell_Get_X(cell2), Cell_Get_Y(cell2));
}

DirType Coord_Direction(coord_t coord1, coord_t coord2);
DirType Target_Direction(target_t target1, target_t target2);
DirType Cell_Direction(cell_t cell1, cell_t cell2);

int Coord_Distance(coord_t coord1, coord_t coord2);

/**
 * Calculate the distance (in leptons) between two targets.
 */
inline int Target_Distance(target_t target1, target_t target2)
{
    return Coord_Distance(As_Coord(target1), As_Coord(target1));
}

int Cell_Distance(cell_t cell1, cell_t cell2);

void Move_Point(int16_t &x, int16_t &y, DirType dir, uint16_t distance);
coord_t Coord_Move(coord_t coord, DirType dir, uint16_t distance);
coord_t Coord_Scatter(coord_t coord, uint16_t distance, BOOL center = false);
BOOL __cdecl Confine_Rect(int &x_pos, int &y_pos, int x, int y, int w, int h);
const int16_t *Coord_Spillage_List(coord_t coord, int size);
const int16_t *Coord_Spillage_List(coord_t coord, const TRect<int> &rect, BOOL skip_match);

#endif // COORD_H
