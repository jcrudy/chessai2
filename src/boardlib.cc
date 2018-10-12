/*
 * boardlib.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: jason
 */
namespace boardlib {

const SquareIndex kNoEnPassant = 0b11111111;
const SquareIndex kSquaresPerBoard = 64;
const SquareIndex kRanksPerBoard = 8;
const SquareIndex kFilesPerBoard = 8;
const Move kNoMove = {0, 0, Piece::NO_PIECE};

const BitBoard kEmpty = 0x0000000000000000ULL;
const BitBoard kFull = 0xFFFFFFFFFFFFFFFFULL;
const BitBoard kSquare0 = BitBoard(0x1ULL << 0);
const BitBoard kSquare1 = BitBoard(0x1ULL << 1);
const BitBoard kSquare2 = BitBoard(0x1ULL << 2);
const BitBoard kSquare3 = BitBoard(0x1ULL << 3);
const BitBoard kSquare4 = BitBoard(0x1ULL << 4);
const BitBoard kSquare5 = BitBoard(0x1ULL << 5);
const BitBoard kSquare6 = BitBoard(0x1ULL << 6);
const BitBoard kSquare7 = BitBoard(0x1ULL << 7);
const BitBoard kSquare8 = BitBoard(0x1ULL << 8);
const BitBoard kSquare9 = BitBoard(0x1ULL << 9);
const BitBoard kSquare10 = BitBoard(0x1ULL << 10);
const BitBoard kSquare11 = BitBoard(0x1ULL << 11);
const BitBoard kSquare12 = BitBoard(0x1ULL << 12);
const BitBoard kSquare13 = BitBoard(0x1ULL << 13);
const BitBoard kSquare14 = BitBoard(0x1ULL << 14);
const BitBoard kSquare15 = BitBoard(0x1ULL << 15);
const BitBoard kSquare16 = BitBoard(0x1ULL << 16);
const BitBoard kSquare17 = BitBoard(0x1ULL << 17);
const BitBoard kSquare18 = BitBoard(0x1ULL << 18);
const BitBoard kSquare19 = BitBoard(0x1ULL << 19);
const BitBoard kSquare20 = BitBoard(0x1ULL << 20);
const BitBoard kSquare21 = BitBoard(0x1ULL << 21);
const BitBoard kSquare22 = BitBoard(0x1ULL << 22);
const BitBoard kSquare23 = BitBoard(0x1ULL << 23);
const BitBoard kSquare24 = BitBoard(0x1ULL << 24);
const BitBoard kSquare25 = BitBoard(0x1ULL << 25);
const BitBoard kSquare26 = BitBoard(0x1ULL << 26);
const BitBoard kSquare27 = BitBoard(0x1ULL << 27);
const BitBoard kSquare28 = BitBoard(0x1ULL << 28);
const BitBoard kSquare29 = BitBoard(0x1ULL << 29);
const BitBoard kSquare30 = BitBoard(0x1ULL << 30);
const BitBoard kSquare31 = BitBoard(0x1ULL << 31);
const BitBoard kSquare32 = BitBoard(0x1ULL << 32);
const BitBoard kSquare33 = BitBoard(0x1ULL << 33);
const BitBoard kSquare34 = BitBoard(0x1ULL << 34);
const BitBoard kSquare35 = BitBoard(0x1ULL << 35);
const BitBoard kSquare36 = BitBoard(0x1ULL << 36);
const BitBoard kSquare37 = BitBoard(0x1ULL << 37);
const BitBoard kSquare38 = BitBoard(0x1ULL << 38);
const BitBoard kSquare39 = BitBoard(0x1ULL << 39);
const BitBoard kSquare40 = BitBoard(0x1ULL << 40);
const BitBoard kSquare41 = BitBoard(0x1ULL << 41);
const BitBoard kSquare42 = BitBoard(0x1ULL << 42);
const BitBoard kSquare43 = BitBoard(0x1ULL << 43);
const BitBoard kSquare44 = BitBoard(0x1ULL << 44);
const BitBoard kSquare45 = BitBoard(0x1ULL << 45);
const BitBoard kSquare46 = BitBoard(0x1ULL << 46);
const BitBoard kSquare47 = BitBoard(0x1ULL << 47);
const BitBoard kSquare48 = BitBoard(0x1ULL << 48);
const BitBoard kSquare49 = BitBoard(0x1ULL << 49);
const BitBoard kSquare50 = BitBoard(0x1ULL << 50);
const BitBoard kSquare51 = BitBoard(0x1ULL << 51);
const BitBoard kSquare52 = BitBoard(0x1ULL << 52);
const BitBoard kSquare53 = BitBoard(0x1ULL << 53);
const BitBoard kSquare54 = BitBoard(0x1ULL << 54);
const BitBoard kSquare55 = BitBoard(0x1ULL << 55);
const BitBoard kSquare56 = BitBoard(0x1ULL << 56);
const BitBoard kSquare57 = BitBoard(0x1ULL << 57);
const BitBoard kSquare58 = BitBoard(0x1ULL << 58);
const BitBoard kSquare59 = BitBoard(0x1ULL << 59);
const BitBoard kSquare60 = BitBoard(0x1ULL << 60);
const BitBoard kSquare61 = BitBoard(0x1ULL << 61);
const BitBoard kSquare62 = BitBoard(0x1ULL << 62);
const BitBoard kSquare63 = BitBoard(0x1ULL << 63);
const BitBoard kSquares[64] = {kSquare0, kSquare1, kSquare2, kSquare3, kSquare4,
		kSquare5, kSquare6, kSquare7, kSquare8, kSquare9, kSquare10, kSquare11,
		kSquare12, kSquare13, kSquare14, kSquare15, kSquare16, kSquare17, kSquare18,
		kSquare19, kSquare20, kSquare21, kSquare22, kSquare23, kSquare24, kSquare25,
		kSquare26, kSquare27, kSquare28, kSquare29, kSquare30, kSquare31, kSquare32,
		kSquare33, kSquare34, kSquare35, kSquare36, kSquare37, kSquare38, kSquare39,
		kSquare40, kSquare41, kSquare42, kSquare43, kSquare44, kSquare45, kSquare46,
		kSquare47, kSquare48, kSquare49, kSquare50, kSquare51, kSquare52, kSquare53,
		kSquare54, kSquare55, kSquare56, kSquare57, kSquare58, kSquare59, kSquare60,
		kSquare61, kSquare62, kSquare63};

#include "zobrist.inc"

};

} // namespace boardlib
