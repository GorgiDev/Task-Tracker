cmake_minimum_required(VERSION 3.16)

project(TaskTrackerApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)  # Enables .qrc resource support
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Source files
set(SOURCES
    src/main.cpp
    src/TaskTrackerApp.cpp
    src/TaskWidget.cpp
    src/AddTaskDialog.cpp
    src/EditTaskDialog.cpp
    src/Task.cpp
    assets/resources.qrc
    winResources.rc
)

# Header files (optional for IDEs)
set(HEADERS
    include/TaskTrackerApp.h
    include/TaskWidget.h
    include/AddTaskDialog.h
    include/EditTaskDialog.h
    include/Task.h
)

add_executable(TaskTrackerApp ${SOURCES})

if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/winResources.rc")
    target_sources(TaskTrackerApp PRIVATE ${APP_ICON_RESOURCE_WINDOWS})
endif()

target_sources(TaskTrackerApp
    PRIVATE
        include/TaskTrackerApp.h
        include/TaskWidget.h
        include/EditTaskDialog.h
        include/AddTaskDialog.h
        include/Task.h
)

set_target_properties(TaskTrackerApp PROPERTIES
    WIN32_EXECUTABLE ON
)

target_include_directories(TaskTrackerApp PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(TaskTrackerApp PRIVATE Qt6::Core Qt6::Widgets)