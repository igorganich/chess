#ifndef GAME_HPP
#define GAME_HPP

#include "figures.hpp"

#define KING(x) new king((x), (figures))
#define QUEEN(x) new queen((x), (figures))
#define ROOK(x) new rook((x), (figures))
#define BISHOP(x) new bishop((x), (figures))
#define HORSE(x) new horse((x), (figures))
#define PAWN(x) new pawn((x), (figures))

#define CELL 56.0f

#define RETURN_STOP_FAIL 0
#define RETURN_STOP_SUCCESS 1
#define RETURN_CONTINUE 2

#define IN_BOUNDS(x) (((x) < (8)) && ((x) >= 0))

using namespace std;
using namespace sf;

class game
{
private:
	std::map<long long, int> posmap;// map в котором будет храниться кол-во повторений той или иной позиции
	figure *board[8][8];
	Vector2i kings[2] =
	{
		Vector2i(4, 0), Vector2i(4, 7),
	};// Тут хранятся поля королей
	bool turn = 0;// Чей сейчас ход. 0 - белые, 1 - черные
	Sprite sBoard;
	Texture figures;
	Texture boardtxt;

	long long compute_hash(string const& s);// Функция получения хеша для сохранения текущей ситуации на доске
	int beat_reachable(move_info &mv);// Может ли побить фигура это поле
	bool is_check_horse(move_info &mv);// Есть ли конь, который даёт шах?
	bool is_check_horvert(move_info &mv);//Есть ли по горизонтали или вертикали фигура, которая даёт шах?
	bool is_check_diag(move_info &mv);// Есть ли по диагонали фигура, дающая шах?
	bool is_check(Vector2i &kingid, move_info &mv);//Под шахом ли король?
	bool no_valid_move();// Остался ли хоть один валидный ход
	bool is_fifty_moves();// Проверяем правило 50 ходов

public:
	game();
	Sprite *get_board_sprite()
	{
		return (&sBoard);
	}
	figure *get_figure(Vector2i &np)
	{
		return board[np.x][np.y];
	}

	bool verify(Vector2i &oldnp, Vector2i &np, int prevret);// Корректен ли такой ход

	int move(Vector2i &oldnp, Vector2i &np, char &tform);// Обрабатываем ход

	bool is_three_times_pos();// Правило троекратного повторения позиции

	bool is_game_end();//Закончилась ли игра
};

#endif