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
    unsigned int crc;
    unsigned char data[127];
};
struct existence_packet
{
    unsigned int id;
    unsigned int crc;
    unsigned int timestamp;
    unsigned char data[127 - (1 * sizeof(unsigned int))];
};
union board_state
{
    unsigned int combined;
    struct
    {
        unsigned short os;
        unsigned short xes;
    };
};
static_assert(sizeof(board_state) == sizeof(unsigned int));
struct snapshop_packet
{
    unsigned int id;
    unsigned int crc;
    unsigned int timestamp;
    board_state board_state;
    unsigned char data[127 - (2 * sizeof(unsigned int))];
};