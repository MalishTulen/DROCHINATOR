#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <TXLib.h>
//#include <io.h>
#include <direct.h>

#include "stack.h"
#include "akinator.h"
#include "grafic_print.h"


// TODO: tree as library
// TODO: right_branch and left_branch instead of positive and negative

errors_t start_drochinator ()
{
    grow_binary_tree ();

    return ALL_GOOD;
}

errors_t grow_binary_tree ()
{
    tree_t stoyak_groota = {};
    tree_t* ptr_data = &stoyak_groota;

    struct_tree_ctor ( ptr_data );

    static int amount_of_pictures = 1;


    if (system ("rm -rf dot_files && mkdir dot_files") == 0)
        printf("Folder created\n");

make_grafic_dump ( ptr_data, &amount_of_pictures );

    choose_mode ( ptr_data, &amount_of_pictures );

    //tree_dump ( ptr_tree );
make_grafic_dump ( ptr_data, &amount_of_pictures );

    return ALL_GOOD;
}

errors_t struct_tree_ctor ( tree_t* ptr_data )
{
    ptr_data->ptr_tree = ( branch_t* ) calloc ( DATA_CAPACITY, sizeof ( branch_t ) );
//fprintf ( stderr, "1ptr_tree = %p\n", ptr_data->ptr_tree );
    ptr_data->size = -1;

    int users_input = 0;
    printf ( "insert tree type ( 1 from file, 2 brand new ):\n" );

    scanf ( "%d", &users_input );

    if ( users_input == 1 )
    {
        FILE* ptr_input_file = fopen ( "save_file.txt", "r" );
        scan_tree ( RIGHT, ( branch_t* )POISON_VALUE, ptr_data, ptr_input_file );
        fclose ( ptr_input_file );
        printf ( "\nSave mode activated!\n");
    }
    else
    {
        int second_answer = 0;
        printf ( "ARE YOU SURE?\n" );
        scanf ( "%d", &second_answer );

        if ( second_answer != 1 )
        {
            printf ( "Processing with normal code!\n" );
            return STOP;
        }
        else
        {
            printf ( "deleting previous data...\n" );
            default_tree_ctor ( ptr_data );
        }
    }
    return ALL_GOOD;
}

errors_t scan_tree ( int direction, branch_t* prev_node, tree_t* ptr_data, FILE* ptr_input_file )
{
    if ( ptr_data->size < DATA_CAPACITY )
    {
        ptr_data->size += 1;

        char tmp_str[ MAX_INPUT_LENGTH ] = {};

        if ( fscanf ( ptr_input_file, "%[^\n]", tmp_str ) == 0 )
            return STOP;
        fgetc ( ptr_input_file );

        char que_or_answ_str[ MAX_INPUT_LENGTH ] = {};

        sscanf ( tmp_str, " { '%[^']", que_or_answ_str );

    //fprintf ( stderr, "cur str: \"%s\"\n", que_or_answ_str );

        strncpy ( ptr_data->ptr_tree [ ptr_data->size ].que_or_answ, que_or_answ_str, MAX_INPUT_LENGTH );

        ptr_data->ptr_tree [ ptr_data->size ].prev = prev_node;

        if ( prev_node != ( branch_t* ) POISON_VALUE )
        {
            if ( direction == LEFT )
                prev_node->left_branch = &ptr_data->ptr_tree [ ptr_data->size ];
            else if ( direction == RIGHT )
                prev_node->right_branch = &ptr_data->ptr_tree [ ptr_data->size ];
        }

        if ( strstr ( tmp_str, "}" ) == 0 )
        {
            int index = ptr_data->size;
            if ( ptr_data->ptr_tree [ index ].left_branch != ( branch_t* )POISON_VALUE )
                scan_tree ( LEFT, &ptr_data->ptr_tree [ index ], ptr_data, ptr_input_file );
            if ( ptr_data->ptr_tree [ index ].right_branch != ( branch_t* )POISON_VALUE )
                scan_tree ( RIGHT, &ptr_data->ptr_tree [ index ], ptr_data, ptr_input_file );
        }
        /*else
fprintf ( stderr, "END STR\n" );*/

    }
    return ALL_GOOD;
}

