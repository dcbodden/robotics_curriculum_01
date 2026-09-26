set(sources
    "btdiag.cpp"
    "main.c"
    "sketch.cpp"
)

set(component_dependencies
    "arduino"
    "bluepad32"
    "bluepad32_arduino"
    "btstack"
)

idf_component_register(
    SRCS "${sources}"
    INCLUDE_DIRS "."
    REQUIRES "${component_dependencies}"
)
