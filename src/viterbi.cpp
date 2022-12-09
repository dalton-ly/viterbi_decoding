//
// Created by Dale on 2022/12/7.
//
#include"../include/viterbi.h"
#include "../include/state_change.h"

std::string int_to_binaryString(int num, int width) {
    std::string result;
    for (int i = 0; i < width; ++i) {
        if (num & (1 << (width - i - 1)))
            result += "1";
        else
            result += "0";
    }
    return result;
}

map<int, state_change> statetable(int constraint_c1, int constraint_c2, int memory_size) {

    map<int, state_change> state_table;

    for (int ID = 0; ID < (1 << (memory_size + 1)); ID += 2) {
        std::string current_state = int_to_binaryString(ID, memory_size);
        state_table.insert(std::pair<int, state_change>(ID, state_change(0, current_state, constraint_c1, constraint_c2,
                                                                         memory_size)));
        state_table.insert(std::pair<int, state_change>(ID + 1,
                                                        state_change(1, current_state, constraint_c1, constraint_c2,
                                                                     memory_size)));
    }
    return state_table;

}


///
/// \param n1: constraint of c1
/// \param n2: constraint of  c2
/// \param memory_size memory depth
/// \param message message word
//todo:注意将8进制转为10进制
vector<int> encoder(const int n1, const int n2, const int memory_size,
                    const vector<int> &message) {
    //convolution encoder, the input is message[] and the output is codeword[]
    vector<int> encoded_message;
    vector<int> reg(memory_size + 1, 0); //包括输入的移位寄存器
    int size = reg.size();
    for (int i: message) {
        //移位
        for (int k = size - 1; k > 0; --k) {
            reg[k] = reg[k - 1];
        }
        reg[0] = i;
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

vector<vector<double>> modulation(const vector<int> &codeword) {
    //BPSK modulation
    int i;
    vector<vector<double>> modulated_codeword(codeword.size(), vector<double>(2, 0.0));
    //0 is mapped to (1,0) and 1 is mapped tp (-1,0)
    for (i = 0; i < codeword.size(); i++) {
        modulated_codeword[i][0] = -1 * (2 * codeword[i] - 1);
        modulated_codeword[i][1] = 0;
    }
    return modulated_codeword;
}

vector<vector<double>> channel(vector<vector<double>> tx_symbol, double sgm) {
    //AWGN channel
    int i, j;
    double u, r, g;
    vector<vector<double>> rx_symbol(tx_symbol.size(), vector<double>(2, 0.0));
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
    return rx_symbol;
}

vector<int> demodulation(vector<vector<double>> rx_symbol) {
    int i;
    double d1, d2;
    vector<int> re_codeword(rx_symbol.size());
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

//memory size: constraint lenth-1
vector<int> decoder(vector<int> re_codeword,
                    int memory_size,
                    int constraint_c1,
                    int constraint_c2) {
    Trellis trellis;
    vector<int> path_metrics(1 << memory_size, std::numeric_limits<int>::max());//2^m行
    auto transition_table = statetable(constraint_c1, constraint_c2, memory_size);
    path_metrics.front() = 0; //从全零状态开始，第一个为0其余无穷大
    for (int i = 0; i < re_codeword.size(); i += 2) { //限定了码率为1/2
        vector<int> current_bits;
        current_bits.assign(re_codeword.begin() + i, re_codeword.begin() + i + 2);//逐2位比较
        update_path_metrics(current_bits, &path_metrics, &trellis, transition_table);
    }

}

void update_path_metrics(const vector<int> &current_bits,
                         vector<int> *path_metrics,
                         Trellis *trellis,
                         const map<int, state_change> &transition_table) {

    std::vector<int> new_path_metrics(path_metrics->size()); //进行迭代计算
    std::vector<int> new_trellis_column(path_metrics->size());
    for (int i = 0; i < path_metrics->size(); ++i) {
        std::pair<int, int> p = Compute_path_metric(current_bits, *path_metrics,
                                                    i, transition_table);
        new_path_metrics[i] = p.first;
        new_trellis_column[i] = p.second;
    }
    *path_metrics = new_path_metrics;
    trellis->push_back(new_trellis_column);
}

/// 更新trellis以及path——metric
/// \param bits 当前接收到的bits
/// \param prev_path_metrics  上一时刻的path metrics
/// \param state  状态 00...0->11...1
/// \param transition_table 状态转移表
/// \return path_metric and previous states
std::pair<int, int> Compute_path_metric(const vector<int> &bits,
                                        const vector<int> &prev_path_metrics,
                                        int state,
                                        const map<int, state_change> &transition_table) {

    vector<int> prev_ID = find_previous_state_ID(int_to_binaryString(state, int(log2(prev_path_metrics.size()))),
                                                 transition_table);
    int source_state1 = std::stoi(transition_table.at(prev_ID[0]).current_state);
    int source_state2 = std::stoi(transition_table.at(prev_ID[1]).current_state);

    int pm1 = prev_path_metrics[source_state1];
    if (pm1 < std::numeric_limits<int>::max()) {
        pm1 += BranchMetric(bits, prev_ID[0], transition_table);
    }
    int pm2 = prev_path_metrics[source_state2];
    if (pm2 < std::numeric_limits<int>::max()) {
        pm2 += BranchMetric(bits, prev_ID[1], transition_table);
    }

    if (pm1 <= pm2) {
        return std::make_pair(pm1, source_state1);
    } else {
        return std::make_pair(pm2, source_state2);
    }

}

/// 根据当前状态查找上一时刻的状态
/// \param next_state 当前状态
/// \param transition 状态表
/// \return 状态来源的ID
vector<int> find_previous_state_ID(const std::string &next_state, const map<int, state_change> &transition) {
    vector<int> previous_state;
    for (int i = 0; i < transition.size(); ++i) {
        if (transition.at(i).next_state == next_state) {
            previous_state.push_back(i);
        }
    }
    return previous_state;
}


/// 比较收到的bits与转移输出的bits之间的汉明距
/// \param received_bits
/// \param source_state
/// \param target_state
/// \return 汉明距离
int BranchMetric(const vector<int> &received_bits, int trans_ID, const map<int, state_change> &trans_table) {
    int distance = 0;
    vector<int> output = trans_table.at(trans_ID).output;
    for (int i = 0; i < received_bits.size(); ++i) {
        if (received_bits[i] != output[i])
            distance += 1;
    }

    return distance;
}