errors_t default_tree_ctor ( tree_t* ptr_data )
{
    ptr_data->size++;
    CHECK ( ptr_data );

    strncpy ( ptr_data->ptr_tree[ 0 ].que_or_answ, GAY_VALUE, MAX_NAME_LEN );

    ptr_data->ptr_tree [ 0 ].prev = ( branch_t* )POISON_VALUE;

    for ( int i = 1; i < DATA_CAPACITY; i++ )
    {
        ptr_data->ptr_tree [ i ].right_branch = NULL;
        ptr_data->ptr_tree [ i ].left_branch = NULL;
        ptr_data->ptr_tree [ i ].prev = ( branch_t* ) POISON_VALUE;
    }

    return ALL_GOOD;
}

int select_mode ()
{
    char users_input[ MAX_INPUT_LENGTH ] = {};
    int scanning = 1;

    while ( scanning )
    {
        scanf ( "%s", users_input );

        QUIT_CHECKER ( users_input );
        if ( strcmp ( users_input, "1" ) != 0 && strcmp ( users_input, "2" ) != 0 && strcmp ( users_input, "3" ) != 0 \
            && strcmp ( users_input, "4" ) != 0 && strcmp ( users_input, "5" ) != 0 && strcmp ( users_input, "stop" ) != 0 )
            printf ( "Wrong input! Accepted: '1', '2', '3', '4', '5' \"stop\"\n" );
        else
            scanning = 0;
    }

    if ( strcmp ( users_input, "1" ) == 0 )
        return REGULAR_AKINATOR;
    else if ( strcmp ( users_input, "2" ) == 0 )
        return COMPARE_MODE;
    else if ( strcmp ( users_input, "3" ) == 0 )
        return GET_ELEM_CHARACTERISTICS;
    else if ( strcmp ( users_input, "4" ) == 0 )
        return SEE_DOCUMENTATION;
    else if ( strcmp ( users_input, "5" ) == 0 )
        return GRAFIC_PRINT;
    else if ( strcmp ( users_input, "stop" ) == 0 )
        return STOP;

    return BAD_STRING_IN_SWITCH;
}

errors_t choose_mode ( tree_t* ptr_data, int* amount_of_pictures )
{
    printf ( "~~~~~~~~~DROCHINATOR~~~~~~~~~\n" );

    int gayming = 1;
    while ( gayming )
    {
        printf ( "Choose what you want to do:\n"
                        "1 - play regular akinator\n"
                        "2 - compare elements from data\n"
                        "3 - check element from data\n"
                        "4 - about programm\n"
                        "5 - make grafic print\n"
                        "'stop' - to quit\n"
                        "Your input:" );

        int chosen_mode = select_mode ();
        switch ( chosen_mode )
        {
            case REGULAR_AKINATOR:
            {

                guesser ( &(ptr_data)->ptr_tree [ 0 ], ptr_data, amount_of_pictures );
                //fprintf ( stderr, "##############\npointer from switch: R:'%p', L:'%p'\n##############\n", ptr_data->ptr_tree[ 0 ].left_branch, ptr_data->ptr_tree[ 0 ].right_branch );
                save_if_need ( ptr_data );
                break;
            }

            case COMPARE_MODE:
            {
                comparer ( ptr_data );
                break;
            }

            case GET_ELEM_CHARACTERISTICS:
            {
                describe_elem ( ptr_data );
                break;
            }

            case SEE_DOCUMENTATION:
            {
                //show_documentation ();
                break;
            }

            case GRAFIC_PRINT:
            {
                make_grafic_dump ( ptr_data, amount_of_pictures );
                printf ( "NEW PICTURE CREATED!\n" );
                break;
            }

            case STOP:
                gayming = 0;
                break;

            default:
                printf ( "UKNOWN MODE!\n");
                break;
        }
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
        if ( ptr->ptr_tree[ i ].right_branch == &ptr->ptr_tree[ i ] )
        {
            printf ( "right_branch cycled in %d index\n", i );
            return CYCLIC_LEFT;
        }

        if ( ptr->ptr_tree[ i ].left_branch == &ptr->ptr_tree[ i ] )
        {
            printf ( "left_branch cycled in %d index\n", i );
            return CYCLIC_RIGHT;
        }

        if ( ptr->ptr_tree[ i ].prev == &ptr->ptr_tree[ i ] )
        {
            printf ( "prev cycled in %d index\n", i );
            return CYCLIC_PREV;
        }
    }

    return ALL_GOOD;
}

