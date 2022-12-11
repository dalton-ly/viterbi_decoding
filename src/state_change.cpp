//
// Created by Dale on 2022/12/8.
//

#include "../include/state_change.h"
#include"../include/viterbi.h"

state_change::state_change(int in, const std::string &current, int constraint_c1, int constraint_c2, int memory_size) {
    input = in;
    current_state = current;
    next_state = (std::to_string(in) + current).substr(0, memory_size); //状态转移
    memory_Size = memory_size;

    int temp_num = std::stoi(current, 0, 2);
    vector<int> temp;
    for (int i = 0; i < current.size(); ++i) {
        if (temp_num & (1 << i))
            temp.push_back(1);
        else
            temp.push_back(0);
    }
    std::reverse(temp.begin(), temp.end());
    output = encoder(constraint_c1, constraint_c2, memory_size, vector<int>(1, in), temp);
}
