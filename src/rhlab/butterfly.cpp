#include <iostream>
#include <stdio.h>
#include <string.h>
#include "butterfly.h"

using namespace std;

void ButterflySimulation::initialize(){
    this->targetDevice->initializeSimulation(SIM_OUTPUT_GPIO_NUM, SIM_INPUT_GPIO_NUM);
    // this->targetDevice.setGpio(0, true); //first output gpio
    // this->targetDevice.getGpio(0);  // first input gpio, which is different

    for(int i = 0; i < SIM_OUTPUT_GPIO_NUM; i++){
        this->output_gpio_tracker[i] = false;
    }

    for(int i = 0; i < SIM_INPUT_GPIO_NUM; i++){
        this->input_gpio_tracker[i] = this->targetDevice->getGpio(i);
    }


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
    for(int i = 0; i < SIM_INPUT_GPIO_NUM; i++){
        this->log() << "g" << i << ": " << this->input_gpio_tracker[i] << endl;
    }

    this->log() << "----- Output -----" << endl;
    for(int i = 0; i < SIM_OUTPUT_GPIO_NUM; i++){
        this->log() << "g" << i << ": " << this->output_gpio_tracker[i] << endl;
    }

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
    int index = atoi(char_array);
    this->input_gpio_tracker[index] = this->targetDevice->getGpio(index);
    return this->targetDevice->getGpio(index);
}

void ButterflySimulation::update_gpio_logic(string s, int o){
    
    char char_array[IS_GPIO_NEXT_CHAR_SIZE + 1];
    strcpy(char_array, s.c_str());
    int index = atoi(char_array);
    this->output_gpio_tracker[index] = (bool)o;
    this->targetDevice->setGpio(index, (bool)o);

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
    // requestReportState();
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
    else if(substring[start_index] == 'x'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);
        my_input_2 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = my_input_1 ^ my_input_2;

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
        while(substring[start_index] == ','){
            start_index++;
            handle_output(substring, start_index, my_output);
        }
    }

    return start_index;
}

bool ButterflySimulation::check_if_same(bool input_gpio_copy[], bool output_gpio_copy[], bool buffer_copy[]){
    
    for(int i = 0; i < SIM_INPUT_GPIO_NUM; i++){
        if(input_gpio_copy[i] != this->input_gpio_tracker[i]){
            return false;
        }
    }

    for(int i = 0; i < SIM_OUTPUT_GPIO_NUM; i++){
        if(output_gpio_copy[i] != this->output_gpio_tracker[i]){
            return false;
        }
    }
    
    
    // check if buffer states are the same:
    for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
        if(buffer_copy[i] != this->buffer[i]){
            return false;
        }
    }

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

    if(my_string_length < 2){
        return;
    }

    // for(int i = 0; i < LED_ARRAY_SIZE; i++){
    //     mState.virtual_led[i] = false;
    // }
    // reportUpdate();

    // Create copies of the header states and buffer states
    bool buffer_copy[BUFFER_ARRAY_SIZE];
    for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
        buffer_copy[i] = this->buffer[i];
    }

    
    bool gpio_input_copy[SIM_INPUT_GPIO_NUM];
    for(int i = 0; i < SIM_INPUT_GPIO_NUM; i++){
        gpio_input_copy[i] = this->input_gpio_tracker[i];
    }

    bool gpio_output_copy[SIM_OUTPUT_GPIO_NUM];
    for(int i = 0; i < SIM_OUTPUT_GPIO_NUM; i++){
        gpio_output_copy[i] = this->output_gpio_tracker[i];
    }
    

    while((saturation_itr_counter < NUM_SATURATION_ITR)){

        // my_header_copy = this->my_header;
        for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
            buffer_copy[i] = this->buffer[i];
        }

        for(int i = 0; i < SIM_INPUT_GPIO_NUM; i++){
            gpio_input_copy[i] = this->input_gpio_tracker[i];
        }

        for(int i = 0; i < SIM_OUTPUT_GPIO_NUM; i++){
            gpio_output_copy[i] = this->output_gpio_tracker[i];
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
            this->log() << "My string: " << my_string << endl;
            this->log() << "Iter " << while_loop_counter << " : " << my_string.substr(index, my_string_length - index);
            while_loop_counter++;
            index += read_logic_gate(my_string.substr(index, my_string_length - index));
            if(my_string[index] == ';'){
                index = index + 1;
            }
        }

        index = 0;
        while_loop_counter = 1;

        if(check_if_same(gpio_input_copy, gpio_output_copy, buffer_copy)){
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

    requestReportState();
    // reportUpdate();
}
