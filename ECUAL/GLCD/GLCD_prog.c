//
// Created by kareem on 9/5/23.
//

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/ERR_STATE.h"
#include "../../MCAL/TIMER0/TIMER0_INTERFACE.h"
#include "../../MCAL/DIO/DIO_INTERFACE.h"
#include "GLCD_config.h"
#include "GLCD_priv.h"
#include "GLCD_prog.h"





ES_t GLCD_init(){

    ES_t Local_ErrorState = ES_OK;
    DIO_SetPinDirection(RS_PORT,RS_PIN,OUTPUT);
    DIO_SetPinDirection(RW_PORT,RW_PIN,OUTPUT);
    DIO_SetPinDirection(EN_PORT,EN_PIN,OUTPUT);

    DIO_SetPinDirection(D7_PORT,D7_PIN,OUTPUT);
    DIO_SetPinDirection(D6_PORT,D6_PIN,OUTPUT);
    DIO_SetPinDirection(D5_PORT,D5_PIN,OUTPUT);
    DIO_SetPinDirection(D4_PORT,D4_PIN,OUTPUT);
    DIO_SetPinDirection(D3_PORT,D3_PIN,OUTPUT);
    DIO_SetPinDirection(D2_PORT,D2_PIN,OUTPUT);
    DIO_SetPinDirection(D1_PORT,D1_PIN,OUTPUT);
    DIO_SetPinDirection(D0_PORT,D0_PIN,OUTPUT);

    DIO_SetPinDirection(CS1_PORT,CS1_PIN,OUTPUT);
    DIO_SetPinDirection(CS2_PORT,CS2_PIN,OUTPUT);
    DIO_SetPinDirection(RST_PORT,RST_PIN,OUTPUT);

    DIO_SetPinValue(CS1_PORT,CS1_PIN,HIGH);
    DIO_SetPinValue(CS2_PORT,CS2_PIN,HIGH);
    DIO_SetPinValue(RST_PORT,RST_PIN,HIGH);

   TIMER0_Init();
   TIMER0_BusyWaitms(20);

    GLCD_send_command(0x3E); //Display OFF
    GLCD_send_command(0x40); //set column to 0
    GLCD_send_command(0XB8); //set page to 0
    GLCD_send_command(0xC0); //set start line to 0
    GLCD_send_command(0x3F); //Display ON

    return Local_ErrorState;



  }


  ES_t  GLCD_send_command(u8 u8_command){
    ES_t Local_ErrorState = ES_OK;
        DIO_SetPinValue(D0_PORT,D0_PIN,(u8_command>>0)&1);
        DIO_SetPinValue(D1_PORT,D1_PIN,(u8_command>>1)&1);
        DIO_SetPinValue(D2_PORT,D2_PIN,(u8_command>>2)&1);
        DIO_SetPinValue(D3_PORT,D3_PIN,(u8_command>>3)&1);
        DIO_SetPinValue(D4_PORT,D4_PIN,(u8_command>>4)&1);
        DIO_SetPinValue(D5_PORT,D5_PIN,(u8_command>>5)&1);
        DIO_SetPinValue(D6_PORT,D6_PIN,(u8_command>>6)&1);
        DIO_SetPinValue(D7_PORT,D7_PIN,(u8_command>>7)&1);

        DIO_SetPinValue(RS_PORT,RS_PIN,LOW);
        DIO_SetPinValue(RW_PORT,RW_PIN,LOW);
        DIO_SetPinValue(EN_PORT,EN_PIN,HIGH);
        TIMER0_BusyWaitms(5);
        DIO_SetPinValue(EN_PORT,EN_PIN,LOW);
        TIMER0_BusyWaitms(5);

      return Local_ErrorState;
  }


ES_t GLCD_send_data(u8 u8_data){


    ES_t Local_ErrorState = ES_OK;
    if(u8_data > 255){
        Local_ErrorState = ES_OUT_OF_RANGE;
    }
    else {

        DIO_SetPinValue(D0_PORT, D0_PIN, (u8_data >> 0) & 1);
        DIO_SetPinValue(D1_PORT, D1_PIN, (u8_data >> 1) & 1);
        DIO_SetPinValue(D2_PORT, D2_PIN, (u8_data >> 2) & 1);
        DIO_SetPinValue(D3_PORT, D3_PIN, (u8_data >> 3) & 1);
        DIO_SetPinValue(D4_PORT, D4_PIN, (u8_data >> 4) & 1);
        DIO_SetPinValue(D5_PORT, D5_PIN, (u8_data >> 5) & 1);
        DIO_SetPinValue(D6_PORT, D6_PIN, (u8_data >> 6) & 1);
        DIO_SetPinValue(D7_PORT, D7_PIN, (u8_data >> 7) & 1);

        DIO_SetPinValue(RS_PORT, RS_PIN, HIGH);
        DIO_SetPinValue(RW_PORT, RW_PIN, LOW);
        DIO_SetPinValue(EN_PORT, EN_PIN, HIGH);

        TIMER0_BusyWaitms(5);
        DIO_SetPinValue(EN_PORT, EN_PIN, LOW);
        TIMER0_BusyWaitms(5);
    }
    return Local_ErrorState;

}

