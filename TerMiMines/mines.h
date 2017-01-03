/*
* Copyright 2016-2017 Riccardo Paolo Bestetti <riccardo [dt] kyogre [at] live [dt] it>
*
* This file is part of TerMiMines.
*
* TerMiMines is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* TerMiMines is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TerMiMines.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

enum _MinesCellState_e { Closed, Open, Flagged, Marked };
typedef enum _MinesCellState_e MinesCellState;

struct _MinesCell_s /* would bit-packing be a good trade-off here? probably not */
{
	MinesCellState state;
	char content; /* mine = 1, empty = 0 */
	char neighbours; /* neighbour bombs, 0-8, so char is more than enough */
};
typedef struct _MinesCell_s MinesCell;

struct _MinesRuleset_s /* same consideration on bit-packing as above :) */
{
	char wraparound; /* does the cell-opening algorithm wrap around the boundries? */
};
typedef struct _MinesRuleset_s MinesRuleset;

enum _MinesGameState_e { New_Game, Game_Playing, Game_Lost, Game_Won };
typedef enum _MinesGameState_e MinesGameState;

struct _MinesBoard_s
{
	MinesCell **board; /* double pointer allows for 2d-array-like access */
	unsigned int width;
	unsigned int height;
	unsigned int mines;
	unsigned int flags;
	unsigned int marks;
	unsigned int open;
	MinesRuleset *ruleset; /* TODO: pointer or copy? let's do pointer for now */
	MinesGameState state;
};
typedef struct _MinesBoard_s MinesBoard;

typedef void (*MinesBoardPopulator)(MinesCell **, unsigned int, unsigned int, int);

/* all the functions assume that you are passing a valid pointer, no void check in here! */

/*
 * allocates memory for a board and initializes all the tiles to empty
 * returns 1 for success and 0 for failure
 */
int mines_init_board(unsigned int width, unsigned int height, MinesRuleset *ruleset, MinesBoard *dest);

/*
 * deallocates memory
 * always returns 1
 */
int mines_destroy_board(MinesBoard *b);

/*
 * populates a board with mines with a seed-based populator
 * accepts a populator function with the following prototype:
 * int populator(MinesCell *cell, unsigned int x, unsigned int y, int seed)
 * where the returned value is the seed for the next cell (usually the same seed that has been passed!)
 */
void mines_populate_board(MinesBoard *b, int seed, MinesBoardPopulator populator);

/* computes the neighbours for all cells and the number of mines */
void mines_compute_board(MinesBoard *b);

/* sets a cell to the passed cell */
void mines_set_cell(MinesBoard *b, unsigned int x, unsigned int y, MinesCell *c);

/*
 * gets a cell into the passed pointer, does boundry checks
 * note that you should provide memory to fit the MinesCell struct
 */
void mines_get_cell(MinesBoard *b, unsigned int x, unsigned int y, MinesCell *c);

/*
 * opens a cell, using flood-fill to open nearby free cells
 * returns the number of cells that have been opened
 */
unsigned int mines_open_cell(MinesBoard *b, unsigned int x, unsigned int y);

/*
 * sets a cell's state to one of the glyphs (flag or mark).
 * this is done in a gameplay-wise way (e.g. if the cell is open it can't be flagged or marked
 * and the call will silently fail. todo: don't fail silently to allow to warn the user when
 * they do something they're not supposed to to by the rules, but maybe that should be all done
 * in the frontend).
 * this also updates the count of flags & marks.
 * this ignores calls with the last parameter set as Open, calls to modify open cells and calls
 * where the new state is the same as the old one.
 */
void mines_set_cell_glyph(MinesBoard *b, unsigned int x, unsigned int y, MinesCellState s);

/* covers all the cells in an existing board allowing it to be replayed */
void mines_replay_board(MinesBoard *b);

