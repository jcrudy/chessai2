/*
 * boardlib.hpp
 *
 *  Created on: Oct 2, 2018
 *      Author: jason
 */

#ifndef SRC_BOARDLIB_H_
#define SRC_BOARDLIB_H_

#include <boost/multiprecision/cpp_int.hpp>

namespace boardlib {


//enum class ZobristKey : uint64_t {};
//enum class SquareIndex : uint8_t {};
typedef uint64_t ZobristKey;
typedef uint8_t SquareIndex;

constexpr SquareIndex kNoEnPassant = 0b11111111;
constexpr SquareIndex kSquaresPerBoard = 64;
constexpr SquareIndex kRanksPerBoard = 8;
constexpr SquareIndex kFilesPerBoard = 8;
constexpr SquareIndex kNumberDistinctOfPiecesForZobrist = 14;

class BitBoard{
private:
	uint64_t value_;
public:
	constexpr BitBoard() : value_(0){}
	constexpr BitBoard(uint64_t value) : value_(value){}

	/*
	 * BitBoards can be copied.  This code is technically not
	 * needed, but the google style guide recommends making it
	 * explicit that copy-ability is intended.
	 */
	BitBoard(const BitBoard& rhs) = default;
	BitBoard& operator=(const BitBoard& rhs) = default;

	/*
	 * Construct a BitBoard containing exactly one square, specified
	 * by the SquareIndex square.  This will be implemented as a lookup
	 * for performance.
	 */
	static BitBoard from_square_index(SquareIndex square);

	/*
	 * BitBoards should have all the normal bitwise operations
	 */
	BitBoard operator&(const BitBoard rhs) const{
		return BitBoard(value_ & rhs.value_);
	}
	BitBoard operator|(const BitBoard rhs) const{
		return BitBoard(value_ | rhs.value_);
	}
	BitBoard operator^(const BitBoard rhs) const{
		return BitBoard(value_ ^ rhs.value_);
	}
	void operator&=(const BitBoard rhs){
		value_ = value_ & rhs.value_;
	}
	void operator|=(const BitBoard rhs){
		value_ = value_ | rhs.value_;
	}
	void operator^=(const BitBoard rhs){
		value_ = value_ ^ rhs.value_;
	}
	BitBoard operator~() const{
		return BitBoard(~value_);
	}
	BitBoard operator-() const{
		return BitBoard(-value_);
	}

	/*
	 * Get the SquareIndex of the most significant 1-bit of this
	 * BitBoard.  The most significant 1-bit is the 1-bit with
	 * the greatest corresponding SquareIndex.
	 */
	SquareIndex greatest_square_index() const{
		decltype(value_) temp_value = value_;
		SquareIndex result = 0;
		while(temp_value >>= 1){
			result++;
		}
		return(result);
	}

	/*
	 * Get the BitBoard of just the least significant 1-bit of this
	 * BitBoard.  The least significant 1-bit is the 1-bit with the
	 * lowest corresponding SquareIndex.
	 */
	BitBoard least_significant_1_bit() const{
		return BitBoard(value_ & (-value_));
	}

	/*
	 * Get the least significant 1-bit, then set that bit to 0.
	 */
	BitBoard pop_least_significant_1_bit(){
		decltype(value_) temp_value = value_ & (-value_);
		value_ ^= temp_value;
		return BitBoard(temp_value);
	}

