//
// Created by Dale on 2022/12/8.
//

#ifndef VITERBI_DECODING_STATE_CHANGE_H
#define VITERBI_DECODING_STATE_CHANGE_H

#include <string>
#include "vector"

class state_change {
public:
    state_change(int in, const std::string &current, int constraint_c1, int constraint_c2, int memory_size);

    int input;
    std::string current_state;
    std::string next_state;
    std::vector<int> output;
};


#endif //VITERBI_DECODING_STATE_CHANGE_H
