SET(SOURCES finite_field.c field_element.c binary_field_extension.c polynom.c)
SET(HEADERS finite_field.h field_element.h binary_field_extension.h finite_fields.h polynom.h)
add_library(FiniteFields STATIC ${HEADERS} ${SOURCES})

target_include_directories(FiniteFields PUBLIC .)
