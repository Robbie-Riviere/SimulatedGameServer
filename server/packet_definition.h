enum packet_meaning
{
    // This is the "Do you have a server?" "Yes I have a server" pair
    YOU_EXIST = 1,
    I_EXIST,
    //
};
/**
 * This is the decoded data from the packet, the id is used to switch to various similar
 */
struct datagram
{
    unsigned int id;
    unsigned char data[127];
};
struct existence_packet
{
    unsigned int id;
    unsigned int timestamp;
    unsigned char data[127 - sizeof(unsigned int)];
};