/*
 * test_board_state.cc
 *
 *  Created on: Oct 18, 2018
 *      Author: jason
 */

#include "catch.hpp"
#include <boardlib.h>

using namespace boardlib;

TEST_CASE("Member functions make_move and unmake_move are inverses."){
	std::string starting_position =
				"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	BoardState board = BoardState::from_fen(starting_position);
	BoardState board_copy = board.copy();
	Move move = Move(12, 28, Piece::NO_PIECE);
	MoveRecord record = board.make_move(move);
	board.unmake_move(record);

	REQUIRE(board == board_copy);

}
