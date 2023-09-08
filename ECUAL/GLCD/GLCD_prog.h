//
// Created by kareem on 9/5/23.
//
#include "../../LIB/ERR_STATE.h"
#ifndef CLIONAVRTEST_GLCD_PROG_H
#define CLIONAVRTEST_GLCD_PROG_H


//GLCD 128x64 KS0 108



ES_t GLCD_init();
ES_t GLCD_send_command(u8 u8_command);
ES_t GLCD_send_data(u8 u8_data);
ES_t GLCD_select_chip(u8 u8_chip);

//use column before page
ES_t go_to_column(u8 u8_column);
ES_t go_to_page(u8 u8_page);
ES_t GLCD_print_char(u8 u8_char);
ES_t GLCD_print_string(u8 *u8_string);
ES_t GLCD_clear_all();








#endif //CLIONAVRTEST_GLCD_PROG_H
