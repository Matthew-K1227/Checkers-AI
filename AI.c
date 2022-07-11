#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <ctype.h>

/******************************************************************************/

#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions
#define CAPITAL_A           'A'     // upper-case 'A' character
#define CAPITAL_P           'P'     // upper-case 'A' character
#define MIN_COL             'A'     // minimum column
#define MAX_COL             'H'     // maximum column
#define NORTH               -1      // north direction of action
#define SOUTH               1       // south direction of action
#define EAST                1       // east direction of action
#define WEST                -1      // west direction of action
#define RETURN_SUCCESS      1       // function call executed successfully
#define RETURN_FAILURE      0       // function call failed
#define NUM_OF_DIR          4       // number of directions


#define ACTION_INPUT                        0   // input action
#define ACTION_COMPUTED                     1   // computed action
#define ACTION_LEGAL                        0   // legal action
#define ACTION_ILLEGAL_SRC_OUTSIDE          1   // source cell outside board
#define ACTION_ILLEGAL_TGT_OUTSIDE          2   // target cell outside board
#define ACTION_ILLEGAL_SRC_EMPTY            3   // source cell is empty
#define ACTION_ILLEGAL_TGT_NOT_EMPTY        4   // target cell is not empty
#define ACTION_ILLEGAL_SRC_HOLDS_OPPONENT   5   // source cell holds opponent
#define ACTION_ILLEGAL_OTHER                6   // other illegal action
#define INPUT_UNKNOWN                       0   // unknown input command
#define INPUT_ACTION                        1   // input action command
#define INPUT_COMMAND_COMPUTE_ACTION        2   // input compute action command
#define INPUT_COMMAND_PLAY                  3   // input play game command
#define TRUE                                1
#define FALSE                               0

#define PLAYER_MENU                 "\n------CHOOSE PLAYER------\n      WHITE=>W \n      BLACK=>B\n"
#define STR_BOARD_SIZE              "BOARD SIZE: %dx%d\n"
#define STR_BLACK_PIECES_STATS      "#BLACK PIECES: %d\n"
#define STR_WHITE_PIECES_STATS      "#WHITE PIECES: %d\n"
#define STR_DELIMITER               "=====================================\n"
#define STR_BOARD_LINE_BREAK        "\n   +---+---+---+---+---+---+---+---+\n"
#define STR_COMPUTED_ACTION_MARKER  "*** "
#define STR_WHITE_ACTION            "WHITE ACTION #%d: "
#define STR_BLACK_ACTION            "BLACK ACTION #%d: "
#define STR_ACTION_INFO             "%c%d-%c%d\n"
#define STR_ACTION_INFO_SFX         "%d-%c%d\n"
#define STR_BOARD_COST              "BOARD COST: %d\n"
#define STR_BLACK_WIN               "BLACK WIN!\n"
#define STR_WHITE_WIN               "WHITE WIN!\n"
#define STR_DOUBLE_SPACE            "  "
#define STR_NEW_LINE                "\n"
#define STR_BOARD_COL_TITLE         "   %c"
#define STR_BOARD_ROW_TITLE         "%2d |"
#define STR_BOARD_ROW_INDENT        "\n   +"
#define STR_BOARD_CELL_CONTENT      " %c |"


#define STR_ERROR_1         "ERROR: Source cell is outside of the board.\n"
#define STR_ERROR_2         "ERROR: Target cell is outside of the board.\n"
#define STR_ERROR_3         "ERROR: Source cell is empty.\n"
#define STR_ERROR_4         "ERROR: Target cell is not empty.\n"
#define STR_ERROR_5         "ERROR: Source cell holds opponent's piece/tower.\n"
#define STR_ERROR_6         "ERROR: Illegal action.\n"

/******************************************************************************/
/*******************************GLOBAL VARIABLES*******************************/

int LAT[NUM_OF_DIR] = {NORTH,SOUTH,SOUTH,NORTH};
int LON[NUM_OF_DIR] = {EAST,EAST,WEST,WEST};

/******************************************************************************/
/********************************TYPE DEFINITION*******************************/

typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

    
typedef struct action {
  int t_row;  //target row
  int t_col;  //target col 
  int s_row;  //source row
  int s_col;  //source col
}action_t;


