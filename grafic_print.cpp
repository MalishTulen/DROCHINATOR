#include <stdio.h>
#include <stdlib.h>

#include "akinator.h"
#include "grafic_print.h"

errors_t make_grafic_dump ( tree_t* ptr_data, int* amount_of_pictures )
{
    CHECK ( ptr_data );

    const char  dot_file[] = "print.dot";
    const char  output_file[] = "a";

    FILE* ptr_dot_file = fopen ( "print.dot", "w" );

    create_dot_code ( ptr_data, ptr_dot_file );

    fclose ( ptr_dot_file );

    make_picture ( dot_file, output_file, amount_of_pictures );

    return ALL_GOOD;
}

errors_t create_dot_code ( tree_t* ptr_data, FILE* ptr_dot_file )
{
    fprintf ( ptr_dot_file, "digraph data{\n" );
    fprintf ( ptr_dot_file, "rankdir = TB;\n" );

    for ( int i = 0; i < DATA_CAPACITY; i++ )
    {
        int checker_if_leaf = check_if_leaf ( &ptr_data->ptr_tree[ i ] );
            if ( checker_if_leaf == LEAF )
                fprintf ( ptr_dot_file, "node%p[ shape = record, label = \" {<f0> NODE%d(%p) | <f1> %s | <f2> prev = (%p) | { <f3> ДА ( %p ) | <f4> ПИЗДА ( %p ) } } \"];\n", &ptr_data->ptr_tree[ i ], i, &ptr_data->ptr_tree[ i ], ptr_data->ptr_tree [ i ].que_or_answ,ptr_data->ptr_tree [ i ].prev, ptr_data->ptr_tree [ i ].positive_result, ptr_data->ptr_tree [ i ].negative_result );
            if ( checker_if_leaf == NOT_LEAF )
                fprintf ( ptr_dot_file, "node%p[ shape = record, label = \" {<f0> NODE%d(%p) | <f1> %s? | <f2> prev = (%p) | { <f3> ДА ( %p ) | <f4> ПИЗДА ( %p ) } } \"];\n", &ptr_data->ptr_tree[ i ], i, &ptr_data->ptr_tree[ i ], ptr_data->ptr_tree [ i ].que_or_answ,ptr_data->ptr_tree [ i ].prev, ptr_data->ptr_tree [ i ].positive_result, ptr_data->ptr_tree [ i ].negative_result );
    }
//fprintf ( stderr, "HUI1\n");
    branch_connector ( &ptr_data->ptr_tree [ 0 ], ptr_data, ptr_dot_file );
//fprintf ( stderr, "HUI2\n");
    fprintf ( ptr_dot_file, "}\n" );

    return ALL_GOOD;
}

errors_t make_picture ( const char* dot_file, const char* output_file, int* amount_of_pictures )
{
    char cmd_command [ CMD_COMMAND_LEN ] = {};
    sprintf ( cmd_command, "dot %s -T png -o %s%d.png", dot_file, output_file, *amount_of_pictures );

    ( *amount_of_pictures )++;
    //fprintf ( stderr, "command = '%s'\n", cmd_command );
    system ( cmd_command );

    return ALL_GOOD;
}

errors_t branch_connector ( branch_t* ptr_index, tree_t* ptr_data, FILE* ptr_dot_file )
{
    //int checker_if_leaf = 0;
//fprintf ( stderr, "POSIT = '%p'\n", ptr_index->positive_result );
    if ( ptr_index->positive_result != NULL)
    {
        fprintf ( ptr_dot_file, "node%p:<f3>->node%p\n", ptr_index, ptr_index->positive_result );
        branch_connector ( ptr_index->positive_result, ptr_data, ptr_dot_file );
        //checker_if_leaf++;
    }
    if ( ptr_index->negative_result != NULL)
    {
        fprintf ( ptr_dot_file, "node%p:<f4>->node%p\n", ptr_index, ptr_index->negative_result );
        branch_connector ( ptr_index->negative_result, ptr_data, ptr_dot_file );
        //checker_if_leaf += 2;
    }

    /*if ( checker_if_leaf == 0 )
        return ALL_GOOD;
    else
        if ( checker_if_leaf == 1 )
        else if ( checker_if_leaf == 2)
            branch_connector ( ptr_index->negative_result, ptr_data, ptr_dot_file );
        else if ( checker_if_leaf == 3)
            branch_connector ( ptr_index->positive_result, ptr_data, ptr_dot_file );
*/
    return ALL_GOOD;
}
