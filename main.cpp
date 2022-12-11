#include "include/viterbi.h"

using std::vector;
using std::cout;
using std::cin;
using std::endl;
int message_length; //the length of message
int codeword_length; //the length of codeword
int state_num;      //the number of the state of encoder structure memory size?
// channel coefficient
double N0, sgm;

vector<int> message;
vector<int> codeword;
vector<int> re_codeword;
vector<int> de_message;

vector<vector<double>> tx_symbol;//(codeword_length,vector<double>(2));
vector<vector<double>> rx_symbol;


void test_statetable();

void test() {
    int i;
    float SNR, start, finish;
    long int bit_error, seq, seq_num;
    double BER;
    double progress;
    int memory_depth;
    int constraint_C1,constraint_C2;
    cout<<"please enter the message_length"<<endl;
    cin>>message_length;
    cout<<"please enter the memory depth"<<endl;
    cin>>memory_depth;
    cout<<"please enter the constraint number of c1 (base 10)"<<endl;
    cin>>constraint_C1;
    cout<<"please enter the constraint number of c2 (base 10)"<<endl;
    cin>>constraint_C2;

    float code_rate = 0.5;
    //random seed
    srand((int) time(nullptr));

    //input the SNR and frame number
    cout << "Enter start SNR" << endl;
    cin >> start;
    cout << "Enter finish SNR" << endl;
    cin >> finish;
    cout << "Please input the number of message:" << endl;
    cin >> seq_num;
    for (SNR = start; SNR <= finish; SNR++) {
        //channel noise
        N0 = (1.0 / code_rate) / pow(10.0, (float) (SNR) / 10.0);
        sgm = sqrt(N0 / 2);

        bit_error = 0;

        for (seq = 1; seq <= seq_num; seq++) {
            //generate binary message randomly
            /****************
            Pay attention that message is appended by 0 whose number is equal to the state of encoder structure.
            ****************/
            for (i = 0; i < message_length - state_num; i++) {
                message.push_back(rand() % 2);
            }


            //convolutional encoder
            codeword = encoder(constraint_C1, constraint_C2, memory_depth, message);

            //BPSK modulation
            tx_symbol = modulation(codeword);

            //AWGN channel
            rx_symbol = channel(tx_symbol, sgm);

            //BPSK demodulation, it's needed in hard-decision Viterbi decoder
            re_codeword = demodulation(rx_symbol);

            //convolutional decoder
            de_message = decoder(re_codeword, memory_depth, constraint_C1, constraint_C2);

            //calculate the number of bit error
            for (i = 0; i < message_length; i++) {
                if (message[i] != de_message[i])
                    bit_error++;
            }

            progress = (double) (seq * 100) / (double) seq_num;

            //calculate the BER
            BER = (double) bit_error / (double) (message_length * seq);

            //print the intermediate result
            printf("Progress=%2.1f, SNR=%2.1f, Bit Errors=%2.1d, BER=%E\r", progress, SNR, bit_error, BER);
        }

        //calculate the BER
        BER = (double) bit_error / (double) (message_length * seq_num);

        //print the final result
        printf("Progress=%2.1f, SNR=%2.1f, Bit Errors=%2.1d, BER=%E\n", progress, SNR, bit_error, BER);
    }
    system("pause");
}


int main() {
    test();
    return 0;
}


void test_statetable() {
    auto table = statetable(7, 5, 2);
    for (int i = 0; i < (8); ++i) {
        cout << "input:" << table.at(i).input << "    current:" << table.at(i).current_state << " next:"
             << table.at(i).next_state << endl;
    }
}
