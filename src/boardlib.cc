/*
 * boardlib.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: jason
 */

#include <boardlib.h>

namespace boardlib{




//void BitBoard::operator&=(const BitBoard rhs){
//	value_ = value_ & rhs.value_;
//}
//void BitBoard::operator|=(const BitBoard rhs){
//	value_ = value_ | rhs.value_;
//}
//void BitBoard::operator^=(const BitBoard rhs){
//	value_ = value_ ^ rhs.value_;
//}

SquareIndex BitBoard::greatest_square_index() const{
	decltype(value_) temp_value = value_;
	SquareIndex result = 0;
	while(temp_value >>= 1){
		result++;
	}
	return(result);
}

BitBoard BitBoard::least_significant_1_bit() const{
	return BitBoard(value_ & (-value_));
}

BitBoard BitBoard::pop_least_significant_1_bit(){
	decltype(value_) temp_value = value_ & (-value_);
	value_ ^= temp_value;
	return BitBoard(temp_value);
}

SquareIndex BitBoard::population_count() const{
	// TODO: Potentially not portable.
	return (SquareIndex) __builtin_popcountll(value_);
}


//void BitBoard::step_east(){
//	value_ = ((value_ & (~kFileH.value_)) << 1);
//}
//
//void BitBoard::step_north(){
//	value_ = value_ << 8;
//}
//
//void BitBoard::step_west(){
//	value_ = (value_ & (~kFileA.value_)) >> 1;
//}
//
//void BitBoard::step_south(){
//	value_ = value_ >> 8;
//}
//
//void BitBoard::step_northeast(){
//	value_ = (value_ & (~kFileH.value_)) << 9;
//}
//
//void BitBoard::step_northwest(){
//	value_ = (value_ & (~kFileA.value_)) << 7;
//}
//
//void BitBoard::step_southeast(){
//	value_ = (value_ & (~kFileH.value_)) >> 7;
//}
//
//void BitBoard::step_southwest(){
//	value_ = (value_ & (~kFileA.value_)) >> 9;
//}

SquareIndex square_index_of(const SquareIndex rank_index, const SquareIndex file_index){
	return (kFilesPerBoard * rank_index) + file_index;
}

SquareIndex rank_index_of(const SquareIndex square){
	return (square / kFilesPerBoard);
}

SquareIndex file_index_of(const SquareIndex square){
	return square % kFilesPerBoard;
}

SquareIndex diag_index_of(const SquareIndex square){
	return((rank_index_of(square) - file_index_of(square)) & 15);
}

SquareIndex anti_diag_index_of(const SquareIndex square){
	return(7 ^ (rank_index_of(square) + file_index_of(square)));
}

std::string square_index_to_algebraic(const SquareIndex square){
	char algebraic_file = 'a' + file_index_of(square);
	std::string result;
	result += algebraic_file;
	result += std::to_string(((int) rank_index_of(square) + 1));
	return result;
//	return "" + algebraic_file + std::to_string(((int) rank_index_of(square) + 1));
}

SquareIndex algebraic_to_square_index(const std::string algebraic){
	SquareIndex file_index = (SquareIndex) algebraic[0] - 'a';
	SquareIndex rank_index = (SquareIndex) std::stoi(algebraic.substr(1)) - 1;
	return square_index_of(rank_index, file_index);
}

char piece_to_fen(const Piece piece){
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

Piece fen_to_piece(const char piece_char){
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

int piece_index_of(Piece piece){
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

PieceKind kind_of(const Piece piece){
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

bool is_diagonal_slider(const PieceKind kind){
	switch(kind){
	case PieceKind::QUEEN:
		return true;
	case PieceKind::BISHOP:
		return true;
	default:
		return false;
	}
}

bool is_non_diagonal_slider(const PieceKind kind){
	switch(kind){
	case PieceKind::QUEEN:
		return true;
	case PieceKind::ROOK:
		return true;
	default:
		return false;
	}
}

bool is_slider(const PieceKind kind){
	switch(kind){
	case PieceKind::QUEEN:
		return true;
	case PieceKind::ROOK:
		return true;
	case PieceKind::BISHOP:
		return true;
	default:
		return false;
	}
}

Color color_of(const Piece piece){
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

/*
 * Represent the minimum information needed to define the state of the board.
 */
BitBoard& BoardStateCore::get_piece_kind_bit_board(const PieceKind kind){
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

BitBoard& BoardStateCore::get_color_bit_board(const Color color){
	switch(color){
	case Color::WHITE:
		return white_;
	case Color::BLACK:
		return black_;
	default:
		throw "Invalid Color passed to get_color_bit_board.";
	}
}

void BoardStateCore::raw_place_pieces(const Piece piece, const BitBoard places){
	const Color color = color_of(piece);
	const PieceKind kind = kind_of(piece);
	get_color_bit_board(color) |= places;
	get_piece_kind_bit_board(kind) |= places;
}

void BoardStateCore::raw_place_piece(const Piece piece, const SquareIndex square){
	raw_place_pieces(piece, BitBoard::from_square_index(square));
}

void BoardStateCore::raw_unplace_pieces(const Piece piece, const BitBoard places){
	const BitBoard unplaces = ~places;
	const Color color = color_of(piece);
	const PieceKind kind = kind_of(piece);
	get_color_bit_board(color) &= unplaces;
	get_piece_kind_bit_board(kind) &= unplaces;
}

void BoardStateCore::raw_unplace_piece(const Piece piece, const SquareIndex square){
	raw_unplace_pieces(piece, BitBoard::from_square_index(square));
}

BoardStateCore::BoardStateCore(){
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

bool BoardStateCore::operator==(const BoardStateCore& rhs) const{
	return kings_==rhs.kings_ && queens_==rhs.queens_ && bishops_==rhs.bishops_ &&
			knights_==rhs.knights_ && rooks_==rhs.rooks_ && pawns_==rhs.pawns_ &&
			white_==rhs.white_ && black_==rhs.black_ && en_passant_==rhs.en_passant_ &&
			whites_turn_==rhs.whites_turn_ && white_castle_king_==rhs.white_castle_king_
			&& white_castle_queen_==rhs.white_castle_queen_ &&
			black_castle_king_==rhs.black_castle_king_ &&
			black_castle_queen_==rhs.black_castle_queen_ && valid_==rhs.valid_;
}

RecordEntry::RecordEntry(const ZobristKey hash, const BoardStateCore state){
	hash_ = hash;
	state_ = state;
}

bool RecordEntry::operator==(const RecordEntry& rhs) const{
	return hash_==rhs.hash_ && state_==rhs.state_;
}

bool Move::operator==(const Move& rhs) const{
	return from_square==rhs.from_square && to_square==rhs.to_square
			&& promotion==rhs.promotion;
}

bool MoveRecord::operator==(const MoveRecord& rhs) const{
	return from_square==rhs.from_square && to_square == rhs.to_square &&
			captured_square==rhs.captured_square && castled_from_square==rhs.castled_from_square
			&& castled_to_square==rhs.castled_to_square && moved_piece==rhs.moved_piece &&
			placed_piece==rhs.placed_piece && captured_piece==rhs.captured_piece &&
			castled_piece==rhs.castled_piece && lost_white_castle_king==rhs.lost_white_castle_king
			&& lost_white_castle_queen==rhs.lost_white_castle_queen &&
			lost_black_castle_king==rhs.lost_black_castle_king &&
			lost_black_castle_queen==rhs.lost_black_castle_queen &&
			en_passant_square_before==rhs.en_passant_square_before &&
			en_passant_square_after==rhs.en_passant_square_after &&
			en_passant_piece_before==rhs.en_passant_piece_before &&
			en_passant_piece_after==rhs.en_passant_piece_after &&
			halfmove_clock_before==rhs.halfmove_clock_before &&
			threefold_repetition_clock_before==rhs.threefold_repetition_clock_before &&
			halfmove_clock_after==rhs.halfmove_clock_after &&
			threefold_repetition_clock_after==rhs.threefold_repetition_clock_after;
}

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


std::vector<std::string> BoardState::tokenize_fen_rank(std::string rank_string){
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

void BoardState::raw_place_piece(const Piece piece, const SquareIndex square){
	core_.raw_place_piece(piece, square);
	piece_map_[square] = piece;
}

void BoardState::raw_unplace_piece(const SquareIndex square){
	const Piece piece = get_piece_at(square);
	raw_unplace_piece(piece, square);
}
void BoardState::raw_unplace_piece(const Piece piece, const SquareIndex square){
	core_.raw_unplace_piece(piece, square);
	piece_map_[square] = Piece::NO_PIECE;
}

void BoardState::raw_set_en_passant(Piece piece, SquareIndex square){
	raw_place_piece(piece, square);
	en_passant_square_ = square;
}

void BoardState::raw_unset_en_passant(){
	if(en_passant_square_ != kNoEnPassant){
		raw_unplace_piece(en_passant_square_);
		en_passant_square_ = kNoEnPassant;
	}
}

void BoardState::raw_change_en_passant(Piece en_passant_piece_before,
			Piece en_passant_piece_after,
			SquareIndex en_passant_square_after){
	if(en_passant_square_ != kNoEnPassant){
		raw_unplace_piece(en_passant_piece_before, en_passant_square_);
	}
	if(en_passant_piece_after != Piece::NO_PIECE){
		raw_place_piece(en_passant_piece_after, en_passant_square_after);
		en_passant_square_ = en_passant_square_after;
	}
}



BoardState::BoardState(BoardState&& rhs) = default;

BoardState::BoardState(){
	halfmove_clock_ = 0;
	fullmove_counter_ = 0;
	halfmove_counter_ = 0;
	threefold_repetition_clock_ = 0;

	// The piece_map_ is initialized empty, to match the default initialization
	// of core_.
	piece_map_ = {Piece::NO_PIECE};

	hash_ = 0;

	// Avoid allocation during game by reserving a bunch of memory now
	record_.reserve(10000);

	// Assume there is no en passant in the empty BoardState
	en_passant_square_ = kNoEnPassant;
}

bool BoardState::operator==(const BoardState& rhs) const{
	return core_==rhs.core_ && halfmove_clock_==rhs.halfmove_clock_ &&
			fullmove_counter_==rhs.fullmove_counter_ &&
			halfmove_counter_==rhs.halfmove_counter_ &&
			threefold_repetition_clock_==rhs.threefold_repetition_clock_ &&
			piece_map_==rhs.piece_map_ &&
			hash_==rhs.hash_ && record_==rhs.record_ &&
			en_passant_square_==rhs.en_passant_square_;
}

BoardState BoardState::copy() const{
	BoardState result;
	result.core_ = core_;
	result.halfmove_clock_ = halfmove_clock_;
	result.fullmove_counter_ = fullmove_counter_;
	result.halfmove_counter_ = halfmove_counter_;
	result.threefold_repetition_clock_ = threefold_repetition_clock_;
	result.piece_map_ = piece_map_;
	result.hash_ = hash_;
	result.record_ = record_;
	result.en_passant_square_ = en_passant_square_;
	result.move_targets_ = move_targets_;
	result.move_origins_ = move_origins_;
	return result;
}

ZobristKey BoardState::get_hash() const{
	return hash_;
}

Piece BoardState::get_piece_at(const SquareIndex square) const{
	return piece_map_[square];
}

bool BoardState::get_whites_turn() const{
	return core_.whites_turn_;
}
void BoardState::set_whites_turn(const bool value){
	core_.whites_turn_ = value;
}
bool BoardState::get_white_castle_king() const{
	return core_.white_castle_king_;
}
void BoardState::set_white_castle_king(const bool value){
	core_.white_castle_king_ = value;
}

bool BoardState::get_white_castle_queen() const{
	return core_.white_castle_queen_;
}
void BoardState::set_white_castle_queen(const bool value){
	core_.white_castle_queen_ = value;
}

bool BoardState::get_black_castle_king() const{
	return core_.black_castle_king_;
}
void BoardState::set_black_castle_king(const bool value){
	core_.black_castle_king_ = value;
}

bool BoardState::get_black_castle_queen() const{
	return core_.black_castle_queen_;
}
void BoardState::set_black_castle_queen(const bool value){
	core_.black_castle_queen_ = value;
}

void BoardState::compute_move_tables(){
	// TODO: This
}

void BoardState::update_redundant_data(){
	occupied_ = (core_.white_ | core_.black_) ^ core_.en_passant_;
	unoccupied_ = ~occupied_;
	if(core_.whites_turn_){
		own_ = core_.white_;
		opponent_ = core_.black_ ^ core_.en_passant_;
	}else{
		own_ = core_.black_;
		opponent_ = core_.white_ ^ core_.en_passant_;
	}
	own_complement_ = ~own_;
	own_king_ = own_ & core_.kings_;
	opponent_non_diagonal_sliders_ = opponent_ & core_.rooks_ & core_.queens_;
	opponent_diagonal_sliders_ = opponent_ & core_.bishops_ & core_.queens_;
	opponent_kinghts_ = opponent_ & core_.knights_;
	opponent_pawns_ = opponent_ & core_.pawns_;

	own_king_square_ = own_king_.greatest_square_index();
}

void BoardState::update_move_tables(const MoveRecord& record){
	// TODO: This
}

void BoardState::downdate_move_tables(const MoveRecord& record){
	// TODO: This
}

SquareIndex BoardState::get_en_passant_target(
		const SquareIndex en_passant_square){
	const SquareIndex en_passant_rank = rank_index_of(en_passant_square);
	const SquareIndex target_rank = en_passant_rank==3?2:5;
	const SquareIndex target_file = file_index_of(en_passant_square);
	return square_index_of(target_rank, target_file);
}

MoveRecord BoardState::compute_move_record(const Move& move){
	const Piece moved_piece = get_piece_at(move.from_square);
	const PieceKind moved_piece_kind = kind_of(moved_piece);
	const Color moved_piece_color = color_of(moved_piece);
	const Piece to_piece = get_piece_at(move.to_square);
	const PieceKind to_piece_kind = kind_of(to_piece);
	const Color to_piece_color = color_of(to_piece);
	MoveRecord result;
	result.from_square = move.from_square;
	result.to_square = move.to_square;
	result.moved_piece = moved_piece;
	SquareIndex from_file, to_file, delta_file, from_rank,
		castled_from_file, to_rank, delta_rank, en_passant_rank_after;


	// Check for special move types.
	from_file = file_index_of(result.from_square);
	if(to_piece_kind == PieceKind::EN_PASSANT
			&& moved_piece_kind == PieceKind::PAWN){
		// Assuming the current position was achieved legally,
		// the above conditions guarantee that this move is an
		// en passant capture.  Proceed accordingly.
		result.captured_square = get_en_passant_target(move.to_square);
		result.captured_piece = get_piece_at(result.captured_square);
		result.placed_piece = result.moved_piece;

	}else if(moved_piece_kind == PieceKind::KING){

		to_file = file_index_of(result.to_square);
		delta_file = (from_file > to_file)?(from_file - to_file):(to_file - from_file);
		if(delta_file == 2){
			// Assuming this is a legal move, the above conditions
			// guarantee that this move is a castle.  Proceed accordingly.
			from_rank = rank_index_of(result.from_square);
			if(from_file < 4){
				result.castled_from_square = square_index_of(from_rank, 0);
			}else{
				result.castled_from_square = square_index_of(from_rank, 7);
			}
			result.castled_piece = get_piece_at(result.castled_from_square);

			castled_from_file = file_index_of(result.castled_from_square);
			if(castled_from_file < 4){
				result.castled_to_square = square_index_of(from_rank, 3);
			}else{
				result.castled_to_square = square_index_of(from_rank, 5);
			}


		}
	}else if(to_piece != Piece::NO_PIECE){
		// Since en passant has already been ruled out, and since we are
		// assuming the move is legal, this is a regular capture.
		result.captured_square = result.to_square;
		result.captured_piece = to_piece;
	} // End check for special move types

	// Check for promotion.
	if(move.promotion != Piece::NO_PIECE){
		result.placed_piece = move.promotion;
	}else{
		result.placed_piece = result.moved_piece;
	}

	// Check whether a pawn advanced 2 squares, creating a new potential
	// en passant opportunity.
	if(moved_piece_kind == PieceKind::PAWN){
		from_rank = rank_index_of(result.from_square);
		from_file = file_index_of(result.from_square);
		to_rank = rank_index_of(result.to_square);
		delta_rank = (moved_piece_color == Color::BLACK)?(from_rank - to_rank):(to_rank - from_rank);
		if(delta_rank == 2){
			// Assuming this is a legal move, it has created a potential en passant
			// opportunity.  Our board representation does not require there to be a
			// pawn present capable of capturing the en passant square, so there was no
			// need to check for that.
			en_passant_rank_after = (moved_piece_color == Color::BLACK)?(to_rank + 1):(from_rank + 1);
			result.en_passant_square_after = square_index_of(en_passant_rank_after, from_file);
			result.en_passant_piece_after = (moved_piece_color == Color::BLACK)?(Piece::BLACK_EN_PASSANT):(Piece::WHITE_EN_PASSANT);
		}
	}

	// Set the members for pre-existing en passant opportunity, if any.
	result.en_passant_square_before = en_passant_square_;
	if(result.en_passant_square_before != kNoEnPassant){
		result.en_passant_piece_before = get_piece_at(result.en_passant_square_before);
	} // Otherwise, result.en_passant_piece_before is already Piece::NO_PIECE.

	// Determine changes to castle rights.
	if(result.moved_piece == Piece::WHITE_KING){
		result.lost_white_castle_king = core_.white_castle_king_;
		result.lost_white_castle_queen = core_.white_castle_queen_;
	}else if(result.moved_piece == Piece::BLACK_KING){
		result.lost_black_castle_king = core_.white_castle_king_;
		result.lost_black_castle_queen = core_.white_castle_queen_;
	}else if(result.moved_piece == Piece::WHITE_ROOK){
		if(from_file==0){
			result.lost_white_castle_queen = core_.white_castle_queen_;
		}else if(from_file==7){
			result.lost_white_castle_king = core_.white_castle_king_;
		}
	}else if(result.moved_piece == Piece::BLACK_ROOK){
		if(from_file==0){
			result.lost_black_castle_queen = core_.black_castle_queen_;
		}else if(from_file==7){
			result.lost_black_castle_king = core_.black_castle_king_;
		}
	}

	// Determine changes to threefold repetition and halfmove
	// clocks.
	if((to_piece != Piece::NO_PIECE) || (moved_piece_kind == PieceKind::PAWN)){
		// In this case, both clocks will be reset.
		result.halfmove_clock_after = 0;
		result.threefold_repetition_clock_after = 0;
	}else if(result.lost_white_castle_king || result.lost_white_castle_queen ||
			result.lost_black_castle_king || result.lost_black_castle_queen){
		result.halfmove_clock_after = halfmove_clock_ + 1;
		result.threefold_repetition_clock_after = 0;
	}else{
		result.halfmove_clock_after = halfmove_clock_ + 1;
		result.threefold_repetition_clock_after = threefold_repetition_clock_ + 1;
	}
	result.halfmove_clock_before = halfmove_clock_;
	result.threefold_repetition_clock_before = threefold_repetition_clock_;

	return result;
}

MoveRecord BoardState::make_move(const Move move){
	const MoveRecord record = compute_move_record(move);
	apply_move_record(record);
	return record;
}

void BoardState::unmake_move(const MoveRecord& record){
	// TODO: This

	// Downdate the move tables.
	downdate_move_tables(record);

	// Downdate the Zobrist hash (which is the same as updating).
	hash_ = ZobristHasher::update(hash_, record);

	// Change whose turn it is.
	set_whites_turn(!get_whites_turn());

	// Decrement the counters.
	fullmove_counter_ -= core_.whites_turn_?0:1;
	halfmove_counter_--;

	// Set the clocks.
	halfmove_clock_ = record.halfmove_clock_before;
	threefold_repetition_clock_ = record.threefold_repetition_clock_before;

	// Downdate castle rights.
	core_.white_castle_king_ ^= record.lost_white_castle_king;
	core_.white_castle_queen_ ^= record.lost_white_castle_queen;
	core_.black_castle_king_ ^= record.lost_black_castle_king;
	core_.black_castle_queen_ ^= record.lost_black_castle_queen;

	// Unset the en passant square.
	raw_unset_en_passant();

	// Downdate the board based on the moved, placed, captured, and
	// castled pieces.
	raw_unplace_piece(record.placed_piece, record.to_square);
	if(record.castled_piece != Piece::NO_PIECE){
		raw_unplace_piece(record.castled_piece, record.castled_to_square);
		raw_place_piece(record.castled_piece, record.castled_from_square);
	}
	if(record.captured_piece != Piece::NO_PIECE){
		raw_place_piece(record.captured_piece, record.captured_square);
	}
	raw_place_piece(record.moved_piece, record.from_square);

	// Set the en passant square to its old value
	if(record.en_passant_piece_before != Piece::NO_PIECE){
		raw_set_en_passant(record.en_passant_piece_before, record.en_passant_square_before);
	}
}

void BoardState::apply_move_record(const MoveRecord& record){
	// Remove previous en passant position
	raw_unset_en_passant();

	// Update the board based on the moved, placed, captured, and
	// castled pieces.
	raw_unplace_piece(record.moved_piece, record.from_square);
	if(record.captured_piece != Piece::NO_PIECE){
		raw_unplace_piece(record.captured_piece, record.captured_square);
	}
	if(record.castled_piece != Piece::NO_PIECE){
		raw_unplace_piece(record.castled_piece, record.castled_from_square);
		raw_place_piece(record.castled_piece, record.castled_to_square);
	}
	raw_place_piece(record.placed_piece, record.to_square);

	// Update the en passant square.
	if(record.en_passant_piece_after != Piece::NO_PIECE){
		raw_set_en_passant(record.en_passant_piece_after, record.en_passant_square_after);
	}

	// Update castle rights.
	core_.white_castle_king_ ^= record.lost_white_castle_king;
	core_.white_castle_queen_ ^= record.lost_white_castle_queen;
	core_.black_castle_king_ ^= record.lost_black_castle_king;
	core_.black_castle_queen_ ^= record.lost_black_castle_queen;

	// Update the clocks.
	halfmove_clock_ = record.halfmove_clock_after;
	threefold_repetition_clock_ = record.threefold_repetition_clock_after;

	// Advance the counters.
	fullmove_counter_ += core_.whites_turn_?0:1;
	halfmove_counter_++;

	// Change whose turn it is.
	set_whites_turn(!get_whites_turn());

	// Update the Zobrist hash.
	hash_ = ZobristHasher::update(hash_, record);

	// Update the move tables.
	update_move_tables(record);
}

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

	// Set whose turn it is.
	if(turn_part.size() != 1){
		throw "Improperly formated string passed to BoardState::from_fen.";
	}
	if(turn_part[0] == 'w'){
		result.set_whites_turn(true);
	}else{
		result.set_whites_turn(false);
	}

	// Set castle rights.
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

	// Set en passant.
	if(en_passant_part != "-"){
		result.raw_set_en_passant(
				result.core_.whites_turn_?Piece::WHITE_EN_PASSANT:Piece::BLACK_EN_PASSANT,
				algebraic_to_square_index(en_passant_part));
	}

	// Set the clocks.
	result.halfmove_clock_ = stoi(halfmove_clock_part);
	result.fullmove_counter_ = stoi(fullmove_counter_part);

	// Calculate the current Zobrist hash.
	result.hash_ = ZobristHasher::hash(result);

	// Calculate available moves.
	result.compute_move_tables();

	return result;
}

std::string BoardState::to_fen() const{
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

BitBoard BoardState::compute_pinned_squares(const SquareIndex square){
	const BitBoard square_in_question = BitBoard::from_square_index(square);
	const BitBoard square_in_question_complement = ~square_in_question;
	const BitBoard own_minus_square_in_question = own_ & (square_in_question_complement);

	// The propagator is the set of squares attacking sliders can move along.  It
	// will be used to compute which sliders are able to attack the king.
	const BitBoard propagator = square_in_question | unoccupied_;

	const SquareIndex square_rank_index = rank_index_of(square);
	const SquareIndex square_file_index = file_index_of(square);
	const SquareIndex square_diag_index = diag_index_of(square);
	const SquareIndex square_anti_diag_index = anti_diag_index_of(square);
	const SquareIndex king_rank_index = rank_index_of(own_king_square_);
	const SquareIndex king_file_index = file_index_of(own_king_square_);
	const SquareIndex king_diag_index = diag_index_of(own_king_square_);
	const SquareIndex king_anti_diag_index = anti_diag_index_of(own_king_square_);

	BitBoard ray;
	// At most one of the following can match.
	if(square_rank_index == king_rank_index){
		if(square_file_index > king_file_index){
			// Square is to the east of king
			ray = BitBoard::slide_east(own_king_, propagator).step_east();
		}else{// square_file_index < king_file_index
			// Square is to the west of king
			ray = BitBoard::slide_west(own_king_, propagator).step_west();
		}

		if(ray & opponent_non_diagonal_sliders_){
			return ray;
		}else{
			return kFull;
		}
	}else if(square_file_index == king_file_index){
		if(square_rank_index > king_rank_index){
			// Square is to the north of king
			ray = BitBoard::slide_north(own_king_, propagator).step_north();
		}else{// square_rank_index < king_rank_index
			// Square is to the south of king
			ray = BitBoard::slide_south(own_king_, propagator).step_south();
		}

		if(ray & opponent_non_diagonal_sliders_){
			return ray;
		}else{
			return kFull;
		}
	}else if(square_diag_index == king_diag_index){
		if(square_rank_index > king_rank_index){
			// Square is northeast of king
			ray = BitBoard::slide_northeast(own_king_, propagator).step_northeast();
		}else{ //square_rank_index < king_rank_index
			// Square is southwest of king
			ray = BitBoard::slide_southwest(own_king_, propagator).step_southwest();
		}

		if(ray & opponent_diagonal_sliders_){
			return ray;
		}else{
			return kFull;
		}
	}else if(square_anti_diag_index == king_anti_diag_index){
		if(square_rank_index > king_rank_index){
			// Square is northwest of king
			ray = BitBoard::slide_northwest(own_king_, propagator).step_northwest();
		}else{ //square_rank_index < king_rank_index
			// Square is southeast of king
			ray = BitBoard::slide_southeast(own_king_, propagator).step_southeast();
		}

		if(ray & opponent_diagonal_sliders_){
			return ray;
		}else{
			return kFull;
		}
	}

	// If we got here, there was no pinning.
	return kFull;

}

BitBoard BoardState::compute_blocking_mask(SquareIndex square){
	const BitBoard square_board = BitBoard::from_square_index(square);
	const Piece attacking_piece = piece_map_[square];
	const PieceKind attacking_piece_kind = kind_of(attacking_piece);
	if(!is_slider(attacking_piece_kind)){
		// In this case the attacking piece can't be blocked.
		return kEmpty;
	}

	// Since we are assuming that the king is currently being threatened
	// by only the piece at the given square, we now know that any squares between
	// the king and the attacker are potential blocking positions.
	const SquareIndex square_rank_index = rank_index_of(square);
	const SquareIndex square_file_index = file_index_of(square);
	const SquareIndex square_diag_index = diag_index_of(square);
	const SquareIndex square_anti_diag_index = anti_diag_index_of(square);
	const SquareIndex king_rank_index = rank_index_of(own_king_square_);
	const SquareIndex king_file_index = file_index_of(own_king_square_);
	const SquareIndex king_diag_index = diag_index_of(own_king_square_);
	const SquareIndex king_anti_diag_index = anti_diag_index_of(own_king_square_);


	// At most one of the following can match.
	if(square_rank_index == king_rank_index){
		if(square_file_index > king_file_index){
			// Square is to the east of king
			return BitBoard::slide_east(own_king_, unoccupied_).step_east() ^ own_king_;
		}else{// square_file_index < king_file_index
			// Square is to the west of king
			return BitBoard::slide_west(own_king_, unoccupied_).step_west() ^ own_king_;
		}
	}else if(square_file_index == king_file_index){
		if(square_rank_index > king_rank_index){
			// Square is to the north of king
			return BitBoard::slide_north(own_king_, unoccupied_) ^ own_king_;
		}else{// square_rank_index < king_rank_index
			// Square is to the south of king
			return BitBoard::slide_south(own_king_, unoccupied_) ^ own_king_;
		}
	}else if(square_diag_index == king_diag_index){
		if(square_rank_index > king_rank_index){
			// Square is northeast of king
			return BitBoard::slide_northeast(own_king_, unoccupied_).step_northeast() ^ own_king_;
		}else{ //square_rank_index < king_rank_index
			// Square is southwest of king
			return BitBoard::slide_southwest(own_king_, unoccupied_).step_southwest() ^ own_king_;
		}
	}else if(square_anti_diag_index == king_anti_diag_index){
		if(square_rank_index > king_rank_index){
			// Square is northwest of king
			return BitBoard::slide_northwest(own_king_, unoccupied_).step_northwest() ^ own_king_;
		}else{ //square_rank_index < king_rank_index
			// Square is southeast of king
			return BitBoard::slide_southeast(own_king_, unoccupied_).step_southeast() ^ own_king_;
		}
	}
	// If we got here, the assumptions of this function were not satisfied.
	throw "Assumption violated in call to compute_blocking_mask.";
}

BitBoard BoardState::compute_possible_pinning_mask(const SquareIndex square){
	const BitBoard square_board = BitBoard::from_square_index(square);
	BitBoard result = kEmpty;
	BitBoard tmp;

	tmp = BitBoard::slide_east(square_board, unoccupied_);
	tmp = tmp.step_east() & own_;
	result |= tmp;

	tmp = BitBoard::slide_northeast(square_board, unoccupied_);
	tmp = tmp.step_northeast() & own_;
	result |= tmp;

	tmp = BitBoard::slide_north(square_board, unoccupied_);
	tmp = tmp.step_north() & own_;
	result |= tmp;

	tmp = BitBoard::slide_northwest(square_board, unoccupied_);
	tmp = tmp.step_northwest() & own_;
	result |= tmp;

	tmp = BitBoard::slide_west(square_board, unoccupied_);
	tmp = tmp.step_west() & own_;
	result |= tmp;

	tmp = BitBoard::slide_southwest(square_board, unoccupied_);
	tmp = tmp.step_southwest() & own_;
	result |= tmp;

	tmp = BitBoard::slide_south(square_board, unoccupied_);
	tmp = tmp.step_south() & own_;
	result |= tmp;

	tmp = BitBoard::slide_southeast(square_board, unoccupied_);
	tmp = tmp.step_southeast() & own_;
	result |= tmp;

	return result;
}

BitBoard BoardState::compute_threatening_squares(SquareIndex square){
	const BitBoard square_board = BitBoard::from_square_index(square);
	BitBoard result = kEmpty;
	BitBoard tmp;

	/*
	 * Check for attacking knights.
	 */
	tmp = square_board.knight_step() & opponent_kinghts_;
	if(tmp){
		result &= tmp;
	}
	if(!result){
		return result;
	}
	/*
	 * Check for pawn attacks.
	 */
	if(core_.whites_turn_){
		tmp = square_board.step_northeast() & opponent_pawns_;
		if(tmp){
			result &= tmp;
		}
		if(!result){
			return result;
		}
		tmp = square_board.step_northwest() & opponent_pawns_;
		if(tmp){
			result &= tmp;
		}
		if(!result){
			return result;
		}
	}else{
		tmp = square_board.step_southeast() & opponent_pawns_;
		if(tmp){
			result &= tmp;
		}
		if(!result){
			return result;
		}
		tmp = square_board.step_southwest() & opponent_pawns_;
		if(tmp){
			result &= tmp;
		}
		if(!result){
			return result;
		}
	}

	/*
	 * For each cardinal direction, check for sliding attackers.
	 */
	tmp = BitBoard::slide_east(square_board, unoccupied_).step_east();
	if(tmp & opponent_non_diagonal_sliders_){
		result &= tmp;
	}
	if(!result){
		return result;
	}

	tmp = BitBoard::slide_northeast(square_board, unoccupied_).step_northeast();
	if(tmp & opponent_diagonal_sliders_){
		result &= tmp;
	}
	if(!result){
		return result;
	}

	tmp = BitBoard::slide_north(square_board, unoccupied_).step_north();
	if(tmp & opponent_non_diagonal_sliders_){
		result &= tmp;
	}
	if(!result){
		return result;
	}

	tmp = BitBoard::slide_northwest(square_board, unoccupied_).step_northwest();
	if(tmp & opponent_diagonal_sliders_){
		result &= tmp;
	}
	if(!result){
		return result;
	}

	tmp = BitBoard::slide_west(square_board, unoccupied_).step_west();
	if(tmp & opponent_non_diagonal_sliders_){
		result &= tmp;
	}
	if(!result){
		return result;
	}

	tmp = BitBoard::slide_southwest(square_board, unoccupied_).step_southwest();
	if(tmp & opponent_diagonal_sliders_){
		result &= tmp;
	}
	if(!result){
		return result;
	}

	tmp = BitBoard::slide_south(square_board, unoccupied_).step_south();
	if(tmp & opponent_non_diagonal_sliders_){
		result &= tmp;
	}
	if(!result){
		return result;
	}

	tmp = BitBoard::slide_southeast(square_board, unoccupied_).step_southeast();
	if(tmp & opponent_diagonal_sliders_){
		result &= tmp;
	}

	return result;
}

ZobristKey ZobristHasher::get_table_entry(SquareIndex square, Piece piece){
	return zobrist_table_[kSquaresPerBoard * square + piece_index_of(piece)];
}

ZobristKey ZobristHasher::hash(const BoardState& state){
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

ZobristKey ZobristHasher::update(const ZobristKey previous_key, const MoveRecord& record){
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



// See https://stackoverflow.com/questions/8016780/undefined-reference-to-static-constexpr-char
// for an unsatisfying explanation as to why this external declaration is necessary.
constexpr std::array<ZobristKey, kSquaresPerBoard*(ZobristHasher::kNumberOfPieceTypes)> ZobristHasher::zobrist_table_;


}; // namespace boardlib
