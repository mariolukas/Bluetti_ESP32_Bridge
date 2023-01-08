#include "BluettiConfig.h"
#include "MQTT.h"
#include "PayloadParser.h"
#include "BWifi.h"


uint16_t parse_uint_field(uint8_t data[]){
    return ((uint16_t) data[0] << 8 ) | (uint16_t) data[1];
}

bool parse_bool_field(uint8_t data[]){
    return (data[1]) == 1;
}

float parse_decimal_field(uint8_t data[], uint8_t scale){
    uint16_t raw_value = ((uint16_t) data[0] << 8 ) | (uint16_t) data[1];  
    return (raw_value) / pow(10, scale);
}

float parse_version_field(uint8_t data[]){

   uint16_t low = ((uint16_t) data[0] << 8 ) | (uint16_t) data[1];    
   uint16_t high = ((uint16_t) data[2] << 8) | (uint16_t) data[3];   
   long val = (low ) | (high << 16) ;

   return (float) val/100;
}

uint64_t parse_serial_field(uint8_t data[]){

   uint16_t val1 = ((uint16_t) data[0] << 8 ) | (uint16_t) data[1];
   uint16_t val2 = ((uint16_t) data[2] << 8 ) | (uint16_t) data[3];
   uint16_t val3 = ((uint16_t) data[4] << 8 ) | (uint16_t) data[5];
   uint16_t val4 = ((uint16_t) data[6] << 8 ) | (uint16_t) data[7];

   uint64_t sn =  ((((uint64_t) val1) | ((uint64_t) val2 << 16)) | ((uint64_t) val3 << 32)) | ((uint64_t) val4 << 48);

   return  sn;
}

String parse_string_field(uint8_t data[]){
    return String((char*) data);
}

String pase_enum_field(uint8_t data[]){
    return "";
}

void pase_bluetooth_data(uint8_t page, uint8_t offset, uint8_t* pData, size_t length){
    char mqttMessage[200];
    switch(pData[1]){
      // range request

      case 0x03:

        for(int i=0; i< sizeof(bluetti_device_state)/sizeof(device_field_data_t); i++){

            // filter fields not in range
            if(bluetti_device_state[i].f_page == page && 
               bluetti_device_state[i].f_offset >= offset &&
               bluetti_device_state[i].f_offset <= (offset + length)/2 &&
               bluetti_device_state[i].f_offset + bluetti_device_state[i].f_size-1 >= offset &&
               bluetti_device_state[i].f_offset + bluetti_device_state[i].f_size-1 <= (offset + length)/2
              ){
    
                uint8_t data_start = (2* ((int)bluetti_device_state[i].f_offset - (int)offset)) + HEADER_SIZE;
                uint8_t data_end = (data_start + 2 * bluetti_device_state[i].f_size);
                uint8_t data_payload_field[data_end - data_start];
                
                int p_index = 0;
                for (int i=data_start; i<= data_end; i++){
                      data_payload_field[p_index] = pData[i-1];
                      p_index++;
                }

                switch (bluetti_device_state[i].f_type){
                 
                  case UINT_FIELD:
                    publishTopic(bluetti_device_state[i].f_name, String(parse_uint_field(data_payload_field)));
                    break;
    
                  case BOOL_FIELD:
                    publishTopic(bluetti_device_state[i].f_name, String((int)parse_bool_field(data_payload_field)));
                    break;
    
                  case DECIMAL_FIELD:
                    publishTopic(bluetti_device_state[i].f_name, String(parse_decimal_field(data_payload_field, bluetti_device_state[i].f_scale ), 2) );
                    break;
    
                  case SN_FIELD:  
                    char sn[16];
                    sprintf(sn, "%lld", parse_serial_field(data_payload_field));
                    publishTopic(bluetti_device_state[i].f_name, String(sn));
                    break;
    
                  case VERSION_FIELD:
                    publishTopic(bluetti_device_state[i].f_name, String(parse_version_field(data_payload_field),2) );    
                    break;

                  case STRING_FIELD:
                    publishTopic(bluetti_device_state[i].f_name, parse_string_field(data_payload_field));
                    break;

                  case ENUM_FIELD:
                    publishTopic(bluetti_device_state[i].f_name, pase_enum_field(data_payload_field));
                    break;
                  default:
                    break;
                  
                }
                
            }
            else{
              //AddtoMsgView(String(millis()) + ": skip filtered field: "+ String(bluetti_device_state[i].f_name));
            }
        }
        
        break; 
      case 0x06:
        AddtoMsgView(String(millis()) + ":skip 0x06 request! page: "+ String(page) + " offset: " + offset);
        break;
      default:
        AddtoMsgView(String(millis()) + ":skip unknow request! page: "+ String(page) + " offset: " + offset);
        break;
    }
    
}
