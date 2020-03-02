#include <go2/input.h>

#include <unistd.h>
#include "INIReader.h"
#include <map>
#include <string>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

typedef struct
{
	std::string key_hot;
    	std::string key_light_up;
    	std::string key_light_down;
    	std::string key_volume_up;
    	std::string key_volume_down;
} configuration;


std::map<std::string,go2_button_state_t*> key_map;
go2_gamepad_state_t outGamepadState;


int main()
{
    configuration config={
        "f1","up","down","right","left"
    };

    INIReader reader("/home/odroid/.config/hotkey_for_oga/config.ini");
    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'config.ini'\n";
    }

    config.key_hot=reader.GetString("key_config","key_hot","f1");
    config.key_light_up=reader.GetString("key_config","key_light_up","up");
    config.key_light_down=reader.GetString("key_config","key_light_down","down");
    config.key_volume_up=reader.GetString("key_config","key_volume_up","right");
    config.key_volume_down=reader.GetString("key_config","key_volume_down","left");


     key_map.insert(std::pair<std::string,go2_button_state_t*>("up",&outGamepadState.dpad.up));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("down",&outGamepadState.dpad.down));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("left",&outGamepadState.dpad.left));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("right",&outGamepadState.dpad.right));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("a",&outGamepadState.buttons.a));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("b",&outGamepadState.buttons.b));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("x",&outGamepadState.buttons.x));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("y",&outGamepadState.buttons.y));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("f1",&outGamepadState.buttons.f1));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("f2",&outGamepadState.buttons.f2));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("f3",&outGamepadState.buttons.f3));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("f4",&outGamepadState.buttons.f4));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("f5",&outGamepadState.buttons.f5));
     key_map.insert(std::pair<std::string,go2_button_state_t*>("f6",&outGamepadState.buttons.f6));

    go2_input_t* input = go2_input_create();

    std::ifstream backlight_ifs("/sys/devices/platform/backlight/backlight/backlight/brightness",std::ios::in);
    if(!backlight_ifs){
        printf("brightness file open fail!\n");
        return 0;
	}
    std::stringstream ss;
    char line[256]={0};
    backlight_ifs.getline(line,256);
    ss<<line;
    int backlight_value=100;
    ss>>backlight_value;
    backlight_ifs.close();

    std::ifstream backlightmax_ifs("/sys/devices/platform/backlight/backlight/backlight/max_brightness",
        std::ios::in);
    if(!backlightmax_ifs){
        printf("brightnessmax file open fail!\n");
        return 0;
	}
    ss.clear();
    memset(line,0,sizeof(line));
    backlightmax_ifs.getline(line,256);
    ss<<line;
    int backlight_value_max = 150;
    ss>>backlight_value_max; 
    backlightmax_ifs.close();
    

    int keydown=0;
    while(1)
    {
        go2_input_gamepad_read(input,&outGamepadState);

        if (*key_map[config.key_hot] && key_map[config.key_light_up] && *key_map[config.key_light_up])
        {
            backlight_value += 20;
            if (backlight_value >= backlight_value_max)
            {
                backlight_value = backlight_value_max;
            }


            std::ofstream backlight_ofs("/sys/devices/platform/backlight/backlight/backlight/brightness",std::ios::out);
            backlight_ofs<<backlight_value<<std::endl;
            backlight_ofs.close();

            keydown = 1;
        }
        else if (*key_map[config.key_hot] && key_map[config.key_light_down] && *key_map[config.key_light_down])
        {
            backlight_value -= 20;
            if (backlight_value <= 0)
            {
                backlight_value = 0;
            }

            std::ofstream backlight_ofs("/sys/devices/platform/backlight/backlight/backlight/brightness",std::ios::out);
            backlight_ofs<<backlight_value<<std::endl;
            backlight_ofs.close();

            keydown = 1;
        }

        if (*key_map[config.key_hot] && key_map[config.key_volume_up] && *key_map[config.key_volume_up])
        {
            system("/usr/bin/amixer -q sset Playback 10%+ ");
            keydown = 1;
        }
        else if (*key_map[config.key_hot] && key_map[config.key_volume_down] && *key_map[config.key_volume_down])
        {
           
            system("/usr/bin/amixer -q sset Playback 10%- ");

            keydown = 1;
        }

        if(keydown)
        {
            usleep(1000000);
            keydown=0;
        }
        else
        {
            usleep(2000);
        }
    }
    go2_input_destroy(input);
    return 0;
}
