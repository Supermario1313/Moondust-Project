# Major
set(CALIBRATOR_VERSION_1 2)
# Minor
set(CALIBRATOR_VERSION_2 2)
# Revision
set(CALIBRATOR_VERSION_3 0)
# Patch
set(CALIBRATOR_VERSION_4 0)
# Type of version: "-alpha","-beta","-dev", or "" aka "release"
set(CALIBRATOR_VERSION_REL "")

add_definitions(-DCALIBRATOR_VERSION_1=${CALIBRATOR_VERSION_1})
add_definitions(-DCALIBRATOR_VERSION_2=${CALIBRATOR_VERSION_2})
add_definitions(-DCALIBRATOR_VERSION_3=${CALIBRATOR_VERSION_3})
add_definitions(-DCALIBRATOR_VERSION_4=${CALIBRATOR_VERSION_4})
add_definitions(-DCALIBRATOR_VERSION_REL=${CALIBRATOR_VERSION_REL})

set(CALIBRATOR_VERSION_STRING "${CALIBRATOR_VERSION_1}.${CALIBRATOR_VERSION_2}")

if(NOT ${CALIBRATOR_VERSION_3} EQUAL 0 OR NOT ${CALIBRATOR_VERSION_4} EQUAL 0)
    string(CONCAT CALIBRATOR_VERSION_STRING "${CALIBRATOR_VERSION_STRING}" ".${CALIBRATOR_VERSION_3}")
endif()

if(NOT ${CALIBRATOR_VERSION_4} EQUAL 0)
    string(CONCAT CALIBRATOR_VERSION_STRING "${CALIBRATOR_VERSION_STRING}" ".${CALIBRATOR_VERSION_4}")
endif()

if(NOT "${CALIBRATOR_VERSION_REL}" STREQUAL "")
    string(CONCAT CALIBRATOR_VERSION_STRING "${CALIBRATOR_VERSION_STRING}" "${CALIBRATOR_VERSION_REL}")
endif()

message("== Playable character Calibrator version ${CALIBRATOR_VERSION_STRING} ==")

