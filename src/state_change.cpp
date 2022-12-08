//
// Created by Dale on 2022/12/8.
//

#include "../include/state_change.h"
#include"../include/viterbi.h"

state_change::state_change(int in, std::string current, int constraint_c1, int constraint_c2, int memory_size) {
    current_state = current;
    next_state = (std::to_string(in) + current).substr(0, memory_size); //状态转移
    input=in;
    output=encoder(constraint_c1,constraint_c2,memory_size,vector<int>(1,in));
}