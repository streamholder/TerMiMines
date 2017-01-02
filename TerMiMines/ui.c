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

#include "ui.h"

/*
* will find a better solution later
*/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


/* ─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╠╟╡╢╣╤╥╦╧╨╩╪╫╬ */

const char *UI_NEW_GAME = "New game";
const char *UI_PLAYING = "Playing";
const char *UI_WON = "You won";
const char *UI_LOST = "You lost the game";

const char * _ui_state_to_string(MinesGameState state)
{
	switch (state)
	{
	case New_Game:
		return UI_NEW_GAME;
		break;
	case Game_Playing:
		return UI_PLAYING;
		break;
	case Game_Won:
		return UI_WON;
		break;
	case Game_Lost:
		return UI_LOST;
		break;
	}
}

const char *UI_FORMAT = ""
"Game status: %s\n"
"Mines remaining: %i\n"
"\n";

#define putstring(s) fputs(s, stdout)

void ui_print_board(MinesBoard *board) /* to heavily optimize */
{
	unsigned int x, y;
	MinesCell cell;

	printf(UI_FORMAT, _ui_state_to_string(board->state), board->mines - board->flags);

	/* print first row  */
	for (x = 0; x < board->width; x++)
	{
		putstring("+-");
	}
	putstring("+\n");

	/* print everything else row by row */
	for (y = 0; y < board->height; y++)
	{
		/* | | | | */
		for (x = 0; x < board->width; x++)
		{
			putchar('|');

			mines_get_cell(board, x, y, &cell);
			switch (cell.state)
			{
			case Closed:
				putchar(176); /* ░ */
				break;
			case Flagged:
				putchar('>');
				break;
			case Marked:
				putchar('?');
				break;
			case Open:
				if (cell.content) /* bomb */
				{
					putchar('o');
				}
				else /* empty */
				{
					putchar(cell.neighbours ? '0' + cell.neighbours : ' ');
				}
				break;
			}
		}

		putstring("|\n");

		/* +-+-+-+ */
		for (x = 0; x < board->width; x++)
		{
			putstring("+-");
		}

		putstring("+\n");
	}
}
