#include "gunits.h"
#include "debug.h"
#include "stdio.h"
#include <stdlib.h>

void PrintMessage(unsigned char *message, unsigned long length){
    unsigned int i,j;
    printf(" -- Message Start -- \r\n");
    for( i = 0; i < length; i +=16){
        printf("DATA - ");
        for(j = 0; j < 16 && i + j < length; j++){
            printf("%02X ",message[i+j]);
        }
        printf(" [%d] \r\n", j);
    }
    printf(" -- Message End -- \r\n");
}
EncyptedMessage* newEncyptedMessage(unsigned long HeaderLength, unsigned long SeqLength, unsigned long DataLength, unsigned long ICVLength){
    EncyptedMessage* out = (EncyptedMessage*)malloc(sizeof(EncyptedMessage));
    out->Header = (unsigned char*)malloc(HeaderLength);
    out->HeaderLength = HeaderLength;
    out->Data = (unsigned char*)malloc(DataLength);
    out->DataLength = DataLength;
    out->ICV = (unsigned char*)malloc(ICVLength);
    out->ICVLength = ICVLength;
    out->Seq = (unsigned char*)malloc(SeqLength);
    out->SeqLength = SeqLength;
    return out;
}
Message* newMessage(unsigned long HeaderLength, unsigned long SeqLength, unsigned long DataLength){
    Message* out = (Message*)malloc(sizeof(Message));
    out->Header = (unsigned char*)malloc(HeaderLength);
    out->HeaderLength = HeaderLength;
    out->Data = (unsigned char*)malloc(DataLength);
    out->DataLength = DataLength;
    out->Seq = (unsigned char*)malloc(SeqLength);
    out->SeqLength = SeqLength;
    return out;
}