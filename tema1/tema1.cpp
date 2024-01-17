#include "lab/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "components/transform.h"
#include "core/engine.h"
#include "utils/gl_utils.h"

#include "lab/tema1/transform3D.h"

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
    cameraWorldPosition_p1 = glm::vec3(0, 10, -5);
    cameraWorldPosition_p2 = glm::vec3(5, 10, -5);

    terrainSize = glm::vec3(5, 0.25, 5);
    no_enemies = 25;
    no_poles = 12;

    player1.movementSpeed = 2.0f;
    player1.has_lights = true;

    player2.movementSpeed = 2.0f;
    player2.has_lights = true;

    InitializeCharacterPositions(&player1, glm::vec3(0.25f, 2.0f, 0.0f));
    InitializeCharacterPositions(&player2, glm::vec3(5.25f, 2.0f, 0.0f));

    for (int i = 0; i < no_enemies; i++) {
        SpawnEnemy();
    }

    point_light_positions[0] = player1.point_light_position;
    spot_light_positions[0] = player1.spot_light_position;
    int no_spots = 1;

    for (int i = 1; i <= no_poles / 4; i++)
    {
        for (int j = 1; j <= no_poles / 3; j++) {
            LightningPole pole;
            int random_pos_x = (rand() % 30) - 20;
            int random_pos_z = (rand() % 30) - 10;
            pole.verticalPole.position = glm::vec3( random_pos_x, 2, random_pos_z );
            pole.verticalPole.scale = glm::vec3(0.5f, 8, 0.5f);

            pole.horizontalPole.position = pole.verticalPole.position;
            pole.horizontalPole.position.y = 6;
            pole.horizontalPole.scale = glm::vec3(5, 0.5, 0.5);

            pole.spot_light_position_1 = glm::vec3(
                pole.verticalPole.position.x - 2.5,
                2,
                pole.verticalPole.position.z
                );

            pole.spot_light_position_2 = glm::vec3(
                pole.verticalPole.position.x + 2.5,
                2,
                pole.verticalPole.position.z
            );

            spot_light_positions[no_spots] = pole.spot_light_position_1;
            no_spots++;

            spot_light_positions[no_spots] = pole.spot_light_position_2;
            no_spots++;

            pole.color = glm::vec3(0.9, 0.1, 0.1);
            poles.push_back(pole);
        }
    }

    // Light & material properties
    {
        // set spot lights for player 1 [0], poles and player 2 [25]
        for (int i = 0; i < 26; i++) {
            spot_light_directions[i] = glm::vec3(0, -1, 0);
            spot_light_angles[i] = glm::radians(50.0f);

            spot_light_colors[i] = glm::vec3(1);
        }

        //  set point lights for player 1 and 2 only
        for (int i = 0; i < 2; i++) {

            point_light_colors[i] = glm::vec3(
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f
            );
        }
    }
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    color = glm::vec3(0);
    viewport_space = {0, 0, 1280, 720};
    cullFace = GL_BACK;
    
    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("HomeworkShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

}

void Tema1::SpawnEnemy()
{
    Character enemy;
    glm::vec3 initial_position = glm::vec3(rand() % 60 - 30, 2.0f, rand() % 60 - 30);
    InitializeCharacterPositions(&enemy, initial_position);
    enemies.push_back(enemy);
}

