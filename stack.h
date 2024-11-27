#ifndef STACK_HEADER
#define STACK_HEADER

enum ERROR_CODES
{
    ERROR_SIZE       =    0,
    ERROR_CAPACITY   =    1,
    ERROR_DATA_PTR   =    2,
    ERROR_S_LARGER_C =    3,
    ERROR_ZERO_SIZE  =  666,
    BAD_HASH         = 1234,
};

enum PETUSHKI
{
    PETUSHOK1       = 12345678,
    PETUSHOK2       = 12345678,
    PETUSHOK1_ERROR =      666,
    PETUSHOK2_ERROR =      667
};

enum FUNC_CHECK
{
    FUNC_DONE = 0
};

typedef int Stack_Elem_Data_t;

struct stack_t
{
    Stack_Elem_Data_t*     data_ptr;            // *((char *) data - sizeof(canary_t)) = 0xFEE1DEAD
    int                    size;
    int                    capacity;
    #ifdef CANARY_PROTECT
        int equalazer;
    #endif

    #ifdef HASH_PROTECT
        unsigned long hehesh;
    #endif
};

int give_equalazer ( stack_t* ptr_stk, int capacity );


#ifdef CANARY_PROTECT
    typedef long long canary_t;

    #define CANARY_INIT( ptr_stk )                                                                                                                \
    *( Stack_Elem_Data_t* ) ( ( char* ) ptr_stk->data_ptr - sizeof ( canary_t ) )                                                    = PETUSHOK1;  \
                                                                                                                                                   \
    *( Stack_Elem_Data_t* ) ( ( char* ) ptr_stk->data_ptr + ptr_stk->capacity * sizeof ( Stack_Elem_Data_t )  + ptr_stk->equalazer ) = PETUSHOK2;  \

    #define PETUHRINT( ptr_stk )                                                                                                                  \
    printf ( "PETUH1: %d\n"  ,   *( Stack_Elem_Data_t* ) ( ( char* ) ptr_stk->data_ptr - sizeof ( canary_t ) ) );                                  \
    printf ( "PETUH2: %d\n"  ,   *( Stack_Elem_Data_t* ) ( ( char* ) ( ptr_stk->data_ptr + ptr_stk->capacity ) + ptr_stk->equalazer ) );           \

    #define PETUHCHECK( ptr_stk )                                                                                                                 \
    if ( *( ( Stack_Elem_Data_t* ) ( ( char* ) ( ptr_stk->data_ptr ) - sizeof ( canary_t ) ) ) != PETUSHOK1 )                                      \
    {                                                                                                                                              \
        printf ( "**\n%d\n***", ( *( ( Stack_Elem_Data_t* ) ( char* ) (ptr_stk->data_ptr ) - sizeof ( canary_t ) ) ) );                            \
        return PETUSHOK1_ERROR;                                                                                                                    \
    }                                                                                                                                              \
                                                                                                                                                   \
    if ( *( ( Stack_Elem_Data_t* ) (  ( char* ) ( ptr_stk->data_ptr + ptr_stk->capacity ) + ptr_stk->equalazer ) ) != PETUSHOK2 )                  \
    {                                                                                                                                              \
        printf ( "**\n%d\n***", *( ( Stack_Elem_Data_t* ) (  ( char* ) ( ptr_stk->data_ptr + ptr_stk->capacity ) + ptr_stk->equalazer ) ) );       \
        return PETUSHOK2_ERROR;                                                                                                                    \
    }                                                                                                                                              \

    #define POPA_PETUHA( ptr_stk )                                                                                                                \
    *( Stack_Elem_Data_t* ) ( ( char* ) ptr_stk->data_ptr - sizeof ( canary_t ) ) = PETUSHOK1;                                                     \
    *( Stack_Elem_Data_t* ) ( ( char* ) ptr_stk->data_ptr + new_capacity * sizeof ( Stack_Elem_Data_t )  + ptr_stk->equalazer ) = PETUSHOK2;
#else

    #define CANARY_INIT( ptr_stk )
    #define PETUHRINT( ptr_stk )
    #define PETUHCHECK( ptr_stk )
    #define POPA_PETUHA( ptr_stk )

#endif

#define STACK_ASSERT( ptr_stk  )      ({                                                                            \
                                                                                                                    \
int c;                                                                                                              \
if ( ( c = StackCheck ( ptr_stk )) != FUNC_DONE )                                                                   \
{                                                                                                                   \
    switch ( c )                                                                                                    \
    {                                                                                                               \
        case 1:                                                                                                     \
        {                                                                                                           \
            fprintf ( stderr, "\nERROR ( CAPACITY < 0 )  in file %s in func (%s), "                                 \
                              "line %d\n\nPress Enter to continue", __FILE__, __func__, __LINE__);                  \
            getchar();                                                                                              \
            break;                                                                                                  \
        }                                                                                                           \
        case 2:                                                                                                     \
        {                                                                                                           \
            fprintf ( stderr, "\nERROR ( BAD DATA PTR )  in file %s in func (%s), "                                 \
                              "line %d\n\nPress Enter to continue", __FILE__, __func__, __LINE__);                  \
            getchar();                                                                                              \
            break;                                                                                                  \
        }                                                                                                           \
        case 3:                                                                                                     \
        {                                                                                                           \
            fprintf ( stderr, "\nERROR ( SIZE > CAPACITY )  in file %s in func (%s), "                              \
                              "line %d\n\nPress Enter to continue", __FILE__, __func__, __LINE__);                  \
            getchar();                                                                                              \
            break;                                                                                                  \
        }                                                                                                           \
        case 666:                                                                                                   \
        {                                                                                                           \
            fprintf ( stderr, "\nERROR ( YOU CANT POP EMPTY STACK )  in file %s in func (%s), "                     \
                              "line %d\n\nPress Enter to continue", __FILE__, __func__, __LINE__);                  \
            getchar();                                                                                              \
            break;                                                                                                  \
        }                                                                                                           \
        case 0:                                                                                                     \
        {                                                                                                           \
            fprintf ( stderr, "\nERROR ( SIZE < 0 )  in file %s in func (%s), "                                     \
                              "line %d\n\nPress Enter to continue", __FILE__, __func__, __LINE__);                  \
            getchar();                                                                                              \
            break;                                                                                                  \
        }                                                                                                           \
        case 1234:                                                                                                  \
        {                                                                                                           \
            fprintf ( stderr, "\nERROR ( BAD HASH DETECTED )  in file %s in func (%s), "                            \
                              "line %d\n\nPress Enter to continue", __FILE__, __func__, __LINE__);                  \
            getchar();                                                                                              \
            break;                                                                                                  \
        }                                                                                                           \
                                                                                                                    \
        default:                                                                                                    \
        {                                                                                                           \
            fprintf ( stderr, "UNKNOWN ERROR" );                                                                    \
            getchar();                                                                                              \
            exit (52);                                                                                              \
        }                                                                                                           \
    }                                                                                                               \
}                                                                                                                   \
})



int           StackPush         ( stack_t* ptr_data, Stack_Elem_Data_t value );
int           StackCtor         ( stack_t* ptr_data, size_t new_capacity );
int           StackDump         ( stack_t* ptr_data );
int           StackCheck        ( stack_t* ptr_stk );
int           StackPop          ( stack_t* ptr_data );
int           recalloc          ( stack_t* ptr_stk, int new_capacity );
int           cleaner_realloc   ( stack_t* ptr_stk );
int           StackDtor         ( stack_t* ptr_stk );

#endif
