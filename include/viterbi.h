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
#define pi M_PI
using std::vector;
using std::map;
void statetable(int constraint_c1, int constraint_c2, int memory_size);

vector<int> encoder(int n1, const int n2, const int memory_size, const vector<int>& message);

vector<vector<double>> modulation(const vector<int>& codeword);

void demodulation(vector<vector<double>> rx_symbol, vector<int> re_codeword);

vector<vector<double>> channel(vector<vector<double>> tx_symbol, double seg);

vector<int> decoder(vector<int> re_codeword);


#endif //VITERBI_DECODING_VITERBI_H
