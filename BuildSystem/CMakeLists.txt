cmake_minimum_required(VERSION 2.6)
project(SPM)

# to run (Windows, Mingw32-gcc)
#   cmake -G "MinGW Makefiles"
#   mingw32-make all
# or Linux
#   cmake -G "Unix Makefiles"
#   make all

SET(MAIN_NAME spm_unoptimised)
SET(TEST_NAME spm_unittests)
SET(OPTI_NAME spm)

IF (WIN32)
  INCLUDE_DIRECTORIES("C:/Users/Alist/OneDrive/Projects/Software/SPM/Boost/boost_1_58_0/")
  LINK_DIRECTORIES("C:/Users/Alist/OneDrive/Projects/Software/SPM/Boost/boost_1_58_0/stage/lib")
  
  # Generic Build Settings
  SET(LINK_OPTIONS "-static ../../../icon.o")
  SET(COMPILE_OPTIONS_MAIN "-DBOOST_THREAD_USE_LIB=1 -DBOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN -O0 -g3 -Wall -Wno-deprecated-declarations -fmessage-length=0 -fopenmp -static -std=c++11")
  SET(COMPILE_OPTIONS_OPTI "-DBOOST_THREAD_USE_LIB=1 -DBOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN -DOPTIMIZE -O2 -g0 -Wall  -Wno-deprecated-declarations -fmessage-length=0 -fopenmp -static -std=c++11")
  SET(COMPILE_OPTIONS_TEST "-DBOOST_THREAD_USE_LIB=1 -DBOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN -DTEST -O0 -g3 -Wall -Wno-deprecated-declarations -fmessage-length=0 -fopenmp -static -std=c++11")
  ADD_CUSTOM_TARGET(icon ALL
    COMMAND windres --output ${PROJECT_SOURCE_DIR}/icon.o --input ${PROJECT_SOURCE_DIR}/spm.rc
  )
  SET(MAIN_LIBRARIES
    gomp
    pthread
    boost_system-mgw81-mt-s-1_58
    boost_filesystem-mgw81-mt-s-1_58
    boost_thread-mgw81-mt-s-1_58
    boost_program_options-mgw81-mt-s-1_58
    boost_chrono-mgw81-mt-s-1_58
  )
  SET(TEST_LIBRARIES boost_unit_test_framework-mgw81-mt-sd-1_58)
  
ELSE (WIN32)
  # wellopensuse131 link directories
  SET(BOOST_LIBS "/opt/boost/boost_1_57_0/stage/lib/")
  SET(BOOST_HEADERS "/opt/boost/boost_1_57_0/")
  SET(LINK_OPTIONS "-static -L ${BOOST_LIBS}")
  #SET(LINK_OPTIONS "-L ${BOOST_LIBS}")
  SET(COMPILE_OPTIONS_LIBR "-DBOOST_THREAD_USE_LIB=1 -O0 -g3 -ggdb -Wall -Werror -fmessage-length=0 -fopenmp -Wno-unused-local-typedefs -I ${BOOST_HEADERS} -std=c++11")
  SET(COMPILE_OPTIONS_MAIN "-DBOOST_THREAD_USE_LIB=1 -O0 -g3 -Wall -fmessage-length=0 -fopenmp -Wno-unused-local-typedefs -I ${BOOST_HEADERS} -std=c++11")
  SET(COMPILE_OPTIONS_OPTI "-DBOOST_THREAD_USE_LIB=1 -DOPTIMIZE -O3 -g0 -Wall -fmessage-length=0 -fopenmp -Wno-unused-local-typedefs -I ${BOOST_HEADERS} -std=c++11")
  SET(COMPILE_OPTIONS_TEST "-DBOOST_THREAD_USE_LIB=1 -DTEST -O0 -g3 -Wall -fmessage-length=0 -fopenmp -Wno-unused-local-typedefs -I ${BOOST_HEADERS} -std=c++11")
  SET(MAIN_LIBRARIES
    boost_system
    boost_filesystem
    boost_thread
    boost_program_options    
    gomp
    pthread
    )
  SET(TEST_LIBRARIES boost_unit_test_framework)
ENDIF (WIN32)

FILE(GLOB_RECURSE SOURCE_FILES ../Source/*.cpp)
FILE(GLOB_RECURSE TEST_FILES ../Source/*.Test.cpp)
list(REMOVE_ITEM SOURCE_FILES ${TEST_FILES})

# Build SPM unoptimised
ADD_EXECUTABLE(${MAIN_NAME} ${SOURCE_FILES})
SET_TARGET_PROPERTIES(${MAIN_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_OPTIONS_MAIN} LINK_FLAGS ${LINK_OPTIONS})
TARGET_LINK_LIBRARIES(${MAIN_NAME} ${MAIN_LIBRARIES})

# Build SPM unit tests
ADD_EXECUTABLE(${TEST_NAME} ${SOURCE_FILES} ${TEST_FILES})
SET_TARGET_PROPERTIES(${TEST_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_OPTIONS_TEST} LINK_FLAGS ${LINK_OPTIONS})
TARGET_LINK_LIBRARIES(${TEST_NAME} ${MAIN_LIBRARIES} ${TEST_LIBRARIES})

# Build SPM optimised
ADD_EXECUTABLE(${OPTI_NAME} ${SOURCE_FILES})
SET_TARGET_PROPERTIES(${OPTI_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_OPTIONS_OPTI} LINK_FLAGS ${LINK_OPTIONS})
TARGET_LINK_LIBRARIES(${OPTI_NAME} ${MAIN_LIBRARIES})

ADD_DEPENDENCIES(${MAIN_NAME} icon)
ADD_DEPENDENCIES(${TEST_NAME} icon)
ADD_DEPENDENCIES(${OPTI_NAME} icon)
