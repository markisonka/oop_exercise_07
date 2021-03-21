cmake_minimum_required(VERSION 3.0)

project(lab7x)

set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_STANDARD 17)

set(FIGS "FiguresAndFactories")
set(TRAP "${FIGS}/Polygon/Trapeze")
set(POLY "${FIGS}/Polygon")
set(ROMB "${FIGS}/Polygon/Rhombus")
set(RANDPOLY "${FIGS}/Polygon/RandomPolygon")
set(CIRC "${FIGS}/Circle")
set(LINE "${FIGS}/Polygon/Line")

add_executable(lab7x main.cpp sdl.cpp
        ${FIGS}/Point.cpp ${FIGS}/Point.h
        ${FIGS}/Figure.h ${FIGS}/Figure.cpp ${FIGS}/Factory.h
        ${POLY}/Polygon.h ${POLY}/Polygon.cpp
        ${CIRC}/Circle.h ${CIRC}/Circle.cpp ${CIRC}/CircleFactory.h ${CIRC}/CircleFactory.cpp
        ${LINE}/Line.h ${LINE}/Line.cpp ${LINE}/LineFactory.h ${LINE}/LineFactory.cpp
        ${TRAP}/Trapeze.h ${TRAP}/Trapeze.cpp ${TRAP}/TrapezeFactory.h ${TRAP}/TrapezeFactory.cpp
        ${ROMB}/Rhombus.h ${ROMB}/Rhombus.cpp ${ROMB}/RhombusFactory.h ${ROMB}/RhombusFactory.cpp
        ${RANDPOLY}/RandomPolygon.h ${RANDPOLY}/RandomPolygon.cpp ${RANDPOLY}/RandomPolygonFactory.h ${RANDPOLY}/RandomPolygonFactory.cpp
        Document.h Document.cpp
        Redactor.h Redactor.cpp)


add_subdirectory(lib/SDL2/)
target_link_libraries(lab7x SDL2-static)
target_include_directories(lab7x PRIVATE ${SDL2_INCLUDE_DIR})

add_subdirectory(lib/imgui/)
target_include_directories(imgui PRIVATE lib/SDL2/include/)
target_link_libraries(lab7x imgui)
