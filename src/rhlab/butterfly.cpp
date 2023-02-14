#include <iostream>
#include <stdio.h>
#include <string.h>
#include "butterfly.h"

using namespace std;

void ButterflySimulation::initialize(){
    this->targetDevice->initializeSimulation(18, 2);
    // this->targetDevice.setGpio(0, true); //first output gpio
    // this->targetDevice.getGpio(0);  // first input gpio, which is different

    // Initialize the structs for that simulates gpio
    gpio_input my_inputs = {
        false,  // g31
        false   // g32
    };
    gpio_output my_outputs = {
        false,  // g06
        false,  // g07
        false,  // g08
        false,  // g09
        false,  // g10
        false,  // g13
        false,  // g14
        false,  // g15
        false,  // g16
        false,  // g17
        false,  // g18
        false,  // g19
        false,  // g20
        false,  // g21
        false,  // g22
        false,  // g23
        false,  // g24
        false  // g25
    };
    gpio_header my_header = {
        my_inputs,
        my_outputs
    };

    this->my_header = my_header;

    // My buffer
    for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
        this->buffer[i] = false;
    }

    for(int i = 0; i < LED_ARRAY_SIZE; i++){
        mState.virtual_led[i] = false;
    }

    setReportWhenMarked(true);
}

void ButterflySimulation::print_gpio_header_states(){
    this->log() << "----- Input -----" << endl;
    this->log() << "g31: " << this->my_header.input.g31 << endl;
    this->log() << "g32: " << this->my_header.input.g32 << endl;

    this->log() << "----- Output -----" << endl;
    this->log() << "g06: " << this->my_header.output.g06 << endl;
    this->log() << "g07: " << this->my_header.output.g07 << endl;
    this->log() << "g08: " << this->my_header.output.g08 << endl;
    this->log() << "g09: " << this->my_header.output.g09 << endl;
    this->log() << "g10: " << this->my_header.output.g10 << endl;
    this->log() << "g13: " << this->my_header.output.g13 << endl;
    this->log() << "g14: " << this->my_header.output.g14 << endl;
    this->log() << "g15: " << this->my_header.output.g15 << endl;
    this->log() << "g16: " << this->my_header.output.g16 << endl;
    this->log() << "g17: " << this->my_header.output.g17 << endl;
    this->log() << "g18: " << this->my_header.output.g18 << endl;
    this->log() << "g19: " << this->my_header.output.g19 << endl;
    this->log() << "g20: " << this->my_header.output.g20 << endl;
    this->log() << "g21: " << this->my_header.output.g21 << endl;
    this->log() << "g22: " << this->my_header.output.g22 << endl;
    this->log() << "g23: " << this->my_header.output.g23 << endl;
    this->log() << "g24: " << this->my_header.output.g24 << endl;
    this->log() << "g25: " << this->my_header.output.g25 << endl;

}

void ButterflySimulation::print_buffer_states(){
    this->log() << "buffer: ";
    for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
        this->log() << this->buffer[i] << " ";
    }
    this->log() << endl;
}

void ButterflySimulation::print_led_states(){
    this->log() << "virtual led: ";
    for(int i = 0; i < LED_ARRAY_SIZE; i++){
        this->log() << mState.virtual_led[i] << " ";
    }
    this->log() << endl;
}

int ButterflySimulation::read_literal_logic(string s){
    return s[0] == 'T';
}

int ButterflySimulation::read_switch_logic(string s){
    return s[0] == 'T';
}

bool ButterflySimulation::read_gpio_logic(string s){
    char char_array[IS_GPIO_NEXT_CHAR_SIZE + 1];
    strcpy(char_array, s.c_str());
    switch(atoi(char_array)){
        // inputs
        case 31:
            return this->my_header.input.g31;
        case 32:
            return this->my_header.input.g32;
        // outputs
        case 06:
            return this->my_header.output.g06;
        case 07:
            return this->my_header.output.g07;
        case 8:
            return this->my_header.output.g08;
        case 9:
            return this->my_header.output.g09;
        case 10:
            return this->my_header.output.g10;
        case 13:
            return this->my_header.output.g13;
        case 14:
            return this->my_header.output.g14;
        case 15:
            return this->my_header.output.g15;
        case 16:
            return this->my_header.output.g16;
        case 17:
            return this->my_header.output.g17;
        case 18:
            return this->my_header.output.g18;
        case 19:
            return this->my_header.output.g19;
        case 20:
            return this->my_header.output.g20;
        case 21:
            return this->my_header.output.g21;
        case 22:
            return this->my_header.output.g22;
        case 23:
            return this->my_header.output.g23;
        case 24:
            return this->my_header.output.g24;
        case 25:
            return this->my_header.output.g25;
        default:
            return false;
    }
}

