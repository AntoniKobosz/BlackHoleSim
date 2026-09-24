#include "SkyRayConfig.hpp"
#include "myGrid.hpp"
#include "myGui.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
static TextureCubemap GenTextureCubemap(Texture2D panorama, int size,
                                        int format);
void UpdateShader(Shader shader, Vector3 cameraPos);
void DrawSegmentedGrid(int slices, float spacing, int subdivPerLine);
Shader GetBlackHoleShader();
Vector3 SphericalCoordinate(Vector3 CartesianCoordinate);

Model GetSkybox(Shader shader, const char *panormaPath) {

    // tworzymy skybox
    Mesh cube = GenMeshCube(1, 1, 1);
    Model skybox = LoadModelFromMesh(cube);
    skybox.materials[0].shader = shader;

    Texture2D panorama = LoadTexture(panormaPath); // HDR
    int pixel_format =
        // RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8;
        RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16;
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture =
        GenTextureCubemap(panorama, 4096, pixel_format);
    UnloadTexture(panorama);

    return skybox;
}
Camera3D GetCamera() {
    Camera3D camera;
    camera.fovy = 70.0f; // 70.0f;
    camera.position = {7, -1, 7};
    camera.projection = CAMERA_PERSPECTIVE;
    camera.target = {0, 0, 0};
    camera.up = {0, 1, 0};

    return camera;
}