errors_t create_new_branch ( tree_t* ptr_data, int direction, branch_t** prev_branch, char* que_or_answ )
{
    CHECK ( ptr_data );

    ptr_data->size += 1;

//fprintf ( stderr, "added1 value: '%s'\n", que_or_answ );
    strncpy ( ptr_data->ptr_tree [ ptr_data->size ].que_or_answ, que_or_answ, MAX_NAME_LEN );
//fprintf ( stderr, "added2 value: '%s'\n", ptr_data->ptr_tree [ ptr_data->size ].que_or_answ );
    ptr_data->ptr_tree [ ptr_data->size ].prev = *prev_branch;
//fprintf ( stderr, "ptr_back: '%p'\n", ptr_data->ptr_tree [ ptr_data->size ].prev );

    if ( direction == LEFT )
    {
//fprintf ( stderr, "********LEFT\n" );
        (*prev_branch)->left_branch = &ptr_data->ptr_tree [ ptr_data->size ];
//fprintf ( stderr, "ptr_left: '%p'\n", (*prev_branch)->right_branch );
    }
    else
    {
//fprintf ( stderr, "********RIGHT\n" );
        (*prev_branch)->right_branch = &ptr_data->ptr_tree [ ptr_data->size ];
//fprintf ( stderr, "ptr_right: '%p'\n", (*prev_branch)->left_branch );
    }

    //fprintf ( stderr, "##############\npointer from branch_creator: R:'%p', L:'%p'\n##############\n", ptr_data->ptr_tree[ 0 ].left_branch, ptr_data->ptr_tree[ 0 ].right_branch );

    return ALL_GOOD;
}

void delay ( int ms )
{
  int delay_end_time = clock() + ms;
  while (clock() < delay_end_time);
}

errors_t guesser ( branch_t* ptr_node, tree_t* ptr_data, int* amount_of_pictures )
{
    //make_grafic_dump ( ptr_data, amount_of_pictures );

    //printf ( "THINKING WHAT QUISTEION TO ASK...\n" );

    int checker_if_leaf = 0;
    if ( check_if_leaf ( ptr_node ) == LEAF )
        checker_if_leaf = 1;
//fprintf ( stderr, "checker_if_leaf = %d\n", checker_if_leaf );


    printf ( "is it '%s'?\n", ptr_node->que_or_answ );

    int peasant_result = scan_answer_yes_no ();
//fprintf ( stderr, "peasant_answer1 = '%d'\n", peasant_result );

    if ( peasant_result == LEFT )
    {
        if ( checker_if_leaf == 1 )
        {
            printf ( "Too easy for me! [%s]\n", ptr_node->que_or_answ );

            return ALL_GOOD;
        }
        else
            guesser ( ptr_node->left_branch, ptr_data, amount_of_pictures );
    }

    else if ( peasant_result == RIGHT )
    {
        if ( checker_if_leaf == 1 )
        {
            if ( create_new_node ( ptr_data, &ptr_node ) != STOP )
                guesser ( &ptr_data->ptr_tree [ 0 ], ptr_data, amount_of_pictures );
            //fprintf ( stderr, "##############\npointer from guesser: R:'%p', L:'%p'\n##############\n", ptr_data->ptr_tree[ 0 ].left_branch, ptr_data->ptr_tree[ 0 ].right_branch );

            return ALL_GOOD;
        }
        else
            guesser ( ptr_node->right_branch, ptr_data, amount_of_pictures );
    }

    else
        return STOP;

    return ALL_GOOD;
}

