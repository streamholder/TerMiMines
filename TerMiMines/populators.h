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

#include "mines.h"

/*
 * Generates test games with various patterns depending on the param. ignores the seed.
 * 0: empty board
 * 1: all bombs
 * 2: checker pattern
 * 3: horizontal lines
 * For other param values, it leaves all content values uninitialized
 */
void populators_test(MinesCell **board, unsigned int width, unsigned int height, int seed, int param);

/*
 * pseudorandom generator. ignores the param.
 */
void populators_pseudorandom(MinesCell **board, unsigned int width, unsigned int height, int seed, int param);

/*
 * populator that should resemble what classical minesweeper implementations do.
 * seed is used for prng, param is the number of bombs.
 */
void populators_nummines(MinesCell **board, unsigned int width, unsigned int height, int seed, int param);