	/*
	 * Get the number of set bits of this BitBoard, which is the number
	 * of pieces it represents.
	 */
	SquareIndex population_count() const{
		// TODO: Potentially not portable.
		return (SquareIndex) __builtin_popcountll(value_);
	}


};


inline SquareIndex square_index_of(SquareIndex rank_index, SquareIndex file_index){
	return (kFilesPerBoard * rank_index) + file_index;
}

inline SquareIndex rank_index_of(SquareIndex square){
	return (square / kFilesPerBoard);
}

inline SquareIndex file_index_of(SquareIndex square){
	return square % kFilesPerBoard;
}

inline std::string square_index_to_algebraic(SquareIndex square){
	char algebraic_file = 'a' + file_index_of(square);
	std::string result;
	result += algebraic_file;
	result += std::to_string(((int) rank_index_of(square) + 1));
	return result;
//	return "" + algebraic_file + std::to_string(((int) rank_index_of(square) + 1));
}

inline SquareIndex algebraic_to_square_index(std::string algebraic){
	SquareIndex file_index = (SquareIndex) algebraic[0] - 'a';
	SquareIndex rank_index = (SquareIndex) std::stoi(algebraic.substr(1)) - 1;
	return square_index_of(rank_index, file_index);
}

//typedef unsigned char SquareIndex;
enum class Piece : unsigned char {
	NO_PIECE,
	WHITE_KING,
	WHITE_QUEEN,
	WHITE_BISHOP,
	WHITE_KNIGHT,
	WHITE_ROOK,
	WHITE_PAWN,
	WHITE_EN_PASSANT,
	BLACK_KING,
	BLACK_QUEEN,
	BLACK_BISHOP,
	BLACK_KNIGHT,
	BLACK_ROOK,
	BLACK_PAWN,
	BLACK_EN_PASSANT
};

/*
 * Get the FEN character representation for a piece.  Represent color by case, with
 * whites capitalized and blacks lower case.  For *_EN_PASSANT pieces, return '*'.
 * For NO_PIECE, return '-'.
 */
inline char piece_to_fen(const Piece piece){
	switch(piece){
	case Piece::WHITE_KING:
		return 'K';
	case Piece::BLACK_KING:
		return 'k';
	case Piece::WHITE_QUEEN:
		return 'Q';
	case Piece::BLACK_QUEEN:
		return 'q';
	case Piece::WHITE_BISHOP:
		return 'B';
	case Piece::BLACK_BISHOP:
		return 'b';
	case Piece::WHITE_KNIGHT:
		return 'N';
	case Piece::BLACK_KNIGHT:
		return 'n';
	case Piece::WHITE_ROOK:
		return 'R';
	case Piece::BLACK_ROOK:
		return 'r';
	case Piece::WHITE_PAWN:
		return 'P';
	case Piece::BLACK_PAWN:
		return 'p';
	case Piece::WHITE_EN_PASSANT:
		return '*';
	case Piece::BLACK_EN_PASSANT:
		return '*';
	case Piece::NO_PIECE:
		return '-';
	default:
		throw "Invalid Piece passed to piece_to_fen.";
	}
}

/*
 * Convert a char, representing a piece in FEN notation, to a Piece.  Since
 * FEN does not indicate en passant in its board representation, instead
 * directly specifying the algebraic location of the en passant square in a
 * special section, this function does not handle en passant at all.
 */
inline Piece fen_to_piece(const char piece_char){
	switch(piece_char){
	case 'K':
		return Piece::WHITE_KING;
	case 'k':
		return Piece::BLACK_KING;
	case 'Q':
		return Piece::WHITE_QUEEN;
	case 'q':
		return Piece::BLACK_QUEEN;
	case 'B':
		return Piece::WHITE_BISHOP;
	case 'b':
		return Piece::BLACK_BISHOP;
	case 'N':
		return Piece::WHITE_KNIGHT;
	case 'n':
		return Piece::BLACK_KNIGHT;
	case 'R':
		return Piece::WHITE_ROOK;
	case 'r':
		return Piece::BLACK_ROOK;
	case 'P':
		return Piece::WHITE_PAWN;
	case 'p':
		return Piece::BLACK_PAWN;
	case '-':
		return Piece::NO_PIECE;
	default:
		throw "Invalid char passed to fen_to_piece.";
	}

}

/*
 * Sometimes we want to use a Piece as an index into an array.
 * In such cases, use this function.  NO_PIECE is intentionally
 * the last index, so arrays that don't need it can simply have
 * size 14 instead of 15.
 */
inline int piece_index_of(Piece piece){
	switch(piece){
		case Piece::WHITE_KING:
			return 0;
		case Piece::BLACK_KING:
			return 1;
		case Piece::WHITE_QUEEN:
			return 2;
		case Piece::BLACK_QUEEN:
			return 3;
		case Piece::WHITE_BISHOP:
			return 4;
		case Piece::BLACK_BISHOP:
			return 5;
		case Piece::WHITE_KNIGHT:
			return 6;
		case Piece::BLACK_KNIGHT:
			return 7;
		case Piece::WHITE_ROOK:
			return 8;
		case Piece::BLACK_ROOK:
			return 9;
		case Piece::WHITE_PAWN:
			return 10;
		case Piece::BLACK_PAWN:
			return 11;
		case Piece::WHITE_EN_PASSANT:
			return 12;
		case Piece::BLACK_EN_PASSANT:
			return 13;
		case Piece::NO_PIECE:
			return 14;
		default:
			return -1;
	};
}

enum class PieceKind : unsigned char {
	NO_PIECE,
	KING,
	QUEEN,
	BISHOP,
	KNIGHT,
	ROOK,
	PAWN,
	EN_PASSANT
};

inline PieceKind kind_of(const Piece piece){
	switch(piece){
	case Piece::NO_PIECE:
		return PieceKind::NO_PIECE;
	case Piece::WHITE_KING:
		return PieceKind::KING;
	case Piece::BLACK_KING:
		return PieceKind::KING;
	case Piece::WHITE_QUEEN:
		return PieceKind::QUEEN;
	case Piece::BLACK_QUEEN:
		return PieceKind::QUEEN;
	case Piece::WHITE_BISHOP:
		return PieceKind::BISHOP;
	case Piece::BLACK_BISHOP:
		return PieceKind::BISHOP;
	case Piece::WHITE_KNIGHT:
		return PieceKind::KNIGHT;
	case Piece::BLACK_KNIGHT:
		return PieceKind::KNIGHT;
	case Piece::WHITE_ROOK:
		return PieceKind::ROOK;
	case Piece::BLACK_ROOK:
		return PieceKind::ROOK;
	case Piece::WHITE_PAWN:
		return PieceKind::PAWN;
	case Piece::BLACK_PAWN:
		return PieceKind::PAWN;
	case Piece::WHITE_EN_PASSANT:
		return PieceKind::EN_PASSANT;
	case Piece::BLACK_EN_PASSANT:
		return PieceKind::EN_PASSANT;
	default:
		/*
		 * This default case will never be reached, since all possibilities
		 * are enumerated above, but CDT was complaining.
		 */
		return PieceKind::NO_PIECE;
	}
}

enum class Color : unsigned char {
	NO_COLOR,
	WHITE,
	BLACK
};

inline Color color_of(const Piece piece){
	switch(piece){
	case Piece::NO_PIECE:
		return Color::NO_COLOR;
	case Piece::WHITE_KING:
		return Color::WHITE;
	case Piece::BLACK_KING:
		return Color::BLACK;
	case Piece::WHITE_QUEEN:
		return Color::WHITE;
	case Piece::BLACK_QUEEN:
		return Color::BLACK;
	case Piece::WHITE_BISHOP:
		return Color::WHITE;
	case Piece::BLACK_BISHOP:
		return Color::BLACK;
	case Piece::WHITE_KNIGHT:
		return Color::WHITE;
	case Piece::BLACK_KNIGHT:
		return Color::BLACK;
	case Piece::WHITE_ROOK:
		return Color::WHITE;
	case Piece::BLACK_ROOK:
		return Color::BLACK;
	case Piece::WHITE_PAWN:
		return Color::WHITE;
	case Piece::BLACK_PAWN:
		return Color::BLACK;
	case Piece::WHITE_EN_PASSANT:
		return Color::WHITE;
	case Piece::BLACK_EN_PASSANT:
		return Color::BLACK;
	default:
		/*
		 * This default case will never be reached, since all possibilities
		 * are enumerated above, but CDT was complaining.
		 */
		return Color::NO_COLOR;
	}
}

//extern const BitBoard kEmpty;
//extern const BitBoard kFull;
//extern const BitBoard kSquares[64];

constexpr BitBoard kEmpty = 0x0000000000000000ULL;
constexpr BitBoard kFull = 0xFFFFFFFFFFFFFFFFULL;
constexpr BitBoard kSquare0 = BitBoard(0x1ULL << 0);
constexpr BitBoard kSquare1 = BitBoard(0x1ULL << 1);
constexpr BitBoard kSquare2 = BitBoard(0x1ULL << 2);
constexpr BitBoard kSquare3 = BitBoard(0x1ULL << 3);
constexpr BitBoard kSquare4 = BitBoard(0x1ULL << 4);
constexpr BitBoard kSquare5 = BitBoard(0x1ULL << 5);
constexpr BitBoard kSquare6 = BitBoard(0x1ULL << 6);
constexpr BitBoard kSquare7 = BitBoard(0x1ULL << 7);
constexpr BitBoard kSquare8 = BitBoard(0x1ULL << 8);
constexpr BitBoard kSquare9 = BitBoard(0x1ULL << 9);
constexpr BitBoard kSquare10 = BitBoard(0x1ULL << 10);
constexpr BitBoard kSquare11 = BitBoard(0x1ULL << 11);
constexpr BitBoard kSquare12 = BitBoard(0x1ULL << 12);
constexpr BitBoard kSquare13 = BitBoard(0x1ULL << 13);
constexpr BitBoard kSquare14 = BitBoard(0x1ULL << 14);
constexpr BitBoard kSquare15 = BitBoard(0x1ULL << 15);
constexpr BitBoard kSquare16 = BitBoard(0x1ULL << 16);
constexpr BitBoard kSquare17 = BitBoard(0x1ULL << 17);
constexpr BitBoard kSquare18 = BitBoard(0x1ULL << 18);
constexpr BitBoard kSquare19 = BitBoard(0x1ULL << 19);
constexpr BitBoard kSquare20 = BitBoard(0x1ULL << 20);
constexpr BitBoard kSquare21 = BitBoard(0x1ULL << 21);
constexpr BitBoard kSquare22 = BitBoard(0x1ULL << 22);
constexpr BitBoard kSquare23 = BitBoard(0x1ULL << 23);
constexpr BitBoard kSquare24 = BitBoard(0x1ULL << 24);
constexpr BitBoard kSquare25 = BitBoard(0x1ULL << 25);
constexpr BitBoard kSquare26 = BitBoard(0x1ULL << 26);
constexpr BitBoard kSquare27 = BitBoard(0x1ULL << 27);
constexpr BitBoard kSquare28 = BitBoard(0x1ULL << 28);
constexpr BitBoard kSquare29 = BitBoard(0x1ULL << 29);
constexpr BitBoard kSquare30 = BitBoard(0x1ULL << 30);
constexpr BitBoard kSquare31 = BitBoard(0x1ULL << 31);
constexpr BitBoard kSquare32 = BitBoard(0x1ULL << 32);
constexpr BitBoard kSquare33 = BitBoard(0x1ULL << 33);
constexpr BitBoard kSquare34 = BitBoard(0x1ULL << 34);
constexpr BitBoard kSquare35 = BitBoard(0x1ULL << 35);
constexpr BitBoard kSquare36 = BitBoard(0x1ULL << 36);
constexpr BitBoard kSquare37 = BitBoard(0x1ULL << 37);
constexpr BitBoard kSquare38 = BitBoard(0x1ULL << 38);
constexpr BitBoard kSquare39 = BitBoard(0x1ULL << 39);
constexpr BitBoard kSquare40 = BitBoard(0x1ULL << 40);
constexpr BitBoard kSquare41 = BitBoard(0x1ULL << 41);
constexpr BitBoard kSquare42 = BitBoard(0x1ULL << 42);
constexpr BitBoard kSquare43 = BitBoard(0x1ULL << 43);
constexpr BitBoard kSquare44 = BitBoard(0x1ULL << 44);
constexpr BitBoard kSquare45 = BitBoard(0x1ULL << 45);
constexpr BitBoard kSquare46 = BitBoard(0x1ULL << 46);
constexpr BitBoard kSquare47 = BitBoard(0x1ULL << 47);
constexpr BitBoard kSquare48 = BitBoard(0x1ULL << 48);
constexpr BitBoard kSquare49 = BitBoard(0x1ULL << 49);
constexpr BitBoard kSquare50 = BitBoard(0x1ULL << 50);
constexpr BitBoard kSquare51 = BitBoard(0x1ULL << 51);
constexpr BitBoard kSquare52 = BitBoard(0x1ULL << 52);
constexpr BitBoard kSquare53 = BitBoard(0x1ULL << 53);
constexpr BitBoard kSquare54 = BitBoard(0x1ULL << 54);
constexpr BitBoard kSquare55 = BitBoard(0x1ULL << 55);
constexpr BitBoard kSquare56 = BitBoard(0x1ULL << 56);
constexpr BitBoard kSquare57 = BitBoard(0x1ULL << 57);
constexpr BitBoard kSquare58 = BitBoard(0x1ULL << 58);
constexpr BitBoard kSquare59 = BitBoard(0x1ULL << 59);
constexpr BitBoard kSquare60 = BitBoard(0x1ULL << 60);
constexpr BitBoard kSquare61 = BitBoard(0x1ULL << 61);
constexpr BitBoard kSquare62 = BitBoard(0x1ULL << 62);
constexpr BitBoard kSquare63 = BitBoard(0x1ULL << 63);
constexpr BitBoard kSquares[64] = {kSquare0, kSquare1, kSquare2, kSquare3, kSquare4,
		kSquare5, kSquare6, kSquare7, kSquare8, kSquare9, kSquare10, kSquare11,
		kSquare12, kSquare13, kSquare14, kSquare15, kSquare16, kSquare17, kSquare18,
		kSquare19, kSquare20, kSquare21, kSquare22, kSquare23, kSquare24, kSquare25,
		kSquare26, kSquare27, kSquare28, kSquare29, kSquare30, kSquare31, kSquare32,
		kSquare33, kSquare34, kSquare35, kSquare36, kSquare37, kSquare38, kSquare39,
		kSquare40, kSquare41, kSquare42, kSquare43, kSquare44, kSquare45, kSquare46,
		kSquare47, kSquare48, kSquare49, kSquare50, kSquare51, kSquare52, kSquare53,
		kSquare54, kSquare55, kSquare56, kSquare57, kSquare58, kSquare59, kSquare60,
		kSquare61, kSquare62, kSquare63};


/*
 * Defined here so that kSquares is available.  Not sure if this is
 * the best way to do it.
 */
BitBoard BitBoard::from_square_index(SquareIndex square){
	return kSquares[square];
}

/*
 * Contains the minimum information needed to define the state of the board.
 */
class BoardStateCore{
private:
	/*
	 * BitBoards to store piece sets.  Each of the 64 bits represents a square.  The square contains a piece of
	 * the specified type iff the corresponding bit is 1.
	 */
	BitBoard kings_;
	BitBoard queens_;
	BitBoard bishops_;
	BitBoard knights_;
	BitBoard rooks_;
	BitBoard pawns_;
	BitBoard white_;
	BitBoard black_;
	/*
	 * The constraint that only one square can be the en passant square at any given time
	 * is not enforced.  It will be up to the make and unmake methods to ensure board states
	 * are always legal.
	 */
	BitBoard en_passant_;

	/*
	 * If whites_turn_ is true, it's white's turn.  Otherwise, it's black's turn.
	 */
	bool whites_turn_;

	/*
	 * Booleans to indicate castle rights
	 */
	bool white_castle_king_;
	bool white_castle_queen_;
	bool black_castle_king_;
	bool black_castle_queen_;

	/*
	 * A boolean to indicate whether this is a valid board state. Usually true, but in some cases there may be
	 * exceptions, such as a completely empty board.  In those cases, valid_ will be false.
	 */
	bool valid_;

	/*
	 * Get a reference the BitBoard for a given kind of piece.  Throws an exception
	 * if it receives NO_PIECE or other invalid input.  This function can be called
	 * as an lvalue.
	 */
	BitBoard& get_piece_kind_bit_board(const PieceKind kind){
		switch(kind){
		case PieceKind::KING:
			return kings_;
		case PieceKind::QUEEN:
			return queens_;
		case PieceKind::BISHOP:
			return bishops_;
		case PieceKind::KNIGHT:
			return knights_;
		case PieceKind::ROOK:
			return rooks_;
		case PieceKind::PAWN:
			return pawns_;
		case PieceKind::EN_PASSANT:
			return en_passant_;
		default:
			throw "Invalid PieceKind passed to get_piece_kind_bit_board.";
		}
	}

	/*
	 * Get a reference the BitBoard for a given color of piece.  Throws an exception
	 * if it receives NO_COLOR or other invalid input.  This function can be called
	 * as an lvalue.
	 */
	BitBoard& get_color_bit_board(const Color color){
		switch(color){
		case Color::WHITE:
			return white_;
		case Color::BLACK:
			return black_;
		default:
			throw "Invalid Color passed to get_color_bit_board.";
		}
	}

	/*
	 * Place a piece at each square specified by places, with no
	 * safety checks of any kind.  Caller is responsible for ensuring that
	 * the requested places are unoccupied and the placements are
	 * otherwise legal.
	 */
	void raw_place_pieces(const Piece piece, const BitBoard places){
		const Color color = color_of(piece);
		const PieceKind kind = kind_of(piece);
		get_color_bit_board(color) |= places;
		get_piece_kind_bit_board(kind) |= places;
	}

	/*
	 * Place a piece at the square specified by square, with no
	 * safety checks of any kind.  Caller is responsible for ensuring that
	 * the requested square is unoccupied and the placement is otherwise legal.
	 */
	void raw_place_piece(const Piece piece, const SquareIndex square){
		raw_place_pieces(piece, BitBoard::from_square_index(square));
	}

	/*
	 * Unplace a piece at each square specified by places, with no
	 * safety checks of any kind.  Caller is responsible for ensuring that
	 * the requested places are occupied and the removals are
	 * otherwise legal.
	 */
	void raw_unplace_pieces(const Piece piece, const BitBoard places){
		const BitBoard unplaces = ~places;
		const Color color = color_of(piece);
		const PieceKind kind = kind_of(piece);
		get_color_bit_board(color) &= unplaces;
		get_piece_kind_bit_board(kind) &= unplaces;
	}

