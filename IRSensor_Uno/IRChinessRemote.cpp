#include "IRChinessRemote.h"

char *IRChinessRemote::FIND_KEY(unsigned long data)
{
    if (data == IR_REPEAT || (data < IR_MIN_VAL && data > IR_MAX_VAL))
    {
        return (char *) "";
    }
    else
    {
        switch (data)
        {
        case 0xF720DF:
            return (char *) "R";
            break;
        case 0xF7A05F:
            return (char *) "G";
            break;
        case 0xF7609F:
            return (char *) "B";
            break;
        case 0xF7E01F:
            return (char *) "W";
            break;
        case 0xF710EF:
            return (char *) "1";
            break;
        case 0xF7906F:
            return (char *) "2";
            break;
        case 0xF750AF:
            return (char *) "3";
            break;
        case 0xF730CF:
            return (char *) "4";
            break;
        case 0xF7B04F:
            return (char *) "5";
            break;
        case 0xF7708F:
            return (char *) "6";
            break;
        case 0xF708F7:
            return (char *) "7";
            break;
        case 0xF78877:
            return (char *) "8";
            break;
        case 0xF748B7:
            return (char *) "9";
            break;
        case 0xF7A857:
            return (char *)"0";
            break;
        case 0xF728D7:
            return (char *) "*";
            break;
        case 0xF76897:
            return (char *) "#";
            break;
        case 0xF7D02F:
            return (char *) "UP";
            break;
        case 0xF7F00F:
            return (char *) "DOWN";
            break;
        case 0xF7C837:
            return (char *) "RIGHT";
            break;
        case 0xF7E817:
            return (char *) "LEFT";
            break;
        case 0xF700FF:
            return (char *) "ROTATE_LEFT";
            break;
        case 0xF7807F:
            return (char *) "ROTATE_RIGHT";
            break;
        case 0xF740BF:
            return (char *) "STOP";
            break;
        case 0xF7C03F:
            return (char *) "PLAY";
            break;
        }
    }
}