void Tema1::InitializeCharacterPositions(Character *player1, glm::vec3 initialPosition)
{
    (* player1).torso.position = initialPosition;
    (*player1).torso.scale = glm::vec3(1.0f, 1.0f, 0.5f);

    (*player1).head.offset = glm::vec3(0, 1.0f, 0);
    (*player1).head.position = (*player1).torso.position + (*player1).head.offset;
    (*player1).head.scale = glm::vec3(0.5f);

    (*player1).rightArm.offset = glm::vec3(-0.75f, 0, 0);
    (*player1).rightArm.position = (*player1).torso.position + (*player1).rightArm.offset;
    (*player1).rightArm.scale = glm::vec3(0.5f, 1.0f, 0.5f);

    (*player1).leftArm.offset = glm::vec3(0.75f, 0, 0);
    (*player1).leftArm.position = (*player1).torso.position + (*player1).leftArm.offset;
    (*player1).leftArm.scale = glm::vec3(0.5f, 1.0f, 0.5f);

    (*player1).rightLeg.offset = glm::vec3(0.25f, -1.0f, 0);
    (*player1).rightLeg.position = (*player1).torso.position + (*player1).rightLeg.offset;
    (*player1).rightLeg.scale = glm::vec3(0.5f, 1.0f, 0.5f);

    (*player1).leftLeg.offset = glm::vec3(-0.25f, -1.0f, 0);
    (*player1).leftLeg.position = (*player1).torso.position + (*player1).leftLeg.offset;
    (*player1).leftLeg.scale = glm::vec3(0.5f, 1.0f, 0.5f);

    if (player1->has_lights) {
        (*player1).point_light_position = player1->head.position + glm::vec3(0, 1.f, 0);
        (*player1).spot_light_position = player1->torso.position + glm::vec3(0, 2.f, 0);
    }
    float color_red = rand() / static_cast<float>(RAND_MAX);
    float color_green = rand() / static_cast<float>(RAND_MAX);
    float color_blue = rand() / static_cast<float>(RAND_MAX);
    player1->color = glm::vec3(color_red, color_green, color_blue);
}

void Tema1::FrameStart()
{

}

void Tema1::DrawText(const char* text, int length, int x, int y)
{
    /*glMatrixMode(GL_PROJECTION);
    double* matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, int(text[i]));
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);*/


}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    glClearColor(color.x, color.y, color.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static auto camera = new gfxc::Camera();

    // draw camera for one or two players
    if (!multiplayer_enabled) {
        camera->SetPositionAndRotation(cameraWorldPosition_p1, glm::quatLookAt(glm::normalize(glm::vec3(0, -3, 3)), glm::vec3(0, 1, 0)));
        viewport_space = transform2D::ViewportSpace(0, 0, resolution.x, resolution.y);
        DrawObjects(camera, viewport_space, deltaTimeSeconds);
    }
    else {
        camera->SetPositionAndRotation(cameraWorldPosition_p1, glm::quatLookAt(glm::normalize(glm::vec3(0, -3, 3)), glm::vec3(0, 1, 0)));
        viewport_space = transform2D::ViewportSpace(0, 0, resolution.x / 2.0, resolution.y);
        DrawObjects(camera, viewport_space, deltaTimeSeconds);

        camera->SetPositionAndRotation(cameraWorldPosition_p2, glm::quatLookAt(glm::normalize(glm::vec3(0, -3, 3)), glm::vec3(0, 1, 0)));
        viewport_space = transform2D::ViewportSpace(resolution.x / 2, 0, resolution.x / 2.0, resolution.y);
        DrawObjects(camera, viewport_space, deltaTimeSeconds);
    }

    // rotate pole lights
    for (int i = 1; i < 25; i++)
    {
        glm::vec3& light_direction = spot_light_directions[i];
        float& angle = spot_light_angles[i];

        glm::vec3 rotationAxis = glm::vec3(1, 0, 0);
        glm::vec4 initial_direction = glm::vec4(light_direction, 1.f);
        glm::mat4 model = glm::mat4(1.f);
        light_direction = initial_direction * glm::rotate(model, deltaTimeSeconds * 1, rotationAxis);
    }
}


void Tema1::FrameEnd()
{

}

