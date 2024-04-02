cmake_minimum_required (VERSION 3.0)

project(cmake_min_test1 VERSION 0.1)
string(TIMESTAMP BUILD_DATE "%Y%m%d")

configure_file("version.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/version.h")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WIN32)
	add_definitions( -DBOOST_ALL_DYN_LINK )
	
	set(Boost_USE_STATIC_LIBS OFF )
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME OFF)
	
	# https://sourceforge.net/projects/boost/files/boost-binaries
	#set(Boost_DEBUG ON)
	set(BOOST_ROOT "d:/Tools/boost_1_78_0/")
	#set(BOOST_INCLUDEDIR "d:/Tools/boost_1_78_0/boost/")
	set(BOOST_LIBRARYDIR "d:/Tools/boost_1_78_0/lib64-msvc-14.3/")
	
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
endif()

find_package(Boost COMPONENTS system thread REQUIRED)

add_executable (${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE 
	Boost::boost 
	Boost::system 
	Boost::thread
)
