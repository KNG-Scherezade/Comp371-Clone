//
// Created by Andre on 2017-11-18.
//

#ifndef PROCEDURALWORLD_LIGHT_H
#include <glm/glm.hpp>

#define PROCEDURALWORLD_LIGHT_H

    class Light{
    public:
        glm::vec3 light_direction;
        glm::vec3 color;
        glm::vec3 light_position;
        glm::vec3 position_light_color;
        //for dayTime calculation
        float daytime_value;
        float nighttime_value;
        glm::vec3 fog_color;
        glm::vec3 daytime;

        Light(glm::vec3 direction){
            this->daytime = direction;
            this->position_light_color = glm::vec3(.4, .35, .25);
        }

        void setDaytime(){
	        nighttime_value = (-daytime.y);
	        daytime_value = 1-nighttime_value;

            if (daytime.y < 0){
                light_direction = daytime;
                color = glm::vec3(.5,.5,.5);
                fog_color = glm::vec3(.6,.81,.92)* nighttime_value;
            } else {
                light_direction = daytime;
                color = glm::vec3(.3,.3,.3);
                fog_color = glm::vec3(.1,.1 ,.15)* nighttime_value;
            }
        };

    };


#endif //PROCEDURALWORLD_LIGHT_H