void Tema1::DrawObjects(gfxc::Camera *camera, const transform2D::ViewportSpace & viewport_space, float deltaTime)
{
    glm::mat4 view = transform3D::View(
        camera->m_transform->GetWorldPosition(),
        camera->m_transform->GetLocalOZVector(),
        camera->m_transform->GetLocalOXVector(),
        camera->m_transform->GetLocalOYVector()
    );

    glm::mat4 projection = transform3D::Perspective(
        glm::radians(60.0f), (float)viewport_space.width / viewport_space.height, 0.1f, 100.0f
    );

    glViewport(viewport_space.x, viewport_space.y, viewport_space.width, viewport_space.height);

    // draw terrain
    glm::mat4 model = glm::mat4(1);
    model *= transform3D::Translate(glm::vec3(0.0f, 0.5f, 0));
    model *= transform3D::Scale(terrainSize);
    RenderMesh(meshes["plane"], shaders["HomeworkShader"], model, view, projection, glm::vec3(0,0,0.7f), camera, player1.torso.position);

    // draw players
    DrawCharacter(deltaTime, view, projection, player1, camera, player1.torso.position);
    if (multiplayer_enabled) {
        DrawCharacter(deltaTime, view, projection, player2, camera, player2.torso.position);
    }

    // draw poles
    for (int i = 0; i < no_poles; i++)
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, poles[i].verticalPole.position);
        model = glm::scale(model, poles[i].verticalPole.scale);
        RenderMesh(meshes["cube"], shaders["HomeworkShader"], model, view, projection, poles[i].color, camera, player1.torso.position);

        model = glm::mat4(1);
        model = glm::translate(model, poles[i].horizontalPole.position);
        model = glm::scale(model, poles[i].horizontalPole.scale);
        RenderMesh(meshes["cube"], shaders["HomeworkShader"], model, view, projection, poles[i].color, camera, player1.torso.position);
    }

    // draw enemies
    for (int i = 0; i < enemies.size(); i++) {
        glm::vec3 direction = glm::normalize(player1.torso.position - enemies[i].torso.position);
        enemies[i].torso.position += direction / 200.0f;

        float dx = enemies[i].torso.position.x - player1.torso.position.x;
        float dz = enemies[i].torso.position.z - player1.torso.position.z;
        enemies[i].rotationAngle = glm::degrees(atan2(dx, dz));

        RotateCharacterBodyParts(&enemies[i]);
        DrawCharacter(deltaTime, view, projection, enemies[i], camera, player1.torso.position);
    }

    // spawn enemies
    while (enemies.size() != no_enemies)
    {
        SpawnEnemy();
    }

    //DrawCoordinateSystem(view, projection);
}

void Tema1::DrawCharacter(float deltaTime, glm::mat4 view, glm::mat4 projection, Character character, gfxc::Camera* camera, glm::vec3 playerPos) {
    
    // get character world position 
    glm::vec3 characterWorldPosition = character.torso.position;
    if (character.leftArm.rotation >= 0 && character.leftArm.rotation <= 60)
    {
        character.leftArm.rotation += 5.0f;
        character.rightLeg.rotation += 5.0f;

        character.rightArm.rotation -= 5.0f;
        character.leftLeg.rotation -= 5.0f;
    }

    //torso
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, characterWorldPosition);
    model = glm::scale(model, character.torso.scale);
    model = glm::rotate(model, glm::radians(character.rotationAngle), glm::vec3(0,1,0));
    RenderMesh(meshes["cube"], shaders["HomeworkShader"], model, view, projection, character.color, camera, character.torso.position);

    //head
    model = glm::mat4(1);
    model = glm::translate(model, character.head.offset + characterWorldPosition);
    model = glm::scale(model, character.head.scale);
    model = glm::rotate(model, glm::radians(character.rotationAngle), glm::vec3(0, 1, 0));
    RenderMesh(meshes["cube"], shaders["HomeworkShader"], model, view, projection, character.color, camera, character.torso.position);

    //left leg
    model = glm::mat4(1);

    model = glm::translate(model, characterWorldPosition);
    model = glm::rotate(model, glm::radians(character.rotationAngle), glm::vec3(0,1,0));
    model = glm::translate(model, -characterWorldPosition);

    model = glm::translate(model, character.torso.position + glm::vec3(0, 0.0, 0));
    model = glm::rotate(model, glm::radians(-character.bodyPartLocalRotation), glm::vec3(1, 0, 0));
    model = glm::translate(model, -(character.torso.position + glm::vec3(0, 0.0, 0)));

    model = glm::translate(model, character.leftLeg.offset + characterWorldPosition);
    model = glm::scale(model, character.leftLeg.scale);

    RenderMesh(meshes["cube"], shaders["HomeworkShader"], model, view, projection, character.color, camera, character.torso.position);

    //right leg
    model = glm::mat4(1);

    model = glm::translate(model, characterWorldPosition);
    model = glm::rotate(model, glm::radians(character.rotationAngle), glm::vec3(0, 1, 0));
    model = glm::translate(model, -characterWorldPosition);

    model = glm::translate(model, character.torso.position + glm::vec3(0, 0.0, 0));
    model = glm::rotate(model, glm::radians(character.bodyPartLocalRotation), glm::vec3(1, 0, 0));
    model = glm::translate(model, -(character.torso.position + glm::vec3(0, 0.0, 0)));

    model = glm::translate(model, character.rightLeg.offset + characterWorldPosition);
    model = glm::scale(model, character.rightLeg.scale);

    RenderMesh(meshes["cube"], shaders["HomeworkShader"], model, view, projection, character.color, camera, character.torso.position);

    //left hand
    model = glm::mat4(1);

    model = glm::translate(model, characterWorldPosition);
    model = glm::rotate(model, glm::radians(character.rotationAngle), glm::vec3(0, 1, 0));
    model = glm::translate(model, -characterWorldPosition);

    model = glm::translate(model, character.torso.position + glm::vec3(0, 0.0, 0));
    model = glm::rotate(model, glm::radians(-character.bodyPartLocalRotation), glm::vec3(1, 0, 0));
    model = glm::translate(model, -(character.torso.position + glm::vec3(0, 0.0, 0)));

    model = glm::translate(model, character.leftArm.offset + characterWorldPosition);
    model = glm::scale(model, character.leftArm.scale);


    RenderMesh(meshes["cube"], shaders["HomeworkShader"], model, view, projection, character.color, camera, character.torso.position);

    //right hand
    model = glm::mat4(1);

    model = glm::translate(model, characterWorldPosition);
    model = glm::rotate(model, glm::radians(character.rotationAngle), glm::vec3(0, 1, 0));
    model = glm::translate(model, -characterWorldPosition);

    model = glm::translate(model, character.torso.position + glm::vec3(0, 0.0, 0));
    model = glm::rotate(model, glm::radians(character.bodyPartLocalRotation), glm::vec3(1, 0, 0));
    model = glm::translate(model, -(character.torso.position + glm::vec3(0, 0.0, 0)));

    model = glm::translate(model, character.rightArm.offset + characterWorldPosition);
    model = glm::scale(model, character.rightArm.scale);

    RenderMesh(meshes["cube"], shaders["HomeworkShader"], model, view, projection, character.color, camera, character.torso.position);
}