typedef struct node node_t;
struct node{ // board node 
    action_t act;    // action 
    board_t  brd;    //  board that result in the action 
    int      cost;   //  cost of the board 
    int      num;    //  upper bound of reachable boards
    node_t     **chrn;  //  children of this node 
};



/******************************************************************************/
/******************************************************************************/
void main_menu(board_t board);
void initialize_board(board_t intial_board);
int  count_board_cells(board_t board, char type);
void print_board(board_t board);
void read_action(board_t board,char player);
void read_input(action_t* A);
void take_action(board_t B, action_t* A);
int  check_action(board_t board, action_t* A,int turn);
int  is_on_board(int row, int col);
void print_error(int status);
void print_action(board_t board, action_t* A, int turn, int computed);
int  get_board_cost(board_t board);
void minmax_ai(board_t board, action_t A, int turn);
int  compute_action(board_t board, action_t* A, int turn);
int  is_player_turn(char player,int turn);
void clone_board(board_t board, board_t new_board);
void construct_tree(node_t* root, int depth, int turn, int iturn);
void construct_child_node(node_t* node, int cid, action_t* A, int depth, int turn);
void set_tree_costs(node_t* node, int turn, int iturn);
void free_tree(node_t* node);
void print_winner(int turn);


/******************************************************************************/
/************************************MAIN**************************************/
int
main(int argc, char *argv[]) {
    board_t board;
    initialize_board(board);
    main_menu(board);
 
    return EXIT_SUCCESS;            // exit program with the success code
}

/* THE END -------------------------------------------------------------------*/
/*******************************STAGE-0 ***************************************/

void 
main_menu(board_t board) {
    char player;
    /*printing intial info of the board*/
    printf(STR_BOARD_SIZE,BOARD_SIZE,BOARD_SIZE);
    printf(STR_BLACK_PIECES_STATS,count_board_cells(board,CELL_BPIECE));
    printf(STR_WHITE_PIECES_STATS,count_board_cells(board,CELL_WPIECE));
    print_board(board);
    printf(PLAYER_MENU);
    scanf("%c",&player);
    if(!(player=='W' || player=='B')){
        printf("\n------**ENTER VALID PLAYER**------\n");
        main_menu(board);
    }
    read_action(board,player);
}

/***************************STAGE-0 HELPER FUNCTIONS***************************/

void
read_action(board_t board,char player){
    assert(board!=NULL);
    action_t A;
    
    int status, turn=0;



    while(turn<INT_MAX) {
        if(is_player_turn(player,turn)) {
            read_input(&A);
            status=check_action(board, &A,turn);
            if(status==ACTION_LEGAL) {
                take_action(board, &A);
                print_action(board,&A,turn,ACTION_INPUT);
                turn++;
            }
            else {
                print_error(status);
            }
        }
        else {
            minmax_ai(board,A,turn);
            turn++;
            }    
    }
}

   


void
read_input(action_t* A){
    assert(A!=NULL);
    char t;
    char s;    
    printf("ENTER SOURCE CELL AND TARGET CELL:\n");                   
    if (scanf(STR_ACTION_INFO,&s,&A->s_row,&t,&A->t_row)==4) {
        
        A->s_row--; A->t_row--;  // adjust source and target rows
        A->s_col=s - CAPITAL_A;  // set source column
        A->t_col=t - CAPITAL_A;  // adjust target column

    }
     printf("Player Input :"STR_ACTION_INFO,A->s_col+CAPITAL_A,A->s_row+1,
        A->t_col+CAPITAL_A,A->t_row+1);
}



void
print_action(board_t board, action_t* A, int turn, int computed ) {
    assert(board!=NULL && A!=NULL);
    printf(STR_DELIMITER);
    if(computed){
        printf(STR_COMPUTED_ACTION_MARKER);
    }
    if(turn%2){
        printf(STR_WHITE_ACTION,turn+1);
    } 
    else {
        printf(STR_BLACK_ACTION,turn+1);
    }
    printf(STR_ACTION_INFO,A->s_col+CAPITAL_A,A->s_row+1,
        A->t_col+CAPITAL_A,A->t_row+1);
    printf(STR_BOARD_COST,get_board_cost(board));
    print_board(board);
}

