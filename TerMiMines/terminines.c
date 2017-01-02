/*
 * Copyright 2016 Riccardo Paolo Bestetti <riccardo [dt] kyogre [at] live [dt] it>
 *
 * This file is part of TerMiMines.
 *
 * TerMiMines is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nome-Programma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TerMiMines.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

#include "mines.h"
#include "ui.h"
#include "populators.h"

/*
 * will find a better solution later
 */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

int main(int argc, char *argv[])
{
	unsigned int x, y;
	MinesBoard game;
	MinesRuleset rules;

	rules.wraparound = 0;

	if (!mines_init_board(40, 10, &rules, &game))
	{
		fprintf(stderr, "Cannot start the game, failed to initialize the board");
		return 1;
	}

	mines_populate_board(&game, 0, &populators_pseudorandom);

	mines_compute_board(&game);

	mines_open_cell(&game, 10, 5); /* random test for ui */

	ui_print_board(&game);

	return 0;
}
