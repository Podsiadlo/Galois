set(LIBMGRS src/libmgrs/mgrs.c src/libmgrs/polarst.c src/libmgrs/tranmerc.c
        src/libmgrs/utm.c src/libmgrs/ups.c)
set(SOURCES src/model/Map.cpp
        src/readers/SrtmReader.cpp src/readers/AsciiReader.cpp
        src/utils/Utils.cpp
        src/readers/InpReader.cpp #[[src/writers/InpWriter.cpp]]
        #[[src/writers/TriangleFormatWriter.cpp]] src/model/Edge.h
        src/productions/Production01.h src/productions/Production0x.cpp
        src/model/GraphAdapter.cpp src/model/GraphAdapter.h src/utils/ProductionHelpers.cpp
        src/productions/Production02.h src/writers/AvsUcdWriter.cpp ${LIBMGRS})

add_executable(longestedge2-cpu src/LongestEdge2.cpp ${SOURCES})

#add_dependencies(longestedge2-cpu )
add_dependencies(apps longestedge2-cpu)
target_link_libraries(longestedge2-cpu PRIVATE Galois::shmem lonestar)
install(TARGETS longestedge2-cpu DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT apps EXCLUDE_FROM_ALL)

add_executable(longestedge2Test test/TestMain.cpp ${SOURCES})
add_dependencies(longestedge2Test longestedge2-cpu)
add_dependencies(apps longestedge2Test)
target_link_libraries(longestedge2Test PRIVATE Galois::shmem lonestar)

add_test(test_longest_edge2 longestedge2Test)
