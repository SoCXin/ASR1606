#include "display.h"
#include "drv_uart.h"

void screen_display(TRANSCEIVER* t)
{
    TRANSCEIVER_IMPLEMENTS* this = (TRANSCEIVER_IMPLEMENTS*)t;

    Display_B("+-----------+\r\n");
    Display_B("|           |\r\n");
    Display_B("| ");

    switch(this->get_status(this))
    {
    case TRANSCEIVER_INCOMING:
        Display_Y("incoming.");
        break;
    case TRANSMITTER_DIAL:
        Display_Y("dial...  ");
        break;
    case TRANSCEIVER_BUSY_LINE:
        Display_Y("in a call");
        break;
    case TRANSMITTER_IDLE:
        Display_Y("idle     ");
        break;
    default:
        break;
    }
    
    Display_B(" |\r\n");
    Display_B("|           |\r\n");
    Display_B("|-----------|\r\n");
    Display_B("|           |\r\n");
    Display_B("| ");

    switch(this->get_status(this))
    {
    case TRANSCEIVER_INCOMING:
        Display_G("[G]");
        break;
    case TRANSMITTER_DIAL:
        Display_N("[G]");
        break;
    case TRANSCEIVER_BUSY_LINE:
        Display_N("[G]");
        break;
    case TRANSMITTER_IDLE:
        Display_G("[G]");
        break;
    default:
        break;
    }
    
    Display_B("   ");

    switch(this->get_status(this))
    {
    case TRANSCEIVER_INCOMING:
        Display_R("[R]");
        break;
    case TRANSMITTER_DIAL:
        Display_R("[R]");
        break;
    case TRANSCEIVER_BUSY_LINE:
        Display_R("[R]");
        break;
    case TRANSMITTER_IDLE:
        Display_N("[R]");
        break;
    default:
        break;
    }

    
    Display_B(" |\r\n");
    Display_B("|           |\r\n");
    Display_B("+-----------+\r\n");
}


// +-----------+
// |           |
// | incoming. |
// |           |
// |-----------|
// |           |
// | [G]   [R] |
// |           |
// +-----------+