void ButterflySimulation::update_gpio_logic(string s, int o){
    char char_array[IS_GPIO_NEXT_CHAR_SIZE + 1];
    strcpy(char_array, s.c_str());
    switch(atoi(char_array)){
        case 31:
            this->my_header.input.g31 = (bool)o;
            break;
        case 32:
            this->my_header.input.g32 = (bool)o;
            break;
        case 06:
            this->my_header.output.g06 = (bool)o;
            break;
        case 07:
            this->my_header.output.g07 = (bool)o;
            break;
        case 8:
            this->my_header.output.g08 = (bool)o;
            break;
        case 9:
            this->my_header.output.g09 = (bool)o;
            break;
        case 10:
            this->my_header.output.g10 = (bool)o;
            break;
        case 13:
            this->my_header.output.g13 = (bool)o;
            break;
        case 14:
            this->my_header.output.g14 = (bool)o;
            break;
        case 15:
            this->my_header.output.g15 = (bool)o;
            break;
        case 16:
            this->my_header.output.g16 = (bool)o;
            break;
        case 17:
            this->my_header.output.g17 = (bool)o;
            break;
        case 18:
            this->my_header.output.g18 = (bool)o;
            break;
        case 19:
            this->my_header.output.g19 = (bool)o;
            break;
        case 20:
            this->my_header.output.g20 = (bool)o;
            break;
        case 21:
            this->my_header.output.g21 = (bool)o;
            break;
        case 22:
            this->my_header.output.g22 = (bool)o;
            break;
        case 23:
            this->my_header.output.g23 = (bool)o;
            break;
        case 24:
            this->my_header.output.g24 = (bool)o;
            break;
        case 25:
            this->my_header.output.g25 = (bool)o;
            break;
        default:
            break;
    }
}

int ButterflySimulation::read_buffer_logic(string s){
    int index = stoi(s);
    return this->buffer[index];
}

void ButterflySimulation::update_buffer_logic(string s, int o){
    int index = stoi(s);
    this->buffer[index] = (bool)o;
}

void ButterflySimulation::update_led_logic(string s, int o){
    int index = stoi(s);
    mState.virtual_led[index] = (bool)o;
}

int ButterflySimulation::read_gate_input(char c){
    int in;
    switch(c){
        case 'L':
            in = IS_LITERAL;
            break;
        case 'g':
            in = IS_GPIO;
            break;
        case 'b':
            in = IS_BUFFER;
            break;
        case 'S':
            in = IS_SWITCH;
            break;
        default:
            break;
    }
    return in;
}

int ButterflySimulation::read_gate_output(char c){
    int out;
    switch(c){
        case 'g':
            out = IS_GPIO;
            break;
        case 'b':
            out = IS_BUFFER;
            break;
        case 'd':
            out = IS_LED;
            break;
        default:
            break;
    }
    return out;
}

int ButterflySimulation::handle_input(string substring, int &start_index){
    int p;
    p = read_gate_input((char)substring[start_index++]);
    int my_input = 0;
    int temp = start_index;
    switch(p){
        case IS_LITERAL:
            my_input = read_literal_logic(substring.substr(temp, IS_LITERAL_NEXT_CHAR_SIZE));
            start_index += IS_LITERAL_NEXT_CHAR_SIZE;
            break;
        case IS_SWITCH:
            my_input = read_switch_logic(substring.substr(temp, IS_SWITCH_NEXT_CHAR_SIZE));
            start_index += IS_SWITCH_NEXT_CHAR_SIZE;
            break;
        case IS_GPIO:
            my_input = read_gpio_logic(substring.substr(temp, IS_GPIO_NEXT_CHAR_SIZE));
            start_index += IS_GPIO_NEXT_CHAR_SIZE;
            break;
        case IS_BUFFER:
            my_input = read_buffer_logic(substring.substr(temp, IS_BUFFER_NEXT_CHAR_SIZE));
            start_index += IS_BUFFER_NEXT_CHAR_SIZE;
            break;
        default:
            break;
    }
    return my_input;
}

void ButterflySimulation::handle_output(string substring, int &start_index, int my_output){
    int p;
    p = read_gate_output((char)substring[start_index++]);
    int temp = start_index;
    switch(p){
        case IS_GPIO:
            update_gpio_logic(substring.substr(temp, IS_GPIO_NEXT_CHAR_SIZE), my_output);
            start_index += IS_GPIO_NEXT_CHAR_SIZE;
            break;
        case IS_BUFFER:
            update_buffer_logic(substring.substr(temp, IS_BUFFER_NEXT_CHAR_SIZE), my_output);
            start_index += IS_BUFFER_NEXT_CHAR_SIZE;
            break;
        case IS_LED:
            update_led_logic(substring.substr(temp, IS_LED_NEXT_CHAR_SIZE), my_output);
            start_index += IS_LED_NEXT_CHAR_SIZE;
            break;
        default:
            break;
    }
}

