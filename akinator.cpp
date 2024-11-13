#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "akinator.h"
#include "grafic_print.h"

errors_t start_drochinator ()
{
    grow_binary_tree ();

    return ALL_GOOD;
}

errors_t grow_binary_tree ()
{
    tree_t stoyak_groota = {};
    tree_t* ptr_data = &stoyak_groota;

    FILE* protocol = fopen ( "protocol.txt", "w" );

    struct_tree_ctor ( ptr_data );

//fprintf ( stderr, "HUI1\n" );
    plant_sapling ( ptr_data );

    //create_new_branch ( ptr_data, LEFT, 0, str1);
    //create_new_branch ( ptr_data, RIGHT, 0, str2);
//fprintf ( stderr, "HUI1\n" );

    fprintf ( stderr, "~~~~~~~~~DROCHINATOR~~~~~~~~~\n" );
    fprintf ( protocol, "~~~~~~~~~DROCHINATOR~~~~~~~~~\n" );

    guesser ( &ptr_data->ptr_tree [ 0 ], ptr_data, protocol );

    //tree_dump ( ptr_tree );
    make_grafic_dump ( ptr_data );

    return ALL_GOOD;
}

errors_t struct_tree_ctor ( tree_t* ptr_data )
{
    ptr_data->ptr_tree = ( branch_t* ) calloc ( DATA_CAPACITY, sizeof ( branch_t ) );
fprintf ( stderr, "1ptr_tree = %p\n", ptr_data->ptr_tree );
    ptr_data->size = 0;

    return ALL_GOOD;
}

errors_t plant_sapling ( tree_t* ptr_data )
{
    CHECK ( ptr_data );

//fprintf ( stderr, "HUI0\n" );
    strncpy ( ptr_data->ptr_tree[ 0 ].que_or_answ, GAY_VALUE, MAX_NAME_LEN );
//fprintf(stderr, "ptr_data = %p,  pr_tree = %p\n", ptr_data, ptr_data->ptr_tree);
//fprintf ( stderr, "HUI3\n" );
    ptr_data->ptr_tree [ 0 ].prev = ( branch_t* )POISON_VALUE;

    for ( int i = 1; i < DATA_CAPACITY; i++ )
    {
        //strncpy ( ptr_data->ptr_tree [ i ].que_or_answ, POISON_STR, MAX_NAME_LEN );
        ptr_data->ptr_tree [ i ].negative_result = NULL;
        ptr_data->ptr_tree [ i ].positive_result = NULL;
        ptr_data->ptr_tree [ i ].prev = ( branch_t* ) POISON_VALUE;
    }

    return ALL_GOOD;
}


errors_t tree_checker ( tree_t* ptr )
{
    if ( !ptr )
        return DATA_PTR_POINTS_ON_MY_BALLS;
    if ( !ptr->ptr_tree )
        return TREE_PTR_POINTS_ON_MY_BALLS;
    if ( ptr->size < 0 )
        return SIZE_UNDER_PLINTUS;

    return ALL_GOOD;
}

errors_t create_new_branch ( tree_t* ptr_data, int direction, branch_t* prev_branch, char* que_or_answ )
{
    CHECK ( ptr_data );

    ptr_data->size++;

    strncpy ( ptr_data->ptr_tree [ ptr_data->size ].que_or_answ, que_or_answ, MAX_NAME_LEN );
    ptr_data->ptr_tree [ ptr_data->size ].prev = prev_branch;

    if ( direction == LEFT )
        prev_branch->positive_result = &ptr_data->ptr_tree [ ptr_data->size ];
    if ( direction == RIGHT )
        prev_branch->negative_result = &ptr_data->ptr_tree [ ptr_data->size ];

    return ALL_GOOD;
}

void delay ( int ms )
{
  int delay_end_time = clock() + ms;
  while (clock() < delay_end_time);
}

