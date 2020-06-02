# Makefile for the codegen

# sources for codegen
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CODEGEN_SRCS
    src/actionDeclaration.cpp
    src/actionDeclaration.h
    src/actionList.cpp
    src/actionList.h
    src/annotations.cpp
    src/annotations.h
    src/argument.cpp
    src/argument.h
    src/assignmentOrMethodCallStatement.cpp
    src/assignmentOrMethodCallStatement.h
    src/backends/top.cpp
    src/backends/top.h
    src/backends/tna.cpp
    src/backends/tna.h
    src/backends/v1model.cpp
    src/backends/v1model.h
    src/baseType.cpp
    src/baseType.h
    src/blockStatement.cpp
    src/blockStatement.h
    src/common.cpp
    src/common.h
    src/conditionalStatement.cpp
    src/conditionalStatement.h
    src/constantDeclaration.cpp
    src/constantDeclaration.h
    src/controlDeclaration.cpp
    src/controlDeclaration.h
    src/controlLocalDeclaration.cpp
    src/controlLocalDeclaration.h
    src/controlLocalDeclarations.cpp
    src/controlLocalDeclarations.h
    src/controlTypeDeclaration.cpp
    src/controlTypeDeclaration.h
    src/declaration.cpp
    src/declaration.h
    src/derivedTypeDeclaration.cpp
    src/derivedTypeDeclaration.h
    src/directApplication.cpp
    src/directApplication.h
    src/direction.cpp
    src/direction.h
    src/emptyStatement.cpp
    src/emptyStatement.h
    src/enumDeclaration.cpp
    src/enumDeclaration.h
    src/errorDeclaration.cpp
    src/errorDeclaration.h
    src/exitStatement.cpp
    src/exitStatement.h
    src/externDeclaration.cpp
    src/externDeclaration.h
    src/expression.cpp
    src/expression.h
    src/expression_boolean.cpp
    src/expression_boolean.h
    src/expression_bit.cpp
    src/expression_bit.h
    src/expression_struct.cpp
    src/expression_struct.h
    src/expressionList.cpp
    src/expressionList.h
    src/functionDeclaration.cpp
    src/functionDeclaration.h
    src/headerStackType.cpp
    src/headerStackType.h
    src/headerTypeDeclaration.cpp
    src/headerTypeDeclaration.h
    src/headerUnionDeclaration.cpp
    src/headerUnionDeclaration.h
    src/identifierList.cpp
    src/identifierList.h
    src/keyElement.cpp
    src/keyElement.h
    src/keyElementList.cpp
    src/keyElementList.h
    src/main.cpp
    src/main.cpp
    src/matchKindDeclaration.cpp
    src/matchKindDeclaration.h
    src/member.cpp
    src/member.h
    src/name.cpp
    src/name.h
    src/nonTableKwName.cpp
    src/nonTableKwName.h
    src/nonTypeName.cpp
    src/nonTypeName.h
    src/optConstructorParameters.cpp
    src/optConstructorParameters.h
    src/optInitializer.cpp
    src/optInitializer.h
    src/optTypeParameters.cpp
    src/optTypeParameters.h
    src/p4parser.cpp
    src/p4parser.h
    src/p4state.cpp
    src/p4state.h
    src/packageTypeDeclaration.cpp
    src/packageTypeDeclaration.h
    src/parameter.cpp
    src/parameter.h
    src/parameterList.cpp
    src/parameterList.h
    src/prefixedNonTypeName.cpp
    src/prefixedNonTypeName.h
    src/prefixedType.cpp
    src/prefixedType.h
    src/realTypeArg.cpp
    src/realTypeArg.h
    src/realTypeArgumentList.cpp
    src/realTypeArgumentList.h
    src/returnStatement.cpp
    src/returnStatement.h
    src/scope.cpp
    src/scope.h
    src/specializedType.cpp
    src/specializedType.h
    src/specifiedIdentifier.cpp
    src/specifiedIdentifier.h
    src/specifiedIdentifierList.cpp
    src/specifiedIdentifierList.h
    src/statOrDeclList.cpp
    src/statOrDeclList.h
    src/statement.cpp
    src/statement.h
    src/statementOrDeclaration.cpp
    src/statementOrDeclaration.h
    src/structField.cpp
    src/structField.h
    src/structFieldList.cpp
    src/structFieldList.h
    src/structTypeDeclaration.cpp
    src/structTypeDeclaration.h
    src/switchStatement.cpp
    src/switchStatement.h
    src/tableDeclaration.cpp
    src/tableDeclaration.h
    src/tableProperty.cpp
    src/tableProperty.h
    src/tablePropertyList.cpp
    src/tablePropertyList.h
    src/tupleType.cpp
    src/tupleType.h
    src/typeArg.cpp
    src/typeArg.h
    src/typeArgumentList.cpp
    src/typeArgumentList.h
    src/typeDeclaration.cpp
    src/typeDeclaration.h
    src/typeName.cpp
    src/typeName.h
    src/typeOrVoid.cpp
    src/typeOrVoid.h
    src/typeParameterList.cpp
    src/typeParameterList.h
    src/typeRef.cpp
    src/typeRef.h
    src/typedefDeclaration.cpp
    src/typedefDeclaration.h
    src/variableDeclaration.cpp
    src/variableDeclaration.h
)
add_cpplint_files (${CMAKE_CURRENT_SOURCE_DIR} "${CODEGEN_SRCS}")

#build_unified(CODEGEN_SRCS)
add_executable(p4bludgeon ${CODEGEN_SRCS})
# message (STATUS "libs ${P4C_LIBRARIES}")
# message (STATUS "deps ${P4C_LIB_DEPS}")
target_link_libraries (p4bludgeon ${P4C_LIBRARIES} ${P4C_LIB_DEPS})
add_dependencies(p4bludgeon genIR frontend)

install(TARGETS p4bludgeon RUNTIME DESTINATION ${P4C_RUNTIME_OUTPUT_DIRECTORY})

add_custom_target(linkp4bludgeon
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/p4bludgeon ${P4C_BINARY_DIR}/p4bludgeon
#  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/p4include ${P4C_BINARY_DIR}/p4include
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${P4C_BINARY_DIR}/p4include ${CMAKE_CURRENT_BINARY_DIR}/p4include
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${P4C_BINARY_DIR}/p4_14include ${CMAKE_CURRENT_BINARY_DIR}/p4_14include
  )
add_dependencies(p4c_driver linkp4bludgeon)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
