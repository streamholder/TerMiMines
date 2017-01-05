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

/*
 * macro to allow easier change to the table in the future, entries must be defined in increasing order of index,
 * let's hope the compiler is smart enough to optimize i out.
 * if anyone figures out a better way to do all this, please submit a patch.
 */
#define UI_ITC_TABLE_ENTRY(start_ch, end_ch) { if (index <= end_ch - start_ch + i) return start_ch + index - i; i += end_ch - start_ch + 1; }
char ui_index_to_char(unsigned int index)
{
	unsigned int i = 0;

	UI_ITC_TABLE_ENTRY('0', '9'); /* 0-9 */
	UI_ITC_TABLE_ENTRY('a', 'z'); /* 10-35 */
	UI_ITC_TABLE_ENTRY('A', 'Z'); /* 36-61 */
	UI_ITC_TABLE_ENTRY('!', '/'); /* 62-75 */
	UI_ITC_TABLE_ENTRY(':', '@'); /* 77-83 */
	UI_ITC_TABLE_ENTRY('[', '`'); /* 84-89 */
	UI_ITC_TABLE_ENTRY('{', '~'); /* 90-93 */

	return 0;
}

/*
 * same as above. this macro takes 3 parameters instead of two for the simple reason that while indices are necessarily continuous
 * (and thus can be kept track of internally using a counter) we are not necessarily using all ascii characters in a continuous range.
 * entries must be defined in increasing order of ascii character number.
 */
#define UI_CTI_TABLE_ENTRY(start_ch, end_ch, start_index) if (ch <= end_ch) return ch - start_ch + start_index;
unsigned int ui_char_to_index(char ch)
{
	if (ch < '!')
	{
		return UI_MAX_VALID_INDEX + 1;
	}

	UI_CTI_TABLE_ENTRY('!', '/', 62);
	UI_CTI_TABLE_ENTRY('0', '9', 0);
	UI_CTI_TABLE_ENTRY(':', '@', 77);
	UI_CTI_TABLE_ENTRY('A', 'Z', 36);
	UI_CTI_TABLE_ENTRY('[', '`', 84);
	UI_CTI_TABLE_ENTRY('a', 'z', 10);
	UI_CTI_TABLE_ENTRY('{', '~', 90);

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
				putchar(x % 2 ? 177 : 176); /* ░ */
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
