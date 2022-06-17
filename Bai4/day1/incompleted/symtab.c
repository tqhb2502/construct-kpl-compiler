/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {

  Type *resultType = (Type *) malloc(sizeof(Type));

  resultType->typeClass = type->typeClass;
  if (type->typeClass == TP_ARRAY) {
    resultType->arraySize = type->arraySize;
    resultType->elementType = duplicateType(type->elementType);
  }

  return resultType;
}

int compareType(Type* type1, Type* type2) {

  if (type1->typeClass == type2->typeClass) {

    if (type1->typeClass == TP_ARRAY) {

      if (type1->arraySize == type2->arraySize) {
        return compareType(type1->elementType, type2->elementType);
      } else return 0;
    } else return 1;
  } else return 0;
}

void freeType(Type* type) {

  switch (type->typeClass) {
  case TP_INT:
  case TP_CHAR:
    free(type);
    break;
  case TP_ARRAY:
    freeType(type->elementType);
    free(type);
  }
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {

  ConstantValue *value = (ConstantValue *) malloc(sizeof(ConstantValue));

  value->type = TP_INT;
  value->intValue = i;

  return value;
}

ConstantValue* makeCharConstant(char ch) {
  
  ConstantValue *value = (ConstantValue *) malloc(sizeof(ConstantValue));

  value->type = TP_CHAR;
  value->charValue = ch;

  return value;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  
  ConstantValue *result = (ConstantValue *) malloc(sizeof(ConstantValue));

  result->type = v->type;
  if (v->type == TP_INT) {
    result->intValue = v->intValue;
  } else {
    result->charValue = v->charValue;
  }

  return result;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* createConstantObject(char *name) {
  
  Object *object = (Object *) malloc(sizeof(Object));

  strcpy(object->name, name);
  object->kind = OBJ_CONSTANT;
  object->constAttrs = (ConstantAttributes *) malloc(sizeof(ConstantAttributes));
  
  return object;
}

Object* createTypeObject(char *name) {
  
  Object *object = (Object *) malloc(sizeof(Object));

  strcpy(object->name, name);
  object->kind = OBJ_TYPE;
  object->typeAttrs = (TypeAttributes *) malloc(sizeof(TypeAttributes));

  return object;
}

Object* createVariableObject(char *name) {
  
  Object *object = (Object *) malloc(sizeof(Object));

  strcpy(object->name, name);
  object->kind = OBJ_VARIABLE;
  object->varAttrs = (VariableAttributes *) malloc(sizeof(VariableAttributes));
  object->varAttrs->scope = symtab->currentScope;

  return object;
}

Object* createFunctionObject(char *name) {
  // TODO
}

Object* createProcedureObject(char *name) {
  // TODO
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
}

void freeObject(Object* obj) {
  // TODO
}

void freeScope(Scope* scope) {
  // TODO
}

void freeObjectList(ObjectNode *objList) {
  // TODO
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
}

void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  addObject(&(symtab->currentScope->objList), obj);
}


