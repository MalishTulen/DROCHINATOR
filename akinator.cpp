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

    //create_new_branch ( ptr_data, LEFT, ptr_data->ptr_tree, str1);
    //create_new_branch ( ptr_data, RIGHT, ptr_data->ptr_tree, str2);
//fprintf ( stderr, "HUI1\n" );
    static int amount_of_pictures = 1;

    choose_mode ( ptr_data, protocol, &amount_of_pictures );

    fclose ( protocol );

    //tree_dump ( ptr_tree );
    make_grafic_dump ( ptr_data, &amount_of_pictures );

    return ALL_GOOD;
}

errors_t struct_tree_ctor ( tree_t* ptr_data )
{
    ptr_data->ptr_tree = ( branch_t* ) calloc ( DATA_CAPACITY, sizeof ( branch_t ) );
fprintf ( stderr, "1ptr_tree = %p\n", ptr_data->ptr_tree );
    ptr_data->size = 0;

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

errors_t choose_mode ( tree_t* ptr_data, FILE* protocol, int* amount_of_pictures )
{
    fprintf ( stderr, "Choose what you want to do:\n"
                      "1 - play regular akinator\n"
                      "2 - compare elements from data\n"
                      "3 - check element from data\n"
                      "4 - whatch documentation\n"
                      "Your input:" );
    char users_input[ MAX_INPUT_LENGTH ] = {};

    int scanning = 1;

    while ( scanning )
    {
        scanf ( "%s^[\n]", users_input );
        if ( strcmp ( users_input, "1" ) != 0 && strcmp ( users_input, "2" ) != 0 && strcmp ( users_input, "3" ) != 0 \
            && strcmp ( users_input, "4" ) != 0 )
            fprintf ( stderr, "Wrong input! Accepted: '1', '2', '3', '4'\n" );
        else
            scanning = 0;
    }

    int chosen_mode = select_mode ( users_input );
    switch ( chosen_mode )
    {
        case REGULAR_AKINATOR:
        {
            fprintf ( stderr, "~~~~~~~~~DROCHINATOR~~~~~~~~~\n" );
            fprintf ( protocol, "~~~~~~~~~DROCHINATOR~~~~~~~~~\n" );

            guesser ( &(ptr_data)->ptr_tree [ 0 ], ptr_data, protocol, amount_of_pictures );
            fprintf ( stderr, "##############\npointer from switch: R:'%p', L:'%p'\n##############\n", ptr_data->ptr_tree[ 0 ].negative_result, ptr_data->ptr_tree[ 0 ].positive_result );
            break;
        }

        case COMPARE_MODE:
            //comparer ( ptr_data );
            break;

        case GET_ELEM_CHARACTERISTICS:
            //get_elem_characteristics ();
            break;

        case SEE_DOCUMENTATION:
            //show_documentation ();
            break;

        default:
            fprintf ( stderr, "UKNOWN MODE!\n");
            break;
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

    for ( int i = 0; i < DATA_CAPACITY; i++ )
    {
        if ( ptr->ptr_tree->positive_result == ptr->ptr_tree )
        {
            fprintf ( stderr, "positive_result cycled in %d index\n", i );
            return CYCLIC_LEFT;
        }

        if ( ptr->ptr_tree->negative_result == ptr->ptr_tree )
        {
            fprintf ( stderr, "negative_result cycled in %d index\n", i );
            return CYCLIC_RIGHT;
        }

        if ( ptr->ptr_tree->prev == ptr->ptr_tree )
        {
            fprintf ( stderr, "prev cycled in %d index\n", i );
            return CYCLIC_PREV;
        }
    }

    return ALL_GOOD;
}

// TODO: direction and prev_branch -> branch_t **node
errors_t create_new_branch ( tree_t* ptr_data, int direction, branch_t** prev_branch, char* que_or_answ )
{
    CHECK ( ptr_data );

    ptr_data->size += 1;

fprintf ( stderr, "added1 value: '%s'\n", que_or_answ );
    strncpy ( ptr_data->ptr_tree [ ptr_data->size ].que_or_answ, que_or_answ, MAX_NAME_LEN );
fprintf ( stderr, "added2 value: '%s'\n", ptr_data->ptr_tree [ ptr_data->size ].que_or_answ );
    ptr_data->ptr_tree [ ptr_data->size ].prev = *prev_branch;
fprintf ( stderr, "ptr_back: '%p'\n", ptr_data->ptr_tree [ ptr_data->size ].prev );

    if ( direction == LEFT )
    {
fprintf ( stderr, "********LEFT\n" );
        (*prev_branch)->positive_result = &ptr_data->ptr_tree [ ptr_data->size ];
fprintf ( stderr, "ptr_left: '%p'\n", (*prev_branch)->positive_result );
    }
    else
    {
fprintf ( stderr, "********RIGHT\n" );
        (*prev_branch)->negative_result = &ptr_data->ptr_tree [ ptr_data->size ];
fprintf ( stderr, "ptr_right: '%p'\n", (*prev_branch)->negative_result );
    }

    fprintf ( stderr, "##############\npointer from branch_creator: R:'%p', L:'%p'\n##############\n", ptr_data->ptr_tree[ 0 ].negative_result, ptr_data->ptr_tree[ 0 ].positive_result );

    return ALL_GOOD;
}

void delay ( int ms )
{
  int delay_end_time = clock() + ms;
  while (clock() < delay_end_time);
}

errors_t guesser ( branch_t* ptr_node, tree_t* ptr_data, FILE* protocol, int* amount_of_pictures )
{
    make_grafic_dump ( ptr_data, amount_of_pictures );

    fprintf ( stderr, "THINKING WHAT QUISTEION TO ASK...\n" );
    fprintf ( protocol, "THINKING WHAT QUISTEION TO ASK...\n" );

    int checker_if_leaf = 0;
    if ( check_if_leaf ( ptr_node ) == LEAF )
        checker_if_leaf = 1;
fprintf ( stderr, "checker_if_leaf = %d\n", checker_if_leaf );


    fprintf ( stderr, "is it '%s'?\n", ptr_node->que_or_answ );
    fprintf ( protocol, "is it '%s'?\n", ptr_node->que_or_answ );

    int peasant_result = scan_answer_yes_no ();
fprintf ( stderr, "peasant_answer1 = '%d'\n", peasant_result );

    if ( peasant_result == LEFT )
    {
        if ( checker_if_leaf == 1 )
        {
            fprintf ( protocol, "yes\n" );
            fprintf ( stderr, "Too easy for me! [%s]\n", ptr_node->que_or_answ );
            fprintf ( protocol, "Too easy for me! [%s]\n", ptr_node->que_or_answ );
            return ALL_GOOD;
        }
        else
            guesser ( ptr_node->positive_result, ptr_data, protocol, amount_of_pictures );
    }

    if ( peasant_result == RIGHT )
    {
        if ( checker_if_leaf == 1 )
        {
            fprintf ( protocol, "no\n" );

            create_new_node ( ptr_data, &ptr_node, protocol, amount_of_pictures );
            fprintf ( stderr, "##############\npointer from guesser: R:'%p', L:'%p'\n##############\n", ptr_data->ptr_tree[ 0 ].negative_result, ptr_data->ptr_tree[ 0 ].positive_result );

            guesser ( &ptr_data->ptr_tree [ 0 ], ptr_data, protocol, amount_of_pictures );

            return ALL_GOOD;
        }
        else
            guesser ( ptr_node->positive_result, ptr_data, protocol, amount_of_pictures );
    }

    return ALL_GOOD;
}

int scan_answer_yes_no ()
{
    static char input [ MAX_INPUT_LENGTH ] = {};

    int scanning = 1;

    while ( scanning )
    {
        scanf ( "%s^[\n]", input );
        if ( strcmp ( input, "yes" ) != 0 && strcmp ( input, "no" ) != 0 )
            fprintf ( stderr, "Wrong input! Accepted: 'yes' or 'no'\n");
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

 errors_t create_new_node ( tree_t* ptr_data, branch_t** ptr_prev, FILE* protocol, int* amount_of_pictures )
 {

    fprintf ( stderr, "no idea what it was....\nSo, what is it?\n" );
    fprintf ( protocol, "no idea what it was....\nSo, what is it?\n" );

    char new_answer[ MAX_INPUT_LENGTH ] = {};
    scanf ( "%s^[\n]", new_answer );
fprintf ( stderr, "peasant_answer3 = '%s'\n", new_answer );
    fprintf ( protocol, "%s\n", new_answer );

    fprintf ( stderr, "Adding new branch to tree...\n" );
    fprintf ( protocol, "Adding new branch to tree...\n" );
// TODO: &(ptr_node->negative_result) ??

    create_new_branch ( ptr_data, LEFT, ptr_prev, new_answer );
fprintf ( stderr, "pointer positive_result: '%p'\n", (*ptr_prev)->positive_result );

    make_grafic_dump ( ptr_data, amount_of_pictures );

fprintf ( stderr, "NODE1\n" );
    char tmp_str[ MAX_INPUT_LENGTH ] = {};
    strncpy ( tmp_str, (*ptr_prev)->que_or_answ, MAX_INPUT_LENGTH );
    create_new_branch ( ptr_data, RIGHT, ptr_prev, tmp_str );
fprintf ( stderr, "NODE2\n" );
fprintf ( stderr, "pointer negative_result: '%p'\n", (*ptr_prev)->negative_result );

    make_grafic_dump ( ptr_data, amount_of_pictures );

    fprintf ( stderr, "Which sigh can separate '%s' and '%s'?\n", (*ptr_prev)->que_or_answ, new_answer );
    fprintf ( protocol, "Which sigh can separate '%s' and '%s'?\n", (*ptr_prev)->que_or_answ, new_answer );

    char sigh [ MAX_INPUT_LENGTH ] = {};
    scanf ( "%s^[\n]", (*ptr_prev)->que_or_answ);
fprintf ( stderr, "peasant_answer4 = '%s'\n", sigh );
    fprintf ( protocol, "%s\n", sigh );

    //strncpy ( (*ptr_prev)->que_or_answ, sigh, MAX_INPUT_LENGTH );
    make_grafic_dump ( ptr_data, amount_of_pictures );

    fprintf ( stderr, "##############\npointer from node creator: R:'%p', L:'%p'\n##############\n", ptr_data->ptr_tree[ 0 ].negative_result, ptr_data->ptr_tree[ 0 ].positive_result );
    return ALL_GOOD;
 }

errors_t func_guesser1 ( int direction, branch_t* ptr_node, tree_t* ptr_data, FILE* protocol, int* amount_of_pictures )
{
    branch_t* ptr_goto = NULL;
    if ( direction == LEFT )
        ptr_goto = ptr_node->positive_result;
    if ( direction == RIGHT )
        ptr_goto = ptr_node->negative_result;

    if ( ptr_goto != NULL )
    {
        guesser ( ptr_goto, ptr_data, protocol, amount_of_pictures );
    }
    else
    {
//--------------------------------------------------------
//TODO make as func create_new_node
        if ( direction == LEFT )
        {
            fprintf ( stderr, "Too easy for me! [%s]\n", ptr_node->que_or_answ );
            fprintf ( protocol, "Too easy for me! [%s]\n", ptr_node->que_or_answ );
            return ALL_GOOD;
        }
        else
        {
            fprintf ( stderr, "no idea what it was....\nSo, what is it?\n" );
            fprintf ( protocol, "no idea what it was....\nSo, what is it?\n" );

            char new_answer[ MAX_INPUT_LENGTH ] = {};
            scanf ( "%s", new_answer );
fprintf ( stderr, "peasant_answer3 = '%s'\n", new_answer );
            fprintf ( protocol, "%s\n", new_answer );

            fprintf ( stderr, "Adding new branch to tree...\n" );
            fprintf ( protocol, "Adding new branch to tree...\n" );
// TODO: &(ptr_node->negative_result) ??
            create_new_branch ( ptr_data, direction, &ptr_node, new_answer );

            fprintf ( stderr, "Which sigh can separate '%s' and '%s'?\n", ptr_node->que_or_answ, new_answer );
            fprintf ( protocol, "Which sigh can separate '%s' and '%s'?\n", ptr_node->que_or_answ, new_answer );

            char sigh [ MAX_INPUT_LENGTH ] = {};
            scanf ( "%s", sigh);
fprintf ( stderr, "peasant_answer4 = '%s'\n", sigh );
            fprintf ( protocol, "%s\n", sigh );

            strncpy ( ptr_node->que_or_answ, sigh, MAX_INPUT_LENGTH );
//-----------------------------------------------------------
        }
    }

    return ALL_GOOD;
}



int select_mode ( char* users_input)
{
    if ( strcmp ( users_input, "1" ) == 0 )
        return REGULAR_AKINATOR;
    else if ( strcmp ( users_input, "2" ) == 0 )
        return COMPARE_MODE;
    else if ( strcmp ( users_input, "3" ) == 0 )
        return GET_ELEM_CHARACTERISTICS;
    else if ( strcmp ( users_input, "4" ) == 0 )
        return SEE_DOCUMENTATION;

    return BAD_STRING_IN_SWITCH;
}
