cmake_minimum_required(VERSION 3.16)
project("SYNTH" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(OUTPUT_DIR "../bin")


set(BUILD_DIR
    "${PROJECT_SOURCE_DIR}/build/")
set(SOURCES_DIR        
    "${PROJECT_SOURCE_DIR}/src/")
set(SOURCES
    ${SOURCES_DIR}/main.cpp
    ${SOURCES_DIR}/Synth.cpp
    #Graphics Sources
    ${SOURCES_DIR}/GUI/gui.cpp
    ${SOURCES_DIR}/GUI/Widget.cpp
    ${SOURCES_DIR}/GUI/Button.cpp
    ${SOURCES_DIR}/GUI/Slider.cpp
    #SYNTH/Synthesizer Sources
    ${SOURCES_DIR}/Synth/audioEngine.cpp
    ${SOURCES_DIR}/Synth/Oscillator.cpp
    ${SOURCES_DIR}/Synth/keyboard.cpp
    ${SOURCES_DIR}/Synth/Amplifier.cpp
    ${SOURCES_DIR}/Synth/Envelops.cpp
    ${SOURCES_DIR}/Synth/Filter.cpp
    ${SOURCES_DIR}/Synth/LadderFilter.cpp
    ${SOURCES_DIR}/Synth/LFO.cpp
    ${SOURCES_DIR}/Synth/Mixer.cpp
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
    SYNTH
    ${SOURCES}
)

target_include_directories(
    SYNTH 
    PUBLIC ${INCLUDES}
    PUBLIC ${EXTERNALS}/SDL/include/
    PUBLIC ${EXTERNALS}/SDL_image/
    PUBLIC ${EXTERNALS}/portaudio/include/
    PUBLIC ${EXTERNALS}/libremidi/include
)

target_link_directories(
    SYNTH
    #add directories where libs are
    PRIVATE ${BUILD_DIR}/ext/libremidi/Release/
    PRIVATE ${BUILD_DIR}/ext/portaudio/Release/
    PRIVATE ${BUILD_DIR}/ext/SDL/Release
    PRIVATE ${BUILD_DIR}/ext/SDL_image/Release
)

target_link_libraries(
    SYNTH 
    #add librairies here
    PUBLIC winmm.lib
    PUBLIC libremidi.lib
    PUBLIC portaudio.lib
    PUBLIC SDL2main.lib
    PUBLIC SDL2.lib
    PUBLIC SDL2_image.lib
    )


#set_target_properties( 
#    SYNTH PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
#)


#add_custom_command(TARGET SYNTH POST_BUILD
#COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    ${BUILD_DIR}/ext/SDL/Release/SDL2.dll
#    ${BUILD_DIR}/ext/SDL_image/Release/SDL2_image.dll
#    ${BUILD_DIR}/ext/SDL_image/external/libpng-1.6.37/Release/libpng16.dll
#    ${BUILD_DIR}/ext/SDL_image/external/zlib-1.2.11/Release/zlib1.dll
#    ${BUILD_DIR}/ext/portaudio/Release/portaudio.dll
#    ${BUILD_DIR}/ext/rtmidi/Release/rtmidi.dll
#    $<TARGET_FILE_DIR:SYNTH>)




