//
// Created by Dale on 2022/12/7.
//

#ifndef VITERBI_DECODING_VITERBI_H
#define VITERBI_DECODING_VITERBI_H

#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<vector>
#include<iostream>
#include<ios>
#include<map>
#include<limits>
#include <utility>
#include <algorithm>
#include "../include/state_change.h"

#define pi M_PI
using std::vector;
using std::map;
using Trellis = vector<vector<int>>;

map<int, state_change> statetable(int constraint_c1, int constraint_c2, int memory_size);

vector<int> encoder(int n1, const int n2, const int memory_size, const vector<int> &message);

vector<int>
encoder(int n1, const int n2, const int memory_size, const vector<int> &message, const vector<int> &current_state);

vector<vector<double>> modulation(const vector<int> &codeword);

vector<int> demodulation(vector<vector<double>> rx_symbol);

vector<vector<double>> channel(vector<vector<double>> tx_symbol, double seg);

vector<int> decoder(vector<int> re_codeword, int memory_size, int constranit_c1, int constraint_c2);

std::string int_to_binaryString(int num, int width);

void update_path_metrics(const vector<int> &current_bits, vector<int> *path_metrics, Trellis *trellis,
                         const map<int, state_change> &transition_table);

std::pair<int, int> Compute_path_metric(const vector<int> &bits, const vector<int> &prev_path_metrics, int state,
                                        const map<int, state_change> &transition_tabble);

vector<int> find_previous_state_ID(const std::string &next_state, const map<int, state_change> &transition);

int BranchMetric(const vector<int> &received_bits, int trans_ID, const map<int, state_change> &trans_table);

#endif //VITERBI_DECODING_VITERBI_H
