#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "level.h"
#include "draw.h"

int main(int argc, char const *argv[]){

    // Initialization
    const int screen_width = 800;
    const int screen_height = 600;


    InitWindow(screen_width,screen_height,"Presente - the game");

    InitAudioDevice();

    Music music = LoadMusicStream("../presente/Musica/Hydrogen.ogg");
    Music victory = LoadMusicStream("../presente/Musica/Static.ogg");

    PlayMusicStream(music);
    PlayMusicStream(victory);
    
    
    bool pause = false;

    SetTargetFPS(60);

    // Initialize level and fill randomly
    level *lvl = level_new(50,40);
    level_fill_random(lvl,6);

    // Initialize state (and add enemies)
    state *sta = state_new();
    state_populate_random(lvl,sta,40);

    // Main loop
    while(!WindowShouldClose()){

        UpdateMusicStream(music);
        UpdateMusicStream(victory);

        if (contador == 0){
            ResumeMusicStream(victory);
            PauseMusicStream(music);
        }

        if (contador != 0)
        {

            if (pause) PauseMusicStream(music);
            else {
                ResumeMusicStream(music);
                PauseMusicStream(victory);
            }
        }

        // Update input depending if keys are pressed or not
        sta->button_state[0] = IsKeyDown(KEY_D);
        sta->button_state[1] = IsKeyDown(KEY_W);
        sta->button_state[2] = IsKeyDown(KEY_A);
        sta->button_state[3] = IsKeyDown(KEY_S);
        sta->button_state[4] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        // Compute the angle the player is looking at from the mouse's position relative to the screen's center
        float mouse_y = GetMouseY()-GetScreenHeight()/2;
        float mouse_x = GetMouseX()-GetScreenWidth()/2;
        sta->aim_angle = atan2(-mouse_y,mouse_x);

        // Update the state
        state_update(lvl,sta);

        // Drawing
        BeginDrawing();

            ClearBackground(RAYWHITE);

            draw_state(lvl, sta);

            DrawText(FormatText("puntaje: %06i", score),5,0,20,RED);
            DrawText(FormatText("HighScore: %06i", maximo),5,40,20,RED);

            DrawText(FormatText("Enemigos: %i", contador),640,550,20,RED);

            if(contador == 0){
                DrawText("Nivel Terminado!", 200, 300, 50, GREEN);
                DrawText("Click esc", 300, 350, 12, GRAY);
                }

            DrawText("Presente profe!",190,200,20,LIGHTGRAY);

        EndDrawing();

    }

    // Close window
    UnloadMusicStream(victory);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    // Free memory
    state_free(sta);
    level_free(lvl);

    return 0;
}
