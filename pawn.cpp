#include "header.hpp"

void pawn::calc_moves(figure* (&board)[8][8], int startx, int starty)
{
	figure::calc_moves(board, startx, starty);
	int nexty = starty + (1 - side * 2);
	if (!board[startx][nexty])
	{
		listx->push(startx);
		listy->push(nexty);
		if (is_moved() == false)
		{
			if (!board[startx][starty + (2 - side * 4)])
			{
				listx->push(startx);
				listy->push(starty + (2 - side * 4));
			}
		}
	}
	if (startx < 7)
	{
		if ((board[startx + 1][nexty] && board[startx + 1][nexty]->get_side() != side) ||
			is_bite_field(startx + 1, nexty, BITE_FIELD_GET))
		{
			listx->push(startx + 1);
			listy->push(nexty);
		}
	}
	if (startx > 0)
	{
		if ((board[startx - 1][nexty] && board[startx - 1][nexty]->get_side() != side) ||
			is_bite_field(startx - 1, nexty, BITE_FIELD_GET))
		{
			listx->push(startx - 1);
			listy->push(nexty);
		}
	}
}

int pawn::move(Vector2i &oldnp, Vector2i &np)
{
	bool ret = first_move;
	first_move = false;
	if (ret && abs(np.y - oldnp.y) == 2)
	{
		is_bite_field(np.x, np.y - (side == 0 ? 1 : -1), BITE_FIELD_SET);
		return (FIRST_MOVE_PAWN);
	}
	if (np.y == 7 || np.y == 0)
		return (MOVE_TRANSFORM);
	if (is_bite_field(np.x, np.y, BITE_FIELD_GET))
		return (PAWN_BEAT_BITE);
	count_moves(COUNT_RESET);// Обнулить счетчик ходов
	return (MOVE_DEFAULT);
}

bool pawn::can_reach(move_info &mv)
{
	if (first_move && abs(mv.y - mv.l) == 2)
	{
		figure *(*board)[8][8] = mv.board;
		if ((*board)[mv.x][mv.y + (side == 0 ? 1 : -1)])
			return (false);// Проверка на препятствие при прыжке на 1й ход пешки
	}
	return (mv.x == mv.i && (side == 0 ? (mv.l - mv.y <= 1 + first_move) : (mv.y - mv.l <= 1 + first_move)));
}