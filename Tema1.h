#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        
    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        //int contor_aripi;
        //int contor_aripi_inapoi;
        int factor;
        int alive;
        int escaped;
        float angularStepDuck;
        float Raza;
        float startX, startY;
        float time;
        int new_duck; //1 cand vrem sa apara o rata pe ecran
        int player_lives;
        int bullets_left;
        float scale_interface;
        glm::ivec2 position_interface;
        glm::ivec2 resolution;
        float Speed;
        float initialSpeed;
        int nr_rate;
        float scale_score;
        float translateX_score;
        float translateY_score;
        float translateX_crosshair;
        float translateY_crosshair;
        float scale_weapon;
        float angularStep_weapon;
        float translateX_weapon, translateY_weapon;
        float translateX_bullet;
        float translateY_bullet;
        float scale_bullet;
        float startX_bullet, startY_bullet;
        float angular_step_bullet;
        int tras, ajuns;
        float R_bullet;
        glm::vec3 mouse_poz;

    };
}   