errors_t guesser ( branch_t* ptr_node, tree_t* ptr_data, FILE* protocol )
{
    delay ( 1000 );

    fprintf ( stderr, "THINKING WHAT QUISTEION TO ASK...\n" );
    fprintf ( protocol, "THINKING WHAT QUISTEION TO ASK...\n" );


    delay ( 1000 );

    int checker_if_leaf = 0;
    if ( check_if_leaf ( ptr_node ) == LEAF )
        checker_if_leaf = 1;
fprintf ( stderr, "checker_if_leaf = %d\n", checker_if_leaf );


    fprintf ( stderr, "is it '%s'?\n", ptr_node->que_or_answ );
    fprintf ( protocol, "is it '%s'?\n", ptr_node->que_or_answ );

    int peasant_result = scan_answer ();

    if ( peasant_result == LEFT )
    {
        fprintf ( protocol, "yes\n" );
        if ( ptr_node->positive_result != NULL )
        {
            guesser ( ptr_node->positive_result, ptr_data, protocol );
        }
        else
        {
            if ( checker_if_leaf == 1 )
            {
                fprintf ( stderr, "then, it might be '%s'\n", ptr_node->que_or_answ );
                fprintf ( protocol, "then, it might be '%s'\n", ptr_node->que_or_answ );

                int peasant_result2 = scan_answer ();

                if ( peasant_result2 == LEFT )
                {
                    fprintf ( stderr, "Too easy for me! [%s]\n", ptr_node->que_or_answ );
                    fprintf ( protocol, "Too easy for me! [%s]\n", ptr_node->que_or_answ );
                    return ALL_GOOD;
                }
            }
            fprintf ( stderr, "no idea what it was....\nSo, what is it?\n" );
            fprintf ( protocol, "no idea what it was....\nSo, what is it?\n" );

            char new_answer[ MAX_INPUT_LENGTH ] = {};
            scanf ( "%s", new_answer );

            fprintf ( stderr, "Adding new branch to tree...\n" );
            fprintf ( protocol, "Adding new branch to tree...\n" );

            delay ( 2000 );

            create_new_branch ( ptr_data, LEFT, ptr_node, new_answer );

            fprintf ( stderr, "Which sigh can separate '%s' and '%s'?\n", ptr_node->que_or_answ, new_answer );
            fprintf ( protocol, "Which sigh can separate '%s' and '%s'?\n", ptr_node->que_or_answ, new_answer );

            char sigh [ MAX_INPUT_LENGTH ] = {};
            scanf ( "%s", sigh);

            strncpy ( ptr_node->que_or_answ, sigh, MAX_INPUT_LENGTH );

            guesser ( ptr_node->positive_result, ptr_data, protocol );
        }

    }

    if ( peasant_result == RIGHT )
    {
        fprintf ( protocol, "no\n" );
        if ( ptr_node->negative_result != NULL )
        {
            guesser ( ptr_node->negative_result, ptr_data, protocol );
        }
        else
        {
            if ( checker_if_leaf == 1 )
            {
                fprintf ( stderr, "then, it might be '%s'\n", ptr_node->que_or_answ );
                fprintf ( protocol, "then, it might be '%s'\n", ptr_node->que_or_answ );

                int peasant_result2 = scan_answer ();

                if ( peasant_result2 == LEFT )
                {
                    fprintf ( stderr, "Too easy for me! [%s]\n", ptr_node->que_or_answ );
                    fprintf ( protocol, "Too easy for me! [%s]\n", ptr_node->que_or_answ );
                    return ALL_GOOD;
                }
            }
            fprintf ( stderr, "no idea what it was....\nSo, what is it?\n" );
            fprintf ( protocol, "no idea what it was....\nSo, what is it?\n" );

            char new_answer[ MAX_INPUT_LENGTH ] = {};
            scanf ( "%s", new_answer );

            fprintf ( stderr, "Adding new branch to tree...\n" );
            fprintf ( protocol, "Adding new branch to tree...\n" );

            delay ( 2000 );

            create_new_branch ( ptr_data, LEFT, ptr_node, new_answer );

            fprintf ( stderr, "Which sigh can separate '%s' and '%s'?\n", ptr_node->que_or_answ, new_answer );
            fprintf ( protocol, "Which sigh can separate '%s' and '%s'?\n", ptr_node->que_or_answ, new_answer );

            char sigh [ MAX_INPUT_LENGTH ] = {};
            scanf ( "%s", sigh);

            strncpy ( ptr_node->que_or_answ, sigh, MAX_INPUT_LENGTH );

            guesser ( ptr_node->negative_result, ptr_data, protocol );
        }

    }

    return ALL_GOOD;

}

int scan_answer ()
{
    static char input [ MAX_INPUT_LENGTH ] = {};

    int scanning = 1;

    while ( scanning )
    {
        scanf ( "%s", input );
        if ( strcmp ( input, "yes" ) != 0 && strcmp ( input, "no" ) != 0 )
        {
            char shit_value [ MAX_INPUT_LENGTH ] = {};
            fprintf ( stderr, "Wrong input! Accepted: 'yes' or 'no'\n");
            sscanf ( input, "%s", shit_value );
        }
        else
            scanning = 0;
    }
    if ( strcmp ( input, "yes" ) == 0 )
        return LEFT;
    if ( strcmp ( input, "no" ) == 0 )
        return RIGHT;
    return SCAN_MY_BALLS_ON_PRINTER;
}

leaf_t check_if_leaf ( branch_t* ptr_node )
{
    if ( ptr_node->positive_result == NULL && ptr_node->negative_result == NULL )
        return LEAF;

    return NOT_LEAF;
}
