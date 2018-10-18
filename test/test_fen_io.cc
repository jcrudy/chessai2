/*
 * test_fen_io.cc
 *
 *  Test creation of BoardState from FEN strings and creation of
 *  FEN strings from BoardState.
 *
 */
#include "catch.hpp"
#include <boardlib.h>


TEST_CASE("Pass-through FEN reading and writing works.") {
	std::vector<std::string> fens = {
			"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
			"rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2",
			"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
			};
	for (std::string fen : fens) {
		REQUIRE(fen == boardlib::BoardState::from_fen(fen).to_fen());
	}
}


