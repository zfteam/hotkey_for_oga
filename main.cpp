#include <go2/input.h>
#include <go2/display.h>
#include <go2/audio.h>

#include <unistd.h>
#include "INIReader.h"
#include <map>
#include <string>
#include <string.h>
#include <iostream>


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

    INIReader reader("config.ini");
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
    go2_display_t* display = go2_display_create();
    go2_audio_t* audio = go2_audio_create(44100);

    int backlight_value= (int)go2_display_backlight_get(display);
    int volume_value = (int)go2_audio_volume_get(audio);
    int keydown=0;

    while(1)
    {
        //memset(&outGamepadState,0,sizeof(go2_gamepad_state_t));
        go2_input_gamepad_read(input,&outGamepadState);

        if (*key_map[config.key_hot] && key_map[config.key_light_up] && *key_map[config.key_light_up])
        {
            backlight_value += 10;
            if (backlight_value >= 100)
            {
                backlight_value = 100;
            }
            printf("backlight up:%d\n", backlight_value);
            go2_display_backlight_set(display, backlight_value);

            keydown = 1;
        }
        else if (*key_map[config.key_hot] && key_map[config.key_light_down] && *key_map[config.key_light_down])
        {
            backlight_value -= 10;
            if (backlight_value <= 0)
            {
                backlight_value = 0;
            }
            printf("backlight down:%d\n", backlight_value);
            go2_display_backlight_set(display, backlight_value);
            keydown = 1;
        }

        if (*key_map[config.key_hot] && key_map[config.key_volume_up] && *key_map[config.key_volume_up])
        {
            volume_value += 10;
            if (volume_value >= 100)
            {
                volume_value = 100;
            }
            printf("volume up:%d\n", volume_value);
            go2_audio_volume_set(audio, volume_value);
            keydown = 1;
        }
        else if (*key_map[config.key_hot] && key_map[config.key_volume_down] && *key_map[config.key_volume_down])
        {
            volume_value -= 10;
            if (volume_value <= 0)
            {
                volume_value = 0;
            }
            printf("volume up:%d\n", volume_value);
            go2_audio_volume_set(audio, volume_value);
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
    go2_display_destroy(display);
    go2_audio_destroy(audio);
    return 0;
}
