#ifndef AKINATOR_HEADER
#define AKINATOR_HEADER


const int       MAX_NAME_LEN        = 30;
const int       DATA_CAPACITY       = 10;
const int       MAX_INPUT_LENGTH    = 50;
const int       CMD_COMMAND_LEN     = 50;
const char      GAY_VALUE[]         = "SKIBIDI_GITLER";
const char      POISON_STR[]        = "I HATE NIGGERS";
const int       LEFT                = 1;
const int       RIGHT               = 2;

enum leaf_t
{
    LEAF     = 5,
    NOT_LEAF = 10,
};

enum errors_t
{
    ALL_GOOD                    = 334,
    DATA_PTR_POINTS_ON_MY_BALLS = 1,
    TREE_PTR_POINTS_ON_MY_BALLS = 2,
    SIZE_UNDER_PLINTUS          = 3,
    SCAN_MY_BALLS_ON_PRINTER    = 4,
};

struct branch_t
{
    char              que_or_answ [ MAX_NAME_LEN ];
    branch_t*         prev;
    branch_t*         positive_result;
    branch_t*         negative_result;
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
        }                                                                  \

errors_t start_drochinator  ();
errors_t grow_binary_tree   ();
errors_t plant_sapling      ( tree_t* ptr_data );
errors_t tree_checker       ( tree_t* ptr_data );
errors_t struct_tree_ctor   ( tree_t* ptr_data );
errors_t create_new_branch  ( tree_t* ptr_data, int direction, branch_t* prev_branch, char* que_or_answ );
errors_t guesser            ( branch_t* ptr_node, tree_t* ptr_data, FILE* protocol );
leaf_t   check_if_leaf      ( branch_t* ptr_node );
int      scan_answer        ();
void     delay              ( int ms );

#endif