int
get_board_cost(board_t board){
    int w_piece_cost=COST_PIECE*count_board_cells(board,CELL_WPIECE);
    int w_tower_cost=COST_TOWER*count_board_cells(board,CELL_WTOWER); 
    int b_piece_cost=COST_PIECE*count_board_cells(board,CELL_BPIECE);
    int b_tower_cost=COST_TOWER*count_board_cells(board,CELL_BTOWER);
    return ((b_piece_cost-w_piece_cost) + (b_tower_cost-w_tower_cost));
}

int 
check_action(board_t board, action_t* A,int turn) {
    assert(board!=NULL && A!=NULL);
    if(!is_on_board(A->s_row,A->s_col)){
        return ACTION_ILLEGAL_SRC_OUTSIDE;
    }
    else if(!is_on_board(A->t_row,A->t_col)){
        return ACTION_ILLEGAL_TGT_OUTSIDE;
    }
    else if(board[A->s_row][A->s_col]==CELL_EMPTY){
        return ACTION_ILLEGAL_SRC_EMPTY;
    }
    else if(board[A->t_row][A->t_col]!=CELL_EMPTY){
        return ACTION_ILLEGAL_TGT_NOT_EMPTY;
    }
    else if(turn%2 && (board[A->s_row][A->s_col]==CELL_BPIECE || 
    board[A->s_row][A->s_col]==CELL_BTOWER)) {
        return ACTION_ILLEGAL_SRC_HOLDS_OPPONENT;
    }
    else if(!(turn%2) && (board[A->s_row][A->s_col]==CELL_WPIECE || 
    board[A->s_row][A->s_col]==CELL_WTOWER)) {
        return ACTION_ILLEGAL_SRC_HOLDS_OPPONENT;
    }
    else if (!((abs(A->s_row-A->t_row)==1 && abs(A->s_col-A->t_col)==1) ||
        (abs(A->s_row-A->t_row)==2 && abs(A->s_col-A->t_col)==2))) {
        return ACTION_ILLEGAL_OTHER;        
    } else if (board[A->s_row][A->s_col]==CELL_BPIECE && A->s_row-A->t_row<0) {
        return ACTION_ILLEGAL_OTHER;        
    } else if (board[A->s_row][A->s_col]==CELL_WPIECE && A->s_row-A->t_row>0) {
        return ACTION_ILLEGAL_OTHER;        
    } else if (abs(A->s_row-A->t_row)==2 && abs(A->s_col-A->t_col)==2) {
        int r = (A->s_row+A->t_row)/2;
        int c = (A->s_col+A->t_col)/2;
        if ((turn%2 && !(board[r][c]==CELL_BPIECE || board[r][c]==CELL_BTOWER)) ||
            (!(turn%2) && !(board[r][c]==CELL_WPIECE || board[r][c]==CELL_WTOWER))) {
            return ACTION_ILLEGAL_OTHER;    
        }
    }
    return ACTION_LEGAL;
}

void
print_error(int status) {
    if (status==ACTION_ILLEGAL_SRC_OUTSIDE) {
        printf(STR_ERROR_1);
    } else if (status==ACTION_ILLEGAL_TGT_OUTSIDE) {
        printf(STR_ERROR_2);
    } else if (status==ACTION_ILLEGAL_SRC_EMPTY) {
        printf(STR_ERROR_3);
    } else if (status==ACTION_ILLEGAL_TGT_NOT_EMPTY) {
        printf(STR_ERROR_4);
    } else if (status==ACTION_ILLEGAL_SRC_HOLDS_OPPONENT) {
        printf(STR_ERROR_5);
    } else if (status==ACTION_ILLEGAL_OTHER) {
        printf(STR_ERROR_6);
    }
}

int
is_on_board(int row, int col) {
    if (row<0 || row>=BOARD_SIZE || col<0 || col>=BOARD_SIZE) {
        return 0;   // cell is outside of the board
    }
    return 1;       // cell is on the board
}