	/*
	 * Unplace a piece at the square specified by square, with no
	 * safety checks of any kind.  Caller is responsible for ensuring that
	 * the requested square is occupied and the removal is otherwise legal.
	 */
	void raw_unplace_piece(const Piece piece, const SquareIndex square){
		raw_unplace_pieces(piece, BitBoard::from_square_index(square));
	}

public:
	// The BoardState, of which BoardStateCore forms the core,
	// should have access to private members.
	friend class BoardState;


	/*
	 * Create BoardStateCore from components.
	 */
//	BoardStateCore(const BitBoard kings, const BitBoard queens, const BitBoard bishops, const BitBoard knights,
//			const BitBoard rooks, const BitBoard pawns, const BitBoard white, const BitBoard black, const BitBoard en_passant,
//			const bool whites_turn, const bool white_castle_king, const bool white_castle_queen, const bool black_castle_king,
//			const bool black_castle_queen, const bool valid){
//		kings_ = kings;
//		queens_ = queens;
//		bishops_ = bishops;
//		knights_ = knights;
//		rooks_ = rooks;
//		pawns_ = pawns;
//		white_ = white;
//		black_ = black;
//		en_passant_ = en_passant;
//		whites_turn_ = whites_turn;
//		white_castle_king_ = white_castle_king;
//		white_castle_queen_ = white_castle_queen;
//		black_castle_king_ = black_castle_king;
//		black_castle_queen_ = black_castle_queen;
//		valid_ = valid;
//	}

	/*
	 * Create an empty board.
	 */
	BoardStateCore(){
		kings_ = kEmpty;
		queens_ = kEmpty;
		bishops_ = kEmpty;
		knights_ = kEmpty;
		rooks_ = kEmpty;
		pawns_ = kEmpty;
		white_ = kEmpty;
		black_ = kEmpty;
		en_passant_ = kNoEnPassant;
		whites_turn_ = false;
		white_castle_king_ = false;
		white_castle_queen_ = false;
		black_castle_king_ = false;
		black_castle_queen_ = false;
		valid_ = false;
	}

	/*
	 * BoardStateCore can be copied.
	 */
	BoardStateCore(const BoardStateCore& rhs) = default;
	BoardStateCore& operator=(const BoardStateCore& rhs) = default;




};

/*
 * A record of a previous board state along with its hash value, used
 * for detecting repetition.
 */
class RecordEntry{
private:
	ZobristKey hash_;
	BoardStateCore state_;
public:
	/*
	 * RecordEntry can be copied.
	 */
	RecordEntry(const RecordEntry& rhs) = default;
	RecordEntry& operator=(const RecordEntry& rhs) = default;

	RecordEntry(const ZobristKey hash, const BoardStateCore state){
		hash_ = hash;
		state_ = state;
	}
};

struct Move{
//private:
	SquareIndex from_square;
	SquareIndex to_square;
	Piece promotion;
//public:
//	/*
//	 * Move can be copied.
//	 */
//	Move(const Move& rhs) = default;
//	Move& operator=(const Move& rhs) = default;
//
//	Move(const SquareIndex from, const SquareIndex to, const Piece promotion){
//		from_square_ = from;
//		to_square_ = to;
//		promotion_ = promotion;
//	}
};

constexpr Move kNoMove = {0, 0, Piece::NO_PIECE};

/*
 * A MoveRecord contains all the information needed to undo a move and to
 * compute the change in Zobrist hash value for a move.
 */
struct MoveRecord{
	// The square the primary moving piece started in.  If the move was a castle,
	// this will be the king.
	SquareIndex from_square;

	// The square the primary moving piece ended in.  If the move was a promotion,
	// this is the square the promoted piece ended in.
	SquareIndex to_square;

	// The square the captured piece occupied.  This is only meaningful if
	// captured_piece is not NO_PIECE.  In the case of an en passant capture,
	// captured_square will be different from to_square.
	SquareIndex captured_square;

	// In the case of a castle, castled_piece will be a WHITE_ROOK or BLACK_ROOK,
	// and castled_from_square and castled_to_square will indicate it's position
	// before and after castling, respectively.
	SquareIndex castled_from_square;
	SquareIndex castled_to_square;

	// The piece that moved.  In case of castling, this is the king rather than the
	// rook.
	Piece moved_piece;

	// The piece that was placed after moving.  In the case of promotion, this will
	// not be the same as the moved_piece.  In all other cases, they will be the same.
	Piece placed_piece;

	// The piece that was captured.  This will be NO_PIECE if no capture was made.
	Piece captured_piece;

	// In case of castling, this will be the rook involved in the castling.
	// It will always be either WHITE_ROOK or BLACK_ROOK or NO_PIECE
	Piece castled_piece;

	// The following four members record all changes in castle rights
	// (which can only be lost, never gained) resulting from this move.
	bool lost_white_castle_king;
	bool lost_white_castle_queen;
	bool lost_black_castle_king;
	bool lost_black_castle_queen;

	// The square of the available en passant capture before the move.  If
	// no en passant capture was available, en_passant_piece_before will be
	// NO_PIECE.
	SquareIndex en_passant_square_before;

	// The square of the available en passant capture after the move.  If
	// no en passant capture is available, en_passant_piece_after will be
	// NO_PIECE.
	SquareIndex en_passant_square_after;

	// WHITE_EN_PASSANT or BLACK_EN_PASSANT if en passant was available before
	// and after the move, respectively. Set to NO_PIECE if no en passant capture
	// is available.
	Piece en_passant_piece_before;
	Piece en_passant_piece_after;

	// The values of the various forced draw clocks before and after the move.
	unsigned int halfmove_clock_before;
	unsigned int threefold_repetition_clock_before;
	unsigned int halfmove_clock_after;
	unsigned int threefold_repetition_clock_after;
};

//public:
//	/*
//	 * MoveRecord can be copied.
//	 */
//	MoveRecord(const MoveRecord& rhs) = default;
//	MoveRecord& operator=(const MoveRecord& rhs) = default;
//
//	MoveRecord(const SquareIndex from_square, const SquareIndex to_square,
//			const Piece moved_piece, const Piece placed_piece,
//			const Piece captured_piece,
//			const unsigned int previous_halfmove_clock,
//			const unsigned int previous_threefold_repetition_clock,
//			const SquareIndex previous_en_passant, const bool lost_white_castle_king,
//			const bool lost_white_castle_queen, const bool lost_black_castle_king,
//			const bool lost_black_castle_queen){
//		from_square_ = from_square;
//		to_square_ = to_square;
//		captured_piece_ = captured_piece;
//		moved_piece_ = moved_piece;
//		placed_piece_ = placed_piece;
//		previous_halfmove_clock_ = previous_halfmove_clock;
//		previous_threefold_repetition_clock_ = previous_threefold_repetition_clock;
//		previous_en_passant_ = previous_en_passant;
//		lost_white_castle_king_ = lost_white_castle_king;
//		lost_white_castle_queen_ = lost_white_castle_queen;
//		lost_black_castle_king_ = lost_black_castle_king;
//		lost_black_castle_queen_ = lost_black_castle_queen;
//
//	}
//
//};

void split_string(const std::string& string_to_split,
		const std::string& delimiter, std::vector<std::string>& output){
	std::string::size_type start_position = 0;
	std::string::size_type end_position;
	std::string::size_type delimiter_size = delimiter.size();
	std::string::size_type token_size;
	std::string::size_type string_to_split_size = string_to_split.size();
	while(start_position < string_to_split_size){
		end_position = string_to_split.find(delimiter, start_position);
		if(end_position == std::string::npos){
			end_position = string_to_split_size;
		}
		token_size = end_position - start_position;
		if(token_size > 0){
			output.push_back(string_to_split.substr(start_position, token_size));
		}
		start_position = end_position + delimiter_size;
	}
}

std::vector<std::string> split_string(const std::string& string_to_split,
		const std::string& delimiter){
	std::vector<std::string> result = std::vector<std::string>();
	split_string(string_to_split, delimiter, result);
	return result;
}



/*
 * A BoardStateCore with some additional redundant information used in search and move generation.
 */
class BoardState{
private:
	BoardStateCore core_;
	unsigned int halfmove_clock_;
	unsigned int fullmove_counter_;
	unsigned int halfmove_counter_;
	unsigned int threefold_repetition_clock_;
	std::array<Piece, kSquaresPerBoard> piece_map_;
	ZobristKey hash_;
	std::vector<RecordEntry> record_;
	std::array<SquareIndex, kSquaresPerBoard> available_moves_;
	SquareIndex en_passant_square_;

	/*
	 * Break the rank_string into tokens.  It is assumed that rank_string
	 * represents a single rank from a FEN string.  The tokens will be either
	 * letters representing pieces or numbers representing empty squares.
	 */
	inline static std::vector<std::string> tokenize_fen_rank(std::string rank_string){
		std::vector<std::string> result;
		std::string piece_token;
		std::string number_token;
		for(char character : rank_string){
			if(!isdigit(character)){
				piece_token += character;
				if(number_token.size() > 0){
					result.push_back(number_token);
					number_token = "";
				}
				result.push_back(piece_token);
				piece_token = "";
			}else{
				number_token += character;
			}
		}
		if(number_token.size() > 0){
			result.push_back(number_token);
		}
		return result;
 	}

	/*
	 * Place a piece at square, with no safety checks of any kind.
	 * It is up to the caller to ensure that the desired square is
	 * empty and that the placement is otherwise legal.  The
	 * available_moves_, en_passant_square_, and hash_ will not be updated.
	 * The caller must update them after the call to ensure a
	 * consistent BoardState.
	 */
	inline void raw_place_piece(Piece piece, SquareIndex square){
		core_.raw_place_piece(piece, square);
		piece_map_[square] = piece;
	}

	/*
	 * Set the en passant square, with no safety checks of any kind.  It's
	 * up to the caller to ensure the desired square is a valid en passant
	 * square.  The available_moves_ and hash_ will not be updated.  The
	 * caller must update them after the call to ensure a consistent
	 * BoardState.
	 */
	inline void raw_set_en_passant(bool whites_turn, SquareIndex square){
		Piece piece;
		if(whites_turn){
			piece = Piece::WHITE_EN_PASSANT;
		}else{
			piece = Piece::BLACK_EN_PASSANT;
		}
		raw_place_piece(piece, square);
		en_passant_square_ = square;
	}

public:
	/*
	 * We should not be copying BoardStates casually.  By deleting the copy
	 * constructors, accidental attempts to copy will be thwarted at compile
	 * time.
	 */
	BoardState(const BoardState& rhs) = delete;
	BoardState& operator=(const BoardState& rhs) = delete;

	/*
	 * We do want to be able to move BoardStates, because we need to in order to
	 * return one from a named constructor (from_fen).
	 */
	BoardState(BoardState&& rhs) = default;

