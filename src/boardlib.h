/*
 * boardlib.hpp
 *
 *  Created on: Oct 2, 2018
 *      Author: jason
 */

#ifndef SRC_BOARDLIB_H_
#define SRC_BOARDLIB_H_

namespace boardlib {

class BitBoard{
private:
	uint64_t value_;
public:
	BitBoard(uint64_t value){
		value_ = value;
	}

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
	void operator&=(const BitBoard rhs) const{
		value_ = value_ & rhs.value_;
	}
	void operator|=(const BitBoard rhs) const{
		value_ = value_ | rhs.value_;
	}
	void operator^=(const BitBoard rhs) const{
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

enum class ZobristKey : uint64_t {};
//typedef uint64_t ZobristKey;
enum class SquareIndex : unsigned char {};

extern const SquareIndex kSquaresPerBoard;
extern const SquareIndex kRanksPerBoard;
extern const SquareIndex kFilesPerBoard;

inline SquareIndex square_index_of(SquareIndex rank_index, SquareIndex file_index){
	return (kFilesPerBoard * (rank_index - 1)) + file_index;
}

inline SquareIndex rank_index_of(SquareIndex square){
	return (square / kFilesPerBoard);
}

inline SquareIndex file_index_of(SquareIndex square){
	return square % kFilesPerBoard;
}

inline std::string square_index_to_algebraic(SquareIndex square){
	char algebraic_file = 'a' + file_index_of(square);
	return "" + algebraic_file + std::to_string(((int) rank_index_of(square) + 1));
}

inline SquareIndex algebraic_to_square_index(std::string algebraic){
	SquareIndex file_index = (SquareIndex) algebraic[0] - 'a';
	SquareIndex rank_index = (SquareIndex) std::stoi(algebraic.substr(1));
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
		throw "Invalid Piece passed to fen_of.";
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

extern const SquareIndex kNoEnPassant;

extern const BitBoard kEmpty;
extern const BitBoard kFull;
extern const BitBoard kSquares[64];

/*
 * Defined here so that kSquares is available.  Not sure if this is
 * the best way to do it.
 */
static BitBoard BitBoard::from_square_index(SquareIndex square){
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
	BoardStateCore(const BitBoard kings, const BitBoard queens, const BitBoard bishops, const BitBoard knights,
			const BitBoard rooks, const BitBoard pawns, const BitBoard white, const BitBoard black, const BitBoard en_passant,
			const bool whites_turn, const bool white_castle_king, const bool white_castle_queen, const bool black_castle_king,
			const bool black_castle_queen, const bool valid){
		kings_ = kings;
		queens_ = queens;
		bishops_ = bishops;
		knights_ = knights;
		rooks_ = rooks;
		pawns_ = pawns;
		white_ = white;
		black_ = black;
		en_passant_ = en_passant;
		whites_turn_ = whites_turn;
		white_castle_king_ = white_castle_king;
		white_castle_queen_ = white_castle_queen;
		black_castle_king_ = black_castle_king;
		black_castle_queen_ = black_castle_queen;
		valid_ = valid;
	}

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

extern const Move kNoMove;

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

void split_string(const std::string string_to_split,
		const std::string delimiter, std::vector<std::string> output){
	std::string::size_type start_position = 0;
	std::string::size_type end_position;
	std::string::size_type delimiter_size = delimiter.size();
	std::string::size_type token_size;
	std::string::size_type string_to_split_size = string_to_split.size();
	while(start_position < string_to_split_size){
		end_position = string_to_split.find(delimiter, start_position);
		token_size = end_position - start_position;
		if(token_size > 0){
			output.push_back(string_to_split.substr(start_position, token_size));
		}
		start_position = end_position + delimiter_size;
	}
}

std::vector<std::string> split_string(const std::string string_to_split,
		const std::string delimiter){
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
	std::array<Piece, static_cast<int>(kSquaresPerBoard)> piece_map_;
	ZobristKey hash_;
	std::vector<RecordEntry> record_;
	std::array<SquareIndex, static_cast<int>(kSquaresPerBoard)> available_moves_;
	SquareIndex en_passant_square_;

	/*
	 * Break the rank_string into tokens.  It is assumed that rank_string
	 * represents a single rank from a FEN string.  The tokens will be either
	 * letters representing pieces or numbers representing empty squares.
	 */
	inline static std::vector<std::string> tokenize_fen_rank(std::string rank_string){
		std::vector<std::string> result;
		std::string current_token;
		for(char character : rank_string){
			current_token += character;
			if(!isdigit(character)){
				result.push_back(current_token);
			}
		}
		result.push_back(current_token);
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

	BoardState(){
		core_ = BoardStateCore();
		halfmove_clock_ = 0;
		fullmove_counter_ = 0;
		halfmove_counter_ = 0;
		threefold_repetition_clock_ = 0;
		piece_map_ = std::array<Piece, static_cast<int>(kSquaresPerBoard)>() = {Piece::NO_PIECE};
		hash_ = 0;
		record_ = std::vector<RecordEntry>();
		// Avoid allocation during game by reserving a bunch of memory now
		record_.reserve(10000);
		available_moves_ = std::array<SquareIndex, static_cast<int>(kSquaresPerBoard)>() = {kEmpty};
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
	inline void compute_available_moves();


	/*
	 * Create a BoardState from a FEN formatted string. This function
	 * doesn't need to be fast.
	 */
	static BoardState from_fen(std::string fen){
		// Start with a blank BoardState, which will be filled in
		// later.
		BoardState result;

		// Break the FEN string into parts.
		std::vector<std::string> parts = split_string(fen, " ");
		if(parts.size < 5 || parts.size > 6){
			throw "Improperly formated string passed to BoardState::from_fen.";
		}
		std::string board_part = parts[0];
		std::string turn_part = parts[1];
		std::string castle_rights_part;
		if(parts.size == 6){
			castle_rights_part = parts[2];
		}
		std::string en_passant_part = parts[parts.size == 6?3:2];
		std::string halfmove_clock_part = parts[parts.size == 6?4:3];
		std::string fullmove_counter_part = parts[parts.size == 6?5:4];

		// Break the board part of the FEN string into ranks.
		std::vector<std::string> rank_parts = split_string(board_part, "/");
		if(rank_parts.size != kRanksPerBoard){
			throw "Improperly formated string passed to BoardState::from_fen.";
		}

		// Place all the pieces in each rank.
		std::string rank_string;
		std::vector<std::string> rank_tokens;
		SquareIndex file_index;
		for(SquareIndex rank_index=0; rank_index<kRanksPerBoard; rank_index++){
			rank_string = rank_parts[rank_index];
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
					file_index += 1;
				}
				if(file_index >= kFilesPerBoard){
					throw "Improperly formated string passed to BoardState::from_fen.";
				}
			}
		}
		// All pieces have been placed with the possible exception of en passant.

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
		if(en_passant_part.size() > 0){
			result.raw_set_en_passant(core_.whites_turn_, algebraic_to_square_index(en_passant_part));
		}

		// Set the clocks
		result.halfmove_clock_ = stoi(halfmove_clock_part);
		result.fullmove_counter_ = stoi(fullmove_counter_part);

		// Calculate the current Zobrist hash
		result.hash_ = ZobristHasher::hash(*this);

		// Calculate available moves
		result.compute_available_moves();

		return result;
	}

	/*
	 * Return a FEN formatted string based on the BoardState. This
	 * function doesn't need to be fast.
	 */
	std::string to_fen() const{
		std::string board_part;
		Piece piece;
		char piece_char;
		int empty_count;
		SquareIndex rank, file;

		// Iterate over the piece_map_ by rank and file,
		// constructing the corresponding part of the FEN
		// string.
		for(rank=0; rank<kRanksPerBoard; rank++){
			empty_count = 0;
			for(file=0; file<kFilesPerBoard; file++){
				SquareIndex square = square_index_of(rank, file);
				piece = piece_map_[square];
				piece_char = piece_to_fen(piece);
				if(piece_char == '-'){
					empty_count++;
				}else{
					if(empty_count > 0){
						board_part << empty_count;
						empty_count = 0;
					}
					board_part += piece_char;
				}
			}
			if(rank < (kRanksPerBoard - 1)){
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
	static const unsigned int kNumberOfPieceTypes = 14;
	static const std::array<ZobristKey, static_cast<int>(kSquaresPerBoard*kNumberOfPieceTypes)> zobrist_table_;
	static const ZobristKey kZobristWhiteCastleKing;
	static const ZobristKey kZobristWhiteCastleQueen;
	static const ZobristKey kZobristBlackCastleKing;
	static const ZobristKey kZobristBlackCastleQueen;
	static const ZobristKey kZobristWhitesTurn;
	inline static ZobristKey get_table_entry(SquareIndex square, Piece piece){
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



} // namespace boardlib
#endif /* SRC_BOARDLIB_H_ */