void
take_action(board_t B, action_t* A) {
    assert(A!=NULL && B!=NULL);
    B[A->t_row][A->t_col]=B[A->s_row][A->s_col];   // transfer piece/tower
    B[A->s_row][A->s_col]=CELL_EMPTY;              // mark source cell as empty
    if (abs(A->s_row-A->t_row)==2 && abs(A->s_col-A->t_col)==2) {
        // mark cell of the captured piece or tower as empty
        B[(A->s_row+A->t_row)/2][(A->s_col+A->t_col)/2]=CELL_EMPTY;
    }
    if (A->t_row==0 && B[A->t_row][A->t_col]==CELL_BPIECE) {
        B[A->t_row][A->t_col]=CELL_BTOWER;     // promote black piece to a tower
    } else if (A->t_row==BOARD_SIZE-1 && B[A->t_row][A->t_col]==CELL_WPIECE) {
        B[A->t_row][A->t_col]=CELL_WTOWER;     // promote white piece to a tower
    }
}

/*******************************STAGE-1****************************************/
void  
minmax_ai(board_t board, action_t A, int turn){
    if(compute_action(board,&A,turn)){
        take_action(board,&A);
        print_action(board,&A,turn,ACTION_COMPUTED);
    }

}

/***************************STAGE-1 HELPER FUNCTIONS***************************/
int 
compute_action(board_t board, action_t* A, int turn) {
    assert(board!=NULL && A!=NULL);
    node_t* root;
    root=(node_t*)malloc(sizeof(*root));
    assert(root!=NULL);
    clone_board(board,root->brd); // root board
    construct_tree(root, TREE_DEPTH, turn, turn); // constructing minmax tree
    set_tree_costs(root,turn,turn);
    for(int i=0; i<root->num; i++){
        if(root->chrn[i]!=NULL && root->cost==root->chrn[i]->cost) {
            *A=root->chrn[i]->act;
            free_tree(root);
            return RETURN_SUCCESS;
        }
    }
    free_tree(root);
    return RETURN_FAILURE;
}


void
set_tree_costs(node_t* node, int turn, int iturn) {
    assert(node!=NULL);
    if(turn-iturn==TREE_DEPTH){
        node->cost=get_board_cost(node->brd);
        return;
    }
    if (turn%2){             //white turn 
        node->cost=INT_MAX;  //intialize node cost
    }
    else {                   //black turn 
        node->cost=INT_MIN;  //intialize node cost 
    }

    for(int i=0; i<node->num; i++) {
        if (node->chrn[i]!=NULL) {
            set_tree_costs(node->chrn[i],turn+1,iturn); //recursive 
            if (turn%2 && node->chrn[i]->cost<node->cost) {
                node->cost=node->chrn[i]->cost; //white aims to minimize 
            }
            if (!(turn%2) && node->chrn[i]->cost>node->cost) {
                node->cost=node->chrn[i]->cost;  //black aims to maximize 
            }
        }
    }
}



void
construct_tree(node_t* root, int depth, int turn, int iturn) {
    assert(root!=NULL);
    // number of possible direction
    if(turn%2){
        root->num = NUM_OF_DIR*(count_board_cells(root->brd,CELL_WPIECE)
                    + count_board_cells(root->brd,CELL_WTOWER));
    }
    else{
        root->num = NUM_OF_DIR*(count_board_cells(root->brd,CELL_BPIECE)
                 + count_board_cells(root->brd,CELL_BTOWER));
    }

    root->chrn=(node_t**)malloc((root->num)*sizeof(node_t*));
    assert(root->chrn!=NULL);
    for(int i=0; i<root->num; i++){
        root->chrn[i]=NULL; //initialize to NULL
    }

    if(depth<=0){
        return; //prune recursion 
    }
    action_t A;
    for(int i=0; i<BOARD_SIZE;i++) {
        for(int j=(i+1)%2; j<BOARD_SIZE; j++){
            if ((root->brd[i][j]==CELL_EMPTY) || // cell is empty
                ((root->brd[i][j]==CELL_BPIECE || root->brd[i][j]==CELL_BTOWER)
                  && turn%2) ||                  // black element for white turn
                ((root->brd[i][j]==CELL_WPIECE || root->brd[i][j]==CELL_WTOWER)
                && !(turn%2))) {   
                continue;
            }

            // construct children starting north-east direction 
            int status;
            int k=0;
            for(int offset=0; offset<NUM_OF_DIR; offset++){
                A.s_row=i;A.s_col=j;A.t_row=i+LAT[offset];A.t_col=j+LON[offset];
                if((status=check_action(root->brd,&A,turn))==ACTION_LEGAL){
                    // move is legal -> construct corresponding child node
                    construct_child_node(root,NUM_OF_DIR*k+offset,&A,depth,turn);
                }

                else if(status==ACTION_ILLEGAL_TGT_NOT_EMPTY){
                    // a capture still possible
                    A.t_row+=LAT[offset];A.t_col+=LON[offset];
                    if(check_action(root->brd,&A,turn)==ACTION_LEGAL){
                        // capture is poosible -> contruct child node 
                        construct_child_node(root,NUM_OF_DIR*k+offset,&A,depth,turn);
                    }
                }
            }

            k++;
        }
    }
}

