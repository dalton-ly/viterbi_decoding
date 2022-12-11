#include "include/viterbi.h"

using std::vector;
using std::cout;
using std::cin;
using std::endl;
int message_length; //the length of message
int codeword_length; //the length of codeword
int state_num;      //the number of the state of encoder structure memory size?
float code_rate = (float) message_length / (float) codeword_length;
// channel coefficient
double N0, sgm;
#define message_length 1000 //the length of message
//#define sta
//#define codeword_length 10 //the length of codeword
vector<int> message;
vector<int> codeword;
vector<int> re_codeword;
vector<int> de_message;

vector<vector<double>> tx_symbol;//(codeword_length,vector<double>(2));
vector<vector<double>> rx_symbol;


void test_statetable();

void test(){
    int i;
    float SNR, start, finish;
    long int bit_error, seq, seq_num;
    double BER;
    double progress;

    //generate state table
//    statetable(0, 0, 0);

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
//                message[i] = rand() % 2;
                message.push_back(rand()%2);
            }
//1
//            message={1,0,1,0,0};

            //convolutional encoder
            codeword=encoder(7, 5, 2, message);

            //BPSK modulation
            tx_symbol=modulation(codeword);

            //AWGN channel
            rx_symbol=channel(tx_symbol, 0.5);

            //BPSK demodulation, it's needed in hard-decision Viterbi decoder
            re_codeword=demodulation(rx_symbol);

            //convolutional decoder
            de_message=decoder(re_codeword,2, 7, 5);

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

//    cout<<int_to_binaryString(10);
//    int constraint_=3;
//    for (int state = 0; state <(1 << (constraint_ - 1)) ; ++state) {
//        int s = (state & ((1 << (constraint_ - 2)) - 1)) << 1;//todo: 这里含义是什么？
//        cout<<( s | 0)<<endl;
//        cout<<(s | 1)<<endl;
//    }
//    decoder(vector<int>{1, 2, 3, 4}, 2, 0, 0);
    //test_statetable();
        test();
    return 0;
}

void test_statetable() {
    auto table= statetable(7, 5, 2);
    for (int i=0;i<(8);++i)
    {
        cout<<"input:"<<table.at(i).input<<"    current:"<<table.at(i).current_state<<" next:"<<table.at(i).next_state<<endl;
    }
}
