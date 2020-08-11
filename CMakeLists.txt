# Makefile for codegen



# sources for codegen
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY)



set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CODEGEN_SRCS
    src/actionDeclaration.cpp
    src/actionList.cpp
    src/annotations.cpp
    src/argument.cpp
    src/assignmentOrMethodCallStatement.cpp
    src/backends/psa.cpp
    src/backends/top.cpp
    src/backends/tna.cpp
    src/backends/v1model.cpp
    src/baseType.cpp
    src/blockStatement.cpp
    src/common.cpp
    src/conditionalStatement.cpp
    src/constantDeclaration.cpp
    src/controlDeclaration.cpp
    src/controlLocalDeclaration.cpp
    src/controlLocalDeclarations.cpp
    src/controlTypeDeclaration.cpp
    src/declaration.cpp
    src/derivedTypeDeclaration.cpp
    src/directApplication.cpp
    src/direction.cpp
    src/emptyStatement.cpp
    src/enumDeclaration.cpp
    src/errorDeclaration.cpp
    src/exitStatement.cpp
    src/externDeclaration.cpp
    src/expression.cpp
    src/expression_boolean.cpp
    src/expression_bit.cpp
    src/expression_int.cpp
    src/expression_struct.cpp
    src/expressionList.cpp
    src/functionDeclaration.cpp
    src/headerStackType.cpp
    src/headerTypeDeclaration.cpp
    src/headerUnionDeclaration.cpp
    src/identifierList.cpp
    src/keyElement.cpp
    src/keyElementList.cpp
    src/main.cpp
    src/matchKindDeclaration.cpp
    src/member.cpp
    src/name.cpp
    src/nonTableKwName.cpp
    src/nonTypeName.cpp
    src/optConstructorParameters.cpp
    src/optInitializer.cpp
    src/options.cpp
    src/optTypeParameters.cpp
    src/p4parser.cpp
    src/p4state.cpp
    src/packageTypeDeclaration.cpp
    src/parameter.cpp
    src/parameterList.cpp
    src/prefixedNonTypeName.cpp
    src/prefixedType.cpp
    src/probabilities.cpp
    src/realTypeArg.cpp
    src/realTypeArgumentList.cpp
    src/returnStatement.cpp
    src/scope.cpp
    src/specializedType.cpp
    src/specifiedIdentifier.cpp
    src/specifiedIdentifierList.cpp
    src/statOrDeclList.cpp
    src/statement.cpp
    src/statementOrDeclaration.cpp
    src/structField.cpp
    src/structFieldList.cpp
    src/structTypeDeclaration.cpp
    src/switchStatement.cpp
    src/tableDeclaration.cpp
    src/tableProperty.cpp
    src/tablePropertyList.cpp
    src/tupleType.cpp
    src/typeArg.cpp
    src/typeArgumentList.cpp
    src/typeDeclaration.cpp
    src/typeName.cpp
    src/typeOrVoid.cpp
    src/typeParameterList.cpp
    src/typeRef.cpp
    src/typedefDeclaration.cpp
    src/variableDeclaration.cpp
)
set (CODEGEN_HDRS
    src/actionDeclaration.h
    src/actionList.h
    src/annotations.h
    src/argument.h
    src/assignmentOrMethodCallStatement.h
    src/backends/psa.h
    src/backends/top.h
    src/backends/tna.h
    src/backends/v1model.h
    src/baseType.h
    src/blockStatement.h
    src/common.h
    src/conditionalStatement.h
    src/constantDeclaration.h
    src/controlDeclaration.h
    src/controlLocalDeclaration.h
    src/controlLocalDeclarations.h
    src/controlTypeDeclaration.h
    src/declaration.h
    src/derivedTypeDeclaration.h
    src/directApplication.h
    src/direction.h
    src/emptyStatement.h
    src/enumDeclaration.h
    src/errorDeclaration.h
    src/exitStatement.h
    src/externDeclaration.h
    src/expression.h
    src/expression_boolean.h
    src/expression_bit.h
    src/expression_int.h
    src/expression_struct.h
    src/expressionList.h
    src/functionDeclaration.h
    src/headerStackType.h
    src/headerTypeDeclaration.h
    src/headerUnionDeclaration.h
    src/identifierList.h
    src/keyElement.h
    src/keyElementList.h
    src/matchKindDeclaration.h
    src/member.h
    src/name.h
    src/nonTableKwName.h
    src/nonTypeName.h
    src/optConstructorParameters.h
    src/optInitializer.h
    src/options.h
    src/optTypeParameters.h
    src/p4parser.h
    src/p4state.h
    src/packageTypeDeclaration.h
    src/parameter.h
    src/parameterList.h
    src/prefixedNonTypeName.h
    src/prefixedType.h
    src/probabilities.h
    src/realTypeArg.h
    src/realTypeArgumentList.h
    src/returnStatement.h
    src/scope.h
    src/specializedType.h
    src/specifiedIdentifier.h
    src/specifiedIdentifierList.h
    src/statOrDeclList.h
    src/statement.h
    src/statementOrDeclaration.h
    src/structField.h
    src/structFieldList.h
    src/structTypeDeclaration.h
    src/switchStatement.h
    src/tableDeclaration.h
    src/tableProperty.h
    src/tablePropertyList.h
    src/tupleType.h
    src/typeArg.h
    src/typeArgumentList.h
    src/typeDeclaration.h
    src/typeName.h
    src/typeOrVoid.h
    src/typeParameterList.h
    src/typeRef.h
    src/typedefDeclaration.h
    src/variableDeclaration.h
)

add_cpplint_files (${CMAKE_CURRENT_SOURCE_DIR} "${CODEGEN_SRCS}" "${CODEGEN_HDRS}")

# extensions are unpleasant with unified builds
build_unified(CODEGEN_SRCS)


#[[include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)]]

add_executable(p4bludgeon ${CODEGEN_SRCS})
target_link_libraries (p4bludgeon ${P4C_LIBRARIES} ${P4C_LIB_DEPS})
add_dependencies(p4bludgeon genIR frontend)


install(TARGETS p4bludgeon RUNTIME DESTINATION ${P4C_RUNTIME_OUTPUT_DIRECTORY})

add_custom_target(linkp4bludgeon
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/p4bludgeon ${P4C_BINARY_DIR}/p4bludgeon
)

add_dependencies(p4c_driver linkp4bludgeon)
