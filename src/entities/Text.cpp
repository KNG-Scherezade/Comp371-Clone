#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/rotate_vector.hpp"
#include <vector>

#include "Entity.hpp"
#include "DrawableEntity.hpp"
#include "Text.hpp"

#include "../constants.hpp"
#include "../loadTexture.hpp"
#include <iostream>

Text::Text(
        const GLuint& shader_program,
        const std::string& message,
        const int line_start,
        const float left_margin,
        const unsigned int FONT_STYLE,
        Entity* parent
) : DrawableEntity(shader_program, parent) {

    this->draw_mode = GL_TRIANGLES;
    this->should_draw_back_face = true;

    if(FONT_STYLE==FONT_STYLE_OUTLINE){
        this->font_map = loadTexture(
                "../textures/outline_text_map.png",
                GL_LINEAR,
                GL_LINEAR,
                true
        );
    }else{
        this->font_map = loadTexture(
                "../textures/mythos_text_map.png",
                GL_LINEAR,
                GL_LINEAR,
                true
        );
    }

    // 0.01 scale for at least 100 chars per line
    float char_width = 0.01;
    float char_height = 0.01;

    float line_height = 0;
    if(line_start<1){
        line_height = 0;
    }else line_height = line_start * char_height;

    //the amount we can overlap on either side
    float char_r_space;
    float char_l_space;

    float l_margin = left_margin;

    if(l_margin<char_width){
        l_margin = char_width;
    }

    // keep track of the width to return line (line width sum accumulator) , start at the left margin
    float line_width_sum = l_margin;

    float cursor;

    for(int i=0, j=0; i<message.length(); i++, j+=4){

        cursor = line_width_sum;

        this->elements.emplace_back(j+2);
        this->elements.emplace_back(j);
        this->elements.emplace_back(j+1);
        this->elements.emplace_back(j+2);
        this->elements.emplace_back(j+1);
        this->elements.emplace_back(j+3);

        //we are not using these but want to keep a consistentency with drawable methods, and they could be used to light text
        this->normals.emplace_back(0.0f, 0.0f, 1.0f);
        this->normals.emplace_back(0.0f, 0.0f, 1.0f);
        this->normals.emplace_back(0.0f, 0.0f, 1.0f);
        this->normals.emplace_back(0.0f, 0.0f, 1.0f);

        //char_l_space, char_r_space and uv coordinates are relative to mythos_text_map
        if(FONT_STYLE==FONT_STYLE_OUTLINE){

            switch(message[i]){
                case 'a':
                case 'A':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0, 0.833333);
                    this->uvs.emplace_back(0.166666, 0.833333);
                    this->uvs.emplace_back(0, 1);
                    this->uvs.emplace_back(0.166666, 1);
                    break;
                case 'b':
                case 'B':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.166666, 0.833333);
                    this->uvs.emplace_back(0.333333, 0.833333);
                    this->uvs.emplace_back(0.166666, 1);
                    this->uvs.emplace_back(0.333333, 1);
                    break;
                case 'c':
                case 'C':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.333333, 0.833333);
                    this->uvs.emplace_back(0.5, 0.833333);
                    this->uvs.emplace_back(0.333333, 1);
                    this->uvs.emplace_back(0.5, 1);
                    break;
                case 'd':
                case 'D':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.5, 0.833333);
                    this->uvs.emplace_back(0.666666, 0.833333);
                    this->uvs.emplace_back(0.5, 1);
                    this->uvs.emplace_back(0.666666, 1);
                    break;
                case 'e':
                case 'E':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.666666, 0.833333);
                    this->uvs.emplace_back(0.833333, 0.833333);
                    this->uvs.emplace_back(0.666666, 1);
                    this->uvs.emplace_back(0.833333, 1);
                    break;
                case 'f':
                case 'F':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.833333, 0.833333);
                    this->uvs.emplace_back(1, 0.833333);
                    this->uvs.emplace_back(0.833333, 1);
                    this->uvs.emplace_back(1, 1);
                    break;
                case 'g':
                case 'G':
                    char_l_space = (float)(char_width*0.2);
                    char_r_space = (float)(char_width*0.36);
                    this->uvs.emplace_back(0, 0.666666);
                    this->uvs.emplace_back(0.166666, 0.666666);
                    this->uvs.emplace_back(0, 0.833333);
                    this->uvs.emplace_back(0.166666, 0.833333);
                    break;
                case 'h':
                case 'H':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.166666, 0.666666);
                    this->uvs.emplace_back(0.333333, 0.666666);
                    this->uvs.emplace_back(0.166666, 0.833333);
                    this->uvs.emplace_back(0.333333, 0.833333);
                    break;
                case 'i':
                case 'I':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.333333, 0.666666);
                    this->uvs.emplace_back(0.5, 0.666666);
                    this->uvs.emplace_back(0.333333, 0.833333);
                    this->uvs.emplace_back(0.5, 0.833333);
                    break;
                case 'j':
                case 'J':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.5, 0.666666);
                    this->uvs.emplace_back(0.666666, 0.666666);
                    this->uvs.emplace_back(0.5, 0.833333);
                    this->uvs.emplace_back(0.666666, 0.833333);
                    break;
                case 'k':
                case 'K':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.666666, 0.666666);
                    this->uvs.emplace_back(0.833333, 0.666666);
                    this->uvs.emplace_back(0.666666, 0.833333);
                    this->uvs.emplace_back(0.833333, 0.833333);
                    break;
                case 'l':
                case 'L':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.833333, 0.666666);
                    this->uvs.emplace_back(1, 0.666666);
                    this->uvs.emplace_back(0.833333, 0.833333);
                    this->uvs.emplace_back(1, 0.833333);
                    break;
                case 'm':
                case 'M':
                    char_l_space = (float)(char_width*0.2);
                    char_r_space = (float)(char_width*0.2);
                    this->uvs.emplace_back(0, 0.5);
                    this->uvs.emplace_back(0.166666, 0.5);
                    this->uvs.emplace_back(0, 0.666666);
                    this->uvs.emplace_back(0.166666, 0.666666);
                    break;
                case 'n':
                case 'N':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.24);
                    this->uvs.emplace_back(0.166666, 0.5);
                    this->uvs.emplace_back(0.333333, 0.5);
                    this->uvs.emplace_back(0.166666, 0.666666);
                    this->uvs.emplace_back(0.333333, 0.666666);
                    break;
                case 'o':
                case 'O':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.2);
                    this->uvs.emplace_back(0.333333, 0.5);
                    this->uvs.emplace_back(0.5, 0.5);
                    this->uvs.emplace_back(0.333333, 0.666666);
                    this->uvs.emplace_back(0.5, 0.666666);
                    break;
                case 'p':
                case 'P':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.5, 0.5);
                    this->uvs.emplace_back(0.666666, 0.5);
                    this->uvs.emplace_back(0.5, 0.666666);
                    this->uvs.emplace_back(0.666666, 0.666666);
                    break;
                case 'q':
                case 'Q':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.666666, 0.5);
                    this->uvs.emplace_back(0.833333, 0.5);
                    this->uvs.emplace_back(0.666666, 0.666666);
                    this->uvs.emplace_back(0.833333, 0.666666);
                    break;
                case 'r':
                case 'R':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.833333, 0.5);
                    this->uvs.emplace_back(1.0, 0.5);
                    this->uvs.emplace_back(0.833333, 0.666666);
                    this->uvs.emplace_back(1.0, 0.666666);
                    break;
                case 's':
                case 'S':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0, 0.333333);
                    this->uvs.emplace_back(0.166666, 0.333333);
                    this->uvs.emplace_back(0, 0.5);
                    this->uvs.emplace_back(0.166666, 0.5);
                    break;
                case 't':
                case 'T':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.24);
                    this->uvs.emplace_back(0.166666, 0.333333);
                    this->uvs.emplace_back(0.333333, 0.333333);
                    this->uvs.emplace_back(0.166666, 0.5);
                    this->uvs.emplace_back(0.333333, 0.5);
                    break;
                case 'u':
                case 'U':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.333333, 0.333333);
                    this->uvs.emplace_back(0.5, 0.333333);
                    this->uvs.emplace_back(0.333333, 0.5);
                    this->uvs.emplace_back(0.5, 0.5);
                    break;
                case 'v':
                case 'V':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.24);
                    this->uvs.emplace_back(0.5, 0.333333);
                    this->uvs.emplace_back(0.666666, 0.333333);
                    this->uvs.emplace_back(0.5, 0.5);
                    this->uvs.emplace_back(0.666666, 0.5);
                    break;
                case 'w':
                case 'W':
                    char_l_space = (float)(char_width*0.16);
                    char_r_space = (float)(char_width*0.16);
                    this->uvs.emplace_back(0.666666, 0.333333);
                    this->uvs.emplace_back(0.833333, 0.333333);
                    this->uvs.emplace_back(0.666666, 0.5);
                    this->uvs.emplace_back(0.833333, 0.5);
                    break;
                case 'x':
                case 'X':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0.833333, 0.333333);
                    this->uvs.emplace_back(1, 0.333333);
                    this->uvs.emplace_back(0.833333, 0.5);
                    this->uvs.emplace_back(1, 0.5);
                    break;
                case 'y':
                case 'Y':
                    char_l_space = (float)(char_width*0.32);
                    char_r_space = (float)(char_width*0.32);
                    this->uvs.emplace_back(0, 0.166666);
                    this->uvs.emplace_back(0.166666, 0.166666);
                    this->uvs.emplace_back(0, 0.333333);
                    this->uvs.emplace_back(0.166666, 0.333333);
                    break;
                case 'z':
                case 'Z':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.166666, 0.166666);
                    this->uvs.emplace_back(0.333333, 0.166666);
                    this->uvs.emplace_back(0.166666, 0.333333);
                    this->uvs.emplace_back(0.333333, 0.333333);
                    break;
                case '1':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.333333, 0.25);
                    this->uvs.emplace_back(0.416666, 0.25);
                    this->uvs.emplace_back(0.333333, 0.333333);
                    this->uvs.emplace_back(0.416666, 0.333333);
                    break;
                case '2':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.5, 0.25);
                    this->uvs.emplace_back(0.583333, 0.25);
                    this->uvs.emplace_back(0.5, 0.333333);
                    this->uvs.emplace_back(0.583333, 0.333333);
                    break;
                case '3':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.666666, 0.166666);
                    this->uvs.emplace_back(0.833333, 0.166666);
                    this->uvs.emplace_back(0.666666, 0.333333);
                    this->uvs.emplace_back(0.833333, 0.333333);
                    break;
                case '4':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.833333, 0.166666);
                    this->uvs.emplace_back(1, 0.166666);
                    this->uvs.emplace_back(0.833333, 0.333333);
                    this->uvs.emplace_back(1, 0.333333);
                    break;
                case '5':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0, 0.0f);
                    this->uvs.emplace_back(0.166666, 0.0f);
                    this->uvs.emplace_back(0, 0.166666);
                    this->uvs.emplace_back(0.166666, 0.16666);
                    break;
                case '6':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.166666, 0);
                    this->uvs.emplace_back(0.333333, 0);
                    this->uvs.emplace_back(0.166666, 0.166666);
                    this->uvs.emplace_back(0.333333, 0.166666);
                    break;
                case '7':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.333333, 0);
                    this->uvs.emplace_back(0.5, 0);
                    this->uvs.emplace_back(0.333333, 0.166666);
                    this->uvs.emplace_back(0.5, 0.166666);
                    break;
                case '8':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.5, 0);
                    this->uvs.emplace_back(0.666666, 0);
                    this->uvs.emplace_back(0.5, 0.166666);
                    this->uvs.emplace_back(0.666666, 0.166666);
                    break;
                case '9':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.666666, 0);
                    this->uvs.emplace_back(0.833333, 0);
                    this->uvs.emplace_back(0.666666, 0.166666);
                    this->uvs.emplace_back(0.833333, 0.166666);
                    break;
                case '0':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.833333, 0);
                    this->uvs.emplace_back(1, 0);
                    this->uvs.emplace_back(0.833333, 0.166666);
                    this->uvs.emplace_back(1, 0.166666);
                    break;
                case '-':
                    char_l_space = (float)(char_width*0.28);
                    char_r_space = (float)(char_width*0.28);
                    this->uvs.emplace_back(0.5, 0.166666);
                    this->uvs.emplace_back(0.583333, 0.166666);
                    this->uvs.emplace_back(0.5, 0.25);
                    this->uvs.emplace_back(0.583333, 0.25);
                    break;
                case ' ':
                    char_l_space = (float)(char_width*0.0);
                    char_r_space = (float)(char_width*0.5);
                    this->uvs.emplace_back(0.0f, 0.0f);
                    this->uvs.emplace_back(0.04, 0.0);
                    this->uvs.emplace_back(0.0f, 0.04f);
                    this->uvs.emplace_back(0.04f, 0.04f);
                    break;
                case ',':
                    char_l_space = char_width*0.0f;
                    char_r_space = char_width*0.8f;
                    this->uvs.emplace_back(0.416666, 0.25f); // 5.0f/12.0f, 3.0f/12.0f
                    this->uvs.emplace_back(0.5, 0.25);
                    this->uvs.emplace_back(0.416666, 0.333333);
                    this->uvs.emplace_back(0.5, 0.333333);
                    break;
                case '.':
                    char_l_space = char_width*0.0f;
                    char_r_space = char_width*0.8f;
                    this->uvs.emplace_back(0.333333, 0.166666); // 2.0f/6.0f, 1.0f/6.0f
                    this->uvs.emplace_back(0.416666, 0.166666); // 5.0f/12.0f, 1.0f/6.0f
                    this->uvs.emplace_back(0.333333, 0.25f); // 2.0f/6.0f, 3.0f/12.0f
                    this->uvs.emplace_back(0.416666, 0.25f);
                    break;
                case ':' :
                    char_l_space = char_width*0.3f;
                    char_r_space = char_width*0.3f;
                    this->uvs.emplace_back(0.416666, 0.166666);
                    this->uvs.emplace_back(0.5, 0.166666);
                    this->uvs.emplace_back(0.416666, 0.25f);
                    this->uvs.emplace_back(0.5, 0.25f);
                    break;
                case '\n' : // new line key
                    line_width_sum = l_margin;
                    line_height += char_height;
                    continue;
                default:
                    char_l_space = 0.0;
                    char_r_space = 0.0;

                    this->uvs.emplace_back(0.0f, 0.0f);
                    this->uvs.emplace_back(0.04, 0.0);
                    this->uvs.emplace_back(0.0f, 0.04f);
                    this->uvs.emplace_back(0.04f, 0.04f);

            }
        }

        //char_l_space, char_r_space and uv coordinates are relative to mythos_text_map

        if(FONT_STYLE==FONT_STYLE_MYTHOS) {
            switch (message[i]) {
                case 'a':
                case 'A':
                    char_l_space = char_width * 0.12f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0, 0.833333);
                    this->uvs.emplace_back(0.166666, 0.833333);
                    this->uvs.emplace_back(0, 1);
                    this->uvs.emplace_back(0.166666, 1);
                    break;
                case 'b':
                case 'B':
                    char_l_space = char_width * 0.24f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.166666, 0.833333);
                    this->uvs.emplace_back(0.333333, 0.833333);
                    this->uvs.emplace_back(0.166666, 1);
                    this->uvs.emplace_back(0.333333, 1);
                    break;
                case 'c':
                case 'C':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;

                    this->uvs.emplace_back(0.333333, 0.833333);
                    this->uvs.emplace_back(0.5, 0.833333);
                    this->uvs.emplace_back(0.333333, 1);
                    this->uvs.emplace_back(0.5, 1);
                    break;
                case 'd':
                case 'D':
                    char_l_space = char_width * 0.16f;
                    char_r_space = char_width * 0.12f;
                    this->uvs.emplace_back(0.5, 0.833333);
                    this->uvs.emplace_back(0.666666, 0.833333);
                    this->uvs.emplace_back(0.5, 1);
                    this->uvs.emplace_back(0.666666, 1);
                    break;
                case 'e':
                case 'E':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.666666, 0.833333);
                    this->uvs.emplace_back(0.833333, 0.833333);
                    this->uvs.emplace_back(0.666666, 1);
                    this->uvs.emplace_back(0.833333, 1);
                    break;
                case 'f':
                case 'F':
                    char_l_space = char_width * 0.32f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.833333, 0.833333);
                    this->uvs.emplace_back(1, 0.833333);
                    this->uvs.emplace_back(0.833333, 1);
                    this->uvs.emplace_back(1, 1);
                    break;
                case 'g':
                case 'G':
                    char_l_space = char_width * 0.12f;
                    char_r_space = char_width * 0.12f;
                    this->uvs.emplace_back(0, 0.666666);
                    this->uvs.emplace_back(0.166666, 0.666666);
                    this->uvs.emplace_back(0, 0.833333);
                    this->uvs.emplace_back(0.166666, 0.833333);
                    break;
                case 'h':
                case 'H':
                    char_l_space = char_width * 0.16f;
                    char_r_space = char_width * 0.16f;
                    this->uvs.emplace_back(0.166666, 0.666666);
                    this->uvs.emplace_back(0.333333, 0.666666);
                    this->uvs.emplace_back(0.166666, 0.833333);
                    this->uvs.emplace_back(0.333333, 0.833333);
                    break;
                case 'i':
                case 'I':
                    char_l_space = char_width * 0.36f;
                    char_r_space = char_width * 0.28f;
                    this->uvs.emplace_back(0.333333, 0.666666);
                    this->uvs.emplace_back(0.5, 0.666666);
                    this->uvs.emplace_back(0.333333, 0.833333);
                    this->uvs.emplace_back(0.5, 0.833333);
                    break;
                case 'j':
                case 'J':
                    char_l_space = char_width * 0.24f;
                    char_r_space = char_width * 0.24f;
                    this->uvs.emplace_back(0.5, 0.666666);
                    this->uvs.emplace_back(0.666666, 0.666666);
                    this->uvs.emplace_back(0.5, 0.833333);
                    this->uvs.emplace_back(0.666666, 0.833333);
                    break;
                case 'k':
                case 'K':
                    char_l_space = char_width * 0.24f;
                    char_r_space = char_width * 0.24f;
                    this->uvs.emplace_back(0.666666, 0.666666);
                    this->uvs.emplace_back(0.833333, 0.666666);
                    this->uvs.emplace_back(0.666666, 0.833333);
                    this->uvs.emplace_back(0.833333, 0.833333);
                    break;
                case 'l':
                case 'L':
                    char_l_space = char_width * 0.32f;
                    char_r_space = char_width * 0.16f;
                    this->uvs.emplace_back(0.833333, 0.666666);
                    this->uvs.emplace_back(1, 0.666666);
                    this->uvs.emplace_back(0.833333, 0.833333);
                    this->uvs.emplace_back(1, 0.833333);
                    break;
                case 'm':
                case 'M':
                    char_l_space = char_width * 0.0f;
                    char_r_space = char_width * 0.0f;
                    this->uvs.emplace_back(0, 0.5);
                    this->uvs.emplace_back(0.166666, 0.5);
                    this->uvs.emplace_back(0, 0.666666);
                    this->uvs.emplace_back(0.166666, 0.666666);
                    break;
                case 'n':
                case 'N':
                    char_l_space = char_width * 0.12f;
                    char_r_space = char_width * 0.16f;
                    this->uvs.emplace_back(0.166666, 0.5);
                    this->uvs.emplace_back(0.333333, 0.5);
                    this->uvs.emplace_back(0.166666, 0.666666);
                    this->uvs.emplace_back(0.333333, 0.666666);
                    break;
                case 'o':
                case 'O':
                    char_l_space = char_width * 0.16f;
                    char_r_space = char_width * 0.12f;
                    this->uvs.emplace_back(0.333333, 0.5);
                    this->uvs.emplace_back(0.5, 0.5);
                    this->uvs.emplace_back(0.333333, 0.666666);
                    this->uvs.emplace_back(0.5, 0.666666);
                    break;
                case 'p':
                case 'P':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.16f;
                    this->uvs.emplace_back(0.5, 0.5);
                    this->uvs.emplace_back(0.666666, 0.5);
                    this->uvs.emplace_back(0.5, 0.666666);
                    this->uvs.emplace_back(0.666666, 0.666666);
                    break;
                case 'q':
                case 'Q':
                    char_l_space = char_width * 0.16f;
                    char_r_space = char_width * 0.12f;
                    this->uvs.emplace_back(0.666666, 0.5);
                    this->uvs.emplace_back(0.833333, 0.5);
                    this->uvs.emplace_back(0.666666, 0.666666);
                    this->uvs.emplace_back(0.833333, 0.666666);
                    break;
                case 'r':
                case 'R':
                    char_l_space = char_width * 0.28f;
                    char_r_space = char_width * 0.12f;
                    this->uvs.emplace_back(0.833333, 0.5);
                    this->uvs.emplace_back(1.0, 0.5);
                    this->uvs.emplace_back(0.833333, 0.666666);
                    this->uvs.emplace_back(1.0, 0.666666);
                    break;
                case 's':
                case 'S':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.28f;
                    this->uvs.emplace_back(0, 0.333333);
                    this->uvs.emplace_back(0.166666, 0.333333);
                    this->uvs.emplace_back(0, 0.5);
                    this->uvs.emplace_back(0.166666, 0.5);
                    break;
                case 't':
                case 'T':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.16f;
                    this->uvs.emplace_back(0.166666, 0.333333);
                    this->uvs.emplace_back(0.333333, 0.333333);
                    this->uvs.emplace_back(0.166666, 0.5);
                    this->uvs.emplace_back(0.333333, 0.5);
                    break;
                case 'u':
                case 'U':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.16f;
                    this->uvs.emplace_back(0.333333, 0.333333);
                    this->uvs.emplace_back(0.5, 0.333333);
                    this->uvs.emplace_back(0.333333, 0.5);
                    this->uvs.emplace_back(0.5, 0.5);
                    break;
                case 'v':
                case 'V':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.16f;
                    this->uvs.emplace_back(0.5, 0.333333);
                    this->uvs.emplace_back(0.666666, 0.333333);
                    this->uvs.emplace_back(0.5, 0.5);
                    this->uvs.emplace_back(0.666666, 0.5);
                    break;
                case 'w':
                case 'W':
                    char_l_space = char_width * 0.0f;
                    char_r_space = char_width * 0.0f;
                    this->uvs.emplace_back(0.666666, 0.333333);
                    this->uvs.emplace_back(0.833333, 0.333333);
                    this->uvs.emplace_back(0.666666, 0.5);
                    this->uvs.emplace_back(0.833333, 0.5);
                    break;
                case 'x':
                case 'X':
                    char_l_space = char_width * 0.16f;
                    char_r_space = char_width * 0.08f;
                    this->uvs.emplace_back(0.833333, 0.333333);
                    this->uvs.emplace_back(1, 0.333333);
                    this->uvs.emplace_back(0.833333, 0.5);
                    this->uvs.emplace_back(1, 0.5);
                    break;
                case 'y':
                case 'Y':
                    char_l_space = char_width * 0.16f;
                    char_r_space = char_width * 0.24f;
                    this->uvs.emplace_back(0, 0.166666);
                    this->uvs.emplace_back(0.166666, 0.166666);
                    this->uvs.emplace_back(0, 0.333333);
                    this->uvs.emplace_back(0.166666, 0.333333);
                    break;
                case 'z':
                case 'Z':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.166666, 0.166666);
                    this->uvs.emplace_back(0.333333, 0.166666);
                    this->uvs.emplace_back(0.166666, 0.333333);
                    this->uvs.emplace_back(0.333333, 0.333333);
                    break;
                case '1':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.333333, 0.25);
                    this->uvs.emplace_back(0.416666, 0.25);
                    this->uvs.emplace_back(0.333333, 0.333333);
                    this->uvs.emplace_back(0.416666, 0.333333);
                    break;
                case '2':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.5, 0.166666);
                    this->uvs.emplace_back(0.666666, 0.166666);
                    this->uvs.emplace_back(0.5, 0.333333);
                    this->uvs.emplace_back(0.666666, 0.333333);
                    break;
                case '3':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.666666, 0.166666);
                    this->uvs.emplace_back(0.833333, 0.166666);
                    this->uvs.emplace_back(0.666666, 0.333333);
                    this->uvs.emplace_back(0.833333, 0.333333);
                    break;
                case '4':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.833333, 0.166666);
                    this->uvs.emplace_back(1, 0.166666);
                    this->uvs.emplace_back(0.833333, 0.333333);
                    this->uvs.emplace_back(1, 0.333333);
                    break;
                case '5':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0, 0.0f);
                    this->uvs.emplace_back(0.166666, 0.0f);
                    this->uvs.emplace_back(0, 0.166666);
                    this->uvs.emplace_back(0.166666, 0.16666);
                    break;
                case '6':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.166666, 0);
                    this->uvs.emplace_back(0.333333, 0);
                    this->uvs.emplace_back(0.166666, 0.166666);
                    this->uvs.emplace_back(0.333333, 0.166666);
                    break;
                case '7':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.333333, 0);
                    this->uvs.emplace_back(0.5, 0);
                    this->uvs.emplace_back(0.333333, 0.166666);
                    this->uvs.emplace_back(0.5, 0.166666);
                    break;
                case '8':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.5, 0);
                    this->uvs.emplace_back(0.666666, 0);
                    this->uvs.emplace_back(0.5, 0.166666);
                    this->uvs.emplace_back(0.666666, 0.166666);
                    break;
                case '9':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.666666, 0);
                    this->uvs.emplace_back(0.833333, 0);
                    this->uvs.emplace_back(0.666666, 0.166666);
                    this->uvs.emplace_back(0.833333, 0.166666);
                    break;
                case '0':
                    char_l_space = char_width * 0.2f;
                    char_r_space = char_width * 0.2f;
                    this->uvs.emplace_back(0.833333, 0);
                    this->uvs.emplace_back(1, 0);
                    this->uvs.emplace_back(0.833333, 0.166666);
                    this->uvs.emplace_back(1, 0.166666);
                    break;
                case ' ':
                    char_l_space = (float)(char_width * 0.0);
                    char_r_space = (float)(char_width * 0.5);
                    this->uvs.emplace_back(0.0f, 0.0f);
                    this->uvs.emplace_back(0.04, 0.0);
                    this->uvs.emplace_back(0.0f, 0.04f);
                    this->uvs.emplace_back(0.04f, 0.04f);
                    break;
                case ',':
                    char_l_space = char_width * 0.0f;
                    char_r_space = char_width * 0.8f;
                    this->uvs.emplace_back(0.416666, 0.25f); // 5.0f/12.0f, 3.0f/12.0f
                    this->uvs.emplace_back(0.5, 0.25);
                    this->uvs.emplace_back(0.416666, 0.333333);
                    this->uvs.emplace_back(0.5, 0.333333);
                    break;
                case '.':
                    char_l_space = char_width * 0.0f;
                    char_r_space = char_width * 0.8f;
                    this->uvs.emplace_back(0.333333, 0.166666); // 2.0f/6.0f, 1.0f/6.0f
                    this->uvs.emplace_back(0.416666, 0.166666); // 5.0f/12.0f, 1.0f/6.0f
                    this->uvs.emplace_back(0.333333, 0.25f); // 2.0f/6.0f, 3.0f/12.0f
                    this->uvs.emplace_back(0.416666, 0.25f);
                    break;
                case '\n' : // new line key
                    line_width_sum = l_margin;
                    line_height += char_height;
                    continue;
                default:
                    char_l_space = 0.0;
                    char_r_space = 0.0;

                    this->uvs.emplace_back(0.0f, 0.0f);
                    this->uvs.emplace_back(0.04, 0.0);
                    this->uvs.emplace_back(0.0f, 0.04f);
                    this->uvs.emplace_back(0.04f, 0.04f);

            }
        }


        //we are 'cutting away' the extra space before and after chars
        line_width_sum += char_width - (char_l_space + char_r_space);

        //we need to check if we need to go to the next line BEFORE assigning the vertices
        if(line_width_sum > 1.0){
            //we are drawing from the top down
            line_height += char_height;
            cursor = l_margin;
            line_width_sum = 0.0f;
        }

        if(line_height>1){
            break;
        }

        this->vertices.emplace_back(cursor - char_l_space, 1.0f - line_height - char_height, 0.0f);
        this->vertices.emplace_back(cursor - char_l_space + char_width, 1.0f - line_height - char_height, 0.0f);
        this->vertices.emplace_back(cursor - char_l_space, 1.0f - line_height, 0.0f);
        this->vertices.emplace_back(cursor - char_l_space + char_width, 1.0f - line_height, 0.0f);

    }

    this->vao = this->initVertexArray(
            this->vertices,
            this->elements,
            this->normals,
            this->uvs,
            &this->vertices_buffer,
            &this->element_buffer,
            &this->normal_buffer,
            &this->uv_buffer
    );


}

Text::~Text()
{
    glDeleteBuffers(1, &this->vertices_buffer);
    glDeleteBuffers(1, &this->element_buffer);
    glDeleteBuffers(1, &this->uv_buffer);
    glDeleteBuffers(1, &this->normal_buffer);
    glDeleteVertexArrays(1, &this->vao);
}

const std::vector<glm::vec3>& Text::getVertices() const
{
    return this->vertices;
}

GLuint Text::getVAO()
{
    return this->vao;
}

const int Text::getColorType()
{
    return COLOR_FONT;
}

GLuint Text::getTextureId()
{
    return this->font_map;
}