int ButterflySimulation::read_logic_gate(string substring){
    int start_index = 0;
    int my_input_1 = -1;
    int my_input_2 = -1;
    int my_output = -1;
    if(substring[start_index] == 'n'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = !my_input_1;

        // Handle output portion
        handle_output(substring, start_index, my_output);
        while(substring[start_index] == ','){
            start_index++;
            handle_output(substring, start_index, my_output);
        }
    }
    else if(substring[start_index] == 'a'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);
        my_input_2 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = my_input_1 & my_input_2;

        // Handle output portion
        handle_output(substring, start_index, my_output);
        while(substring[start_index] == ','){
            start_index++;
            handle_output(substring, start_index, my_output);
        }
    }
    else if(substring[start_index] == 'o'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);
        my_input_2 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = my_input_1 | my_input_2;

        // Handle output portion
        handle_output(substring, start_index, my_output);
        while(substring[start_index] == ','){
            start_index++;
            handle_output(substring, start_index, my_output);
        }
    }
    else if(substring[start_index] == 'y'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = my_input_1;

        // Handle output portion
        handle_output(substring, start_index, my_output);
    }

    return start_index;
}

bool ButterflySimulation::check_if_same(gpio_header h2, bool buffer_copy[]){
    // check if buffer states are the same:
    for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
        if(buffer_copy[i] != this->buffer[i]){
            return false;
        }
    }

    // check if h1 and h2 are the same
    if(this->my_header.input.g31 != h2.input.g31)    return false;
    if(this->my_header.input.g32 != h2.input.g32)    return false;
    if(this->my_header.output.g06 != h2.output.g06)  return false;
    if(this->my_header.output.g07 != h2.output.g07)  return false;
    if(this->my_header.output.g08 != h2.output.g08)  return false;
    if(this->my_header.output.g09 != h2.output.g09)  return false;
    if(this->my_header.output.g10 != h2.output.g10)  return false;
    if(this->my_header.output.g13 != h2.output.g13)  return false;
    if(this->my_header.output.g14 != h2.output.g14)  return false;
    if(this->my_header.output.g15 != h2.output.g15)  return false;
    if(this->my_header.output.g16 != h2.output.g16)  return false;
    if(this->my_header.output.g17 != h2.output.g17)  return false;
    if(this->my_header.output.g18 != h2.output.g18)  return false;
    if(this->my_header.output.g19 != h2.output.g19)  return false;
    if(this->my_header.output.g20 != h2.output.g20)  return false;
    if(this->my_header.output.g21 != h2.output.g21)  return false;
    if(this->my_header.output.g22 != h2.output.g22)  return false;
    if(this->my_header.output.g23 != h2.output.g23)  return false;
    if(this->my_header.output.g24 != h2.output.g24)  return false;
    if(this->my_header.output.g25 != h2.output.g25)  return false;

    return true;
}

void ButterflySimulation::update(double delta){

    ButterflyRequest request;
    bool requestWasRead = readRequest(request);
    if(!requestWasRead) {
        return;
    }
    string my_string = string(request.my_string);

    int index = 0;
    int my_string_length = my_string.length();
    int while_loop_counter = 1;
    int saturation_itr_counter = 0;

    // Create copies of the header states and buffer states
    gpio_header my_header_copy = this->my_header;
    bool buffer_copy[BUFFER_ARRAY_SIZE];
    for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
        buffer_copy[i] = this->buffer[i];
    }

    while((saturation_itr_counter < NUM_SATURATION_ITR)){

        my_header_copy = this->my_header;
        for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
            buffer_copy[i] = this->buffer[i];
        }

        this->log() << endl << "===== GPIO STATES =====" << endl;
        print_gpio_header_states();
        this->log() << endl << "===== BUFFER STATES =====" << endl;
        print_buffer_states();
        this->log() << endl << "===== LED STATES =====" << endl;
        print_led_states();
        this->log() << mState.serialize() << endl;

        this->log() << "===== STRING PROTOCOL =====" << endl;
        while(my_string[index] != '\n' || index > my_string_length){
            this->log() << "Iter " << while_loop_counter << " : " << my_string.substr(index, my_string_length - index);
            while_loop_counter++;
            index += read_logic_gate(my_string.substr(index, my_string_length - index));
            if(my_string[index] == ';'){
                index = index + 1;
            }
        }

        index = 0;
        while_loop_counter = 1;

        if(check_if_same(my_header_copy, buffer_copy)){
            saturation_itr_counter++;
        }
    }

    this->log() << endl << "===== GPIO STATES =====" << endl;
    print_gpio_header_states();
    this->log() << endl << "===== BUFFER STATES =====" << endl;
    print_buffer_states();
    this->log() << endl << "===== LED STATES =====" << endl;
    print_led_states();
    this->log() << mState.serialize() << endl;

    this->log() << "gpio[0] is " << this->targetDevice->getGpio(0) << endl;
    this->targetDevice->setGpio(0, this->targetDevice->getGpio(0));

    requestReportState();
}
