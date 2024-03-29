cmake_minimum_required(VERSION 3.16)
project(SYNTH LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin")
set(BUILD_SHARED_LIBS OFF)

set(BUILD_DIR
    "${PROJECT_SOURCE_DIR}/build/")
set(SOURCES_DIR        
    "${PROJECT_SOURCE_DIR}/src/")
set(SOURCES
    #Graphics Sources
    ${SOURCES_DIR}/GUI/gui.cpp
    ${SOURCES_DIR}/GUI/Widget.cpp
    ${SOURCES_DIR}/GUI/Button.cpp
    ${SOURCES_DIR}/GUI/Slider.cpp
    #Synthesizer Sources
    ${SOURCES_DIR}/Synth/audioEngine.cpp
    ${SOURCES_DIR}/Synth/Oscillator.cpp
    ${SOURCES_DIR}/Synth/keyboard.cpp
    ${SOURCES_DIR}/Synth/Amplifier.cpp
    ${SOURCES_DIR}/Synth/Envelops.cpp
    ${SOURCES_DIR}/Synth/Filter.cpp
    ${SOURCES_DIR}/Synth/LadderFilter.cpp
    ${SOURCES_DIR}/Synth/LFO.cpp
    ${SOURCES_DIR}/Synth/Mixer.cpp

    ${SOURCES_DIR}/main.cpp
    ${SOURCES_DIR}/Synth.cpp
)

set(EXTERNALS
    "${PROJECT_SOURCE_DIR}/ext")

set(INCLUDES
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/GUI"
    "${PROJECT_SOURCE_DIR}/src/Synth"
    )
#set(SDL2           "${PROJECT_SOURCE_DIR}/libs/SDL2/")
#set(SDL2_image     "${PROJECT_SOURCE_DIR}/libs/SDL2_image/")
#set(PORTSYNTH      "${PROJECT_SOURCE_DIR}/libs/portaudio")
#set(LIBREMIDI      "${PROJECT_SOURCE_DIR}/libs/libremidi")

add_subdirectory(${EXTERNALS}/SDL)
add_subdirectory(${EXTERNALS}/SDL_image)
add_subdirectory(${EXTERNALS}/portaudio)
add_subdirectory(${EXTERNALS}/libremidi)

add_executable(
    Synth
    ${SOURCES}
)

add_dependencies(Synth
    SDL2-static
    SDL2_image
    PortAudio
    libremidi)

target_include_directories(
    Synth 
    PUBLIC ${INCLUDES}
    PUBLIC ${EXTERNALS}/SDL/include/
    PUBLIC ${EXTERNALS}/SDL_image/
    PUBLIC ${EXTERNALS}/portaudio/include/
    PUBLIC ${EXTERNALS}/libremidi/include
)

target_link_libraries(
    Synth
    #add librairies here
    PUBLIC libremidi
    PUBLIC PortAudio
    PUBLIC SDL2-static
    PUBLIC SDL2main
    PUBLIC SDL2_image
    )

#Copying application to bin directory
add_custom_command(TARGET Synth POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:Synth>
    ${OUTPUT_DIR})





