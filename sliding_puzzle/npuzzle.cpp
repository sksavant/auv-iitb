#include<iostream>
#include<cstring>
#include<fstream>
#include<vector>
#include<cassert>
#include<cmath>
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
            row.push_back(n*i+j+1);
        }
        idealboard.push_back(row);
    }
    idealboard[n-1][n-1]=0;
    print_board_state(idealboard);
    return idealboard;
}

int find_heuristic(vector<vector<int> > board,vector<vector<int> > idealboard){
    //heuristic: says how near the configuration is to the ideal
    int n=board.size();
    int heuristic_value=0; 
    /*
    //HEURESTIC 1 : //Say heuristic is no.of.blocks that are not in place
    //good enough(NO!) but not too good? To see peformance (NOT AT ALL GOOD)
    for (int i=0; i<board.size(); ++i){
        for (int j=0; j<board.size(); ++j){
            heuristic_value=(board[i][j]==idealboard[i][j])?(heuristic_value):(heuristic_value+1);
        }
    }*/
    
    //HEURESTIC 2 : Manhattan distance
    for (int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            if(board[i][j]!=0){
                heuristic_value=heuristic_value+abs(i-(board[i][j]-1)/n)+abs(j-(board[i][j]-1)%n);
            }
        }
    }
    cout<<heuristic_value<<endl;
    return heuristic_value;
}

vector<vector<int> > moved(vector<vector<int> > board,const int move[2]){
    vector<vector<int> > newboard;
    newboard=board;
    int n=board.size();
    for (int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            if(board[i][j]==0){
                if (i+move[0]<0 || i+move[0]>=n || j+move[1]<0 || j+move[1]>=n){ //If move is not valid rsize the newboard and return
                    return newboard;
                }else{
                    newboard[i][j]=newboard[i+move[0]][j+move[1]];
                    newboard[i+move[0]][j+move[1]]=0;
                    return newboard;
                }
            }
        }
    }
}

int main(){
    //Parameters declaration
    const int moves[4][2]={{0,-1},{1,0},{0,1},{-1,0}}; //up,right,down,left
	vector<vector<vector<int> > > board_states_to_explore;
    vector<vector<vector<int> > > board_states_explored;
    vector<vector<int> > board,idealboard,newboard;
    vector<vector<vector<int> > > all_inputs;
    vector<vector<int> > moves_state;
    vector<int> heuristic;
    bool notfoundthegoal,nosuchboard;
    int boardindex,cost;
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
        heuristic.resize(0);
        moves_state.resize(0);
        board=all_inputs[boardinputindex];
        board_states_to_explore.push_back(board); //put input configuration in the board_states
        boardindex=0;
        moves_state.push_back( {});
        idealboard=get_ideal_board(board.size());
        heuristic.push_back(find_heuristic(board,idealboard));
        print_board_state(board); //printing the intial configuration
        //After getting the board, 
        int i=0;
        while (notfoundthegoal){
            ++i;
            //Check if found the goal, if not found move!
            if (board==idealboard){
                notfoundthegoal=false;
                break;
            }
            for(int moveit=0; moveit<4; ++moveit){
                //go through possible moves for the 0 block
                //Get the new state if possible and append to board_states and 
                newboard.resize(0);
                newboard=moved(board,moves[moveit]);
                if (newboard!=board){ //=> has moved
                    nosuchboard=true; //checking if the board is already in states_to_explore or states_explored
                    for(int bin=0; bin<board_states_to_explore.size(); ++bin){
                        if(newboard==board_states_to_explore[bin]){
                            nosuchboard=false;
                            break;
                        }
                    }
                    if (nosuchboard){
                        for(int bin=0; bin<board_states_explored.size(); ++bin){
                            if(newboard==board_states_explored[bin]){
                                nosuchboard=false;
                                break;
                            }
                        }
                    }
                    if (nosuchboard){ //if it is not there, add it to the states_to_explore
                        print_board_state(newboard); // for debug?
                        board_states_to_explore.push_back(newboard);
                        heuristic.push_back(find_heuristic(newboard,idealboard));
                    }
                }
            }
            //TO remove board from board_states_to_explore
            board_states_to_explore.erase(board_states_to_explore.begin()+boardindex);
            heuristic.erase(heuristic.begin()+boardindex);
            board_states_explored.push_back(board);
            //go through the heurestic vector to find minimum and set it new board and boardindex
            cost=10000;
            boardindex=-1;
            for(int bin=0; bin<heuristic.size(); ++bin){
                if(cost>heuristic[bin]){
                    cost=heuristic[bin];
                    boardindex=bin;
                }
            }
            assert(boardindex!=-1);
            board=board_states_to_explore[boardindex];
            cout<<"NextBoard\n";
            print_board_state(board);

            //set board as the next one to explore : The one with minimum cost+heuristic 
            //sleep(5); To debug
        }
        //set board as the next one to
        if(not notfoundthegoal){
            cout<<"Yay! Found the goal, printing moves\n";
            print_board_state(board);
        }

    }
    return 0;
}