	BoardState(){
//		core_ = BoardStateCore();
		halfmove_clock_ = 0;
		fullmove_counter_ = 0;
		halfmove_counter_ = 0;
		threefold_repetition_clock_ = 0;
		// The piece_map_ is initialized empty, to match the default initialization
		// of core_.
		piece_map_ = {Piece::NO_PIECE};
		hash_ = 0;
//		record_ = std::vector<RecordEntry>();
		// Avoid allocation during game by reserving a bunch of memory now
		record_.reserve(10000);
//		available_moves_ = std::array<SquareIndex, kSquaresPerBoard>() = {kEmpty};
		en_passant_square_ = kNoEnPassant;
	}

//	BoardState(const BoardStateCore core, const unsigned int halfmove_clock,
//			const unsigned int fullmove_counter
//			){
//		core_ = core;
//		halfmove_clock_ = halfmove_clock;
//		fullmove_counter_ = fullmove_counter;
//		halfmove_counter_ = 0;
//		threefold_repetition_clock_ = 0;
//		piece_map_ = std::array<Piece, 64>();
//		hash_ = ZobristHasher::hash(core_);
//		record_ = std::vector<RecordEntry>();
//		// Avoid allocation during game by reserving a bunch of memory now
//		record_.reserve(10000);
//		available_moves_ = std::array<SquareIndex, 64>();
//
//	}

	/*
	 * Get the piece located at square
	 */
	Piece get_piece_at(const SquareIndex square) const{
		return piece_map_[square];
	}

	/*
	 * Getters and setters
	 */
	inline bool get_whites_turn() const{
		return core_.whites_turn_;
	}
	inline void set_whites_turn(const bool value){
		core_.whites_turn_ = value;
	}


	inline bool get_white_castle_king() const{
		return core_.white_castle_king_;
	}
	inline void set_white_castle_king(const bool value){
		core_.white_castle_king_ = value;
	}

	inline bool get_white_castle_queen() const{
		return core_.white_castle_queen_;
	}
	inline void set_white_castle_queen(const bool value){
		core_.white_castle_queen_ = value;
	}

	inline bool get_black_castle_king() const{
		return core_.black_castle_king_;
	}
	inline void set_black_castle_king(const bool value){
		core_.black_castle_king_ = value;
	}

	inline bool get_black_castle_queen() const{
		return core_.black_castle_queen_;
	}
	inline void set_black_castle_queen(const bool value){
		core_.black_castle_queen_ = value;
	}

	/*
	 * Compute the available moves based on current state and
	 * set available_moves_.
	 */
	inline void compute_available_moves(){
		// TODO: This
	}


	/*
	 * Create a BoardState from a FEN formatted string. This function
	 * doesn't need to be fast.
	 */
	static BoardState from_fen(std::string fen);
//	static BoardState from_fen(std::string fen){
//		// Start with a blank BoardState, which will be filled in
//		// later.
//		BoardState result;
//
//		// Break the FEN string into parts.
//		std::vector<std::string> parts = split_string(fen, " ");
//		if(parts.size() < 5 || parts.size() > 6){
//			throw "Improperly formated string passed to BoardState::from_fen.";
//		}
//		std::string board_part = parts[0];
//		std::string turn_part = parts[1];
//		std::string castle_rights_part;
//		if(parts.size() == 6){
//			castle_rights_part = parts[2];
//		}
//		std::string en_passant_part = parts[parts.size() == 6?3:2];
//		std::string halfmove_clock_part = parts[parts.size() == 6?4:3];
//		std::string fullmove_counter_part = parts[parts.size() == 6?5:4];
//
//		// Break the board part of the FEN string into ranks.
//		std::vector<std::string> rank_parts = split_string(board_part, "/");
//		if(rank_parts.size() != kRanksPerBoard){
//			throw "Improperly formated string passed to BoardState::from_fen.";
//		}
//
//		// Place all the pieces in each rank.
//		std::string rank_string;
//		std::vector<std::string> rank_tokens;
//		SquareIndex file_index;
//		for(SquareIndex rank_index=0; rank_index<kRanksPerBoard; rank_index++){
//			rank_string = rank_parts[rank_index];
//			rank_tokens = tokenize_fen_rank(rank_string);
//			file_index = 0;
//			for(std::string token : rank_tokens){
//				if(isdigit(token[0])){
//					file_index += stoi(token);
//				}else{
//					if(token.size() != 1){
//						throw "Improperly formated string passed to BoardState::from_fen.";
//					}
//					result.raw_place_piece(fen_to_piece(token[0]),
//							square_index_of(rank_index, file_index));
//					file_index += 1;
//				}
//				if(file_index >= kFilesPerBoard){
//					throw "Improperly formated string passed to BoardState::from_fen.";
//				}
//			}
//		}
//		// All pieces have been placed with the possible exception of en passant.
//
//		// Set whose turn it is
//		if(turn_part.size() != 1){
//			throw "Improperly formated string passed to BoardState::from_fen.";
//		}
//		if(turn_part[0] == 'w'){
//			result.set_whites_turn(true);
//		}else{
//			result.set_whites_turn(false);
//		}
//
//		// Set castle rights
//		if(castle_rights_part.size() > 4){
//			throw "Improperly formated string passed to BoardState::from_fen.";
//		}
//		for(char castle_right : castle_rights_part){
//			switch(castle_right){
//			case 'K':
//				result.set_white_castle_king(true);
//				break;
//			case 'Q':
//				result.set_white_castle_queen(true);
//				break;
//			case 'k':
//				result.set_black_castle_king(true);
//				break;
//			case 'q':
//				result.set_black_castle_queen(true);
//				break;
//			default:
//				throw "Improperly formated string passed to BoardState::from_fen.";
//			}
//		}
//
//		// Set en passant
//		if(en_passant_part.size() > 0){
//			result.raw_set_en_passant(result.core_.whites_turn_, algebraic_to_square_index(en_passant_part));
//		}
//
//		// Set the clocks
//		result.halfmove_clock_ = stoi(halfmove_clock_part);
//		result.fullmove_counter_ = stoi(fullmove_counter_part);
//
//		// Calculate the current Zobrist hash
//		result.hash_ = ZobristHasher::hash(result);
//
//		// Calculate available moves
//		result.compute_available_moves();
//
//		return result;
//	}

	/*
	 * Return a FEN formatted string based on the BoardState. This
	 * function doesn't need to be fast.
	 */
	std::string to_fen() const{
		std::string board_part;
		Piece piece;
		char piece_char;
		int empty_count;
		SquareIndex negative_rank, rank, file;

		// Iterate over the piece_map_ by rank and file,
		// constructing the corresponding part of the FEN
		// string.
		for(negative_rank=0; negative_rank<kRanksPerBoard; negative_rank++){
			// Weird ordering here because the FEN format does not use the
			// same ordering of ranks as our coordinate system.
			rank = kRanksPerBoard - negative_rank - 1;
			empty_count = 0;
			for(file=0; file<kFilesPerBoard; file++){
				SquareIndex square = square_index_of(rank, file);
				piece = piece_map_[square];
				piece_char = piece_to_fen(piece);
				if(piece_char == '-' || piece_char == '*'){
					empty_count++;
				}else{
					if(empty_count > 0){
						board_part += std::to_string(empty_count);
						empty_count = 0;
					}
					board_part += piece_char;
				}
			}
			if(empty_count > 0){
				board_part += std::to_string(empty_count);
				empty_count = 0;
			}
			if(rank > 0){
				board_part += '/';
			}
		}

		// Construct the part that specifies whose turn it is.
		std::string turn_part;
		if(core_.whites_turn_){
			turn_part += "w";
		}else{
			turn_part += "b";
		}

		// Construct the part that specifies castle rights.
		std::string castle_rights_part;
		if(core_.white_castle_king_){
			castle_rights_part += 'K';
		}
		if(core_.white_castle_queen_){
			castle_rights_part += 'Q';
		}
		if(core_.black_castle_king_){
			castle_rights_part += 'k';
		}
		if(core_.black_castle_queen_){
			castle_rights_part += 'q';
		}

		// Construct the part that specifies the available en
		// passant.
		std::string en_passant_part;
		if(en_passant_square_ != kNoEnPassant){
			en_passant_part += square_index_to_algebraic(en_passant_square_);
		}else{
			en_passant_part += '-';
		}

		// Construct the clock part.
		std::string clock_part;
		clock_part += std::to_string(halfmove_clock_);
		clock_part += ' ';
		clock_part += std::to_string(fullmove_counter_);

		// Put it all together.
		std::string result;
		result += board_part;
		result += ' ';
		result += turn_part;
		if(castle_rights_part != ""){
			result += ' ';
			result += castle_rights_part;
		}
		result += ' ';
		result += en_passant_part;
		result += ' ';
		result += clock_part;

		return result;
	}

};


/*
 * ZobristHasher implements Zobrist hashing and updating.
 */
class ZobristHasher{
private:

