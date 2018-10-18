/*
 * test_zobrist.cc
 *
 *  Created on: Oct 16, 2018
 *      Author: jason
 */
#include "catch.hpp"
#include <boardlib.h>

using namespace boardlib;

TEST_CASE("Zobrist update gives the same results as raw Zobrist computation.") {
	std::string starting_position =
			"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	BoardState board = BoardState::from_fen(starting_position);
	Move move = Move(12, 28, Piece::NO_PIECE);
	board.make_move(move);
	ZobristKey new_hash_should_be = ZobristHasher::hash(board);
	ZobristKey new_hash_should_is = board.get_hash();
	REQUIRE(new_hash_should_is == new_hash_should_be);
}


