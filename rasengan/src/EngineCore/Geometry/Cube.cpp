#include "Cube.h"

Cube::Cube() {
    vertices = {
            //front  four vertices
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f},{ 1.0f, 0.0f }},
            {{0.5f,  -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f }},
            {{0.5f,  0.5f,  -0.5f}, {0.0f, 0.0f, 1.0f},{ 0.0f, 1.0f }},
            {{-0.5f, 0.5f,  -0.5f}, {1.0f, 1.0f, 1.0f},{ 1.0f, 1.0f }},
            //rear four vertices,
            {{-0.5f, -0.5f, 0.5f},  {1.0f, 0.0f, 0.0f},{ 1.0f, 0.0f }},
            {{0.5f,  -0.5f, 0.5f},  {0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f }},
            {{0.5f,  0.5f,  0.5f},  {0.0f, 0.0f, 1.0f},{ 0.0f, 1.0f }},
            {{-0.5f, 0.5f,  0.5f},  {0.0f, 1.0f, 1.0f},{ 1.0f, 1.0f }},
    };

/*    7____________ 6
*    /|            /|
*  3/_|___________/2|
 *  | |          |  |
 *  | |4.........|. |5
 *  | /          | /
 *  |/___________|/
 * 0            1
 */
    //counter-clockwise
    indices = {
            //front
            2, 1, 0,
            0, 3, 2,
            //left
            4, 3, 0,
            7, 3, 4,
            //back
            5, 6, 7,
            7, 4, 5,
            //right
            2, 6, 5,
            5, 1, 2,
            //bottom
            0, 1, 5,
            5, 4, 0,
            //top
            6, 2, 3,
            3, 7, 6,
    };
    this->InitBuffer();
}
