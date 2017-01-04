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

#include <stdio.h>

#include "mines.h"
#include "ui.h"
#include "populators.h"

typedef struct
{
	char o;
	unsigned int x;
	unsigned int y;
}
GameInput;

void game_input(GameInput *c)
{
	char o, x, y;

	printf("(oxy)> ");

	/* rudimentary cross-platform fix, we can probably do something better (?) */
#ifdef _MSC_VER
	scanf_s(" %c%c%c", &o, 1, &x, 1, &y, 1);
#else
	scanf(" %c%c%c", &o, &x, &y);
#endif

	c->o = o;
	c->x = ui_char_to_index(x);
	c->y = ui_char_to_index(y);
}

int main(int argc, char *argv[])
{
	MinesBoard game;
	MinesRuleset rules;
	GameInput c;

	rules.wraparound = 0;

	if (!mines_init_board(40, 10, &rules, &game))
	{
		fprintf(stderr, "Cannot start the game, failed to initialize the board");
		return 1;
	}

	mines_populate_board(&game, 0xDEADBEEF, 80, &populators_nummines);

	mines_compute_board(&game);

	ui_print_board(&game);

	while (game.state != Game_Lost && game.state != Game_Won)
	{
		game_input(&c);

		switch (c.o)
		{
		case 'o':
			mines_open_cell(&game, c.x, c.y);
			break;
		case 'f':
			mines_set_cell_glyph(&game, c.x, c.y, Flagged);
			break;
		case 'm':
			mines_set_cell_glyph(&game, c.x, c.y, Marked);
			break;
		case 'c':
			mines_set_cell_glyph(&game, c.x, c.y, Closed);
			break;
		}

		ui_print_board(&game);
	}

	return 0;
}