int main() {
    // SetConfigFlags(FLAG_VSYNC_HINT);
    SetWindowState(FLAG_WINDOW_UNDECORATED);

    // InitWindow(640, 480, "SkyRay");
    // InitWindow(840, 680, "SkyRay");
    // InitWindow(1200, 900, "SkyRay");
    // InitWindow(1920, 1080, "SkyRay");
    InitWindow(1920, 1200, "SkyRay");
    // InitWindow(2560, 1440, "SkyRay");

    SetWindowPosition(0, 0);
    // ToggleFullscreen();
    // ToggleBorderlessWindowed();
    DisableCursor();

    // GuiLoadStyle("resources/styles/style_cherry.rgs");
    Shader shader = GetBlackHoleShader();
    Model skybox = GetSkybox(shader, "assets/starmap_2020_8k.hdr");
    // Model skybox = GetSkybox(shader, "assets/Chiemsee_bei_Seebruck_Luftbild.png");
    Camera3D camera = GetCamera();

    Shader gridShdr = LoadShader("resources/shaders/gridShdr.vs",
                                 "resources/shaders/gridShdr.fs");
    MyGrid grid = MyGrid(50, 20, 0);
    float th = 0;
    myGui gui;
    gui.Init();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        gui.HandleInput();
        // UpdateCamera(&camera, CAMERA_ORBITAL);
        if (!gui.IsVisible()) {
            UpdateCamera(&camera, CAMERA_FREE);
        };
        UpdateShader(shader, camera.position);

        if (IsKeyPressed(KEY_O))
            SkyRayConfig::SHOW_DEBUG = !SkyRayConfig::SHOW_DEBUG;
        if (IsKeyPressed(KEY_R))
            SkyRayConfig::ORBIT = !SkyRayConfig::ORBIT;

        if (SkyRayConfig::ORBIT) {
            double speed = .01 * 20;
            double dt = GetFrameTime();
            th += dt * speed;
            camera.position =
                (Vector3){(float)cos(th), 0, (float)sin(th)} * 8.0f;
            camera.target = {0, 0, 0};
        }
        camera.fovy = SkyRayConfig::CAMERA_FOV;
        BeginMode3D(camera);

        rlDisableBackfaceCulling();
        rlDisableDepthMask();
        DrawModel(skybox, {0, 0, 0}, 1.0f, WHITE);
        rlEnableDepthMask();
        rlEnableBackfaceCulling();

        // hotfix
        DrawSphere({0,0,0}, 0.4f, BLACK);

        EndMode3D();

        gui.Draw();
        if (SkyRayConfig::SHOW_DEBUG) {
            DrawFPS(10, 10);
            // DrawText(TextFormat("Exposure = 2^%.2f", SkyRayConfig::EXPOSURE),
            //          10, 50, 20, LIME);
            // DrawText(
            DrawText(SkyRayConfig::USE_SPHERICAL ? "Spherical" : "Cartesian",
                     110, 10, 20, LIME);

            DrawText(TextFormat("rs = %.1f", SkyRayConfig::rs), 10, 90, 20,
                     LIME);
            DrawText(TextFormat("maxR = %.1f", SkyRayConfig::maxR), 10, 130, 20,
                     LIME);
            // DrawText(TextFormat("Slices = %d", Slices), 10, 90, 20, LIME);
            // DrawText(TextFormat("GridWidth = %.2f", GridWidth), 10, 130, 20,
            //          LIME);
            // DrawText(TextFormat("Subdivs = %d", Subdivs), 10, 170, 20, LIME);
            BeginMode3D(camera);
            DrawSphere({0, 0, 0}, SkyRayConfig::rs, GOLD);
            DrawSphereWires({0, 0, 0}, SkyRayConfig::rs * 2.6, 8, 9, LIME);
            // DrawSphereWires({0, 0, 0}, SkyRayConfig::maxR, 8, 9, DARKBLUE);
            DrawLine3D({0, 0, -SkyRayConfig::maxR}, {0, 0, SkyRayConfig::maxR},
                       PINK);
            // grid.Update();
            // grid.Draw(gridShdr);
            EndMode3D();
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

Vector3 SphericalCoordinate(Vector3 CartesianCoordinate) {

    float x = CartesianCoordinate.x;
    float y = CartesianCoordinate.y;
    float z = CartesianCoordinate.z;

    float r = Vector3Length(CartesianCoordinate);
    float th = acos(z / r);
    float phi = atan2(y, x);

    return {r, th, phi};
}
Shader GetBlackHoleShader() {

    // stwórz shader
    Shader shader = LoadShader("resources/shaders/skybox.vs",
                               "resources/shaders/skybox.fs");

    int cubeMapSlot = MATERIAL_MAP_CUBEMAP;
    SetShaderValue(shader, GetShaderLocation(shader, "enviromentMap"),
                   &cubeMapSlot, SHADER_UNIFORM_INT);
    SetShaderValue(shader, GetShaderLocation(shader, "rs"), &SkyRayConfig::rs,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, GetShaderLocation(shader, "exposure"),
                   &SkyRayConfig::EXPOSURE, SHADER_UNIFORM_FLOAT);

    return shader;
}
void UpdateShader(Shader shader, Vector3 cameraPos) {
    if (IsKeyDown(KEY_ONE)) {
        SkyRayConfig::EXPOSURE -= SkyRayConfig::EXPOSURE_RATE * GetFrameTime();
    }
    if (IsKeyDown(KEY_TWO)) {
        SkyRayConfig::EXPOSURE += SkyRayConfig::EXPOSURE_RATE * GetFrameTime();
    }
    if (IsKeyDown(KEY_THREE)) {
        SkyRayConfig::maxR /= 1.03;
    }
    if (IsKeyDown(KEY_FOUR)) {
        SkyRayConfig::maxR *= 1.03;
    }

    if (IsKeyPressed(KEY_I)) {
        SkyRayConfig::USE_SPHERICAL = !SkyRayConfig::USE_SPHERICAL;
    }
    int test = SkyRayConfig::USE_SPHERICAL ? 1 : 0;
    SetShaderValue(shader, GetShaderLocation(shader, "useSpherical"), &test,
                   SHADER_UNIFORM_INT);

    SetShaderValue(shader, GetShaderLocation(shader, "maxR"),
                   &SkyRayConfig::maxR, SHADER_UNIFORM_FLOAT);

    SetShaderValue(shader, GetShaderLocation(shader, "exposure"),
                   &SkyRayConfig::EXPOSURE, SHADER_UNIFORM_FLOAT);

    SetShaderValue(shader, GetShaderLocation(shader, "WorldCoords"), &cameraPos,
                   SHADER_UNIFORM_VEC3);
    Vector3 spherPos = SphericalCoordinate(cameraPos);
    SetShaderValue(shader, GetShaderLocation(shader, "WorldCoordsSpherical"),
                   &spherPos, SHADER_UNIFORM_VEC3);
}

// funkcja pomocnicza z raylib/examples/models/skyboxRendering.
static TextureCubemap GenTextureCubemap(Texture2D panorama, int size,
                                        int format) {
    Shader shdrCubemap = LoadShader("resources/shaders/cubemap.vs",
                                    "resources/shaders/cubemap.fs");
    // ustaw adres uquirectangularMap w shaderze na slot 0
    int textureSlot = 0;
    SetShaderValue(shdrCubemap,
                   GetShaderLocation(shdrCubemap, "equirectangularMap"),
                   &textureSlot, SHADER_UNIFORM_INT);

    TextureCubemap cubemap = {0};
    rlDisableBackfaceCulling(); // rzeby widoczne było wnętrze kostki

    // KROK 1: setup framebuffer
    // coś jak informacja o głębii textury
    unsigned int rbo = rlLoadTextureDepth(size, size, true);
    // tworzy pustą teksturę cubemapy
    cubemap.id = rlLoadTextureCubemap(0, size, format, 1);

    unsigned int fbo = rlLoadFramebuffer(); // tworzy bufor w pamięci

    // podłączamy rbo do buffera
    rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH,
                        RL_ATTACHMENT_RENDERBUFFER, 0);
    // podłączamy pustą teksturę do buffora zaczynając od strony X
    rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0,
                        RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);
    // sprawdzenie czy się udało
    if (rlFramebufferComplete(fbo)) {
        TRACELOG(LOG_INFO,
                 "FBO: [ID %i] FrameBuffer object created succesfully", fbo);
    }

    // KROK 2: wyrenderuj ściankay kostki za pomocą shadera
    rlEnableShader(shdrCubemap.id);

    // Macierz rzutu perspektywicznego, kamera z fov 90 (aspect 1 -> kwadrat)
    // idealnie rzutuje widok na ścianę 1/6 nieba
    Matrix matFboProjection = MatrixPerspective(
        90 * DEG2RAD, 1.0, rlGetCullDistanceNear(), rlGetCullDistanceFar());
    // podaje macierz projekcji do shadera na domyślne miejsce
    rlSetUniformMatrix(shdrCubemap.locs[SHADER_LOC_MATRIX_PROJECTION],
                       matFboProjection);

    // definiuje macierze widoku na kolejne ścieny kostki
    Vector3 eye = {0.0f, 0.0f, 0.0f};
    Matrix fboViews[6] = {
        MatrixLookAt(eye, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}),
        MatrixLookAt(eye, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}),
        MatrixLookAt(eye, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}),
        MatrixLookAt(eye, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}),
        MatrixLookAt(eye, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}),
        MatrixLookAt(eye, {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f}),
    };

    rlViewport(0, 0, size, size); // tworzy wirtualne okienko do renderowania

    // Przypisujemy teksturę panoramy do slota '0' który trzeba połączyć poza
    // funkcją z polem samplera w shaderze
    rlActiveTextureSlot(0);
    rlEnableTexture(panorama.id);

    for (int i = 0; i < 6; i++) {
        // podaj aktualną macierz widoku do matView shadera
        // (działa bo matView to domyślna nazwa)
        rlSetUniformMatrix(shdrCubemap.locs[SHADER_LOC_MATRIX_VIEW],
                           fboViews[i]);
        // wybieramy odpowiednią ściankę kostki do buffera
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0,
                            RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
        // trzeba włączyć bo poprzednia komenda wyłącza buffor
        rlEnableFramebuffer(fbo);

        rlClearScreenBuffers();
        // rysuje domyślny sześcian jednostkowy w clip space. mamy ustaloną
        // macierz widoku, która wybiera kierunek rysowania do podania do
        // fragment shadera. wynik zapisujemy do tekstury w bufforze
        rlLoadDrawCube();
    }

    // KROK 3: sprzątanie
    rlDisableShader();
    rlDisableTexture();
    rlDisableFramebuffer();
    rlUnloadFramebuffer(fbo);

    // przywróć wymiary do domyślnych
    rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
    rlEnableBackfaceCulling();

    cubemap.format = format;
    cubemap.width = size;
    cubemap.height = size;
    cubemap.mipmaps = 1;
    return cubemap;
}
