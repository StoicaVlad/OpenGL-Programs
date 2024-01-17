#pragma once

#include <vector>

#include "components/simple_scene.h"

#include "lab/lab2/transform2D.h"

namespace lab
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
         Tema1();
        ~Tema1();

        void Init() override;

     private:

         enum movement_direction
         {
             UP_RIGHT = 0,
             UP = 1,
             UP_LEFT = 2,
             LEFT = 3,
             RIGHT = 4,
             DOWN_RIGHT = 5,
             DOWN = 6,
             DOWN_LEFT = 7
         };

         typedef struct CharacterPart {
             glm::vec3 position;
             glm::vec3 scale;
             glm::vec3 offset;
             float rotation = 0.0f;
         };

         typedef struct Character {
             CharacterPart leftLeg;
             CharacterPart rightLeg;
             CharacterPart leftArm;
             CharacterPart rightArm;
             CharacterPart head;
             CharacterPart torso;
             float bodyPartLocalRotation = 0.0f;
             float bodyPartMaxLocalRotation = 45.0f;
             bool increseRotation = true;
             glm::vec3 color;
             float movementSpeed;
             movement_direction movementDirection;
             float rotationAngle = 0.0f;
             glm::vec3 spot_light_position;
             glm::vec3 point_light_position;
             bool has_lights = false;
             float spot_light_offset = 3.0f;
         };

         typedef struct LightningPole {
             CharacterPart verticalPole;
             CharacterPart horizontalPole;
             glm::vec3 color;
             glm::vec3 spot_light_position_1;
             glm::vec3 spot_light_position_2;
         };

         Character player1;
         Character player2;
         bool multiplayer_enabled = false;

         int no_enemies;
         std::vector<Character> enemies = {};

         int no_poles;
         std::vector<LightningPole> poles = {};

         glm::vec3 point_light_positions[2];
         glm::vec3 point_light_colors[2];

         glm::vec3 spot_light_positions[26];
         glm::vec3 spot_light_directions[26];
         glm::vec3 spot_light_colors[26];
         float spot_light_angles[26];

         int controlled_light_source_index;
         glm::vec3 controlled_light_position;

         int score = 0;
         int maxScore = 15;

        void InitializeCharacterPositions(Character *player1, glm::vec3 initialPosition);
        void RotateCharacterBodyParts(Character* character);
        void SpawnEnemy();

        void DrawObjects(gfxc::Camera *camera, const transform2D::ViewportSpace &viewport_space, float deltaTime);
        void DrawCharacter(float deltaTime, glm::mat4 view, glm::mat4 projection, 
            Character character, gfxc::Camera* camera, glm::vec3 playerPos);

        void ShoootEnemies(Character player);

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

        void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, const glm::mat4 &viewMatrix,
            const glm::mat4 &projectionMatrix, const glm::vec3& object_color, gfxc::Camera* camera, glm::vec3 playerPos);

        void DrawText(const char* text, int length, int x, int y);

     protected:
        transform2D::ViewportSpace viewport_space;
        glm::vec3 color;
        GLenum cullFace;

        glm::vec3 terrainSize;
        glm::vec3 cameraWorldPosition_p1;
        glm::vec3 cameraWorldPosition_p2;
    };
}   // namespace lab
