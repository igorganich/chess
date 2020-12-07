#ifndef FIGURES_HPP
#define FIGURES_HPP

#define MOVE_DEFAULT 0
#define FIRST_MOVE_PAWN 1
#define PAWN_BEAT_BITE 2
#define MOVE_CASTLING 3
#define MOVE_TRANSFORM 4

#define SPRITESIZE 56

#define BITE_FIELD_GET 0
#define BITE_FIELD_SET 1

#define COUNT_INC 1
#define COUNT_RESET 2
#define COUNT_GET 0

using namespace std;
using namespace sf;

class figure;

struct move_info
{
	figure **figmute;
	figure* (*board)[8][8];
	figure **figunmute;
	Vector2i *unmute;
	Vector2i *mute;
	int x;
	int y;
	int i;
	int l;
	bool castling;
};

class figure
{
private:
	int get_next_counter();// Вспомогательная функция для определения координат спрайта в начале игры
protected:
	stack<int> *listx = NULL;
	stack<int> *listy = NULL;//тут хранятся поля возможных ходов для проверки на мат/пат
	Sprite *sprite = NULL;//Указатель на спрайт фигуры
	bool side;//фигура черных или белых
	bool first_move = false;//Ходила ли фигура? Полезно для короля, туры (рокировка) и пешки.
	bool can_reach_horvert(move_info &mv);// Может ли фигура так походить горизонтально или вертикально

	bool can_reach_diag(move_info &mv);// Может ли фигура так походить диагонально

	void calc_way_step(figure* (&board)[8][8], const int startx, const int starty, int stepx, int stepy);//Сохранить возможные хода по прямой (слон, ладья, ферзь)

	void calc_one_step(figure* (&board)[8][8], int startx, int starty, int plusx, int plusy);//Сохранить ход если возможен(король, конь)

	void calc_moves_horvert(figure* (&board)[8][8], const int startx, const int starty);//Сохранить возможные хода по вертикали или горизонтали (ладья, ферзь)

	void calc_moves_diag(figure* (&board)[8][8], const int startx, const int starty);//Сохранить возможные хода по диагонали(слон, ферзь)

public:
	figure();

	figure(bool _side, Texture &txt);

	virtual string get_pos_info()//Получить информацию про фигуру для хеша
	{
		string ret = "" + first_move;
		ret += side;
		return (ret);
	}

	virtual void calc_moves(figure* (&board)[8][8], int startx, int starty)//Вызывается всеми дочерними классами, очищает списки ходов
	{
		delete listx;
		delete listy;
		listx = new stack<int>();
		listy = new stack<int>();
	}

	void get_next_move(move_info &mv);//Получить следующее корректное поле для хода из listx + listy

	bool is_moved()
	{
		return (!first_move);
	}

	virtual int move(Vector2i &oldnp, Vector2i &np)
	{
		first_move = false;
		return (MOVE_DEFAULT);
	}

	virtual bool can_reach(move_info &mv)// Может ли достичь фигура данной точки
	{
		return (false);
	}

	virtual bool can_beat(move_info &mv)// Может ли побить фигура эту фигуру
	{
		return (false);
	}
	Sprite *get_sprite()
	{
		return (sprite);
	}
	bool get_side()
	{
		return side;
	}
	~figure()
	{
		delete sprite;
		delete listx;
		delete listy;
	}
};

class pawn : public figure
{
public:
	static bool is_bite_field(int x, int y, int flag)// Статический метод получения/установки поля взятия на проходе
	{
		static int bitex = -1;
		static int bitey = -1;
		if (flag == BITE_FIELD_GET)
			return (x == bitex && y == bitey);
		bitex = x;
		bitey = y;
		return (false);
	}

	static int count_moves(int flag)// Статический метод для подсчёта для правила 50 ходов
	{
		static int count = 0;
		if (flag == COUNT_INC)
			count++;
		if (flag == COUNT_RESET)
			count = 0;
		return (count);
	}

	pawn(bool _side, Texture &txt) : figure(_side, txt)
	{
		first_move = true;
		sprite->setTextureRect(IntRect(SPRITESIZE * 5, SPRITESIZE * side, SPRITESIZE, SPRITESIZE));
	}

	string get_pos_info()
	{
		return (figure::get_pos_info() + "p");
	}

