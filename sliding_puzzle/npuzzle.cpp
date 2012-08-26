#include<iostream>
#include<cstring>
#include<fstream>
#include<vector>
using namespace std;

//global variables?

void print_board_state(vector<vector<int> > board){
    for (int i=0; i<board.size();++i){
        for(int j=0; j<board[i].size();++j){
            printf("%d ",board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//Input is as follows :
    // <no of. testcases>
    // <n> 
    // <data>
    // .
    // .
    // .
vector<vector<vector<int> > > get_input_states(){
    vector<vector<vector<int> > > states;
    vector<vector<int> > board;
    vector<int> row;
    int val;
    int no_of_inputs,n;
    //n is no. of sides (4 for 15-puzzle)
    fstream fin;
    fin.open("boardstates.in",fstream::in);
    fin>>no_of_inputs;
    states.resize(0);
    for (int i=0; i<no_of_inputs; ++i){
        //Get each state and add to vector<vector<vector<int> > >
        fin>>n;
        board.resize(0);
        for (int j=0; j<n; ++j){
            row.resize(0);
            for(int k=0; k<n; ++k){
                fin>>val;
                //cout<<val<<" "<<endl;
                row.push_back(val);
            }
            board.push_back(row);
        }
        states.push_back(board);
    }
    fin.close();
    return states;
}

vector<vector<int> > get_ideal_board(int n){
    vector<vector<int> > idealboard;
    vector<int> row;
    for (int i=0; i<n; ++i){
        row.resize(0);
        for(int j=0; j<n; ++j){
            row.push_back(n*i+j);
        }
        idealboard.push_back(row);
    }
    return idealboard;
}

bool isgoal(vector<vector<int> > board,vector<vector<int> > idealboard){
    for(int i=0; i<board.size(); ++i){
        for(int j=0; j<board.size(); ++j){
            if (board[i][j]!=idealboard[i][j]){
                return false;
            }
        }
    }
    return true;
}

int find_heurestic(vector<vector<int> > board,vector<vector<int> > idealboard){
    //Heurestic: says how near the configuration is to the ideal
    int heurestic_value=0; //Say heurestic is no.of.blocks that are not in place
    //good enough but not too good? To see peformance
    for (int i=0; i<board.size(); ++i){
        for (int j=0; j<board.size(); ++j){
            heurestic_value=(board[i][j]==idealboard[i][j])?(heurestic_value):(heurestic_value+1);
        }
    }
    cout<<heurestic_value<<endl;
    return heurestic_value;
}

int main(){
    //Parameters declaration
    const int moves[4][2]={{0,-1},{1,0},{0,1},{-1,0}}; //up,right,down,left
	vector<vector<vector<int> > > board_states_to_explore;
    vector<vector<vector<int> > > board_states_explored;
    vector<vector<int> > board,idealboard;
    vector<vector<vector<int> > > all_inputs;
    vector<vector<int> > moves_state;
    vector<int> heurestic;
    bool notfoundthegoal;
    //Has the state of the board
    /*board.resize(n);
	for (int i=0; i<n; i++){
		board[i].resize(n);
	}*/
    //Now initialise the board from an input
    all_inputs=get_input_states();
    for(int boardinputindex=0; boardinputindex<all_inputs.size(); ++boardinputindex){
        notfoundthegoal=true;
        board_states_to_explore.resize(0);
        board_states_explored.resize(0);
        heurestic.resize(0);
        moves_state.resize(0);
        board=all_inputs[boardinputindex];
        board_states_to_explore.push_back(board); //put input configuration in the board_states
        moves_state.push_back( {});
        idealboard=get_ideal_board(board.size());
        heurestic.push_back(find_heurestic(board,idealboard));
        print_board_state(board); //printing the intial configuration
        //After getting the board, 
        while (notfoundthegoal){
            //Check if found the goal, if not found move!
            if (isgoal(board,idealboard)){
                notfoundthegoal=false;
                break;
            }
            for(int moveit=0; moveit<4; ++moveit){
                //go through possible moves for the 0 block
                //Get the new state if possible and append to board_states and 
            }
        }
        if(not notfoundthegoal){
            cout<<"Yay! Found the goal, printing moves\n";
        }

    }
    return 0;
}
