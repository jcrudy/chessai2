cmake_minimum_required(VERSION 3.12)
project(chessai2 VERSION 0.1.0 LANGUAGES CXX)

add_library(boardlib src/boardlib.cc)
add_executable(chessai2 src/chessai2.cc)
add_executable(run_tests test/run_tests.cc test/test_fen_io.cc test/test_zobrist.cc
	test/test_board_state.cc)

target_include_directories(boardlib
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(run_tests
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(run_tests boardlib)
target_link_libraries(chessai2 boardlib)

target_compile_features(chessai2 PRIVATE cxx_std_17)
target_compile_features(boardlib PRIVATE cxx_std_17)
target_compile_features(run_tests PRIVATE cxx_std_17)