int scan_answer_yes_no ()
{
    static char input [ MAX_INPUT_LENGTH ] = {};

    int scanning = 1;

    while ( scanning )
    {
        scanf ( "%s[^\n]", input );
        getchar();

        QUIT_CHECKER ( input );

        if ( strcmp ( input, "yes" ) != 0 && strcmp ( input, "no" ) != 0 )
            printf ( "Wrong input! Accepted: 'yes', 'no' or 'stop'\n");
        else
            scanning = 0;
    }

    if ( strcmp ( input, "yes" ) == 0 )
        return LEFT;
    if ( strcmp ( input, "no" ) == 0 )
        return RIGHT;
    if ( strcmp ( input, END_VALUE ) == 0 )
        return STOP;

    return SCAN_MY_BALLS_ON_PRINTER;
}

leaf_t check_if_leaf ( branch_t* ptr_node )
{
    if ( ptr_node->right_branch == NULL && ptr_node->left_branch == NULL )
        return LEAF;

    return NOT_LEAF;
}

 errors_t create_new_node ( tree_t* ptr_data, branch_t** ptr_prev )
 {

    printf ( "no idea what it was....\nSo, what is it?\n" );

    char new_answer[ MAX_INPUT_LENGTH ] = {};
    scanf ( "%[^\n]", new_answer );
    getchar();

    QUIT_CHECKER ( new_answer );

//fprintf ( stderr, "peasant_answer3 = '%s'\n", new_answer );

    printf ( "Adding new branch to tree...\n" );

    create_new_branch ( ptr_data, LEFT, ptr_prev, new_answer );
//fprintf ( stderr, "pointer right_branch: '%p'\n", (*ptr_prev)->right_branch );

    //make_grafic_dump ( ptr_data, amount_of_pictures );

//fprintf ( stderr, "NODE1\n" );
    char tmp_str[ MAX_INPUT_LENGTH ] = {};
    strncpy ( tmp_str, (*ptr_prev)->que_or_answ, MAX_INPUT_LENGTH );
    create_new_branch ( ptr_data, RIGHT, ptr_prev, tmp_str );
//fprintf ( stderr, "NODE2\n" );
//fprintf ( stderr, "pointer left_branch: '%p'\n", (*ptr_prev)->left_branch );

    //make_grafic_dump ( ptr_data, amount_of_pictures );

    printf ( "Which sigh can separate '%s' and '%s'?\n", (*ptr_prev)->que_or_answ, new_answer );

    scanf ( "%[^\n]", (*ptr_prev)->que_or_answ);
    getchar();
    //strncpy ( (*ptr_prev)->que_or_answ, sigh, MAX_INPUT_LENGTH );
    //make_grafic_dump ( ptr_data, amount_of_pictures );

    //fprintf ( stderr, "##############\npointer from node creator: R:'%p', L:'%p'\n##############\n", ptr_data->ptr_tree[ 0 ].left_branch, ptr_data->ptr_tree[ 0 ].right_branch );
    return ALL_GOOD;
 }

errors_t save_if_need ( tree_t* ptr_data )
{
    FILE* ptr_save_file = fopen ( "save_file.txt", "w" );

    int tab_counter = 0;
    save_node ( &ptr_data->ptr_tree [ 0 ], ptr_data, ptr_save_file, tab_counter );

    fclose ( ptr_save_file );

    return ALL_GOOD;
}

errors_t save_node ( branch_t* ptr_branch, tree_t* ptr_data, FILE* ptr_save_file, int tab_counter )
{
    int checker_if_leaf = check_if_leaf ( ptr_branch );

    tabber ( tab_counter, ptr_save_file );

    fprintf ( ptr_save_file, "{ '%s'", ptr_branch->que_or_answ );

    if ( checker_if_leaf == LEAF )
    {
        fprintf ( ptr_save_file, " nil nil }\n" );

        return ALL_GOOD;
    }

    else
    {
        fprintf ( ptr_save_file, "\n" );


        if ( ptr_branch->left_branch != (branch_t*)POISON_VALUE )
            save_node ( ptr_branch->left_branch, ptr_data, ptr_save_file, (tab_counter)+1 );

        if ( ptr_branch->right_branch != (branch_t*)POISON_VALUE )
            save_node ( ptr_branch->right_branch, ptr_data, ptr_save_file, (tab_counter )+1);

        tabber ( (tab_counter), ptr_save_file );
        fprintf ( ptr_save_file, "}\n" );

        return ALL_GOOD;
    }
    return ALL_GOOD;
}

