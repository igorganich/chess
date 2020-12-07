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
	std::map<long long, int> posmap;// map � ������� ����� ��������� ���-�� ���������� ��� ��� ���� �������
	figure *board[8][8];
	Vector2i kings[2] =
	{
		Vector2i(4, 0), Vector2i(4, 7),
	};// ��� �������� ���� �������
	bool turn = 0;// ��� ������ ���. 0 - �����, 1 - ������
	Sprite sBoard;
	Texture figures;
	Texture boardtxt;

	long long compute_hash(string const& s);// ������� ��������� ���� ��� ���������� ������� �������� �� �����
	int beat_reachable(move_info &mv);// ����� �� ������ ������ ��� ����
	bool is_check_horse(move_info &mv);// ���� �� ����, ������� ��� ���?
	bool is_check_horvert(move_info &mv);//���� �� �� ����������� ��� ��������� ������, ������� ��� ���?
	bool is_check_diag(move_info &mv);// ���� �� �� ��������� ������, ������ ���?
	bool is_check(Vector2i &kingid, move_info &mv);//��� ����� �� ������?
	bool no_valid_move();// ������� �� ���� ���� �������� ���
	bool is_fifty_moves();// ��������� ������� 50 �����

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

	bool verify(Vector2i &oldnp, Vector2i &np, int prevret);// ��������� �� ����� ���

	int move(Vector2i &oldnp, Vector2i &np, char &tform);// ������������ ���

	bool is_three_times_pos();// ������� ������������ ���������� �������

	bool is_game_end();//����������� �� ����
};

#endif