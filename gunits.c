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
    EncyptedMessage* out = malloc(sizeof(EncyptedMessage));
    out->Header = malloc(HeaderLength);
    out->HeaderLength = HeaderLength;
    out->Data = malloc(DataLength*10);
    out->DataLength = DataLength;
    out->ICV = malloc(ICVLength);
    out->ICVLength = ICVLength;
    out->Seq = malloc(SeqLength);
    out->SeqLength = SeqLength;
    return out;
}
Message* newMessage(unsigned long HeaderLength, unsigned long SeqLength, unsigned long DataLength){
    Message* out = malloc(sizeof(Message));
    out->Header = malloc(HeaderLength);
    out->HeaderLength = HeaderLength;
    out->Data = malloc(DataLength*10);
    out->DataLength = DataLength;
    out->Seq = malloc(SeqLength);
    out->SeqLength = SeqLength;
    return out;
}
