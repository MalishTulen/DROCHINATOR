
errors_t default_tree_ctor ( tree_t* ptr_data )
{
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