errors_t tabber ( int cur_tab_amount, FILE* ptr_save_file )
{
//fprintf ( stderr, "tab amount = %d\n", cur_tab_amount );
    for ( int i = 0; i < cur_tab_amount; i++ )
        fprintf ( ptr_save_file, "\t" );

    return ALL_GOOD;
}

errors_t comparer ( tree_t* ptr_data )
{
    char users_input1 [ MAX_INPUT_LENGTH ] = {};
    char users_input2 [ MAX_INPUT_LENGTH ] = {};

    stack_t sigh_array1 = {};
    stack_t sigh_array2 = {};

    stack_t* ptr_stk1 = &sigh_array1;
    stack_t* ptr_stk2 = &sigh_array2;

    StackCtor ( ptr_stk1, DATA_CAPACITY );
    StackCtor ( ptr_stk2, DATA_CAPACITY );

    printf ( "Insert first element:" );
    scan_value ( ptr_data, users_input1, ptr_stk1 );

    printf ( "Insert second element:" );
    scan_value ( ptr_data, users_input2, ptr_stk2 );

    StackDump ( &sigh_array1 );
    StackDump ( &sigh_array2 );

    printf ( "Similarities: " );

    branch_t* index = &ptr_data->ptr_tree [ 0 ];

    int counter = -1;
    int max_limit = min ( ptr_stk1->size, ptr_stk2->size );
    int checker_on_equality = 0;
    while ( counter < max_limit )
    {
//fprintf ( stderr, "counter = %d\n", max_limit );
        counter++;
        int dir1 = StackPop ( ptr_stk1 );
        int dir2 = StackPop ( ptr_stk2 );
//fprintf ( stderr, "dir1 = %d, dir2 = %d\n ", dir1, dir2 );
//fprintf ( stderr, "ERROR1\n" );
        if ( dir1 == dir2 )
        {
            if ( checker_on_equality == 1 )
                printf ( ", " );

            checker_on_equality = 1;
//fprintf ( stderr, "CONNECTIoN!\n" );

            if ( dir1 == RIGHT )
            {
                printf ( "%s", index->que_or_answ );
                index = index->right_branch;
            }
            else if ( dir1 == LEFT )
            {
                printf ( "not %s", index->que_or_answ );
                index = index->left_branch;
            }
        }
        else
        {
            checker_on_equality = 0;
            StackPush ( ptr_stk1, dir1 );
            StackPush ( ptr_stk2, dir2 );
            break;
        }
    }

    printf ( "\b\b\nDifferences:\nobject1: " );

    branch_t* index1 = index;
    while ( ptr_stk1->size > 0 )
    {
        if ( checker_on_equality == 1 )
            printf ( ", " );

        checker_on_equality = 1;

        int dir = StackPop ( ptr_stk1 );

        if ( dir == RIGHT )
            {
                printf ( "%s", index1->que_or_answ );
                index1 = index1->right_branch;
            }
            else if ( dir == LEFT )
            {
                checker_on_equality = 0;
                printf ( "not %s", index1->que_or_answ );
                index1 = index1->left_branch;
            }
    }

    printf ( "\b\b\nobject2: " );
    while ( ptr_stk2->size > 0 )
    {
        if ( checker_on_equality == 1 )
            printf ( ", " );
        checker_on_equality = 1;

        int dir = StackPop ( ptr_stk2 );

        if ( dir == RIGHT )
            {
                printf ( "%s", index->que_or_answ );
                index = index->right_branch;
            }
            else if ( dir == LEFT )
            {
                checker_on_equality = 0;
                printf ( "not %s", index->que_or_answ );
                index = index->left_branch;
            }
    }
    printf ( "\b\b\n" );

    return ALL_GOOD;
}

