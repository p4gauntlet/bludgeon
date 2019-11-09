# Makefile for the toZ3

# sources for toZ3
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY)

set (CODEGEN_SRCS
	scope.cpp
	scope.h
	common.h
	codegen.cpp
	codegen.h
	bit.cpp
	bit.h
	declaration.h
	nonTypeName.h
	name.h
	nonTableKwName.h
	parameterList.h
	nonEmptyParameterList.h
	parameter.h
	direction.h
	packageTypeDeclaration.h
	instantiation.h
	optConstructorParameters.h
	dotPrefix.h
	controlDeclaration.h
	controlTypeDeclaration.h
	controlLocalDeclarations.h
	controlLocalDeclaration.h
	controlBody.h
	typeRef.h
	namedType.h
	prefixedType.h
	typeName.h
	tupleType.h
	headerStackType.h
	specializedType.h
	baseType.h
	typeOrVoid.h
	optTypeParameters.h
	typeParameterList.h
	realTypeArg.h
	typeArg.h
	realTypeArgumentList.h
	typeArgumentList.h
	typeDeclaration.h
	derivedTypeDeclaration.h
	headerTypeDeclaration.h
	headerUnionDeclaration.h
	structTypeDeclaration.h
	structFieldList.h
	structField.h
	enumDeclaration.h
	errorDeclaration.h
	matchKindDeclaration.h
	identifierList.h
	specifiedIdentifierList.h
	specifiedIdentifier.h
	typedefDeclaration.h
	assignmentOrMethodCallStatement.h
	emptyStatement.h
	returnStatement.h
	exitStatement.h
	conditionalStatement.h
	directApplication.h
	statement.h
	blockStatement.h
	statOrDeclList.h
	switchStatement.h
	switchCases.h
	switchCase.h
	switchLabel.h
	statementOrDeclaration.h
	tableDeclaration.h
	tablePropertyList.h
	tableProperty.h
	keyElementList.h
	keyElement.h
	actionList.h
	entriesList.h
	entry.h
	actionRef.h
	actionDeclaration.h
	variableDeclaration.h
	constantDeclaration.h
	optInitializer.h
	initializer.h
	functionDeclaration.h
	argumentList.h
	nonEmptyArgList.h
	argument.h
	kvList.h
	kvPair.h
	expressionList.h
	member.h
	prefixedNonTypeName.h
	lvalue.h
	expression.h
	declaration.cpp
	nonTypeName.cpp
	name.cpp
	nonTableKwName.cpp
	parameterList.cpp
	nonEmptyParameterList.cpp
	parameter.cpp
	direction.cpp
	packageTypeDeclaration.cpp
	instantiation.cpp
	optConstructorParameters.cpp
	dotPrefix.cpp
	controlDeclaration.cpp
	controlTypeDeclaration.cpp
	controlLocalDeclarations.cpp
	controlLocalDeclaration.cpp
	controlBody.cpp
	typeRef.cpp
	namedType.cpp
	prefixedType.cpp
	typeName.cpp
	tupleType.cpp
	headerStackType.cpp
	specializedType.cpp
	baseType.cpp
	typeOrVoid.cpp
	optTypeParameters.cpp
	typeParameterList.cpp
	realTypeArg.cpp
	typeArg.cpp
	realTypeArgumentList.cpp
	typeArgumentList.cpp
	typeDeclaration.cpp
	derivedTypeDeclaration.cpp
	headerTypeDeclaration.cpp
	headerUnionDeclaration.cpp
	structTypeDeclaration.cpp
	structFieldList.cpp
	structField.cpp
	enumDeclaration.cpp
	errorDeclaration.cpp
	matchKindDeclaration.cpp
	identifierList.cpp
	specifiedIdentifierList.cpp
	specifiedIdentifier.cpp
	typedefDeclaration.cpp
	assignmentOrMethodCallStatement.cpp
	emptyStatement.cpp
	returnStatement.cpp
	exitStatement.cpp
	conditionalStatement.cpp
	directApplication.cpp
	statement.cpp
	blockStatement.cpp
	statOrDeclList.cpp
	switchStatement.cpp
	switchCases.cpp
	switchCase.cpp
	switchLabel.cpp
	statementOrDeclaration.cpp
	tableDeclaration.cpp
	tablePropertyList.cpp
	tableProperty.cpp
	keyElementList.cpp
	keyElement.cpp
	actionList.cpp
	entriesList.cpp
	entry.cpp
	actionRef.cpp
	actionDeclaration.cpp
	variableDeclaration.cpp
	constantDeclaration.cpp
	optInitializer.cpp
	initializer.cpp
	functionDeclaration.cpp
	argumentList.cpp
	nonEmptyArgList.cpp
	argument.cpp
	kvList.cpp
	kvPair.cpp
	expressionList.cpp
	member.cpp
	prefixedNonTypeName.cpp
	lvalue.cpp
	expression.cpp
    main.cpp
    )
add_cpplint_files (${CMAKE_CURRENT_SOURCE_DIR} "${CODEGEN_SRCS}")

#build_unified(CODEGEN_SRCS)
add_executable(p4codegen ${CODEGEN_SRCS})
# message (STATUS "libs ${P4C_LIBRARIES}")
# message (STATUS "deps ${P4C_LIB_DEPS}")
# target_link_libraries (p4toz3 ${P4C_LIBRARIES} ${P4C_LIB_DEPS})
# target_link_libraries (p4toz3 ${P4C_LIBRARIES} ${P4C_LIB_DEPS})
target_link_libraries (p4codegen ${P4C_LIBRARIES} ${P4C_LIB_DEPS})
add_dependencies(p4codegen genIR frontend)
#add_dependencies(p4toz3 genIR frontend)

install(TARGETS p4codegen RUNTIME DESTINATION ${P4C_RUNTIME_OUTPUT_DIRECTORY})

add_custom_target(linkp4codegen
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/p4codegen ${P4C_BINARY_DIR}/p4codegen
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${P4C_BINARY_DIR}/p4include ${CMAKE_CURRENT_BINARY_DIR}/p4include
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${P4C_BINARY_DIR}/p4_14include ${CMAKE_CURRENT_BINARY_DIR}/p4_14include
  )
add_dependencies(p4c_driver linkp4codegen)

