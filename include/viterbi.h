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
//#define message_length 5 //the length of message
//#define codeword_length 10 //the length of codeword
#define pi M_PI

void statetable();

std::vector<int> encoder(int n1, const int n2, const int memory_size, const std::vector<int>& message);

std::vector<std::vector<double>> modulation(const std::vector<int>& codeword);

void demodulation(std::vector<std::vector<double>> rx_symbol, std::vector<int> re_codeword);

std::vector<std::vector<double>> channel(std::vector<std::vector<double>> tx_symbol, double seg);

void decoder();


#endif //VITERBI_DECODING_VITERBI_H
