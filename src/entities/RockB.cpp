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
#include <stdexcept>
#include <algorithm>

#include "Entity.hpp"
#include "DrawableEntity.hpp"
#include "RockB.hpp"

#include "../constants.hpp"
#include "../loadTexture.hpp"
#include "../utils.hpp"


RockB::RockB(
        const GLuint &shader_program,
        const float &world_x_location,
        const float &world_z_location,
        const float &x_span,
        const float &z_span,
        Entity *parent
) : DrawableEntity(shader_program, parent)
{
    this->draw_mode = GL_TRIANGLES;

    // modified obsidian material from
    // Advanced Graphics Programming Using OpenGL
    // by Tom McReynolds and David Blythe
    // and copied from slides from COMP371 at Concordia:
    // http://poullis.org/courses/2017/Fall/COMP371/resources/COMP371F17_LightingandShading.pdf

    float ambient = utils::randomFloat(0.02f, 0.7f);

    float diffuseR = utils::randomFloat(0.65f, 0.75f);
    float diffuseG = utils::randomFloat(0.65f, 0.75f);
    float diffuseB = utils::randomFloat(0.7f, 0.85f);

    float specularR = utils::randomFloat(0.3f, 0.5f);
    float specularG = utils::randomFloat(0.3f, 0.5f);
    float specularB = utils::randomFloat(0.3f, 0.5f);

    float shininess = utils::randomFloat(38.4f, 70.8f);

    this->setMaterial(
            glm::vec3(ambient, ambient, ambient),
            glm::vec3(diffuseR, diffuseG, diffuseB),
            glm::vec3(specularR, specularG, specularB),
            shininess
    );

    //generate a sphere
    //this could be useful to change with  +/-  rand() % 5
    int num_arc_points = rand()%5 + 7;
    int num_longitude_lines = rand()%10 + 5;

    generateSphere(&this->vertices, &this->elements, &this->normals, &this->uvs, (num_arc_points-1), num_longitude_lines);

    this->vao = DrawableEntity::initVertexArray(
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

RockB::~RockB()
{
    glDeleteBuffers(1, &this->vertices_buffer);
    glDeleteBuffers(1, &this->element_buffer);
    glDeleteBuffers(1, &this->normal_buffer);
    glDeleteBuffers(1, &this->uv_buffer);
    glDeleteVertexArrays(1, &this->vao);
}

void RockB::generateSphere(
        std::vector<glm::vec3>* vertices,
        std::vector<GLuint>* ebo,
        std::vector<glm::vec3>* normals,
        std::vector<glm::vec2>* uvs,
        const int num_arc_segments,
        const int num_longitude_lines)
{

    //generate one arc with num_arc_segments
    for(float x=-1; x<=1; ){
        float y = sqrtf(1-x*x); //the equation of the arc (a perfect circle) can eventually be swapped out for any quadratic equation (hyperbolic arc)
        vertices->emplace_back(glm::vec3(x, y, 0));
        x+=2.0/num_arc_segments;
    }
    //we rotate the arc around the x-axis for every num_longitude_lines, -1 to connect the penultimate to the origin.
    //we duplicate the end-points in every rotation but then ignore all but the originals for the indicies (element) buffer
    for(int i=1; i<num_longitude_lines; i++){
        for(int j = 0; j<(num_arc_segments+1); j++) {
            vertices->push_back(glm::rotateX((*vertices)[j], glm::radians((360.0f/(float)num_longitude_lines)*i)));
        }
    }


    for(int i=0; i<vertices->size(); i++){
        float scale = float((*vertices)[i].x * ((((float)rand())/(RAND_MAX))));
        if(scale<0.5 && scale>-0.5){
            scale = 0.5f;
        }
        if(scale>0.7){
            scale=0.7;
        }
        if(scale<0){
            scale = std::abs(scale);
        }

        (*vertices)[i].x = (*vertices)[i].x * scale;
        (*vertices)[i].y = (*vertices)[i].y * scale;
        (*vertices)[i].z = (*vertices)[i].z * scale;

    }

    //generate ebo
    for(int l=0; l<(num_longitude_lines-1); l++) {
        int arc_offset = l*(num_arc_segments+1);
        for (int k = 1; k < (num_arc_segments-1); k++) {
            ebo->emplace_back(arc_offset+k);
            ebo->emplace_back(arc_offset+k + 1);
            ebo->emplace_back(arc_offset+k + num_arc_segments+1);
            ebo->emplace_back(arc_offset+k + 1);
            ebo->emplace_back(arc_offset+k + 1 + num_arc_segments+1);
            ebo->emplace_back(arc_offset+k + num_arc_segments+1);
        }
    }
    //join the last arc to the initial arc
    for (int m = 1; m < (num_arc_segments-1); m++){
        int arc_offset = (num_longitude_lines-1)*(num_arc_segments+1);
        ebo->emplace_back(arc_offset+m);
        ebo->emplace_back(arc_offset+m+1);
        ebo->emplace_back(m);
        ebo->emplace_back(arc_offset+m+1);
        ebo->emplace_back(m + 1);
        ebo->emplace_back(m);
    }
    //join 'top' pole
    for(int n = 1; n<num_longitude_lines; n++){
        int arc_offset = (n-1)*(num_arc_segments+1);
        ebo->emplace_back(0);
        ebo->emplace_back(arc_offset+1);
        ebo->emplace_back(arc_offset+1+num_arc_segments+1);
    }

    //join last triangle of top pole
    ebo->emplace_back(0);
    ebo->emplace_back((num_longitude_lines-1)*(num_arc_segments+1)+1);
    ebo->emplace_back(1);

    //join 'bottom' pole
    for(int n = 1; n<num_longitude_lines; n++){
        int arc_offset = (n-1)*(num_arc_segments+1);
        ebo->emplace_back(arc_offset+1+(num_arc_segments-2));
        ebo->emplace_back(num_arc_segments);//end point
        ebo->emplace_back(arc_offset+1+(num_arc_segments-2)+num_arc_segments+1);
    }

    //join last triangle of bottom pole
    ebo->emplace_back((num_longitude_lines-1)*(num_arc_segments+1)+1+(num_arc_segments-2));
    ebo->emplace_back(num_arc_segments);
    ebo->emplace_back(num_arc_segments-1);

    //reverse ebo
    std::reverse(ebo->begin(), ebo->end());


    //normals and uvs (is emplace or push_back better here ?)
    for(int i = 0; i<vertices->size(); i++) {
        uvs->emplace_back(
                //u coordinate determined by averaging out y and z (then bring the range between 0 - 1)
                (((*vertices)[i].y + (*vertices)[i].z) / 2.0f) * 0.5f + 0.5f,
                //v coordinate is basically the x component
                (*vertices)[i].x * 0.5f + 0.5f);
        normals->emplace_back((*vertices)[i] - glm::vec3(0.0f));
    }

        // calculate normals
        //------------------

        //step 1: find the surface normal for each triangle in the element buffer

        std::vector<glm::vec3>surfaceNormals;

        for(int i=1; i<ebo->size()+1; i++){
            if(i%3 == 0){// then element at i-1 is the third vertex of the triangle

                //std::cout<<"element at "<<i<<" :"<<elements[i-1]<<std::endl;
                // vertex A is at i-3, vertex B is i-2, vertex C is i-1
                glm::vec3 line_seg_BA = (*vertices)[(*ebo)[i-2]] - (*vertices)[(*ebo)[i-3]];
                glm::vec3 line_seg_BC = (*vertices)[(*ebo)[i-2]] - (*vertices)[(*ebo)[i-1]];
                //  this is right or this cube, but if the normals look inverted in another application, just switch the order of the cross product operation: cross(line_seg_BA, line_seg_BC)
                glm::vec3 normal = glm::cross(line_seg_BC, line_seg_BA);
                surfaceNormals.emplace_back(normal);
            }
        }

        //step 2: find the average of the surface normals of the surfaces this vertex is part of

        std::vector<glm::vec3>connectedSurfaces;

        for(int i=0; i<vertices->size(); i++){
            // vector that will hold all normals of all the surfaces this vertex is part of
            connectedSurfaces.clear();
            for(int j=0; j<ebo->size(); j++){
                if(i==(*ebo)[j]){//the vertice is being drawn
                    //j/3 gives us the location of this surface normal
                    connectedSurfaces.emplace_back(surfaceNormals[j/3]);
                }
            }

            if(!connectedSurfaces.empty()){
                glm::vec3 sum = glm::vec3(0.0f);
                for(int k=0; k<connectedSurfaces.size(); k++){
                    sum += connectedSurfaces[k];
                }
                //average the normal for this vertex and update the normals buffer
                (*normals)[i] = glm::normalize(glm::vec3(sum.x/connectedSurfaces.size(), sum.y/connectedSurfaces.size(), sum.z/connectedSurfaces.size()));
            }
        }



}


const std::vector<glm::vec3>& RockB::getVertices() const
{
    return this->vertices;
}

GLuint RockB::getVAO()
{
    return this->vao;
}

const int RockB::getColorType()
{
    return COLOR_LIGHTING;
}

GLuint RockB::getTextureId()
{
    static GLuint rock_texture = loadTexture(
            "../textures/stoneC.jpg",
            GL_LINEAR,
            GL_LINEAR
    );
    return rock_texture;
}
