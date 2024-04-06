set(PROJECT TrendViewOpenSource)

file(GLOB SOURCE *.cpp)
file(GLOB HEADER *.h)
file(GLOB QCUSTOMPLOT_SOURCE "../qcustomplot/*.cpp")
file(GLOB QCUSTOMPLOT_HEADER "../qcustomplot/*.h")

include_directories(
    "../qcustomplot"
)

find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

add_compile_options(-DTRENDVIEWOPENSOURCE_LIB)
add_compile_options(-DQCUSTOMPLOT_COMPILE_LIBRARY)

qt5_wrap_cpp(moc_sources ../qcustomplot/qcustomplot.h)

add_library(${PROJECT} SHARED ${SOURCE} ${HEADER} ${QCUSTOMPLOT_SOURCE} ${QCUSTOMPLOT_HEADER}  ${moc_sources})
target_link_libraries(${PROJECT} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport )
set_property(TARGET ${PROJECT} PROPERTY FOLDER "${THIS_FOLDER}")

source_group("src" FILES ${lib_SOURCES})
source_group("headers" FILES ${lib_HEADERS})