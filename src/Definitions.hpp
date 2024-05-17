#pragma once

// GLFW window hints

#ifndef BASIL_GLFW_VERSION_MAJOR
    #define BASIL_GLFW_VERSION_MAJOR 4
#endif

#ifndef BASIL_GLFW_VERSION_MINOR
    #define BASIL_GLFW_VERSION_MINOR 5
#endif

#ifndef BASIL_GLFW_UNINITIALIZED_WINDOW_TITLE
    #define BASIL_GLFW_UNINITIALIZED_WINDOW_TITLE "Uninitialized BasilGL Window"
#endif


// Logging defaults

#ifndef BASIL_DEFAULT_LOGGING_LEVEL
    #ifdef DEBUG_BUILD
        #define BASIL_DEFAULT_LOGGING_LEVEL LogLevel::DEBUG
    #else
        #define BASIL_DEFAULT_LOGGING_LEVEL LogLevel::INFO
    #endif
#endif


// Process defaults

#ifndef BASIL_DEFAULT_PROCESS_ORDINAL
    #define BASIL_DEFAULT_PROCESS_ORDINAL ProcessOrdinal::MAIN
#endif

#ifndef BASIL_DEFAULT_PROCESS_PRIVILEGE
    #define BASIL_DEFAULT_PROCESS_PRIVILEGE ProcessPrivilege::NONE
#endif

#ifndef BASIL_DEFAULT_PROCESS_NAME
    #define BASIL_DEFAULT_PROCESS_NAME "unnamed process"
#endif


// Window defaults

#ifndef BASIL_DEFAULT_WINDOW_WIDTH
    #define BASIL_DEFAULT_WINDOW_WIDTH 400
#endif

#ifndef BASIL_DEFAULT_WINDOW_HEIGHT
    #define BASIL_DEFAULT_WINDOW_HEIGHT 600
#endif

#ifndef BASIL_DEFAULT_WINDOW_TITLE
    #define BASIL_DEFAULT_WINDOW_TITLE "BasilGL"
#endif

#ifndef BASIL_DEFAULT_WINDOW_WIDTH_TEST
    #define BASIL_DEFAULT_WINDOW_WIDTH_TEST 2
#endif

#ifndef BASIL_DEFAULT_WINDOW_HEIGHT_TEST
    #define BASIL_DEFAULT_WINDOW_HEIGHT_TEST 1
#endif

#ifndef BASIL_INVERT_CURSOR_Y_AXIS
    // Defaults to true to match UV coordinates with mouse coordinates
    #define BASIL_INVERT_CURSOR_Y_AXIS true
#endif

#ifndef BASIL_PIXEL_ASPECT_RATIO
    // Defaults to square pixels
    #define BASIL_PIXEL_ASPECT_RATIO 1.0
#endif