	static constexpr unsigned int kNumberOfPieceTypes = 14;
	static constexpr ZobristKey kZobristWhiteCastleKing = 0x4813FCAA66F292EDULL;
	static constexpr ZobristKey kZobristWhiteCastleQueen = 0xA8B3B3DEEFBFFA3DULL;
	static constexpr ZobristKey kZobristBlackCastleKing = 0xC0426A0CE5346059ULL;
	static constexpr ZobristKey kZobristBlackCastleQueen = 0x4B15253AB6FE7D5ULL;
	static constexpr ZobristKey kZobristWhitesTurn = 0x17297DB8E6145787ULL;
	static constexpr std::array<ZobristKey, kSquaresPerBoard*kNumberOfPieceTypes> zobrist_table_ = { 0x91B7584A2265B1F5ULL,
					0xCD613E30D8F16ADFULL, 0x1027C4D1C386BBC4ULL,
					0x1E2FEB89414C343CULL, 0xC2CE6F447ED4D57BULL,
					0x78E510617311D8A3ULL, 0x612E7696A6CECC1BULL,
					0x35BF992DC9E9C616ULL, 0x7CE42C8218072E8CULL,
					0xE4B06CE60741C7A8ULL, 0x63CA828DD5F4B3B2ULL,
					0x9B810E766EC9D286ULL, 0xC4647159C324C985ULL,
					0xB2221A58008A05A6ULL, 0x442E3D437204E52DULL,
					0xCD447E35B8B6D8FEULL, 0x9755D4C13A902931ULL,
					0x1A2B8F1FF1FD42A2ULL, 0x51431193E6C3F339ULL,
					0x5B6E6E307D4BEDCULL, 0xA648A7DD06839EB9ULL,
					0x25B413F8A9A021EULL, 0xE1988AD9F06C144AULL,
					0xAFBD67F9619699CFULL, 0xF8130C4237730EDFULL,
					0xB9D179E06C0FD4F5ULL, 0x8712B8BC076F3787ULL,
					0xC381E88F38C0C8FDULL, 0xF06D3FEF701966A0ULL,
					0x8D88348A7EED8D14ULL, 0x587FD2803BAB6C39ULL,
					0xAD45F23D3B1A11DFULL, 0xC2CD789A380208A9ULL,
					0xF3C64AF775A89294ULL, 0xED2F89D94A2F20AAULL,
					0x6A8AC4BA05805975ULL, 0xEA90A8F0D66B829EULL,
					0xEC148CB48E73CA47ULL, 0x19999E3FA46D6753ULL,
					0xA11D459A2F978D87ULL, 0xB94067EDFE175330ULL,
					0x4BE03DB0DC2574BDULL, 0xBE3EDC0A1EF2A4F0ULL,
					0xE5446DD4552B82F6ULL, 0xF9270F4EB8B333A8ULL,
					0x803468B6B610A9F7ULL, 0xF79B17AEEFBA91FCULL,
					0x81F9C1F66C0F3459ULL, 0xE901E35CD47D380DULL,
					0x3099FDF5AB99254AULL, 0x48BEAB134DA98F1DULL,
					0xF9341C68966BAEA1ULL, 0x7FD63116E1EA24C4ULL,
					0xF0DFB4A5D8A064DFULL, 0x64B2D2BC815A47C5ULL,
					0xDA71144896C8DA19ULL, 0x7AF027BC08D6AF57ULL,
					0xBE6521CC3E2434E3ULL, 0x677F6CBDCC22AF58ULL,
					0xAA2CA1AF6A107B75ULL, 0x5DFBD3D12C4A3698ULL,
					0xE1FAB9D78C7E134FULL, 0xC69D4BD8B3FA7AA7ULL,
					0xBCFBB050ACAB1A6BULL, 0x1622BD795FEC898FULL,
					0xA9EC0806705FCA16ULL, 0x1BA1621582283D15ULL,
					0x29E821A4C74803E3ULL, 0xD707107E855C3844ULL,
					0x5EDA92D864AC5DB9ULL, 0xBB968A437D5C8DFCULL,
					0x78255D6807923986ULL, 0x4EFBC8D60B21FBACULL,
					0xD92A4AA2B410D93CULL, 0x9D643C25FBB230BBULL,
					0x9403560D97DAE38DULL, 0xA5AC06D864C2F2E3ULL,
					0x2B28FEF02B9C014EULL, 0x3A1890C78092B4D4ULL,
					0x326324DFB695FFBULL, 0x33138131C541013DULL,
					0xEB8AC8CE8A245E6BULL, 0x8C5FE8F8DC3BF364ULL,
					0x678A5AA33B6FE507ULL, 0x5804F92283868A29ULL,
					0xD8F33418F3D4E711ULL, 0x5A702CFA93EA5C4EULL,
					0xE8E5B4617589A82BULL, 0xA8C24D4244EF7FEBULL,
					0x9BE3CECB8C497C68ULL, 0xBAB9F87FF5059285ULL,
					0x62397BC701762741ULL, 0xDB610487C89DA11BULL,
					0xF463B337D20B5D59ULL, 0xF03EDCA7E2DCAA37ULL,
					0x83333218BD91A1B7ULL, 0x21167D8FCF23CAE8ULL,
					0xC703806984C81999ULL, 0x349AAE908FB5262CULL,
					0xF320CD576D14475BULL, 0x7B297D0B0E5E18BAULL,
					0x5D5F576CDEB8FC4CULL, 0x8DED3C9691EB79FAULL,
					0xF0E642F43328AD08ULL, 0x69D495DD81355C53ULL,
					0xD037CDFF7C240D49ULL, 0x6A17B9AF5B569643ULL,
					0x67DBA858989008ULL, 0x8A449EBE89D9BF02ULL,
					0xC9546B439F9D0129ULL, 0x54C56C9A9CC9AF4EULL,
					0x99901C0475491BC3ULL, 0xCDF8440407295E42ULL,
					0xA2A7AE1F3AC7652CULL, 0x8CFE5CD12D5DB79BULL,
					0x2E47DC0E959F3A51ULL, 0x1773308CDC6B13ABULL,
					0x8D103ED3CC667E97ULL, 0xD9ED17E3CC0E95EEULL,
					0xEE52BDB6D1020A15ULL, 0x84F3DD6415AF341ULL,
					0xF18DD1EED77C96C0ULL, 0x12093D26AC512B01ULL,
					0xDE3A5DB5154ED512ULL, 0x73F7BA8E0445D656ULL,
					0xC10FAA4003BA33DBULL, 0x47FC816AC16E2284ULL,
					0x44C5B4763FE31D03ULL, 0xCC1B0C3E1C07724EULL,
					0x2F429CE59FF3078FULL, 0x4A5012DC582C18C9ULL,
					0x2ADF559A11CBC288ULL, 0x4155D7EF28DD37EBULL,
					0xF3B37F32870266C4ULL, 0xA81AA40A2B0B8C12ULL,
					0xA5F09E6345DDB87DULL, 0x4B63E0EFB62AC1FEULL,
					0xB3DF44A47467537AULL, 0x7F1A355E526EB523ULL,
					0x1D3B993F79490EABULL, 0x4FDF8E1A060CEA63ULL,
					0x57E54ACC62F5680CULL, 0xCBD3F5E06BC15385ULL,
					0x4227DE213023580CULL, 0x40E2A20A1BD7CE73ULL,
					0xBAEB41A5E65A8149ULL, 0xFA0B85188296F5EAULL,
					0xF72F2BB83586FCA7ULL, 0x6E80FA489B0BCA16ULL,
					0xF9BDDEA5D12982E4ULL, 0x39B21C95055455E8ULL,
					0x65B675CD0492C4F5ULL, 0x90B20BB257E8454ULL,
					0xF5BB9188B80599E9ULL, 0x721754EF2904ACECULL,
					0x819D7CA7B46108CCULL, 0x6D39EB43AD9CEDDEULL,
					0xD50E00978B7199CDULL, 0xFA1B1BF13879399BULL,
					0xA17A4340F9C08FEFULL, 0xB1EEDAFFCC3D5506ULL,
					0x736A947A843FDDA7ULL, 0x861E02EC39235BC0ULL,
					0x7DBF924A6048457ULL, 0xACC66A576518093DULL,
					0xCDAAAC43936AA40CULL, 0xA8EA37F7523D2A54ULL,
					0x6D21F4CDA185CC8EULL, 0xBCC99AE80F0C8A89ULL,
					0x202CC8284C717095ULL, 0x364E433FF7C882F4ULL,
					0xC250A03E023033DULL, 0x121B28004E6F5A94ULL,
					0x1391F9B9DBC799B0ULL, 0xEACC110E4F73FD94ULL,
					0x4C41D9C0F07534FEULL, 0x28804790BE6C6FE9ULL,
					0x909FF4976A8A43EFULL, 0x21615022409A8A78ULL,
					0x8F8B2B83022BC320ULL, 0xD9BC1D97E0F3A7EFULL,
					0x973082D609B4E5D2ULL, 0x37B4000BD1C51F86ULL,
					0xE69BAE29F652D008ULL, 0x75FA6DD891FDE85CULL,
					0xD3F21DCC2BE88B46ULL, 0xDEB0E066DE26E655ULL,
					0xC7AF3626F9495568ULL, 0x9F7A7DAFB43ADC4FULL,
					0x994940E82458CC8ULL, 0x334DE73D60C290D0ULL,
					0x1959B9EF58D07674ULL, 0x92C9357D34ACCD78ULL,
					0xE585552FAC954AB5ULL, 0x976699CC6ED5D1BFULL,
					0x7E0AB2ED31B1C27EULL, 0xF01DBF291ABB8BA3ULL,
					0x63DB01FCAA7C314BULL, 0x810D2E304BCB6B22ULL,
					0x4673B757FF2E341ULL, 0x9CB471A55349DA48ULL,
					0x66FEC086DF229650ULL, 0x4806AA81E65150B5ULL,
					0x282EE0BC04A1BDE4ULL, 0xDB87872D336B1A45ULL,
					0xCFA6CF3E53E6D093ULL, 0x903715C8FCAF4A5AULL,
					0x2298BDB1C85F0D46ULL, 0x6DE2B33B56CEF8ECULL,
					0x443BAAC536891EEBULL, 0x18AE013EACA91679ULL,
					0x611575C2D67393D6ULL, 0x8C31406DEEA3D685ULL,
					0xEA190B2A58068A9DULL, 0xD6730839E1E48557ULL,
					0x88C9DA8AAFE673F6ULL, 0xC49872C67C081BB7ULL,
					0x88534206FC4A447EULL, 0x10B8FE223C116549ULL,
					0xA57AF35B9B81635ULL, 0x220D672B15AD9A9DULL,
					0x2AA3300B2B711343ULL, 0x89C80C4DE9367ED9ULL,
					0x449C4CA23685156BULL, 0x550D40DDC2557035ULL,
					0x8181E84D99A74924ULL, 0x415AC400D7547080ULL,
					0x56BEFA395E3C536CULL, 0x1D296588571CEEEEULL,
					0x3C35612E4A8D15D8ULL, 0xF1A9A658DE0F39A7ULL,
					0xC78FEC459A9E994CULL, 0xB7115C02F44D7E40ULL,
					0x7D2186D3E323CE54ULL, 0x947810D822A608BFULL,
					0xC52F4FBE8D19821FULL, 0x521B18A91AB1C42FULL,
					0x6816DE060A04EF48ULL, 0x6156C4DF12BCCDCBULL,
					0xFDC1786BDDBD358FULL, 0x25B7501AC9C1FFEFULL,
					0x20012170D418F7AFULL, 0x1D5C482557450E65ULL,
					0x96605D959D7CD4F6ULL, 0xED192DA3C82AD589ULL,
					0x139F711060C73494ULL, 0x8CDECE75921EBCE6ULL,
					0x90E32E8239455353ULL, 0xF3C668B114ED2049ULL,
					0x5D698C8B44480030ULL, 0x4BA955F3E4096150ULL,
					0x88C780F6907F9669ULL, 0x1D43D1FFECD1345EULL,
					0xE592067375305DB7ULL, 0x1B943CFC46F57327ULL,
					0xBB662A8C979CB06ULL, 0x4BB57B5CD3E89D32ULL,
					0x9D19EE45032B7328ULL, 0x3B96D91ABA018EAULL,
					0x69DD649317788B95ULL, 0xD37C99611D775B7CULL,
					0xCA357568E2934BF1ULL, 0x301BA9880A3EFB80ULL,
					0xC91752A33D589CABULL, 0x96380ED6FCF7F49DULL,
					0x297A21D76BC78BF5ULL, 0x736EBF511D95389BULL,
					0xAE4ECF4B2AD9A40AULL, 0x28B09A933DCDB856ULL,
					0xD85328B6BE773448ULL, 0x6F62E63A1A5356B5ULL,
					0xF6F62C28E927DB48ULL, 0xCE75F4BA60D6C766ULL,
					0x8AFD2973F8633958ULL, 0xD17F6494E8C2D219ULL,
					0x8CDA80A34B452123ULL, 0xB62C228E40DF7C9AULL,
					0x50806F017A1D556CULL, 0x35263B4519A2105CULL,
					0x51423286A6ECC31FULL, 0x6FAADB10A248CFFULL,
					0xC96FA75802B087F8ULL, 0xECF45CCBFB8A99A2ULL,
					0xB9FAD67E4BA927C3ULL, 0x51FBFCC798B8DA9FULL,
					0x642A357C732902F4ULL, 0x6607B61550332CB8ULL,
					0x106EE2AB101E75EBULL, 0x513DD1A6E9D40F2BULL,
					0x99F86C8DF845AED9ULL, 0x74B31BFBF8449560ULL,
					0x40041E001C823D9EULL, 0xC8FEA5D73716E7EAULL,
					0xC725BD979E289761ULL, 0xE4264C9FFADE312DULL,
					0xDE1BF0CD8AFC5BEEULL, 0x780B25D9B02D3504ULL,
					0x5B177A38A96DFB2CULL, 0x2EE7AF97425375BEULL,
					0x3534CCAE8AA67235ULL, 0x32FFD03D4EAC98D6ULL,
					0x5C47577B3F12D68EULL, 0xD1EA041814D4954EULL,
					0x16E3E38047E1A38BULL, 0xC0D76560FBBE9381ULL,
					0x172A401272A9B8A4ULL, 0x93090287A6EA2981ULL,
					0x56C11669A4BA3161ULL, 0x3A389B09F0D3FA5CULL,
					0xF772F8EA63F666E0ULL, 0xA8266954E896A65ULL,
					0x2FD2F79253C617EBULL, 0xCAF078B051158DE5ULL,
					0x9439C746D8DDD2EFULL, 0xEBDDB098E4BC6E82ULL,
					0x3EEFE7344D84E990ULL, 0x19D7B4035596DFDEULL,
					0x9C842B6A8B525B4FULL, 0xCEBCC1BA943863A5ULL,
					0x179030DA98910052ULL, 0x385C1B333EBEBE3EULL,
					0xCEEA590B05373B76ULL, 0x66DAA3653E67026CULL,
					0x449FD49B12840EA1ULL, 0xDE1827478D1BC13AULL,
					0xBAAAD6511227932FULL, 0x581F255133BB4C2ULL,
					0x289EB06A2A866B4ULL, 0xC02FC22A4A7347FAULL,
					0x5BF3F74DCACC9EC8ULL, 0x780587F07E465B19ULL,
					0xDBEEF77ADCD69029ULL, 0x19D6D73B2778507CULL,
					0xC71A5B11805DB06AULL, 0x53FDF07CCB8409D6ULL,
					0x825F854213BD488EULL, 0xAA4DA822F3009A5CULL,
					0x2DF810B92C599859ULL, 0x2649C1B0C6B5A1C6ULL,
					0x243BD888FC2222D2ULL, 0xDD946658D2511C38ULL,
					0x4E3D4D0F51DD5D5CULL, 0xB59641D21B5C56D3ULL,
					0xD5AE305B83ACFB7EULL, 0x9A15A311EB5AF9F9ULL,
					0x20552F5F4B2220A4ULL, 0x34ECF2EDE4CD6075ULL,
					0x8BA56D3424452ECFULL, 0xB8FE2F4BE91553A9ULL,
					0xC79D444008216B65ULL, 0xD22F02F350E9E079ULL,
					0x9F9F80D0E730CB28ULL, 0xAC153076CDC98666ULL,
					0x8D8E3B13E83B3AB1ULL, 0xF1878D5FD739543BULL,
					0xFCA7CB5FBF05F8FAULL, 0x3497553CB0894F5AULL,
					0x4C8670622D9B8EBFULL, 0x899918A76EC15D38ULL,
					0xC6E5973286BEF29ULL, 0xDCB284F8B6FEBC3AULL,
					0x3F4ED95AAAF38C2FULL, 0xC71C5CF140A980BDULL,
					0xAE9C8563107D72D5ULL, 0x725A9A5BF6A07500ULL,
					0x6E1FB6ADCEE9A4FDULL, 0x400E67ED8C9CF440ULL,
					0x707C70B48A97B9D8ULL, 0x89BE4B4BD9EE50E2ULL,
					0x2C8261B740C1A65ULL, 0xD6172ADF654D479AULL,
					0x2BE893F456B30574ULL, 0x7C5C483D420A4323ULL,
					0xCB06718C063FA2B6ULL, 0xEEC1754CA57D041EULL,
					0xF9EF954E6AABCB78ULL, 0x4D759889213147BULL,
					0xB11379A20FF44F65ULL, 0x947F81435ADD92D1ULL,
					0x97F2A70223669676ULL, 0x237475E120087497ULL,
					0xFBB41D1442553A33ULL, 0x46E3DB95D4350B28ULL,
					0x906704C365D60B6EULL, 0x2C139C1966AD51FDULL,
					0x16D8E80E9CC930D3ULL, 0x7C6A47A73BC8996BULL,
					0x2D75C25D01EA0639ULL, 0x5136BF628758FF4DULL,
					0xE49DF6BB803AF506ULL, 0xEBA1A9D3A61A59E3ULL,
					0xEE1B8CC470358A27ULL, 0xA39CC4B2AFBF5310ULL,
					0x39C97AB1BB3E780FULL, 0x501FC6F43D061F79ULL,
					0xAFDBE9D27EBD0E05ULL, 0xF4DFC9A57A946602ULL,
					0xB67D153D399DAB3CULL, 0x564274036988F668ULL,
					0x9C7D498A8F76DC87ULL, 0xBA6CAC4AE82D2FEFULL,
					0xA745BA6DEAEED19BULL, 0xF8EC2D3446752B5CULL,
					0x382F21E4A57B7700ULL, 0xEBEE35210C56A92DULL,
					0xC360B3B71251310BULL, 0xA5319F4782FE3A4AULL,
					0x5E6279DBE09EDD5AULL, 0x82FA4D7A28D2E08EULL,
					0xCADFF918C41A66D9ULL, 0x342F22BAE20CEA4AULL,
					0x4C78C7AB4FD24206ULL, 0x4CB05EC1B14B69DCULL,
					0x8D64B3ADD9577B6BULL, 0x2A4926F05F221DFCULL,
					0xB386D25CB38742ADULL, 0x76FBB6EDBC85E5DEULL,
					0x15C0CDD59836404CULL, 0x1F8CE97ADB34FA8DULL,
					0x9B29B54BE587DD21ULL, 0x83924F05F5C7B9AAULL,
					0x60900772923C4E5DULL, 0x27E125A42D206ADAULL,
					0x6D3FAD4C40270546ULL, 0xF112CFD037B5DBACULL,
					0xB8378D8291CBE386ULL, 0xC842C19AC1FBE94CULL,
					0x7EBA03520D589A58ULL, 0x64C371CFAE7FBA11ULL,
					0xA310A849B7975B28ULL, 0x624C4B62591550FFULL,
					0xD87064FC83DAB265ULL, 0x8B5230ED2A30363BULL,
					0xFE8B2B79BADA7947ULL, 0x863043D70A6BE26CULL,
					0x1724925FFB314DA0ULL, 0x4153BBC7CED5669FULL,
					0x19DE2DEDA0E20045ULL, 0xBCA5F87B447C999DULL,
					0x156EAB79E9B161F4ULL, 0xF98DDC84F59DC887ULL,
					0xF81F5C80239DC599ULL, 0x9DED54FDC69806EAULL,
					0xF78047CFD788C7CCULL, 0xAFC6EE6FA8E33C94ULL,
					0x14FE7EBCB34DEC74ULL, 0xD9D9320E71EF5E7AULL,
					0x3DB18A28EC9F6FBFULL, 0xD9DB30AFF8A10E70ULL,
					0xF0A3A66861E1E80DULL, 0xE746EBEBCD7E80A2ULL,
					0x65B184F76ED3F30BULL, 0xE8FB46B52A2D551FULL,
					0x702938155351D2C1ULL, 0x9F55C5FC20572AEBULL,
					0x7CEB5FB4E8ACABFFULL, 0x36469FABF59CD100ULL,
					0x6E6716981E830596ULL, 0x88B7CC6B99C61AA8ULL,
					0xE8C7A01D68815FDAULL, 0xA9172A051E3B25E5ULL,
					0x47158A7E4BA44898ULL, 0x60FC47FA3F8B1BAAULL,
					0x8F332483BFE4440EULL, 0xF5B5B9340106BB05ULL,
					0x8742CED2309944E2ULL, 0x943EC25A70536E9BULL,
					0x7E30F1105628748ULL, 0xF91C85FDA0A59518ULL,
					0x3E0363339B0A6817ULL, 0x42A95D35D5D8575DULL,
					0x2C400B9534E41E75ULL, 0x25FE3A1848E772BAULL,
					0x335082DC8AD6C1C4ULL, 0x4FA6961145F21E94ULL,
					0xC1E6415A95F2EE55ULL, 0xD51536644039D142ULL,
					0x72470ADDAEFBA2AEULL, 0xDC7A4BEECA84EBCAULL,
					0xDAE720B2CF03FD21ULL, 0x2B00B570F93EE7CCULL,
					0x5B616E428B9DD3D4ULL, 0x6B82ED5C7DA5AD52ULL,
					0x1F2E490CDB0F0126ULL, 0x357D6F2EC4E199A1ULL,
					0xE1018CC5920F3663ULL, 0x346F3293621D1733ULL,
					0xCF80F75148B75541ULL, 0xE76DB5EF1BAF02CFULL,
					0x62EBC92CEBB898AULL, 0x91BE34EB1E39EF8EULL,
					0x3621F97BF4CC645ULL, 0x4BE1B2488B97EF45ULL,
					0xAC859F8FF706A832ULL, 0xB96CC27AC2D532FAULL,
					0xA63E0C32F93897B0ULL, 0x133F3B0A22F7D343ULL,
					0x5FAC971A80185844ULL, 0xCE33DD7092947D94ULL,
					0x6FEA51CA4FAE2CF5ULL, 0xAD611A3E80C6BCBDULL,
					0xC234472F5B58796AULL, 0x52DD34D68744D3C0ULL,
					0x1FB7F62800375C0DULL, 0xB7CCBA58713B831BULL,
					0x59A78B137315D969ULL, 0x8A0F42834E0751D7ULL,
					0x56E0A246663F423BULL, 0xBB2B92C3C87868FAULL,
					0x92484194AEF4259CULL, 0x1CF3D1797E0750EAULL,
					0xEAF5C033A5CD95E7ULL, 0x61E406A660A7A7B7ULL,
					0x8E903FD93433B60CULL, 0xFEAD3BED00FDFEAEULL,
					0xA2B249AB47122FAAULL, 0xB8E7DF9B992149E8ULL,
					0xBD1296CDE1B4A960ULL, 0xBA7725A3D454F36DULL,
					0x32E9C06982CE49DEULL, 0xEC5DF2C7FCAD3888ULL,
					0x99D026A7762A2BA5ULL, 0x84546026D5A7EB2EULL,
					0xEFFE76E068B1F3C9ULL, 0xB64E172FBEA01CA0ULL,
					0xFCD26DADFCD2CF1EULL, 0xB3F0B94C4E2A89F5ULL,
					0x730B19EC2B999F07ULL, 0xAB3920349EBA8775ULL,
					0x3286423887ECBE86ULL, 0x86B46F015C03151CULL,
					0xADB5555600E6A305ULL, 0x9450085B63A029A5ULL,
					0xF86668C16D05C818ULL, 0x5604C3B667BE9998ULL,
					0x9F22CE0ADC7A9283ULL, 0xF977EDF4959D133DULL,
					0xB312AD6FBBDC55A2ULL, 0xF7ADC0AEE5DD6001ULL,
					0x1157C8B3BFAF9E2FULL, 0xFCD58C0F7E21B8AAULL,
					0x3F64C50CBEEAAC97ULL, 0xF78D9952A3EE54D4ULL,
					0x4A77814EA6142E5BULL, 0x55198C0A1326797ULL,
					0xB8A61715683115A8ULL, 0x27F52FA9A117511FULL,
					0xC76330AFA23C4B27ULL, 0x65B699ECEFE6F675ULL,
					0x452FAC9AC850320AULL, 0x2D9B4F22D8A50636ULL,
					0x12CB2F3FC47ADDC9ULL, 0xC6AD0327D0B93207ULL,
					0x297C0D69AFF956CULL, 0xE9A413CA59758F83ULL,
					0xCC5D375A43BBBA66ULL, 0x6940776CB540CCE4ULL,
					0xAF5E490BDFBAAAFAULL, 0x4DBD3DC98B53C16BULL,
					0x764A44E326EE0EACULL, 0x4264D159D53DDE5EULL,
					0x2B6C57637C0B03EEULL, 0x82A4C12E779409B9ULL,
					0x45547D9D0B9E8D4DULL, 0x193FD24D82A1C54CULL,
					0x9733EF95BEA7C879ULL, 0x11DB6ACF6C2F5ECCULL,
					0x1126D71A5AECE68FULL, 0x714699BDA826E5F1ULL,
					0x2A04FF67050DC58CULL, 0xB5D28DEE81D57930ULL,
					0x29606598F23562B7ULL, 0x17D259ADB0C12C60ULL,
					0xA2CF179F66E47927ULL, 0x469A8A20B05C4A59ULL,
					0x4DED5FAA9AE0E1B9ULL, 0x873116F03579C67EULL,
					0x3CBB5615352C5F80ULL, 0x557D728CE2D28DA8ULL,
					0x118CC43E44E1B856ULL, 0xB2FE7205132BA600ULL,
					0xE90C0722D4A74958ULL, 0xA8A6217585F049FEULL,
					0x77CAB1F95E42E3E0ULL, 0x8EC2361582F2E770ULL,
					0xCBBEAB0BC9A0E0CULL, 0x4C0015082B265442ULL,
					0xBC2E9FF5A72F6600ULL, 0xFF11DC91B6A3CE92ULL,
					0x8E65E4CFD0A410DAULL, 0x5B1916CD450F0864ULL,
					0xBD6679C09C1317A3ULL, 0x647EC1543B6BD0A4ULL,
					0x6653C3B78FA09FA2ULL, 0x7BCEC85D2C1FFACCULL,
					0x427005F6CA2E3611ULL, 0x9C434723DDE138D8ULL,
					0xB74F34105463852DULL, 0x423E96D038E9DE81ULL,
					0x9C25B2DBF6BAD673ULL, 0x3E85B0A9B4E9A806ULL,
					0xA92CD2DED802CB08ULL, 0xDA0DBC7807D11B6BULL,
					0xDE518343E63EA3D6ULL, 0x6710B0E79F5904A6ULL,
					0xED9140C051080DEBULL, 0xEEE133EA6E883110ULL,
					0x3F98E0EEC2F7C23FULL, 0x44E32DBDC910C201ULL,
					0x1291F006309D57EDULL, 0xBB798E9BA03A1915ULL,
					0xDEFD56702A66B259ULL, 0x9442F362F919CB32ULL,
					0x94D8CD47718E3BAFULL, 0xEED4B1F0E9C3DEEEULL,
					0x25EF2114BA6E736CULL, 0xF20AB3059B33D947ULL,
					0x759AAEEE431162A4ULL, 0x299BF22D86CEC133ULL,
					0xC7495DF9237C9540ULL, 0xE4D4AD86235A63D5ULL,
					0x70D07EBAB73B6062ULL, 0x4F4C8DB65C706106ULL,
					0x6697F21EC05A32A3ULL, 0x1DA77D913D90FD27ULL,
					0x34C8D03AB7D9365CULL, 0xAE7024EDB7EE1A9AULL,
					0x117746184E34FA77ULL, 0x3A4548F21B3C137BULL,
					0x524550A465A24E8AULL, 0xEDB924D87E0B6723ULL,
					0xF48FE7D31997E8F3ULL, 0xB83DA502FCF9616ULL,
					0xCF39EFD70E2AF641ULL, 0x5F5E71B98F6A644ULL,
					0xC09F025EE38D62A7ULL, 0xAEECB544377054CFULL,
					0x7E94F5AB08E2FAD3ULL, 0x874E263FB4345622ULL,
					0xB9559250D09DFA6CULL, 0xE31E1292F6D0AC1DULL,
					0x7139BED19CF94BC1ULL, 0xA9B576D757AA5AE1ULL,
					0x464A8296D67E8ECFULL, 0x9CFD717D1E39A54CULL,
					0x2C354A1BB150A78DULL, 0x38D9431F18610C9FULL,
					0x3BB42D9D66531DAFULL, 0x732701337EB9D1C8ULL,
					0xC02823EC60BDADCEULL, 0xF9333F742B2935F2ULL,
					0x3C593E7F3B51D375ULL, 0x489CBAFFD1F559AFULL,
					0x8C09786B766B5E3CULL, 0x63BC6601947678F5ULL,
					0x73A26890363F89C2ULL, 0x42041769B705FBF3ULL,
					0x7F0FAD3B5482909FULL, 0x1C66EED297F7634BULL,
					0x36BEB903E8D424EEULL, 0x142FAB55FE909103ULL,
					0x3F207910BD4F091ULL, 0x1569570CC2534B4ULL,
					0x7AFB6462DB8AE021ULL, 0xE38A59AA51CFA14EULL,
					0xD910DDD76215F679ULL, 0x4986F3A6948B82B1ULL,
					0x322578EBEB391D06ULL, 0x28FA361A6661B877ULL,
					0xD3005630E149A837ULL, 0xA5632A15C23105D9ULL,
					0xCB320DB826FB5E56ULL, 0x7CC0424E9E6ED7CULL,
					0x63243E5303E2E7C4ULL, 0xE055AF1C252A66D8ULL,
					0x8AE63AB1AA311156ULL, 0x909311ED0E9F654FULL,
					0x4111329A61263FDDULL, 0x145B523821464B6DULL,
					0xA6F38E3E767FE953ULL, 0x4DABB96DD708F3A0ULL,
					0x3B27030E7F524F3ULL, 0x89778FB7091489CDULL,
					0x8660194D0F93FB05ULL, 0x21013EEFD733230AULL,
					0xEEF208450AF5E8D2ULL, 0xC7E21846460A02ECULL,
					0x6EB8F85F1E10553BULL, 0x30AB1C2E174E3F4BULL,
					0x7FE9DA2007124B2FULL, 0x215C1C0BA3340D96ULL,
					0x477E4A80BE9F0A63ULL, 0xD12FF4BFAFD03FB9ULL,
					0x312218D0D87ABBFFULL, 0x72904D18A9BB6DCBULL,
					0x546E197B63C3817CULL, 0x4499E3AFA18D58B8ULL,
					0x42850DA8F8375D93ULL, 0xA2B73A66A4401DABULL,
					0x3ED43AB33E3B4290ULL, 0x968240EF0F683985ULL,
					0xC9B8056FEF6709E9ULL, 0x2CDEEC51972AB68BULL,
					0x6DB076BD59805A17ULL, 0xB2B3D2229AF865DFULL,
					0xA36CF2B98F6D0AAAULL, 0xF819B75085AD0C99ULL,
					0xE7B128FD0F90E49CULL, 0x8C0354BE5A6D1EFCULL,
					0x89C08E1C69A36E9AULL, 0xB62E96933309CDB1ULL,
					0x8951D454E14E939AULL, 0xEB7FEC926C931D1AULL,
					0x11F10C60A9921B68ULL, 0x446056BFB6AAFAE5ULL,
					0x9C546496BE47CC7AULL, 0xF8ECAE24B89B02F9ULL,
					0x128137EAC090BC84ULL, 0x2D75C843406797B6ULL,
					0x18B8A008F9F59771ULL, 0xF078F6C26A89353ULL,
					0x340E8462EB2C79D4ULL, 0x6D9814D5DAC504E5ULL,
					0xB7EF083DA277078ULL, 0xA31A7B190D8509DBULL,
					0xE9901243175A1163ULL, 0x83497471D0246CCAULL,
					0x8049E97A781B5120ULL, 0x196A8D845EC8E9D7ULL,
					0x500C48E1FC147A78ULL, 0x206A985A0A452B53ULL,
					0x87EE17B880E180BULL, 0xAA0CB6F5717F5EEDULL,
					0xE539D34D20D1EB7DULL, 0xC36E5359652B0ED7ULL,
					0xE61541B6B528614CULL, 0x723280C3E1DF6F91ULL,
					0xBC937D7E064D7A2FULL, 0x451E07EA8646422CULL,
					0x40008E261722EBBEULL, 0x534E570FCCE695F7ULL,
					0x4D455C7115F6063EULL, 0xDC14F82708C0E4A2ULL,
					0xEE3BDCB625D4DD2ULL, 0x42D15CD3BB8C1409ULL,
					0xBC377F13502E5056ULL, 0x42A305D521480046ULL,
					0x6153AF71CB6915C1ULL, 0x1DFCA10CCE9244CBULL,
					0xAD83C3FBDB19A0BBULL, 0x181437224DC232A6ULL,
					0xD765194F6CC1AEAFULL, 0x80B380113ED1E0EBULL,
					0x3495D62A8EA32F2EULL, 0xEC4C277B5481E736ULL,
					0x8262CDC556B2A3E4ULL, 0x641E9E8DC89B69D3ULL,
					0xE54E1AD1F4CFD336ULL, 0x7B2CCE17958A3855ULL,
					0x21358EE61ACCD407ULL, 0xD08EF562A70F268FULL,
					0x86143E1472D837AFULL, 0x8F036FBEFCEF9215ULL,
					0xD810C3F6B82962A8ULL, 0x94D43EDED5B48AD0ULL,
					0x8523E065B3877F0EULL, 0x7BFAAEA891E53CBULL,
					0xFAD32CAFE595E3CBULL, 0xFDBB37B8D4E4DB03ULL,
					0xBE40F38E4A945554ULL, 0x3331824728333E0EULL,
					0x63A522E35ECF615DULL, 0x53001B63856558B2ULL,
					0x68D52EB618EDE6C3ULL, 0x20599249586AC6E6ULL,
					0x109ADA70932D0488ULL, 0x4CED509A0B27B4C9ULL,
					0xCC88EBD1D0A079F5ULL, 0x889F5E9AA6AF9B40ULL,
					0x6AE70FF2504B60B5ULL, 0x519CD4CC4C5EC38DULL,
					0x45CDA9495A450D23ULL, 0xBFAD326153461EB3ULL,
					0x852571D4BF9E995CULL, 0x2345A9D8045432FULL,
					0x1F327A7486B059DCULL, 0x512E2BEA2614E7E7ULL,
					0xBA0FF0B7EA174C4EULL, 0xC8E2896A5358BF46ULL,
					0x92B7563053DB4391ULL, 0x73AA1107119FE69FULL,
					0x4794AB91FABAB7B5ULL, 0x7442A8CC7ACD7A45ULL,
					0x5D39F1B8E9B2D06AULL, 0xBDDBF0CAED7852CEULL,
					0x616A43DEF841AD26ULL, 0xE3BBA436D0CD14A1ULL,
					0x1402F91CECE9D8EDULL, 0x943735D4EC1B2724ULL,
					0xE5C9BEBCD266EA8ULL, 0xC7950FA2273EA38ULL,
					0x7DFF04AE8611F8B9ULL, 0xDA64B870935AC8D9ULL,
					0x407DBB94FE145171ULL, 0x3ED03C49C8B0DA28ULL,
					0x92E38012B3F2513DULL, 0x56B1B132BF246424ULL,
					0xFD983DF55C905C22ULL, 0xCC025364F13B7619ULL,
					0x5EC127B3A4BC7977ULL, 0x4EB0FF74670F2134ULL,
					0x9927A8FD76EE29AAULL, 0x5727D740FAD13805ULL,
					0x81EE476C88399110ULL, 0x77148A52AF4C782ULL,
					0x400839A925FA97DDULL, 0x389C1CCFAFEF1AC1ULL,
					0x2226FF4390120EA1ULL, 0x1CDDEE9CE8247487ULL,
					0xC42DDDC22F41F7CDULL, 0xF06F217A693E6D5DULL,
					0x9EA4F0BBBA5B99CDULL, 0xCFCD69020CD3AEE8ULL,
					0xFA85459D1966A3BBULL, 0xAE6AC89A8BB3835BULL,
					0xB6F3D08A4406D47FULL, 0x344FEFE11B604336ULL,
					0x11180CD942FE9CA9ULL, 0x923B3BEAA1D3FF82ULL,
					0xA41AAFAC86C0ABFEULL, 0xDAEB22A514185D06ULL,
					0xCB517E6A12A3C54BULL, 0x37A6437BD9C2B0CFULL,
					0xD69871BCA4AB4EECULL, 0x82F01B582C61CBECULL,
					0x6E9D7077DCA1284FULL, 0x9721C6E50597EBC1ULL,
					0xE66743DC5E3C1D96ULL, 0x7C969920D8FE4338ULL,
					0xCEB52FC3B5D4CE45ULL, 0x384DA68248A3FF76ULL,
					0x334C76B8E42B0627ULL, 0x7E5D933D991BA3CEULL,
					0xE61BACEBDD90F85BULL, 0x3C377DA0E48E1B4DULL,
					0x73C2F6F06CE9E732ULL, 0x5DFE36F1ACF424D9ULL,
					0xE9A1A2588B62CCBAULL, 0x3056DDB0F1DA2B29ULL,
					0x7B6EB806CC544333ULL, 0x129C03B0B9CF3DDEULL,
					0xD73ECD63D0646CF9ULL, 0xF9EEF8DBFF876918ULL,
					0x68457E4141ADFE67ULL, 0x21EA0E2338C9127ULL,
					0x883062FABF2D288BULL, 0x6176A3CAC53482ECULL,
					0xE0463F9F83A81A4EULL, 0x138FCC237CB10028ULL,
					0x9DA7FDF2675BB4B3ULL, 0x8293D779E1F86D03ULL,
					0x940A3AEBCBD5DA31ULL, 0x6CF3EEEA95A8303BULL,
					0x5A11494F0A453E8CULL, 0xFCE5D2C6D9E46A51ULL,
					0x1A38311755D3871ULL, 0xF5E4C4BB30942540ULL,
					0xB22CC3474CA27B41ULL, 0xA45F419CB0FB4BC8ULL,
					0x8A70103F0168E969ULL, 0xD265BCD71EBB3EF7ULL,
					0x8332AC334D7AB56DULL, 0xFE725A5EE31EF8FBULL,
					0x50C80450BF323EF2ULL, 0xC6CD35FFF885CE63ULL,
					0xA521DADD8B03EE7CULL, 0xFE692199926C8264ULL,
					0x48525E8A8D2707D7ULL, 0x6953E9E8868F8154ULL,
					0xF08B56528AC32BBDULL, 0xED421259D18DA490ULL,
					0x8492C7B5F40FF922ULL, 0x9A4F17B66886663AULL,
					0x94C05053A14566E6ULL, 0x73D87FD74EC9521CULL,
					0x218586644D49FFCEULL, 0x71B5FF55819E0387ULL,
					0x23EC75979615A32EULL, 0xC5C328968CCC6FF2ULL,
					0xF97C4298FA01208BULL, 0x40B3D0C629B87BAFULL,
					0x27586DAA2FC706BULL, 0x6C96EDFAFE99958FULL,
					0xA95B3B44BC735CA7ULL, 0x947AA9290DF617BULL,
					0x6BBE026B5E4D0C25ULL
			};


