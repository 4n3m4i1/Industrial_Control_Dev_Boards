#include <pico/stdlib.h>
#include "Curly_Mini_Pins.h"


void main(){
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);

    bool ledval = false;

    while(1){
        gpio_put(BOARD_LED, ledval);
        ledval = !ledval;
        sleep_ms(750);
    }

}