set(sources
    "actuator_output.cpp"
    "btdiag.cpp"
    "control_telemetry.cpp"
    "main.c"
    "shared_console.cpp"
    "sketch.cpp"
)

set(component_dependencies
    "arduino"
    "bluepad32"
    "bluepad32_arduino"
    "btstack"
    "esp_driver_uart"
)

idf_component_register(
    SRCS "${sources}"
    INCLUDE_DIRS "."
    REQUIRES "${component_dependencies}"
)