errors_t scan_value ( tree_t* ptr_data, char* user_input, stack_t* sign_array )
{
    int scanning = 1;
    while ( scanning )
    {
        scanf ( " %[^\n]", user_input );
        getchar();
        QUIT_CHECKER ( user_input );

        int counter = 0;
        for ( int i = 0; i < sign_array->capacity; i++ )
        {
            if ( check_if_leaf ( &ptr_data->ptr_tree [ i ] ) == LEAF && strcmp ( ptr_data->ptr_tree [ i ].que_or_answ, user_input ) == 0 )
            {
                scanning = 0;
            }
            else
            {
                if ( counter == sign_array->capacity - 1 )
                    printf ( "Your input is not good >:(  [%s]\nInput again:", user_input );
                counter++;
            }
        }
    }

//fprintf ( stderr, "scan value 1\n" );
    get_description ( ptr_data, user_input, sign_array );
//fprintf ( stderr, "scan value 2\n" );

    return ALL_GOOD;
}

errors_t get_description ( tree_t* ptr_data, char* users_input, stack_t* sigh_array )
{
    int elem_index = -1;

    for ( int i = 0; i < DATA_CAPACITY; i++ )
    {
        if ( strcmp ( ptr_data->ptr_tree [ i ].que_or_answ, users_input ) == 0 )
        {
//fprintf ( stderr, "HAVE FOUND RIGHT ELEM!\n" );
            elem_index = i;
            break;
        }
    }

//fprintf ( stderr, "description 1\n" );
//fprintf ( stderr, "elem_index = '%s'\n", ptr_data->ptr_tree [ elem_index ].que_or_answ );
    get_characteristic ( &ptr_data->ptr_tree [ elem_index ], sigh_array );
//fprintf ( stderr, "description 2\n" );
    print_description ( ptr_data, sigh_array, &users_input );
//fprintf ( stderr, "description 3\n" );

    return ALL_GOOD;
}

errors_t get_characteristic ( branch_t* ptr_node, stack_t* sigh_array )
{
//StackDump ( sigh_array );
//fprintf ( stderr, "characteristic 1\n" );
//fprintf ( stderr, "ptr_to_prev = '%p', ptr_node_adress = '%p', prevs_right = '%p', prevs_left = '%p'\n", ( branch_t* )ptr_node->prev, ptr_node, ptr_node->prev->right_branch, ptr_node->prev->left_branch );
    if ( ptr_node->prev != ( branch_t* )POISON_VALUE )
    {
//fprintf ( stderr, "characteristic 1.1\n" );
        if ( ptr_node == ptr_node->prev->left_branch )
        {
            StackPush ( sigh_array, LEFT );
//fprintf ( stderr, "LEFT\n" );
        }
        else if ( ptr_node == ptr_node->prev->right_branch)
        {
            StackPush ( sigh_array, RIGHT );
//fprintf ( stderr, "RIGHT\n" );
        }
        else
            return STOP;

//fprintf ( stderr, "characteristic 1.2\n" );
//fprintf ( stderr, "characteristic 2\n" );
        get_characteristic ( ptr_node->prev, sigh_array );
        return ALL_GOOD;
    }
//fprintf ( stderr, "characteristic 3\n" );

    return ALL_GOOD;
}

errors_t print_description ( tree_t* ptr_data, stack_t* sigh_array, char** users_input )
{
//StackDump ( sigh_array );
//fprintf ( stderr, "size = %d\n", sigh_array->size );

    int counter = sigh_array->size;
    print_characteristic ( &ptr_data->ptr_tree[ 0 ], sigh_array, &counter );
    printf ( "%s\n", *users_input );

    return ALL_GOOD;
}

errors_t print_characteristic ( branch_t* ptr_node, stack_t* sigh_array, int* counter )
{
    *(counter) -= 1;
    if ( *counter > -1 )
    {
        if ( sigh_array->data_ptr [ *counter ] == LEFT )
        {
            printf ( "not %s->", ptr_node->que_or_answ );
            print_characteristic ( ptr_node->left_branch, sigh_array, counter);
        }
        else
        {
            printf ( "%s->", ptr_node->que_or_answ );
            print_characteristic ( ptr_node->right_branch, sigh_array, counter);
        }
    }

    return ALL_GOOD;
}

errors_t describe_elem ( tree_t* ptr_data )
{
    char users_input[ MAX_INPUT_LENGTH ] = {};

    stack_t sigh_array = {};
    StackCtor ( &sigh_array, DATA_CAPACITY );

    printf ( "Insert elem:" );
    scan_value ( ptr_data, users_input, &sigh_array );

    return ALL_GOOD;
}


