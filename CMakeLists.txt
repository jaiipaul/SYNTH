cmake_minimum_required(VERSION 3.16)
project("AUDIO" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(OUTPUT_DIR "../bin")

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
    #Audio/Synthesizer Sources
    ${SOURCES_DIR}/Synth/audioEngine.cpp
    ${SOURCES_DIR}/Synth/Oscillator.cpp
    ${SOURCES_DIR}/Synth/keyboard.cpp
    ${SOURCES_DIR}/Synth/Amplifier.cpp
    ${SOURCES_DIR}/Synth/Envelops.cpp
    ${SOURCES_DIR}/Synth/Filter.cpp
)

set(INCLUDES       
    "${PROJECT_SOURCE_DIR}/include/"
    "${PROJECT_SOURCE_DIR}/include/GUI"
    "${PROJECT_SOURCE_DIR}/include/Synth")

set(SDL2           "${PROJECT_SOURCE_DIR}/libs/SDL2/")
set(SDL2_image     "${PROJECT_SOURCE_DIR}/libs/SDL2_image/")
set(PORTAUDIO      "${PROJECT_SOURCE_DIR}/libs/portaudio")

include_directories( 
    ${INCLUDES}   
    ${SDL2}/include
    ${SDL2_image}/include
    ${PORTAUDIO}/include
)

add_executable(
    AUDIO
    ${SOURCES}
)

target_link_libraries(
    AUDIO 
    #add librairies here
    PUBLIC ${PORTAUDIO}/build/Release/portaudio.lib
    PUBLIC ${SDL2}/lib/x64/SDL2.lib
    PUBLIC ${SDL2}/lib/x64/SDL2main.lib
    PUBLIC ${SDL2_image}/lib/x64/SDL2_image.lib
    )


target_include_directories(
    AUDIO
    PRIVATE ${INCLUDES}
    ${SDL2}/include
    ${SDL2_image}/include
    ${PORTAUDIO}/include
)

set_target_properties( 
    AUDIO PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

add_custom_command(TARGET AUDIO POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2}/lib/x64/SDL2.dll
        ${SDL2_image}/lib/x64/SDL2_image.dll
        ${SDL2_image}/lib/x64/libpng16-16.dll
        ${SDL2_image}/lib/x64/zlib1.dll
        ${PORTAUDIO}/build/Release/portaudio.dll
        $<TARGET_FILE_DIR:AUDIO>)


