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

vector<int> message;
vector<int> codeword;
vector<int> re_codeword;
vector<int> de_message;

vector<vector<double>> tx_symbol;//(codeword_length,vector<double>(2));
vector<vector<double>> rx_symbol;


int main() {
    int i;
    float SNR, start, finish;
    long int bit_error, seq, seq_num;
    double BER;
    double progress;

    //generate state table
    statetable();

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
            for (i = message_length - state_num; i < message_length; i++) {
//                message[i] = 0;
                message.push_back(rand()%2);
            }

            //convolutional encoder
            codeword=encoder(0, 0, 0, message);

            //BPSK modulation
            tx_symbol=modulation(message);

            //AWGN channel
            channel(std::vector<std::vector<double>>(), 0);

            //BPSK demodulation, it's needed in hard-decision Viterbi decoder
            //demodulation();

            //convolutional decoder
            decoder();

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

    return 0;
}