
typedef struct Message{
    unsigned char *Header;
    unsigned long HeaderLength;
    unsigned char *Seq;
    unsigned long SeqLength;
    unsigned char *Data;
    unsigned long DataLength;
} Message;

typedef struct EncyptedMessage{
    unsigned char *Header;
    unsigned long HeaderLength;
    unsigned char *Seq;
    unsigned long SeqLength;
    unsigned char *Data;
    unsigned long DataLength;
    unsigned char *ICV;
    unsigned long ICVLength;
} EncyptedMessage;

EncyptedMessage* newEncyptedMessage(unsigned long HeaderLength, unsigned long SeqLength, unsigned long DataLength, unsigned long ICVLength);
Message* newMessage(unsigned long HeaderLength, unsigned long SeqLength, unsigned long DataLength);

void PrintMessage(unsigned char *message, unsigned long length);