void Tema1::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix,
    const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix, const glm::vec3& object_color, gfxc::Camera* camera, glm::vec3 playerPos)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    GLint pointLightLocation = glGetUniformLocation(shader->GetProgramID(), "point_light_positions");
    glUniform3fv(pointLightLocation, 2, glm::value_ptr(point_light_positions[0]));

    GLint spotLightLocation = glGetUniformLocation(shader->GetProgramID(), "spot_light_positions");
    glUniform3fv(spotLightLocation, 25, glm::value_ptr(spot_light_positions[0]));

    GLint spotLightDirectionLocation = glGetUniformLocation(shader->GetProgramID(), "spot_light_directions");
    glUniform3fv(spotLightDirectionLocation, 25, glm::value_ptr(spot_light_directions[0]));

    GLint pointLightColorLocation = glGetUniformLocation(shader->GetProgramID(), "point_light_colors");
    glUniform3fv(pointLightColorLocation, 2, glm::value_ptr(point_light_colors[0]));

    GLint spotLightColorLocation = glGetUniformLocation(shader->GetProgramID(), "spot_light_colors");
    glUniform3fv(spotLightColorLocation, 25, glm::value_ptr(spot_light_colors[0]));

    GLint spotLightAngleLocation = glGetUniformLocation(shader->GetProgramID(), "spot_light_angles");
    glUniform1fv(spotLightAngleLocation, 25, spot_light_angles);

    glm::vec3 eye_position = camera->m_transform->GetWorldPosition();
    GLint eyePositionLocation = glGetUniformLocation(shader->GetProgramID(), "eye_position");
    glUniform3fv(eyePositionLocation, 1, glm::value_ptr(eye_position));

    glm::vec3 material_ka = object_color;
    glm::vec3 material_kd = object_color;
    glm::vec3 material_ks = object_color;
    int material_shininess = 1000;
    GLint locMaterialShininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(locMaterialShininess, material_shininess);

    GLint locMaterialKd = glGetUniformLocation(shader->program, "material_kd");  // diffuse light
    glUniform3fv(locMaterialKd, 1, glm::value_ptr(material_kd));

    GLint locMaterialKs = glGetUniformLocation(shader->program, "material_ks");  // specular light
    glUniform3fv(locMaterialKs, 1, glm::value_ptr(material_ks));

    GLint locMaterialKa = glGetUniformLocation(shader->program, "material_ka");  // specular light
    glUniform3fv(locMaterialKa, 1, glm::value_ptr(material_ka));


    int modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");
    glm::mat4 view = viewMatrix;
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");
    glm::mat4 projection = transform3D::Perspective(
        glm::radians(60.0f), (float)viewport_space.width / viewport_space.height, 0.1f, 100.0f
    );
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    int playerLocation = glGetUniformLocation(shader->GetProgramID(), "player_position");
    const glm::vec3 world_player_position = player1.torso.position; //playerPos;
    glUniform3fv(playerLocation, 1, glm::value_ptr(world_player_position));

    GLint colorValueLoc = glGetUniformLocation(shader->GetProgramID(), "in_color");
    glUniform3fv(colorValueLoc, 1, glm::value_ptr(object_color));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema1::ShoootEnemies(Character player)
{
    glm::vec3 circleCenter;
    float radius = 2;
    switch (player.movementDirection)
    {
    case UP:
    {
        circleCenter = player.torso.position + glm::vec3(0, 0, radius);
        break;
    }
    case DOWN:
    {
        circleCenter = player.torso.position + glm::vec3(0, 0, -radius);
        break;
    }
    case LEFT:
    {
        circleCenter = player.torso.position + glm::vec3(radius, 0, 0);
        break;
    }
    case RIGHT:
    {
        circleCenter = player.torso.position + glm::vec3(-radius, 0, 0);
        break;
    }
    case UP_LEFT:
    {
        circleCenter = player.torso.position + glm::vec3(radius / (sqrt(2)), 0, radius / (sqrt(2)));
        break;
    }
    case UP_RIGHT:
    {
        circleCenter = player.torso.position + glm::vec3(-radius / (sqrt(2)), 0, radius / (sqrt(2)));
        break;
    }
    case DOWN_LEFT:
    {
        circleCenter = player.torso.position + glm::vec3(-radius / (sqrt(2)), 0, -radius / (sqrt(2)));
        break;
    }
    case DOWN_RIGHT:
    {
        circleCenter = player.torso.position + glm::vec3(radius / (sqrt(2)), 0, -radius / (sqrt(2)));
        break;
    }
    }
    
    vector<Character>::iterator it;
    vector<int> to_remove_indexes = {};
    for (it = enemies.begin(); it != enemies.end(); it++)
    {
        if (distance((* it).torso.position, circleCenter) < radius) {
            to_remove_indexes.push_back(it - enemies.begin());
        }
    }

    for (int i = 0; i < to_remove_indexes.size(); i++)
    {
        enemies.erase(enemies.begin() + to_remove_indexes[i]);
        for (int j = i; j < to_remove_indexes.size(); j++)
        {
            to_remove_indexes[j]--;
        }
    }

    if (to_remove_indexes.size() > 0) {
        score += to_remove_indexes.size();
        cout << "SCORE: " << score << endl;
        if (score >= maxScore) {
            cout << "CONGRATS! YOU WON!\n";
            exit(0);
        }
    }
}

