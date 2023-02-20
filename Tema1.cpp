#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

#define N 100
#define Segments 20
#define PI 3.14
#define Escape_Time 6
#define Speed_Bullet 4000
using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

  

   
    translateX = 0;
    translateY = 0;
    Raza = 0;
    initialSpeed = 400;
    Speed = initialSpeed;
    translateX_score = 3.50 * resolution.x / 5;
    translateY_score = 4.50 * resolution.y / 5;

    scaleX = 30;
    scaleY = 30;
    new_duck = 1; // 1 atunci cand trebuie sa apara o rata noua
    nr_rate = 0;  // contor pt a stii cand trb sa incrementam viteza ratelor
    player_lives = 3;  // nr de vieti ale playerului
    bullets_left = 3;   
    scale_interface = 20;
    position_interface.x = 25;
    position_interface.y = resolution.y - 25;

    scale_score = 1;
    scale_bullet = 5;
    
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-2,-1,0),glm::vec3(0.588,0.294,0)),
        VertexFormat(glm::vec3(-2,1,0),glm::vec3(0.588,0.294,0)),
        VertexFormat(glm::vec3(0,3,0), glm::vec3(0.588,0.294,0)),
        VertexFormat(glm::vec3(3,0,0), glm::vec3(0.588,0.294,0)),
        VertexFormat(glm::vec3(0,-3,0), glm::vec3(0.588,0.294,0)),
        VertexFormat(glm::vec3(-0.5, 0, 0), glm::vec3(0.588,0.294,0)),
        VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.588,0.294,0)),
        VertexFormat(glm::vec3(3,1, 0), glm::vec3(0,1,0)),
        VertexFormat(glm::vec3(4, 0, 0), glm::vec3(0,1,0)),
        VertexFormat(glm::vec3(3, -1, 0), glm::vec3(0,1,0)),
        VertexFormat(glm::vec3(2, 0, 0), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(3, 0, 0), glm::vec3(0,1,0))
    };

    vector<VertexFormat>square_vertices
    {
        VertexFormat(glm::vec3(-2, -3, 0),glm::vec3(0,0,0)),
        VertexFormat(glm::vec3(-2,3, 0),glm::vec3(0,0,0)),
        VertexFormat(glm::vec3(4,3,0), glm::vec3(0,0,0)),
        VertexFormat(glm::vec3(4,-3,0), glm::vec3(0,0,0))
    };

    vector<unsigned int>square_indices
    {
        0,3, 2, 1
    };

    vector<VertexFormat>vertices_bullets
    {
        VertexFormat(glm::vec3(-1, -3, 0),glm::vec3(1,1,0)),
        VertexFormat(glm::vec3(-1,3, 0),glm::vec3(1,1,0)),
        VertexFormat(glm::vec3(1,3,0), glm::vec3(1,1,0)),
        VertexFormat(glm::vec3(1,-3,0), glm::vec3(1,1,0))
    };

    vector<unsigned int>indices_bullets
    {
       3, 2,0, 1
    };

    vector<VertexFormat>vertices_head;
    for (int i = 0; i <= Segments; i++)
    {
        vertices_head.push_back(VertexFormat(glm::vec3(3 + cos(i * 2 * PI / Segments), sin(i * 2 * PI / Segments), 0), glm::vec3(0, 1, 0)));
    }
    vector<unsigned int> indices_head;
    for (int i = 0; i < Segments; i++)
    {
        indices_head.push_back(Segments - i);
    }

    vector<VertexFormat>vertices_crosshair;
    for (int i = 0; i <= Segments; i++)
    {
        vertices_crosshair.push_back(VertexFormat(glm::vec3(cos(i * 2 * PI / Segments), sin(i * 2 * PI / Segments), 0), glm::vec3(1, 1, 1)));
    }
    vector<unsigned int> indices_crosshair;
    for (int i = 0; i < Segments; i++)
    {
        indices_crosshair.push_back(Segments - i);
    }

    meshes["crosshair_1"] = new Mesh("crosshair_1");
    meshes["crosshair_1"]->SetDrawMode(GL_LINE_LOOP);
    meshes["crosshair_1"]->InitFromData(vertices_crosshair, indices_crosshair);

    
    



    vector<VertexFormat>vertices_life;

    for (int i = 0; i <= Segments; i++)
    {
        vertices_life.push_back(VertexFormat(glm::vec3(cos(i * 2 * PI / Segments), sin(i * 2 * PI / Segments), 0), glm::vec3(1, 0, 0)));
    }

    vector<unsigned int> indices_life;
    for (int i = 0; i < Segments; i++)
    {
        indices_life.push_back(Segments - i);
    }


    vector<unsigned int> indices_body =
    {
            0, 3, 1,
    };

    vector<unsigned int> indices_wingL =
    {
            5, 6, 2,
    };

    vector<unsigned int> indices_wingR =
    {
            5, 6, 4
    };

    vector<VertexFormat> vertices_beak =
    {
        VertexFormat(vertices_head.at(2).position, glm::vec3(1,1,0)),
        VertexFormat(vertices_head.at(16).position, glm::vec3(1,1,0)),
        VertexFormat(glm::vec3(5, -0.2, 0), glm::vec3(1,1,0))
    };

    vector<unsigned int> indices_beak =
    {
        2,1,0
    };


    vector<VertexFormat> score_square_vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0,0,1)),
        VertexFormat(glm::vec3(1,0, 0), glm::vec3(0,0,1)),
        VertexFormat(glm::vec3(1,1, 0), glm::vec3(0,0,1)),
        VertexFormat(glm::vec3(0,1, 0), glm::vec3(0,0,1))
    };

    vector<unsigned int> indices_score =
    {
        1, 2, 0, 3
    };

    vector<VertexFormat> rectangular_green =
    {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0,1,0)),
        VertexFormat(glm::vec3(0, resolution.y / 7, 0), glm::vec3(0,1,0)),
        VertexFormat(glm::vec3(resolution.x, resolution.y / 7, 0), glm::vec3(0,1,0)),
        VertexFormat(glm::vec3(resolution.x, 0, 0), glm::vec3(0,1,0))
    };

    vector<unsigned int> rectangular_indices =
    {
        0, 1, 3, 2
    };

    vector<VertexFormat> weapon_vertices =
    {
        VertexFormat(glm::vec3(-2,1, 0), glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(2, 1, 0), glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(2, 0, 0), glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(1, 0, 0),glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(1, -2, 0), glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(0.5, -2, 0), glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(0.5, 0, 0), glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(-1.5, 0, 0), glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(-1.5, -2, 0), glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(-2, -2, 0), glm::vec3(0.58f, 0.294f, 0)),
        VertexFormat(glm::vec3(-2, 0, 0), glm::vec3(0.58f, 0.294f, 0))
    };

    vector<unsigned int> weapon_indices =
    {
        10, 9, 8,
        10, 8, 7, 
        0, 10, 7,
        0, 7, 6,
        0, 6, 3,
        0, 3, 1,
        6, 5, 4,
        6, 4, 3,
        3, 2, 1
    };

    vector<VertexFormat> bullet_square_vertices =
    {
        VertexFormat(glm::vec3(-1, -1, 0), glm::vec3(0.5, 0.5, 0.5)),
        VertexFormat(glm::vec3(-1, 1, 0), glm::vec3(0.5, 0.5, 0.5)),
        VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 0.5)),
        VertexFormat(glm::vec3(1, -1, 0), glm::vec3(0.5, 0.5, 0.5))
    };

    vector<unsigned int> bullet_Square_indices =
    {
        0, 1, 3, 2
    };

    meshes["bullet"] = new Mesh("bullet");
    meshes["bullet"]->SetDrawMode(GL_TRIANGLE_STRIP);
    meshes["bullet"]->InitFromData(bullet_square_vertices, bullet_Square_indices);
    
    meshes["weapon"] = new Mesh("weapon");
    meshes["weapon"]->SetDrawMode(GL_TRIANGLES);
    
    angularStep_weapon = 0;
    meshes["weapon"]->InitFromData(weapon_vertices, weapon_indices);

    meshes["grass"] = new Mesh("grass_rect");
    meshes["grass"]->SetDrawMode(GL_TRIANGLE_STRIP);
    meshes["grass"]->InitFromData(rectangular_green, rectangular_indices);

    meshes["duck_body"] = new Mesh("body");
    meshes["duck_body"]->SetDrawMode(GL_TRIANGLES);
    meshes["duck_body"]->InitFromData(vertices, indices_body);
    meshes["duck_wingL"] = new Mesh("wingL");
    meshes["duck_wingL"]->InitFromData(vertices, indices_wingL);
    meshes["duck_wingR"] = new Mesh("wingR");
    meshes["duck_wingR"]->InitFromData(vertices, indices_wingR);
    meshes["duck_head"] = new Mesh("head");
    meshes["duck_head"]->SetDrawMode(GL_TRIANGLE_FAN);
    meshes["duck_head"]->InitFromData(vertices_head, indices_head);

    meshes["duck_beak"] = new Mesh("beak");
    meshes["duck_beak"]->SetDrawMode(GL_TRIANGLES);
    meshes["duck_beak"]->InitFromData(vertices_beak, indices_beak);

    meshes["square_duck"] = new Mesh("square");
    meshes["square_duck"]->SetDrawMode(GL_LINE_LOOP);
    meshes["square_duck"]->InitFromData(square_vertices, square_indices);

    meshes["circle_lives"] = new Mesh("circle");
    meshes["circle_lives"]->SetDrawMode(GL_TRIANGLE_FAN);
    meshes["circle_lives"]->InitFromData(vertices_life, indices_life);

    meshes["square_bullets"] = new Mesh("square_b");
    meshes["square_bullets"]->SetDrawMode(GL_TRIANGLE_STRIP);
    meshes["square_bullets"]->InitFromData(vertices_bullets, indices_bullets);

    meshes["square_score"] = new Mesh("square_score");
    meshes["square_score"]->SetDrawMode(GL_TRIANGLE_STRIP);
    meshes["square_score"]->InitFromData(score_square_vertices, indices_score);

    
    translateX_weapon = resolution.x / 2;
    translateY_weapon = 50;
    scale_weapon = 25;
    R_bullet = 0;

    startX_bullet = 2 * scale_weapon + 1; // punem in varful pustii, 2 pt ca varful pustii este la x = 2 in coord lui
    startY_bullet = 0.5 * scale_weapon; // practic pozitionam si pe y in varful pustii
    tras = 0;
    angular_step_bullet = angularStep_weapon;       // at cand glontul este netras se roteste odata cu pusca
    
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
  

    glm::mat3 weapon_mat = glm::mat3(1);
    weapon_mat *= transform2D::Translate(translateX_weapon, translateY_weapon);
    weapon_mat *= transform2D::Rotate(angularStep_weapon);
    weapon_mat *= transform2D::Scale(scale_weapon, scale_weapon);

    if (tras == 1)
    {
        R_bullet += (Speed_Bullet * deltaTimeSeconds); 
        
        float dist = sqrt(pow(mouse_poz.x - startX_bullet, 2) + pow(mouse_poz.y - startY_bullet, 2));

        if (R_bullet >= dist*scale_bullet) // verificam daca glontul a ajuns la destinatie
        {
            ajuns = 1;
            angular_step_bullet = angularStep_weapon;
            tras = 0;
            R_bullet = 0; 
        }

    }
    else
    {
        R_bullet = 0;
        angular_step_bullet = angularStep_weapon;
    }

    translateX_bullet = R_bullet * cos(angular_step_bullet);
    translateY_bullet = R_bullet * sin(angular_step_bullet);

    glm::mat3 bullet_mat = glm::mat3(1);
    bullet_mat *= transform2D::Translate(translateX_bullet, translateY_bullet);
    bullet_mat *= transform2D::Translate(translateX_weapon, translateY_weapon);
   
    
    bullet_mat *= transform2D::Rotate(angular_step_bullet);
    
    bullet_mat *= transform2D::Translate(startX_bullet, startY_bullet );
    bullet_mat *= transform2D::Scale(scale_bullet, scale_bullet);

    RenderMesh2D(meshes["bullet"], shaders["VertexColor"], bullet_mat);


    RenderMesh2D(meshes["weapon"], shaders["VertexColor"], weapon_mat);




    RenderMesh2D(meshes["grass"], shaders["VertexColor"], glm::mat3(1) );

    glm::mat3 score_mat = glm::mat3(1);
    score_mat *= transform2D::Translate(translateX_score, translateY_score);
    score_mat *= transform2D::Scale(scale_score, 1);
    score_mat *= transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(meshes["square_score"], shaders["VertexColor"], score_mat);

    glm::mat3 crosshair_mat = glm::mat3(1);
    crosshair_mat *= transform2D::Translate(translateX_crosshair, translateY_crosshair);
    crosshair_mat *= transform2D::Scale(3, 3);
    RenderMesh2D(meshes["crosshair_1"], shaders["VertexColor"], crosshair_mat);

    crosshair_mat *= transform2D::Scale(3, 3);
    RenderMesh2D(meshes["crosshair_1"], shaders["VertexColor"], crosshair_mat);
    if (player_lives == 0)
    {
        //exit(1)
        new_duck = 0;
    }
    if (bullets_left == 0 && escaped == 0 && alive == 1)
    {
        escaped = 1;
        player_lives--;
    }
    
    if (new_duck == 1)
    {
        
        if (nr_rate % 5 == 0)
        {
            Speed = initialSpeed + (nr_rate / 5) * initialSpeed / 5;
        }
        nr_rate++;
        alive = 1;
        time = 0;
        bullets_left = 3;
       
        angularStep = PI / 8;
        angularStepDuck = 0;
        Raza = 0;
        while (fabs(angularStepDuck) < PI / 15)
        {
            angularStepDuck = rand() % 60 + 30;
            angularStepDuck = angularStepDuck * PI / 180;
        }

        startX = resolution.x / 2;
        startY = -resolution.y / 2.5;

        factor = 1;

        escaped = 0;
        new_duck = 0;
    }
    Raza +=  Speed * deltaTimeSeconds;
    time += deltaTimeSeconds;
    if (time > Escape_Time && escaped == 0 && alive == 1)
    {
        escaped = 1;
        player_lives--;
    }

    if (alive == 1 && escaped == 0)
    {
        translateX = startX + Raza * cos(angularStepDuck);
        translateY = startY + Raza * sin(angularStepDuck);
        if (translateY > resolution.y - 4 * scaleX) // ca sa nu iasa din ecran nici macar capul ratei
        {
            angularStepDuck = -angularStepDuck;
            Raza = 0;
            startX = translateX;
            startY = resolution.y - 4 * scaleY;
        }
        if (translateY < 4 * scaleY && startY > 4*scaleY)
        {
            angularStepDuck = -angularStepDuck;
            Raza = 0;
            startX = translateX;
            startY = 4 * scaleX;
        }
        if (translateX < 4 * scaleX)
        {
            angularStepDuck = -PI - angularStepDuck;
            Raza = 0;
            startX = 4 * scaleX;
            startY = translateY;
        }
        if (translateX > resolution.x - 4 * scaleX)
        {
            angularStepDuck = PI - angularStepDuck;
            Raza = 0;
            startX = resolution.x - 4 * scaleX;
            startY = translateY;
        }
    }

    if (escaped == 1)
    {
        translateY += Speed * 2 * deltaTimeSeconds;
        angularStepDuck = PI/2;
        if (translateY > resolution.y + 4 * scaleY)
        {
            new_duck = 1;
        }

    }

    if (alive == 0)
    {
        translateY -= initialSpeed * 5 * deltaTimeSeconds;
        angularStepDuck = -PI / 2;
        if (translateY < -4 * scaleY)
        {
            new_duck = 1;
        }

    }

    for (int i = 0, offset = 0; i < player_lives; i++, offset += 50)
    {
        glm::mat3 lives_matrix = glm::mat3(1);
        lives_matrix *= transform2D::Translate(position_interface.x + offset, position_interface.y);
        lives_matrix *= transform2D::Scale(scale_interface, scale_interface);
        RenderMesh2D(meshes["circle_lives"], shaders["VertexColor"], lives_matrix);
    }

    for (int i = 0, offset = 0; i < bullets_left; i++, offset += 50)
    {
        glm::mat3 bullets_matrix = glm::mat3(1);
        bullets_matrix *= transform2D::Translate(position_interface.x + offset, position_interface.y - 50);
        bullets_matrix *= transform2D::Scale(scale_interface/3, scale_interface/3);
        RenderMesh2D(meshes["square_bullets"], shaders["VertexColor"], bullets_matrix);
    }
    
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(angularStepDuck);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    
        RenderMesh2D(meshes["duck_head"], shaders["VertexColor"], modelMatrix);

        RenderMesh2D(meshes["duck_beak"], shaders["VertexColor"], modelMatrix);


        RenderMesh2D(meshes["duck_body"], shaders["VertexColor"], modelMatrix);
        
   
        if (alive == 1)
        {
            if (angularStep >= PI / 8)
            {
                factor = -Speed/ 200;
            }

            if (angularStep < -PI / 8)
            {
                factor = Speed/200;
            }

            angularStep += factor * deltaTimeSeconds;
        }
        else
        {
            angularStep = PI/4;
        }
        modelMatrix *= transform2D::Rotate(+angularStep);
   
   
        RenderMesh2D(meshes["duck_wingL"], shaders["VertexColor"], modelMatrix);
    

        modelMatrix *= transform2D::Rotate(-2*angularStep);

        RenderMesh2D(meshes["duck_wingR"], shaders["VertexColor"], modelMatrix);

     



}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    translateX_crosshair = mouseX;
    translateY_crosshair = resolution.y - mouseY;
    float tg_X = translateX_weapon - translateX_crosshair;
    float tg_Y = translateY_crosshair - translateY_weapon;
    angularStep_weapon = PI/2 + atan(tg_X/ tg_Y);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
   
        mouse_poz = glm::vec3(float(mouseX),resolution.y - float(mouseY), 1.0);
        glm::mat3 mod_mat = glm::mat3(1);
        mod_mat *= transform2D::Scale(1.0 / scaleX, 1.0 / scaleY);
        mod_mat *= transform2D::Rotate(-angularStepDuck);
        mod_mat *= transform2D::Translate(-translateX, -translateY);
        mouse_poz = mod_mat * mouse_poz;

        float pozX1 = meshes["square_duck"]->vertices.at(0).position.x;
        float pozX2 = meshes["square_duck"]->vertices.at(2).position.x;
        float pozY1 = meshes["square_duck"]->vertices.at(0).position.y;
        float pozY2 = meshes["square_duck"]->vertices.at(2).position.y;
        if (mouse_poz.x >= pozX1 && mouse_poz.x <= pozX2 && mouse_poz.y >= pozY1 && mouse_poz.y <= pozY2 && escaped == 0 && bullets_left > 0)
        {
            alive = 0;
            scale_score = scale_score + 0.1;
        }
        angular_step_bullet = angularStep_weapon;
        tras = 1;
        bullets_left--;
        
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
