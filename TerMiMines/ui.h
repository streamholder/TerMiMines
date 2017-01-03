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

#define UI_MAX_VALID_INDEX 93

/*
 * maps an index number to a printable character from the non-extended ascii table.
 * it first uses the numbers, then the lowercase letters, then the uppercase letters and finally
 * all the symbols, for a total of 94 printable characters. this is done for easy-as-possible input
 * from the users, since these characters are used to indicate the coordinates of cells.
 * it returns 0 for index > UI_MAX_VALID_INDEX = 93.
 */
char ui_index_to_char(unsigned int index);

/*
 * does the same mapping as ui_index_to_char, reversed.
 * it returns 94 for invalid input character.
 */
unsigned int ui_char_to_index(char ch);

/*
 * prints the game state and the board.
 */
void ui_print_board(MinesBoard *board);

