#ifndef _PROBABILITIES_H_
#define _PROBABILITIES_H_

struct probabilities {
    // TODO: Basetype
    // assignment or method call
    short ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN = 75;
    short ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CALL = 25;
    // probabilities of assignment types
    short ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN_BIT = 100;
    short ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN_STRUCTLIKE = 0;
    // probabilities of method calls
    short ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION = 44;
    short ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_FUNCTION = 45;
    short ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE = 15;
    short ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_BUILT_IN = 1;

    // probabilities for the types of constant declarations
    short CONSTANTDECLARATION_TYPE_BASE = 80;
    short CONSTANTDECLARATION_TYPE_INT = 20;

    // probabilities for unary bit expressions
    short EXPRESSION_BIT_UNARY_NEG = 20;
    short EXPRESSION_BIT_UNARY_CMPL = 20;
    short EXPRESSION_BIT_UNARY_CAST = 10;
    short EXPRESSION_BIT_UNARY_FUNCTION = 50;
    // probabilities for binary bit expressions
    short EXPRESSION_BIT_BINARY_MUL = 5;
    short EXPRESSION_BIT_BINARY_DIV = 5;
    short EXPRESSION_BIT_BINARY_MOD = 5;
    short EXPRESSION_BIT_BINARY_ADD = 10;
    short EXPRESSION_BIT_BINARY_SUB = 10;
    short EXPRESSION_BIT_BINARY_ADDSAT = 10;
    short EXPRESSION_BIT_BINARY_SUBSAT = 10;
    short EXPRESSION_BIT_BINARY_LSHIFT = 5;
    short EXPRESSION_BIT_BINARY_RSHIFT = 5;
    short EXPRESSION_BIT_BINARY_BAND = 10;
    short EXPRESSION_BIT_BINARY_BOR = 10;
    short EXPRESSION_BIT_BINARY_BXOR = 10;
    short EXPRESSION_BIT_BINARY_CONCAT = 5;
    // probabilities for ternary bit expressions
    short EXPRESSION_BIT_BINARY_SLICE = 50;
    short EXPRESSION_BIT_BINARY_MUX = 50;
    // probabilities for bit expressions
    short EXPRESSION_BIT_VAR = 20;
    short EXPRESSION_BIT_INT_LITERAL = 5;
    short EXPRESSION_BIT_BIT_LITERAL = 25;
    short EXPRESSION_BIT_UNARY = 10;
    short EXPRESSION_BIT_BINARY = 20;
    short EXPRESSION_BIT_TERNARY = 10;

    // probabilities for boolean expressions
    short EXPRESSION_BOOLEAN_VAR = 15;
    short EXPRESSION_BOOLEAN_LITERAL = 20;
    short EXPRESSION_BOOLEAN_NOT = 35;
    short EXPRESSION_BOOLEAN_LAND = 5;
    short EXPRESSION_BOOLEAN_LOR = 5;
    short EXPRESSION_BOOLEAN_CMP = 10;
    short EXPRESSION_BOOLEAN_FUNCTION = 5;
    short EXPRESSION_BOOLEAN_BUILT_IN = 5;
    // probabilities for comparisons
    short EXPRESSION_BOOLEAN_CMP_EQU = 50;
    short EXPRESSION_BOOLEAN_CMP_NEQ = 50;

    // probabilities for structlike expressions
    short EXPRESSION_STRUCT_VAR = 50;
    short EXPRESSION_STRUCT_LITERAL = 30;
    short EXPRESSION_STRUCT_FUNCTION = 20;

    // probabilities for the function return types
    short FUNCTIONDECLARATION_TYPE_BASE = 90;
    short FUNCTIONDECLARATION_TYPE_STRUCT = 9;
    short FUNCTIONDECLARATION_TYPE_VOID = 1;

    // probabilities for types in header structures
    short HEADERTYPEDECLARATION_FIELD_BASE = 100;
    short HEADERTYPEDECLARATION_FIELD_STRUCT = 0;

    // probabilities for state transitions in the parser
    short P4STATE_TRANSITION_ACCEPT = 50;
    short P4STATE_TRANSITION_REJECT = 10;
    short P4STATE_TRANSITION_STATE = 30;
    short P4STATE_TRANSITION_SELECT = 10;

    // probabilities for the parameter type
    short PARAMETER_TYPE_BASE = 90;
    short PARAMETER_TYPE_STRUCT = 10;
    // probabilities for the parameter direction
    short PARAMETER_DIR_IN = 33;
    short PARAMETER_DIR_OUT = 33;
    short PARAMETER_DIR_INOUT = 33;

    // probabilities for statements
    short STATEMENT_SWITCH = 5;
    short STATEMENT_ASSIGNMENTORMETHODCALL = 85;
    short STATEMENT_IF = 3;
    short STATEMENT_RETURN = 2;
    short STATEMENT_EXIT = 3;
    short STATEMENT_BLOCK = 2;

    // probabilities for statements or declarations
    short STATEMENTORDECLARATION_VAR = 10;
    short STATEMENTORDECLARATION_CONSTANT = 10;
    short STATEMENTORDECLARATION_STATEMENT = 80;

    // probabilities for types in struct structures
    short STRUCTTYPEDECLARATION_FIELD_BASE = 75;
    short STRUCTTYPEDECLARATION_FIELD_STRUCT = 25;
    short STRUCTTYPEDECLARATION_FIELD_STACK = 0;
    // probabilities for the headers struct type
    short STRUCTTYPEDECLARATION_HEADERS_HEADER = 90;
    short STRUCTTYPEDECLARATION_HEADERS_STACK = 10;

    // probabilities for types in struct structures
    short TYPEDECLARATION_HEADER = 75;
    short TYPEDECLARATION_STRUCT = 25;
    short TYPEDECLARATION_UNION = 0;

    // probabilities for types in struct structures
    short TYPEDEFDECLARATION_BASE = 75;
    short TYPEDEFDECLARATION_STRUCTLIKE = 25;
    short TYPEDEFDECLARATION_STACK = 0;

    // probabilities for the types of constant declarations
    short VARIABLEDECLARATION_TYPE_BASE = 80;
    short VARIABLEDECLARATION_TYPE_STRUCT = 15;
    short VARIABLEDECLARATION_TYPE_STACK = 5;

};
extern probabilities PCT;

#endif
