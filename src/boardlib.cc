/*
 * boardlib.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: jason
 */

#include <boardlib.h>
using namespace boardlib;

// See https://stackoverflow.com/questions/8016780/undefined-reference-to-static-constexpr-char
// for an unsatisfying explanation as to why this external declaration is necessary.
constexpr std::array<ZobristKey, kSquaresPerBoard*(ZobristHasher::kNumberOfPieceTypes)> ZobristHasher::zobrist_table_;
