# Makefile for the codegen

# sources for codegen
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CODEGEN_SRCS
    actionDeclaration.cpp
    actionDeclaration.h
    actionList.cpp
    actionList.h
    annotations.cpp
    annotations.h
    argument.cpp
    argument.h
    assignmentOrMethodCallStatement.cpp
    assignmentOrMethodCallStatement.h
    backends/top.cpp
    backends/top.h
    backends/tna.cpp
    backends/tna.h
    backends/v1model.cpp
    backends/v1model.h
    baseType.cpp
    baseType.h
    blockStatement.cpp
    blockStatement.h
    common.cpp
    common.h
    conditionalStatement.cpp
    conditionalStatement.h
    constantDeclaration.cpp
    constantDeclaration.h
    controlDeclaration.cpp
    controlDeclaration.h
    controlLocalDeclaration.cpp
    controlLocalDeclaration.h
    controlLocalDeclarations.cpp
    controlLocalDeclarations.h
    controlTypeDeclaration.cpp
    controlTypeDeclaration.h
    declaration.cpp
    declaration.h
    derivedTypeDeclaration.cpp
    derivedTypeDeclaration.h
    directApplication.cpp
    directApplication.h
    direction.cpp
    direction.h
    emptyStatement.cpp
    emptyStatement.h
    enumDeclaration.cpp
    enumDeclaration.h
    errorDeclaration.cpp
    errorDeclaration.h
    exitStatement.cpp
    exitStatement.h
    expression.cpp
    expression.h
    expression_boolean.cpp
    expression_boolean.h
    expression_bit.cpp
    expression_bit.h
    expression_struct.cpp
    expression_struct.h
    expressionList.cpp
    expressionList.h
    functionDeclaration.cpp
    functionDeclaration.h
    headerStackType.cpp
    headerStackType.h
    headerTypeDeclaration.cpp
    headerTypeDeclaration.h
    headerUnionDeclaration.cpp
    headerUnionDeclaration.h
    identifierList.cpp
    identifierList.h
    keyElement.cpp
    keyElement.h
    keyElementList.cpp
    keyElementList.h
    main.cpp
    main.cpp
    matchKindDeclaration.cpp
    matchKindDeclaration.h
    member.cpp
    member.h
    name.cpp
    name.h
    nonTableKwName.cpp
    nonTableKwName.h
    nonTypeName.cpp
    nonTypeName.h
    optConstructorParameters.cpp
    optConstructorParameters.h
    optInitializer.cpp
    optInitializer.h
    optTypeParameters.cpp
    optTypeParameters.h
    p4parser.cpp
    p4parser.h
    p4state.cpp
    p4state.h
    packageTypeDeclaration.cpp
    packageTypeDeclaration.h
    parameter.cpp
    parameter.h
    parameterList.cpp
    parameterList.h
    prefixedNonTypeName.cpp
    prefixedNonTypeName.h
    prefixedType.cpp
    prefixedType.h
    realTypeArg.cpp
    realTypeArg.h
    realTypeArgumentList.cpp
    realTypeArgumentList.h
    returnStatement.cpp
    returnStatement.h
    scope.cpp
    scope.h
    specializedType.cpp
    specializedType.h
    specifiedIdentifier.cpp
    specifiedIdentifier.h
    specifiedIdentifierList.cpp
    specifiedIdentifierList.h
    statOrDeclList.cpp
    statOrDeclList.h
    statement.cpp
    statement.h
    statementOrDeclaration.cpp
    statementOrDeclaration.h
    structField.cpp
    structField.h
    structFieldList.cpp
    structFieldList.h
    structTypeDeclaration.cpp
    structTypeDeclaration.h
    switchStatement.cpp
    switchStatement.h
    tableDeclaration.cpp
    tableDeclaration.h
    tableProperty.cpp
    tableProperty.h
    tablePropertyList.cpp
    tablePropertyList.h
    tupleType.cpp
    tupleType.h
    typeArg.cpp
    typeArg.h
    typeArgumentList.cpp
    typeArgumentList.h
    typeDeclaration.cpp
    typeDeclaration.h
    typeName.cpp
    typeName.h
    typeOrVoid.cpp
    typeOrVoid.h
    typeParameterList.cpp
    typeParameterList.h
    typeRef.cpp
    typeRef.h
    typedefDeclaration.cpp
    typedefDeclaration.h
    variableDeclaration.cpp
    variableDeclaration.h
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
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
