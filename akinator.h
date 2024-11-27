#ifndef AKINATOR_HEADER
#define AKINATOR_HEADER


const int       MAX_NAME_LEN        = 30;
const int       DATA_CAPACITY       = 10;
const int       MAX_INPUT_LENGTH    = 50;
const int       CMD_COMMAND_LEN     = 50;
const char      GAY_VALUE[]         = "SKIBIDI_GITLER";
const char      POISON_STR[]        = "I HATE NIGGERS";
const char      END_VALUE[]         = "stop";
const int       LEFT                = 1;
const int       RIGHT               = 2;

enum leaf_t
{
    LEAF     = 5,
    NOT_LEAF = 10,
};

enum modes_t
{
    REGULAR_AKINATOR         = 1,
    COMPARE_MODE             = 2,
    GET_ELEM_CHARACTERISTICS = 3,
    SEE_DOCUMENTATION        = 4,
};

enum errors_t
{
    ALL_GOOD                    = 334,
    DATA_PTR_POINTS_ON_MY_BALLS = 1,
    TREE_PTR_POINTS_ON_MY_BALLS = 2,
    SIZE_UNDER_PLINTUS          = 3,
    SCAN_MY_BALLS_ON_PRINTER    = 4,
    BAD_STRING_IN_SWITCH        = 5,
    CYCLIC_LEFT                 = 6,
    CYCLIC_RIGHT                = 7,
    CYCLIC_PREV                 = 8,
    BAD_INPUT_CODE              = 9,
    STOP                        = 69,
};

struct branch_t
{
    char              que_or_answ [ MAX_NAME_LEN ];
    branch_t*         prev;
    branch_t*         left_branch;
    branch_t*         right_branch;
};

struct tree_t
{
    branch_t*   ptr_tree;
    int         size;
};

const long long POISON_VALUE = 0xD01B0EB7;

#define CHECK( ptr )                                                       \
        if ( tree_checker ( ptr ) == DATA_PTR_POINTS_ON_MY_BALLS )         \
        {                                                                  \
            fprintf ( stderr, "BAD_DATA_PTR '%s'\n", #ptr );               \
            return DATA_PTR_POINTS_ON_MY_BALLS;                            \
        }                                                                  \
        if ( tree_checker ( ptr ) == TREE_PTR_POINTS_ON_MY_BALLS )         \
        {                                                                  \
            fprintf ( stderr, "BAD_TREE_PTR '%s'\n", #ptr );               \
            return TREE_PTR_POINTS_ON_MY_BALLS;                            \
        }                                                                  \
        if ( tree_checker ( ptr ) == SIZE_UNDER_PLINTUS )                  \
        {                                                                  \
            fprintf ( stderr, "BAD_SIZE '%s'\n", #ptr );                   \
            return SIZE_UNDER_PLINTUS;                                     \
        }

#define QUIT_CHECKER(str)                                                  \
    if ( strcmp ( str, END_VALUE ) == 0 )                                  \
        return STOP;

errors_t start_drochinator      ();
errors_t choose_mode            ( tree_t* ptr_data, int* amount_of_pictures );
errors_t grow_binary_tree       ();
errors_t struct_tree_ctor       ( tree_t* ptr_data );
errors_t scan_tree              ( int direction, branch_t* prev_node, tree_t* ptr_data, FILE* ptr_input_file );
errors_t default_tree_ctor      ( tree_t* ptr_data );
errors_t tree_checker           ( tree_t* ptr_data );
errors_t create_new_branch      ( tree_t* ptr_data, int direction, branch_t** prev_branch, char* que_or_answ );
errors_t create_new_node        ( tree_t* ptr_data, branch_t** ptr_prev );
errors_t guesser                ( branch_t* ptr_node, tree_t* ptr_data, int* amount_of_pictures );
leaf_t   check_if_leaf          ( branch_t* ptr_node );
errors_t save_if_need           ( tree_t* ptr_data );
errors_t save_node              ( branch_t* ptr_branch, tree_t* ptr_data, FILE* ptr_save_file, int tab_counter );
errors_t tabber                 ( int cur_tab_amount, FILE* ptr_save_file );
errors_t comparer               ( tree_t* ptr_data );
errors_t get_description        ( tree_t* ptr_data, char* users_input, stack_t* sigh_array );
errors_t scan_value             ( tree_t* ptr_data, char* user_input, stack_t* sign_array );
errors_t get_characteristic     ( branch_t* ptr_node, stack_t* sigh_array );
errors_t print_description      ( tree_t* ptr_data, stack_t* sigh_array, char** users_input );
errors_t print_characteristic   ( branch_t* ptr_node, stack_t* sigh_array, int* counter );
errors_t describe_elem          ( tree_t* ptr_data );
errors_t print_elem_from_stack  ( branch_t* ptr_node, stack_t* sigh_array, int index, int* counter );

int      scan_answer_yes_no ();
void     delay              ( int ms );
int      select_mode        ();


#endif
