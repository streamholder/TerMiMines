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

#include "ui.h"

/*
* will find a better solution later
*/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

/* ─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╠╟╡╢╣╤╥╦╧╨╩╪╫╬ */

/* this is mostly to stop the compiler complaining about code paths not returning any values */
const char *UI_INVALID_STATE = "If you are reading this, the game is broken in some bad way";

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

	return UI_INVALID_STATE;
}

const char *UI_FORMAT = ""
"Game status: %s\n"
"Mines remaining: %i\n"
"\n";

#define putstring(s) fputs(s, stdout)

/* todo: rewrite with a macro, should become way smaller */
char ui_index_to_char(unsigned int index)
{
	/* numbers 0-9 */
	if (index <= 9)
	{
		return '0' + index;
	}

	/* lowercase letters */
	if (index <= 35) /* 10 + 26 */
	{
		return 'a' + index - 10;
	}

	/* uppercase letters */
	if (index <= 61) /* 10 + 26 + 26 */
	{
		return 'A' + index - 36;
	}

	/* first 15 ascii symbols */
	if (index <= 76) /* 10 + 26 + 26 + 15 */
	{
		return '!' + index - 62;
	}

	/* 7 more ascii symbols */
	if (index <= 83) /* 10 + 26 + 26 + 15 + 7 */
	{
		return ':' + index - 77;
	}

	/* 6 more ascii symbols */
	if (index <= 89) /* 10 + 26 + 26 + 15 + 7 + 6 */
	{
		return '[' + index - 84;
	}

	/* last 4 ascii symbols */
	if (index <= 93) /* 10 + 26 + 26 + 15 + 7 + 6 + 4 */
	{
		return '{' + index - 90;
	}

	return 0;
}

unsigned int ui_char_to_index(char ch)
{
	if (ch < '!')
	{
		return UI_MAX_VALID_INDEX + 1;
	}

	if (ch <= 47)
	{
		return ch - '!' + 62;
	}

	if (ch <= '9')
	{
		return ch - '0';
	}

	if (ch <= '@')
	{
		return ch - ':' + 77;
	}

	if (ch <= 'Z')
	{
		return ch - 'A' + 36;
	}

	if (ch <= '`')
	{
		return ch - '[' + 84;
	}

	if (ch <= 'z')
	{
		return ch - 'a' + 10;
	}

	if (ch <= '~')
	{
		return ch - '{' + 90;
	}

	return UI_MAX_VALID_INDEX + 1;
}

void ui_print_board(MinesBoard *board) /* to heavily optimize */
{
	unsigned int x, y;
	MinesCell cell;

	printf(UI_FORMAT, _ui_state_to_string(board->state), board->mines - board->flags);

	/* print x index  */
	putchar(' ');
	for (x = 0; x < board->width; x++)
	{
		putchar(' ');
		putchar(ui_index_to_char(x));
	}
	putchar('\n');

	/* print first row  */
	putchar(' ');
	for (x = 0; x < board->width; x++)
	{
		putstring("+-");
	}
	putstring("+\n");

	/* print everything else row by row */
	for (y = 0; y < board->height; y++)
	{
		/* | | | | */
		putchar(ui_index_to_char(y));
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
		putchar(' ');
		for (x = 0; x < board->width; x++)
		{
			putstring("+-");
		}

		putstring("+\n");
	}
}