void Tema1::RotateCharacterBodyParts(Character* character)
{
    if (character->bodyPartLocalRotation >= character->bodyPartMaxLocalRotation) {
        character->increseRotation = false;
    }
    if (character->bodyPartLocalRotation <= -character->bodyPartMaxLocalRotation) {
        character->increseRotation = true;
    }
    
    character->bodyPartLocalRotation += character->increseRotation ? 1 : -1;
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    float world_distance = player1.movementSpeed * deltaTime;

    // player1 movement
    if (window->KeyHold(GLFW_KEY_W)) {
        RotateCharacterBodyParts(&player1);

        player1.torso.position.z += world_distance;
        player1.head.position.z += world_distance;

        player1.rightArm.position.z += world_distance;
        player1.leftArm.position.z += world_distance;

        player1.rightLeg.position.z += world_distance;
        player1.rightLeg.position.z += world_distance;

        spot_light_positions[0].z = player1.torso.position.z + player1.spot_light_offset;
        spot_light_positions[0].x = player1.torso.position.x;
        point_light_positions[0].z += world_distance;

        cameraWorldPosition_p1.z += world_distance;
        player1.rotationAngle = 0.f;
        player1.movementDirection = UP;

    }
    if (window->KeyHold(GLFW_KEY_S)) {
        RotateCharacterBodyParts(&player1);

        player1.torso.position.z -= world_distance;
        player1.head.position.z -= world_distance;

        player1.rightArm.position.z -= world_distance;
        player1.leftArm.position.z -= world_distance;

        player1.rightLeg.position.z -= world_distance;
        player1.rightLeg.position.z -= world_distance;

        spot_light_positions[0].z = player1.torso.position.z - player1.spot_light_offset;
        spot_light_positions[0].x = player1.torso.position.x;
        point_light_positions[0].z -= world_distance;

        cameraWorldPosition_p1.z -= world_distance;
        player1.rotationAngle = 180.f;
        player1.movementDirection = DOWN;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        RotateCharacterBodyParts(&player1);

        player1.torso.position.x -= world_distance;
        player1.head.position.x -= world_distance;

        player1.rightArm.position.x -= world_distance;
        player1.leftArm.position.x -= world_distance;
        
        player1.rightLeg.position.x -= world_distance;
        player1.rightLeg.position.x -= world_distance;

        spot_light_positions[0].z = player1.torso.position.z;
        spot_light_positions[0].x = player1.torso.position.x - player1.spot_light_offset;
        point_light_positions[0].x -= world_distance;

        cameraWorldPosition_p1.x -= world_distance;
        player1.rotationAngle = 90.0f;
        player1.movementDirection = RIGHT;

        if (window->KeyHold(GLFW_KEY_S)) {
            player1.rotationAngle = 45.0f;
            player1.movementDirection = DOWN_RIGHT;
            spot_light_positions[0].z = player1.torso.position.z - player1.spot_light_offset;
        }

        if (window->KeyHold(GLFW_KEY_W)) {
            player1.rotationAngle = -45.0f;
            player1.movementDirection = UP_RIGHT;
            spot_light_positions[0].z = player1.torso.position.z + player1.spot_light_offset;
        }
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        RotateCharacterBodyParts(&player1);

        player1.torso.position.x += world_distance;
        player1.head.position.x += world_distance;

        player1.rightArm.position.x += world_distance;
        player1.leftArm.position.x += world_distance;

        player1.rightLeg.position.x += world_distance;
        player1.rightLeg.position.x += world_distance;

        spot_light_positions[0].z = player1.torso.position.z;
        spot_light_positions[0].x = player1.torso.position.x + player1.spot_light_offset;
        point_light_positions[0].x += world_distance;

        cameraWorldPosition_p1.x += world_distance;
        player1.rotationAngle = -90.0f;
        player1.movementDirection = LEFT;
        
        if (window->KeyHold(GLFW_KEY_S)) {
            player1.rotationAngle = -45.0f;
            player1.movementDirection = DOWN_LEFT;
            spot_light_positions[0].z = player1.torso.position.z - player1.spot_light_offset;
        }

        if (window->KeyHold(GLFW_KEY_W)) {
            player1.rotationAngle = 45.0f;
            player1.movementDirection = UP_LEFT;
            spot_light_positions[0].z = player1.torso.position.z + player1.spot_light_offset;
        }
    }

    if (!window->KeyHold(GLFW_KEY_A) && !window->KeyHold(GLFW_KEY_D) && !window->KeyHold(GLFW_KEY_W) && !window->KeyHold(GLFW_KEY_S))
    {
        if (player1.bodyPartLocalRotation != 0.0)
        {
            player1.bodyPartLocalRotation += (player1.bodyPartLocalRotation > 0) ? -1 : 1;
        }
    }


    // player 2 movement
    if (window->KeyHold(GLFW_KEY_UP)) {
        RotateCharacterBodyParts(&player2);

        player2.torso.position.z += world_distance;
        player2.head.position.z += world_distance;

        player2.rightArm.position.z += world_distance;
        player2.leftArm.position.z += world_distance;

        player2.rightLeg.position.z += world_distance;
        player2.rightLeg.position.z += world_distance;

        spot_light_positions[25].z = player2.torso.position.z + player2.spot_light_offset;
        spot_light_positions[25].x = player2.torso.position.x;
        point_light_positions[1].z += world_distance;

        cameraWorldPosition_p2.z += world_distance;
        player2.rotationAngle = 0.f;
        player2.movementDirection = UP;
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        RotateCharacterBodyParts(&player2);

        player2.torso.position.z -= world_distance;
        player2.head.position.z -= world_distance;

        player2.rightArm.position.z -= world_distance;
        player2.leftArm.position.z -= world_distance;

        player2.rightLeg.position.z -= world_distance;
        player2.rightLeg.position.z -= world_distance;

        spot_light_positions[25].z = player2.torso.position.z - player2.spot_light_offset;
        spot_light_positions[25].x = player2.torso.position.x;
        point_light_positions[1].z -= world_distance;

        cameraWorldPosition_p2.z -= world_distance;
        player2.rotationAngle = 180.f;
        player2.movementDirection = DOWN;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        RotateCharacterBodyParts(&player2);

        player2.torso.position.x -= world_distance;
        player2.head.position.x -= world_distance;

        player2.rightArm.position.x -= world_distance;
        player2.leftArm.position.x -= world_distance;

        player2.rightLeg.position.x -= world_distance;
        player2.rightLeg.position.x -= world_distance;

        spot_light_positions[25].z = player2.torso.position.z;
        spot_light_positions[25].x = player2.torso.position.x - player2.spot_light_offset;
        point_light_positions[1].x -= world_distance;

        cameraWorldPosition_p2.x -= world_distance;
        player2.rotationAngle = 90.0f;
        player2.movementDirection = RIGHT;

        if (window->KeyHold(GLFW_KEY_DOWN)) {
            player2.rotationAngle = 45.0f;
            player2.movementDirection = DOWN_RIGHT;
            spot_light_positions[25].z = player2.torso.position.z - player2.spot_light_offset;
        }

        if (window->KeyHold(GLFW_KEY_UP)) {
            player2.rotationAngle = -45.0f;
            player2.movementDirection = UP_RIGHT;
            spot_light_positions[25].z = player2.torso.position.z + player2.spot_light_offset;
        }
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        RotateCharacterBodyParts(&player2);

        player2.torso.position.x += world_distance;
        player2.head.position.x += world_distance;

        player2.rightArm.position.x += world_distance;
        player2.leftArm.position.x += world_distance;

        player2.rightLeg.position.x += world_distance;
        player2.rightLeg.position.x += world_distance;

        spot_light_positions[25].z = player2.torso.position.z;
        spot_light_positions[25].x = player2.torso.position.x + player2.spot_light_offset;
        point_light_positions[1].x += world_distance;

        cameraWorldPosition_p2.x += world_distance;
        player2.rotationAngle = -90.0f;
        player2.movementDirection = LEFT;

        if (window->KeyHold(GLFW_KEY_DOWN)) {
            player2.rotationAngle = -45.0f;
            player2.movementDirection = DOWN_LEFT;
            spot_light_positions[25].z = player2.torso.position.z - player2.spot_light_offset;
        }

        if (window->KeyHold(GLFW_KEY_UP)) {
            player2.rotationAngle = 45.0f;
            player2.movementDirection = UP_LEFT;
            spot_light_positions[25].z = player2.torso.position.z + player2.spot_light_offset;
        }
    }

    if (!window->KeyHold(GLFW_KEY_LEFT) && !window->KeyHold(GLFW_KEY_RIGHT) && !window->KeyHold(GLFW_KEY_UP) && !window->KeyHold(GLFW_KEY_DOWN))
    {
        if (player2.bodyPartLocalRotation != 0.0)
        {
            player2.bodyPartLocalRotation += (player2.bodyPartLocalRotation > 0) ? -1 : 1;
        }
    }


    static auto camera = GetSceneCamera();
    camera->SetPosition(cameraWorldPosition_p2);
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_ENTER) {
        multiplayer_enabled = true;
        point_light_positions[1] = player2.point_light_position;
        spot_light_positions[25] = player2.spot_light_position;
    }

    if (key == GLFW_KEY_SPACE) {
        ShoootEnemies(player1);
    }

    if (key == GLFW_KEY_RIGHT_SHIFT && multiplayer_enabled) {
        ShoootEnemies(player2);
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
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
