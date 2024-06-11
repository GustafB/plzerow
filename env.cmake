# parses an optional .env file and sets contained variables
# this is done before creating the project in the event
# that you want to specify a certain compiler for example.

set(ENV_FILE_PATH "${CMAKE_SOURCE_DIR}/.env")

if(EXISTS "${ENV_FILE_PATH}")
    file(READ "${ENV_FILE_PATH}" ENV_FILE_CONTENTS)

    string(REGEX REPLACE ";" "\\\\;" ENV_FILE_CONTENTS_ESCAPED "${ENV_FILE_CONTENTS}")
    string(REPLACE "\n" ";" ENV_FILE_LINES "${ENV_FILE_CONTENTS_ESCAPED}")

    foreach(ENV_LINE IN LISTS ENV_FILE_LINES)
        if (NOT ENV_LINE MATCHES "^#.*" AND NOT ENV_LINE STREQUAL "")
            string(REPLACE "=" ";" ENV_KV_PAIR "${ENV_LINE}")
            list(GET ENV_KV_PAIR 0 ENV_KEY)
            list(GET ENV_KV_PAIR 1 ENV_VALUE)

            set(ENV{${ENV_KEY}} ${ENV_VALUE})
            message(STATUS "Setting env var: ${ENV_KEY}=${ENV_VALUE}")
        endif()
    endforeach()
else()
    message(STATUS "No .env file found at ${ENV_FILE_PATH}. Skipping environment variable setup.")
endif()

