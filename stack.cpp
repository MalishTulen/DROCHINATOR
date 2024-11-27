#include <stdio.h>
#include <malloc.h>
#include <assert.h>


//#define CANARY_PROTECT
//#define HASH_PROTECT

#include "stack.h"
//#include "hash.h"

#ifdef HASH_PROTECT
    int give_equalazer ( stack_t * ptr_stk, int capacity )
    {
        if ( ( capacity * sizeof ( int ) ) % sizeof ( canary_t ) == 0 )
            ptr_stk->equalazer = 0;
        else
            ptr_stk->equalazer = sizeof ( canary_t ) - capacity * sizeof ( int ) % sizeof ( canary_t );

        return FUNC_DONE;
    }
#endif
#ifdef HASH_PROTECT
    bool hash_check ( stack_t* ptr_stk )
    {
        unsigned long check_hash_sum = 0;

        check_hash_sum = one_hash ( ptr_stk );

        if ( ptr_stk->hehesh == check_hash_sum )
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    unsigned long one_hash ( stack_t * ptr_stk )
    {
        unsigned long hash_coefficient = 5381;
        unsigned long hash_result      =    0;

        for ( int i = 0; i < ptr_stk->capacity; i++)
        {
            hash_result += (hash_coefficient << 5) + hash_coefficient + *( ptr_stk->data_ptr + i );
        }

        return hash_result;
    }
#endif



int StackCtor ( stack_t * ptr_stk, size_t capacity )
{
    ptr_stk->size     =        0;
    ptr_stk->capacity = capacity;
    #ifdef CANARY_PROTECT
        give_equalazer ( ptr_stk, capacity );
    #endif
//fprintf ( stderr, "p1\n" );
    #ifdef CANARY_PROTECT
        size_t size = ( 2 * sizeof ( canary_t ) + capacity + ptr_stk->equalazer );
    #else
        size_t size = ( capacity );
    #endif
//fprintf ( stderr, "p1\n" );

    Stack_Elem_Data_t * tmp_ptr = ( Stack_Elem_Data_t * ) calloc ( size, sizeof ( Stack_Elem_Data_t ) );
    #ifdef CANARY_PROTECT
        ptr_stk->data_ptr = ( Stack_Elem_Data_t* ) ( ( char* ) tmp_ptr + sizeof ( canary_t ) );
    #else
        ptr_stk->data_ptr = ( tmp_ptr );
    #endif
//fprintf ( stderr, "p1\n" );

    #ifdef HASH_PROTECT
        ptr_stk->hehesh = GIVE_ME_HASH( ptr_stk );
    #endif
    CANARY_INIT( ptr_stk );
//fprintf ( stderr, "STKDMP1\n" );

    //StackDump ( ptr_stk );
//fprintf ( stderr, "STKDMP2\n" );
    STACK_ASSERT ( ptr_stk );
//fprintf ( stderr, "p134\n" );
    return FUNC_DONE;
}


int StackDump ( stack_t * ptr_stk )
{
//fprintf ( stderr, "STKDMP1\n" );
    printf ( "\nsize=%d\n"   ,   ptr_stk->size );
    printf ( "capacity=%d\n" ,   ptr_stk->capacity );

    #ifdef HASH_PROTECT
        printf ( "hash=%lu\n"    ,   ptr_stk->hehesh );
    #endif

    #ifdef CANARY_PROTECT
        printf ( "equalazer=%d\n",   ptr_stk->equalazer );
    #endif

    printf ( "stack_t:[ " );
    for ( int i = 0 ; i < ( ptr_stk->capacity ); i++ )
    {
        printf ( "(%d) ", *(ptr_stk->data_ptr + i )  );
    }
    printf ( "]\n" );
//fprintf ( stderr, "STKDMP2\n" );
//fprintf ( stderr, "STKDMP3\n" );
    PETUHRINT ( ptr_stk );

    return FUNC_DONE;
}

int StackCheck ( stack_t * ptr_stk )
{
    if ( ptr_stk->size < 0 )
    {
        return ( ERROR_SIZE );
    }

    if ( ptr_stk->capacity < 0 )
    {
        return ( ERROR_CAPACITY );
    }
    if ( ( ptr_stk->data_ptr ) == NULL )
    {
        return ( ERROR_DATA_PTR );
    }
    if ( ptr_stk->size > ptr_stk->capacity )
    {
        return ( ERROR_S_LARGER_C );
    }
    PETUHCHECK ( ptr_stk );

    #ifdef HASH_PROTECT
        if ( ZABULLIL_HASH( ptr_stk ) == true )
        {
            return ( BAD_HASH );
        }
    #endif
    return FUNC_DONE;
}

int StackPop ( stack_t * ptr_stk )
{
    STACK_ASSERT( ptr_stk );
//fprintf ( stderr, "bef everything\n" );
    if ( ptr_stk->size == 0 )
    {
        printf ( "\nStack have zero elements! Try to add elements before using StackPopa!");
        return ERROR_ZERO_SIZE;
    }
    else
    {
//fprintf ( stderr, "bef recalloc\n" );
        if ( ptr_stk->size < ptr_stk->capacity / 2 )
        {
        ptr_stk->data_ptr = ( Stack_Elem_Data_t* ) realloc ( ptr_stk->data_ptr, ( ptr_stk->capacity ) / 4 * 3 );
        }
        int ruka = ptr_stk->data_ptr [ ptr_stk->size ];
//fprintf ( stderr, "aft recalloc\n" );
        printf ("%d", ruka );


        *( Stack_Elem_Data_t* )( ( char* ) ptr_stk->data_ptr + ptr_stk->size * sizeof( Stack_Elem_Data_t )) = 0;
        (ptr_stk->size)--;

        #ifdef HASH_PROTECT
            ptr_stk->hehesh = GIVE_ME_HASH( ptr_stk );
        #endif
        STACK_ASSERT( ptr_stk );

        return ( ruka );
    }
}

int StackPush ( stack_t * ptr_stk, Stack_Elem_Data_t value )
{
    STACK_ASSERT( ptr_stk );

    if ( ptr_stk->size == ptr_stk->capacity )
    {
        recalloc ( ptr_stk, ( (ptr_stk->capacity) * 2 ) );
        ( ptr_stk->capacity ) *= 2;
    }
    ptr_stk->data_ptr[ ptr_stk->size ] = value;
    (ptr_stk->size)++;
    #ifdef HASH_PROTECT
        ptr_stk->hehesh = GIVE_ME_HASH ( ptr_stk );
    #endif
    STACK_ASSERT( ptr_stk );

    return FUNC_DONE;
}

int recalloc ( stack_t* ptr_stk, int new_capacity )
{
    #ifdef CANARY_PROTECT
        give_equalazer ( ptr_stk, new_capacity );
        Stack_Elem_Data_t * tmp_ptr = ( Stack_Elem_Data_t* ) realloc ( ( Stack_Elem_Data_t* ) ( ( char* ) ptr_stk->data_ptr - sizeof ( canary_t) ), ( char ) new_capacity + 2 * sizeof ( canary_t)  + ptr_stk->equalazer );
        ptr_stk->data_ptr = ( Stack_Elem_Data_t* ) ( ( char* ) tmp_ptr + sizeof ( canary_t ) );
    #else
        Stack_Elem_Data_t * tmp_ptr = ( Stack_Elem_Data_t* ) realloc ( ( char* ) ptr_stk->data_ptr, new_capacity * sizeof ( Stack_Elem_Data_t )  );
        ptr_stk->data_ptr = tmp_ptr;
    #endif
//fprintf ( stderr, "p3\n" );
    if (ptr_stk->capacity < new_capacity )
        cleaner_realloc ( ptr_stk );
//fprintf ( stderr, "p4\n" );

    POPA_PETUHA( ptr_stk );

    STACK_ASSERT( ptr_stk );

    return FUNC_DONE;
}

int cleaner_realloc ( stack_t* ptr_stk )
{
    STACK_ASSERT( ptr_stk  );

    for ( int i = ptr_stk->capacity; i < ((ptr_stk->capacity * 2) ); i++ )
    {
        *( ptr_stk->data_ptr + i  ) = 0;
    }

    STACK_ASSERT( ptr_stk );

    return FUNC_DONE;
}

int StackDtor ( stack_t* ptr_stk )
{
    STACK_ASSERT( ptr_stk  );

    ptr_stk->size = 0;
    ptr_stk->capacity = 0;

    #ifdef CANARY_PROTECT
        ptr_stk->equalazer = 0;
    #endif

    #ifdef HASH_PROTECT
        FREE_HASH( ptr_stk );
    #endif

    free ( ptr_stk->data_ptr );

    return FUNC_DONE;
}

