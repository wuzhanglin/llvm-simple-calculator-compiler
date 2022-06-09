FILE(GLOB_RECURSE SOURCES "*.cpp")
FILE(GLOB_RECURSE HEADERS "*.h")

IF(CMAKE_GENERATOR MATCHES "Xcode")
  add_executable(CalculatorCompiler ${SOURCES} ${HEADERS})
ELSE()
  add_executable(CalculatorCompiler ${SOURCES})
ENDIF()

target_link_libraries(CalculatorCompiler PRIVATE ${llvm_libs})