	/*
	 * Get the Zobrist number for a particular square and piece.  Used
	 * only for computing Zobrist hash values.
	 */
	static ZobristKey get_table_entry(SquareIndex square, Piece piece){
//		return kZobristWhiteCastleKing;
		return zobrist_table_[kSquaresPerBoard * square + piece_index_of(piece)];
	}

public:

	/*
	 * Compute the Zobrist hash value for the board state.
	 */
	static ZobristKey hash(const BoardState& state){
		Piece piece;
		ZobristKey result = 0;
		for(SquareIndex i=0; i<kSquaresPerBoard; i++){
			piece = state.get_piece_at(i);
			if(piece != Piece::NO_PIECE){
				result ^= get_table_entry(i, piece);
			}
		}
		if(state.get_whites_turn()){
			result ^= kZobristWhitesTurn;
		}
		if(state.get_white_castle_king()){
			result ^= kZobristWhiteCastleKing;
		}
		if(state.get_white_castle_queen()){
			result ^= kZobristWhiteCastleQueen;
		}
		if(state.get_black_castle_king()){
			result ^= kZobristBlackCastleKing;
		}
		if(state.get_black_castle_queen()){
			result ^= kZobristBlackCastleQueen;
		}
		return result;
	}

	/*
	 * Compute the updated Zobrist hash value if a board with
	 * previous_key has just made move_record.  Equivalently, compute the
	 * updated Zobrist hash value if a board with previous_key has just unmade
	 * move_record.  The operation of zobrist updating by a MoveRecord
	 * is its own inverse.
	 */
	static ZobristKey update(const ZobristKey previous_key, const MoveRecord& record){
		ZobristKey result = previous_key;

		if(record.lost_black_castle_queen){
			result ^= kZobristBlackCastleQueen;
		}
		if(record.lost_black_castle_king){
			result ^= kZobristBlackCastleKing;
		}
		if(record.lost_white_castle_queen){
			result ^= kZobristWhiteCastleQueen;
		}
		if(record.lost_white_castle_king){
			result ^= kZobristWhiteCastleKing;
		}

		// The turn always flips, obviously.
		result ^= kZobristWhitesTurn;

		// Account for all changed pieces, including available
		// en passant captures, which are treated as pieces.
		result ^= get_table_entry(record.from_square, record.moved_piece);
		if(record.captured_piece != Piece::NO_PIECE){
			result ^= get_table_entry(record.captured_square, record.captured_piece);
		}
		result ^= get_table_entry(record.to_square, record.placed_piece);
		if(record.castled_piece != Piece::NO_PIECE){
			result ^= get_table_entry(record.castled_from_square, record.castled_piece);
			result ^= get_table_entry(record.castled_to_square, record.castled_piece);
		}
		if(record.en_passant_piece_after != Piece::NO_PIECE){
			result ^= get_table_entry(record.en_passant_square_after,
					record.en_passant_piece_after);
		}
		if(record.en_passant_piece_before != Piece::NO_PIECE){
			result ^= get_table_entry(record.en_passant_square_before,
					record.en_passant_piece_before);
		}

		return result;
	}


};


BoardState BoardState::from_fen(std::string fen){
	// Start with a blank BoardState, which will be filled in
	// later.
	BoardState result;

	// Break the FEN string into parts.
	std::vector<std::string> parts = split_string(fen, " ");
	if(parts.size() < 5 || parts.size() > 6){
		throw "Improperly formated string passed to BoardState::from_fen.";
	}
	std::string board_part = parts[0];
	std::string turn_part = parts[1];
	std::string castle_rights_part;
	if(parts.size() == 6){
		castle_rights_part = parts[2];
	}
	std::string en_passant_part = parts[parts.size() == 6?3:2];
	std::string halfmove_clock_part = parts[parts.size() == 6?4:3];
	std::string fullmove_counter_part = parts[parts.size() == 6?5:4];

	// Break the board part of the FEN string into ranks.
	std::vector<std::string> rank_parts = split_string(board_part, "/");
	if(rank_parts.size() != kRanksPerBoard){
		throw "Improperly formated string passed to BoardState::from_fen.";
	}

	// Place all the pieces in each rank.
	std::string rank_string;
	std::vector<std::string> rank_tokens;
	SquareIndex file_index;
	for(SquareIndex rank_index=0; rank_index<kRanksPerBoard; rank_index++){
		rank_string = rank_parts[kRanksPerBoard - rank_index - 1];
		rank_tokens = tokenize_fen_rank(rank_string);
		file_index = 0;
		for(std::string token : rank_tokens){
			if(isdigit(token[0])){
				file_index += stoi(token);
			}else{
				if(token.size() != 1){
					throw "Improperly formated string passed to BoardState::from_fen.";
				}
				result.raw_place_piece(fen_to_piece(token[0]),
						square_index_of(rank_index, file_index));
				file_index++;
			}
			if(file_index > kFilesPerBoard){
				throw "Improperly formated string passed to BoardState::from_fen.";
			}
		}
	}
	// All pieces have now been placed with the possible exception of en passant.

	// Set whose turn it is
	if(turn_part.size() != 1){
		throw "Improperly formated string passed to BoardState::from_fen.";
	}
	if(turn_part[0] == 'w'){
		result.set_whites_turn(true);
	}else{
		result.set_whites_turn(false);
	}

	// Set castle rights
	if(castle_rights_part.size() > 4){
		throw "Improperly formated string passed to BoardState::from_fen.";
	}
	for(char castle_right : castle_rights_part){
		switch(castle_right){
		case 'K':
			result.set_white_castle_king(true);
			break;
		case 'Q':
			result.set_white_castle_queen(true);
			break;
		case 'k':
			result.set_black_castle_king(true);
			break;
		case 'q':
			result.set_black_castle_queen(true);
			break;
		default:
			throw "Improperly formated string passed to BoardState::from_fen.";
		}
	}

	// Set en passant
	if(en_passant_part != "-"){
		result.raw_set_en_passant(result.core_.whites_turn_, algebraic_to_square_index(en_passant_part));
	}

	// Set the clocks
	result.halfmove_clock_ = stoi(halfmove_clock_part);
	result.fullmove_counter_ = stoi(fullmove_counter_part);

	// Calculate the current Zobrist hash
	result.hash_ = ZobristHasher::hash(result);

	// Calculate available moves
	result.compute_available_moves();

	return result;
}


} // namespace boardlib
#endif /* SRC_BOARDLIB_H_ */