	void calc_moves(figure* (&board)[8][8], int startx, int starty);//Сохранение возможных ходов пешки для проверки на мат

	int move(Vector2i &oldnp, Vector2i &np);

	bool can_reach(move_info &mv);

	bool can_beat(move_info &mv)
	{
		return (abs(mv.x - mv.i) == 1 && (side == 0 ? (mv.y == mv.l - 1) : (mv.y == mv.l + 1)));
	}
};

class horse : public figure
{
public:
	horse(bool _side, Texture &txt) : figure(_side, txt)
	{
		sprite->setTextureRect(IntRect(SPRITESIZE * 1, SPRITESIZE * side, SPRITESIZE, SPRITESIZE));
	}

	bool can_reach(move_info &mv);

	void calc_moves(figure* (&board)[8][8], int startx, int starty);

	string get_pos_info()
	{
		return (figure::get_pos_info() + "h");
	}

	bool can_beat(move_info &mv)
	{
		return (can_reach(mv));
	}
};



class rook : public figure
{
public:
	rook(bool _side, Texture &txt) : figure(_side, txt)
	{
		first_move = true;
		sprite->setTextureRect(IntRect(SPRITESIZE * 0, SPRITESIZE * side, SPRITESIZE, SPRITESIZE));
	}
	bool can_reach(move_info &mv)
	{
		return can_reach_horvert(mv);
	}
	bool can_beat(move_info &mv)
	{
		return (can_reach(mv));
	}

	string get_pos_info()
	{
		return (figure::get_pos_info() + "r");
	}

	void calc_moves(figure* (&board)[8][8], int startx, int starty)
	{
		figure::calc_moves(board, startx, starty);
		figure::calc_moves_horvert(board, startx, starty);
	}
};

class bishop : public figure
{
public:
	bishop(bool _side, Texture &txt) : figure(_side, txt)
	{
		sprite->setTextureRect(IntRect(SPRITESIZE * 2, SPRITESIZE * side, SPRITESIZE, SPRITESIZE));
	}

	bool can_reach(move_info &mv)
	{
		return(can_reach_diag(mv));
	}

	string get_pos_info()
	{
		return (figure::get_pos_info() + "b");
	}

	bool can_beat(move_info &mv)
	{
		return (can_reach(mv));
	}

	void calc_moves(figure* (&board)[8][8], int startx, int starty)
	{
		figure::calc_moves(board, startx, starty);
		figure::calc_moves_diag(board, startx, starty);
	}
};

class king : public figure
{
public:
	king(bool _side, Texture &txt) : figure(_side, txt)
	{
		first_move = true;
		sprite->setTextureRect(IntRect(SPRITESIZE * 4, SPRITESIZE * side, SPRITESIZE, SPRITESIZE));
	}

	int move(Vector2i &oldnp, Vector2i &np);

	string get_pos_info()
	{
		return (figure::get_pos_info() + "k");
	}

	bool can_reach(move_info &mv);

	void calc_moves(figure* (&board)[8][8], int startx, int starty);

	bool can_beat(move_info &mv)
	{
		return (abs(mv.x - mv.i) < 2 && abs(mv.y - mv.l) < 2);
	}

	bool verify(Vector2i &oldnp, Vector2i &np, figure* (&board)[8][8])
	{
		return (true);
	}
};

class queen : public figure
{
public:
	queen(bool _side, Texture &txt) : figure(_side, txt)
	{
		sprite->setTextureRect(IntRect(SPRITESIZE * 3, SPRITESIZE * side, SPRITESIZE, SPRITESIZE));
	}

	bool can_reach(move_info &mv)
	{
		return (can_reach_horvert(mv) ||
			can_reach_diag(mv));
	}

	string get_pos_info()
	{
		return (figure::get_pos_info() + "q");
	}

	bool can_beat(move_info &mv)
	{
		return (can_reach(mv));
	}

	void calc_moves(figure* (&board)[8][8], int startx, int starty)
	{
		figure::calc_moves(board, startx, starty);
		figure::calc_moves_diag(board, startx, starty);
		figure::calc_moves_horvert(board, startx, starty);
	}

	bool verify(Vector2i &oldnp, Vector2i &np, figure* (&board)[8][8])
	{
		return (1);
	}
};

#endif