// constructing child node 
void 
construct_child_node(node_t* node,int cid, action_t* A, int depth, int turn) {
    assert(node!=NULL && A!=NULL);
    node->chrn[cid]=(node_t*)malloc(sizeof(node_t));
    assert(node->chrn[cid]!=NULL);
    clone_board(node->brd,node->chrn[cid]->brd);
    node->chrn[cid]->act=*A;
    take_action(node->chrn[cid]->brd,&node->chrn[cid]->act);
    // recursive call to construct child
    construct_tree(node->chrn[cid],depth-1,turn+1,turn);
}

void
clone_board(board_t board, board_t new_board){
    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++){
            new_board[i][j]=board[i][j]; //copy cell from old board to new board
        }
    }
}

/******************************HELPER FUNCTIONS********************************/

void 
initialize_board(board_t intial_board) {

    for (int row=0; row<BOARD_SIZE; row++) {

        for (int col = 0; col < BOARD_SIZE; col++) {
            /*condition to place the white pieces*/
            if(((row+col+1)%2)==0 && row<ROWS_WITH_PIECES) {

                intial_board[row][col]=CELL_WPIECE;  
            }
            /*condition to place the black pieces*/
            else if(((row+col+1)%2)==0 && (BOARD_SIZE-row<=ROWS_WITH_PIECES)) {

                intial_board[row][col]=CELL_BPIECE;
            }
            /*empty cell placement*/
             else {

                intial_board[row][col]=CELL_EMPTY; 
            }
        }
    }
}

//counts pieces and tower in the board 
int
count_board_cells(board_t board, char type) {
    assert(board!=NULL);
    int row, col, count=0;                        // initiaize result to zero
    for (row=0; row<BOARD_SIZE; row++) {        // iterate over rows
        for (col=0; col<BOARD_SIZE; col++) {    // iterate over columns
            count += (board[row][col]==type);      // count cells of interest
        }
    }
    return count;
}


//prints the board 
void 
print_board(board_t board) {
    assert(board!=NULL);
    printf(STR_DOUBLE_SPACE);
    for (char col=MIN_COL; col<=MAX_COL; col++) {
        printf(STR_BOARD_COL_TITLE,col);
    }
    printf(STR_BOARD_LINE_BREAK);
    for(int row=0; row<BOARD_SIZE;row++) {
        printf(STR_BOARD_ROW_TITLE,row+1);
         for(int col=0; col<BOARD_SIZE; col++) {
            
            printf(STR_BOARD_CELL_CONTENT, board[row][col]);

        }
        printf(STR_BOARD_LINE_BREAK);
    }
}

// print winner 
void
print_winner(int turn) {
    if (turn%2) {
        printf(STR_BLACK_WIN);      // black win!
    } else {
        printf(STR_WHITE_WIN);      // white win!
    }
}

int 
is_player_turn(char player,int turn) {
    if ((turn%2 && player=='W') || (!(turn%2) && player=='B')) {
        return TRUE;      
    } else {
        return FALSE;      
    }
}


// free memeory allocated for min max tree 
void
free_tree(node_t* node) {
    assert(node!=NULL);
    for(int i=0; i<node->num; i++) {
        if(node->chrn[i]!=NULL){
            free_tree(node->chrn[i]);
        }
    }
    free(node->chrn);   // free children
    free(node);         // free input node 
}
