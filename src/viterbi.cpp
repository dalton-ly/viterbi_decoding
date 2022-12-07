//
// Created by Dale on 2022/12/7.
//
#include"../include/viterbi.h"

void statetable() {

}

///
/// \param n1: constraint of c1
/// \param n2: constraint of  c2
/// \param memory_size memory depth
/// \param message message word
//todo:注意将8进制转为10进制
std::vector<int> encoder(const int n1, const int n2, const int memory_size,
                         const std::vector<int> &message) {
    //convolution encoder, the input is message[] and the output is codeword[]
    std::vector<int> encoded_message;
    std::vector<int> reg(memory_size + 1, 0); //包括输入的移位寄存器
    int size = reg.size();
    for (int i=0;i<message.size();++i) {
        //移位
        for (int k = size-1; k >0; --k) {
            reg[k]=reg[k-1];
        }
        reg[0]=message[i];
        
        int temp_c1 = 0;
        int temp_c2 = 0;
        for (int j = 0; j < size; ++j) {
            if (n1 & (1 << j)) {
                temp_c1 = reg[size - 1 - j] ^ temp_c1;
            }
            if (n2 & (1 << j)) {
                temp_c2 = reg[size - 1 - j] ^ temp_c2;
            }
        }
        encoded_message.push_back(temp_c1);
        encoded_message.push_back(temp_c2);
    }
    return encoded_message;
}

std::vector<std::vector<double>> modulation(const std::vector<int>& codeword) {
    //BPSK modulation
    int i;
    std::vector<std::vector<double>> modulated_codeword(codeword.size(),std::vector<double>(2,0.0));
    //0 is mapped to (1,0) and 1 is mapped tp (-1,0)
    for (i = 0; i < codeword.size(); i++) {
        modulated_codeword[i][0] = -1 * (2 * codeword[i] - 1);
        modulated_codeword[i][1] = 0;
    }
    return modulated_codeword;
}

std::vector<std::vector<double>> channel(std::vector<std::vector<double>> tx_symbol, double sgm) {
    //AWGN channel
    int i, j;
    double u, r, g;
    std::vector<std::vector<double>> rx_symbol(tx_symbol.size(),std::vector<double>(2,0.0));
    for (i = 0; i < tx_symbol.size(); i++) {
        for (j = 0; j < 2; j++) {
            u = (float) rand() / (float) RAND_MAX;
            if (u == 1.0)
                u = 0.999999;
            r = sgm * sqrt(2.0 * log(1.0 / (1.0 - u)));//2log(1/1-rand)^0.5

            u = (float) rand() / (float) RAND_MAX;
            if (u == 1.0)
                u = 0.999999;
            g = (float) r * cos(2 * pi * u);

            rx_symbol[i][j] = tx_symbol[i][j] + g;
        }
    }
    return  rx_symbol;
}

std::vector<int> demodulation(std::vector<std::vector<double>> rx_symbol) {
    int i;
    double d1, d2;
    std::vector<int> re_codeword(rx_symbol.size());
    for (i = 0; i < rx_symbol.size(); i++) {
        d1 = (rx_symbol[i][0] - 1) * (rx_symbol[i][0] - 1) + rx_symbol[i][1] * rx_symbol[i][1];
        d2 = (rx_symbol[i][0] + 1) * (rx_symbol[i][0] + 1) + rx_symbol[i][1] * rx_symbol[i][1];
        if (d1 < d2)
            re_codeword[i] = 0;
        else
            re_codeword[i] = 1;
    }
    return re_codeword;
}

void decoder() {

}