ES_t GLCD_select_chip(u8 u8_chip){
    ES_t Local_ErrorState = ES_OK;
    if(u8_chip == 0){
        DIO_SetPinValue(CS1_PORT,CS1_PIN,HIGH);
        DIO_SetPinValue(CS2_PORT,CS2_PIN,LOW);
    }
    else if(u8_chip == 1){
        DIO_SetPinValue(CS1_PORT,CS1_PIN,LOW);
        DIO_SetPinValue(CS2_PORT,CS2_PIN,HIGH);
    }
    else{
        Local_ErrorState = ES_OUT_OF_RANGE;
    }
    return Local_ErrorState;
}

ES_t go_to_column(u8 u8_column){
    ES_t Local_ErrorState = ES_OK;
    if(u8_column < 64 && u8_column >= 0){
        GLCD_select_chip(0);
        GLCD_send_command(0x40 | u8_column);
    }
    else if(u8_column < 128){
        GLCD_select_chip(1);
        GLCD_send_command(0x40 | (u8_column - 64));
    }
    else{
        Local_ErrorState = ES_OUT_OF_RANGE;
    }
    return Local_ErrorState;
}

ES_t go_to_page(u8 u8_page){
    ES_t Local_ErrorState = ES_OK;
    if(u8_page < 8 && u8_page >= 0){
        GLCD_send_command(0xB8 | u8_page);
    }
    else{
        Local_ErrorState = ES_OUT_OF_RANGE;
    }
    return Local_ErrorState;
}

ES_t GLCD_print_char(u8 u8_char){
   char *local_char = char_array[u8_char-32];

    for (int i = 0; i < 6; i++) {
        GLCD_send_data(*(local_char+i));
    }

}

ES_t GLCD_print_string(u8 *u8_string){
    ES_t Local_ErrorState = ES_OK;
    if(u8_string == NULL){
        Local_ErrorState = ES_NULL_POINTER;
    }else {


        u8 u8_index = 0;
        while (*(u8_string + u8_index) != '\0') {
            GLCD_print_char(*(u8_string + u8_index));
            u8_index++;
        }
    }
    return Local_ErrorState;
}

ES_t GLCD_draw_horizontal_line(u8 u8_column,u8 u8_page,u8 u8_length){
    ES_t Local_ErrorState = ES_OK;
    u8 u8_index;
    if(u8_column < 64 && u8_page < 8  && u8_length <= 64){
        go_to_column(u8_column);
        go_to_page(u8_page);
        for(u8_index = 0; u8_index < u8_length; u8_index++){
            GLCD_send_data(0x80);
        }
    } else if(u8_column < 128 && u8_page < 8 && u8_length <= 64){
        go_to_column(u8_column);
        go_to_page(u8_page);
        for(u8_index = 0; u8_index < u8_length; u8_index++){
            GLCD_send_data(0x80);
        }
    } else{
        Local_ErrorState = ES_OUT_OF_RANGE;
    }

    return Local_ErrorState;
}

ES_t GLCD_draw_vertical_line(u8 u8_column,u8 u8_page,u8 u8_length){
    ES_t Local_ErrorState = ES_OK;
    u8 u8_index;
    if(u8_column < 64 && u8_page < 8  && u8_length <= 8){
        go_to_column(u8_column);
        go_to_page(u8_page);
        for(u8_index = 0; u8_index < u8_length; u8_index++){
            go_to_column(u8_column);
            go_to_page(u8_page + u8_index);
            GLCD_send_data(0xff);
        }
    } else if(u8_column < 128 && u8_page < 8 && u8_length <= 8){
        go_to_column(u8_column);
        go_to_page(u8_page);
        for(u8_index = 0; u8_index < u8_length; u8_index++){
            go_to_column(u8_column);
            go_to_page(u8_page + u8_index);
            GLCD_send_data(0xff);
        }
    } else{
        Local_ErrorState = ES_OUT_OF_RANGE;
    }
    return Local_ErrorState;
}
ES_t GLCD_clear_all(){
    ES_t Local_ErrorState = ES_OK;
    u8 u8_page;
    u8 u8_column;

    DIO_SetPinValue(CS1_PORT,CS1_PIN,HIGH);
    DIO_SetPinValue(CS2_PORT,CS2_PIN,HIGH);

    for(u8_page = 0; u8_page < 8; u8_page++){

        go_to_page(u8_page);

        for(u8_column = 0; u8_column < 128; u8_column++){
            GLCD_send_data(0x00);
        }

    }
    go_to_page(0);
    go_to_column(0);

    return Local_ErrorState;
}
