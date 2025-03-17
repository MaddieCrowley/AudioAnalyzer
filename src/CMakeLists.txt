 project(AudioAnalyzer VERSION 0.0.1 LANGUAGES CXX)

 set(CMAKE_CXX_STANDARD 17)
 set(CMAKE_CXX_STANDARD_REQUIRED ON)

 set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

 option(X11 "X11 native version" OFF)
 option(SDL3 "SDL3 version" ON)


 option(PULSE "pulse-audio native support" ON)
 option(RTAUDIO "RTAUDIO version" OFF)

 if (X11)
     find_package(X11 REQUIRED)
     list(APPEND EXTRA_LIBS ${X11_LIBRARIES})
     list(APPEND EXTRA_SOURCES gui/xwin.cpp)
     add_compile_definitions(X11)
 elseif (SDL3)
     find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
     list(APPEND EXTRA_LIBS SDL3::SDL3)
     list(APPEND EXTRA_SOURCES gui/sdl.cpp)
     add_compile_definitions(SDL3)
 endif ()

 if (PULSE)
     #FIND PULSE AUDIO
     find_library(PULSEAUDIO_LIBRARY
             NAMES pulse
             DOC "The PulseAudio library"
     )
     list(APPEND EXTRA_LIBS pulse-simple)
     list(APPEND audio/pulse.cpp)
     add_compile_definitions(RTAUDIO)
 elseif (RTAUDIO)
     #FIND RT AUDIO
     find_library(RTAUDIO_LIBRARY
             NAMES rtaudio
     )
     list(APPEND EXTRA_LIBS rtaudio)
     list(APPEND EXTRA_SOURCES audio/rt.cpp)
     add_compile_definitions(RTAUDIO)
 endif ()

 add_executable(AudioAnalyzer main.cpp ${EXTRA_SOURCES}
 )

 target_link_libraries(AudioAnalyzer
 PRIVATE ${EXTRA_LIBS})
