/* Test typedef declarations with specifiers_ordine */

/* Basic typedef - known case */
typedef int MyInt;

/* Typedef of struct */
typedef struct MyStruct {
    int     int a;
    int     int b;
} MyStruct;

/* Typedef of enum */
typedef enum MyEnum {
    VALUE_A,
    VALUE_B
} MyEnum;

/* Using typedef names (known - defined above) */
MyInt my_int_var;
MyStruct my_struct_var;
MyEnum my_enum_var;

/* Typedef with const qualifier on usage */
const MyInt const_my_int;

/* Typedef pointer */
typedef int* IntPtr;
IntPtr ptr_var;
