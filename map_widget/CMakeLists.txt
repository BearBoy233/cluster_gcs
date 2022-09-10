##############################################################################
# CMake
##############################################################################

# cmake_minimum_required(VERSION 2.8.0)
cmake_minimum_required(VERSION 3.1.2)

project(map_widget)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

##############################################################################
# Catkin
##############################################################################

# qt_build provides the qt cmake glue, roscpp the comms for a default talker
# find_package(catkin REQUIRED COMPONENTS qt_build roscpp)
# include_directories(${catkin_INCLUDE_DIRS})

set(QT Core Gui Widgets OpenGL Qml QuickWidgets Network Positioning Sensors Multimedia WebEngineWidgets WebKitWidgets)
set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL Qt5::Qml Qt5::QuickWidgets 
        Qt5::Network Qt5::Positioning Qt5::Sensors Qt5::Multimedia Qt5::WebEngineWidgets Qt5::WebKitWidgets
   )
find_package(Qt5 REQUIRED ${QT})

find_package(OpenCV REQUIRED) # 3 /4 
find_package(catkin REQUIRED COMPONENTS
    roscpp
    message_generation
    rviz
    OpenCV
    std_msgs
    sensor_msgs
    cv_bridge
    image_transport

    mavcomm_msgs
)

include_directories(	
	${catkin_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
	# ${CMAKE_CURRENT_BINARY_DIR}/..
)

#generate_messages(
#  DEPENDENCIES
  #geometry_msgs nav_msgs std_msgs sensor_msgs
#)

# Use this to define what the package will export (e.g. libs, headers).
# Since the default here is to produce only a binary, we don't worry about
# exporting anything. 
# catkin_package()

catkin_package(
	INCLUDE_DIRS include
	#  LIBRARIES control DEPENDS system_lib
   	CATKIN_DEPENDS roscpp
)

##############################################################################
# Qt Environment
##############################################################################
# this comes from qt_build's qt-ros.cmake which is automatically 
# included via the dependency call in package.xml
# rosbuild_prepare_qt4(QtCore QtGui) # Add the appropriate components to the component list here
##############################################################################
# Sections
##############################################################################
# file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
# file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
# file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/map_widget/*.hpp)
# QT4_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
# QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
# QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})
##############################################################################
# Sources
##############################################################################
# file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)
##############################################################################
# Binaries
##############################################################################
# add_executable(map_widget ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
# target_link_libraries(map_widget ${QT_LIBRARIES} ${catkin_LIBRARIES})
# install(TARGETS map_widget RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
QT5_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})

file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
QT5_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})

file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/map_widget/*.hpp)
QT5_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)

add_executable(map_widget ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP} ${HEADERS})
target_link_libraries(map_widget ${QT_LIBRARIES} ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
target_link_libraries( map_widget -lpthread -lavcodec  -lavformat  -lavutil  -lswscale -lswresample -lavdevice -lavfilter)

install(TARGETS map_widget RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

message(STATUS "opencv version: ${OpenCV_VERSION}")
message(STATUS "opencv dir: ${OpenCV_INCLUDE_DIRS}")

