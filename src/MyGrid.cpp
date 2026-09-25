#include "MyGrid.hpp"
#include "rlgl.h"

void MyGrid::Draw() {
    // rysuje linie wzdłóż X, a następnie wzdlóż Y. każdą jako łamaną przez co
    // vertex shader może poprawnie zniekształcić
    // domyślnie jeśli subdivPerLine = slices to wszystkie wierzchołki leżą na
    // przeciąciach linii co da efekt jakby same wierzchołki kraty przesunąć.
    // jeśli subdivPerline jest duże, to  linie wydają się krzywymi

    rlBegin(RL_LINES);

    int N = (bendsPerEdge + 1) * slices; // number of verices per line
    float d = width / N;
    float a = width / (float)slices;

    // narysuj linie wzdłóż OX
    for (int j = 0; j <= slices; j++) {
        float z = -width / 2.0 + j * a;

        for (int i = 0; i < N; i++) {

            float x1 = -width / 2.0 + i * d;
            float x2 = x1 + d;
            rlVertex3f(x1, 0.0, z);
            rlVertex3f(x2, 0.0, z);
        }
    }
    // narysuj linie wzdłóż OZ
    for (int j = 0; j <= slices; j++) {
        float x = -width / 2.0 + j * a;

        for (int i = 0; i < N; i++) {

            float z1 = -width / 2.0 + i * d;
            float z2 = z1 + d;
            rlVertex3f(x, 0.0, z1);
            rlVertex3f(x, 0.0, z2);
        }
    }
}

void MyGrid::Draw(Shader shader) {

    BeginShaderMode(shader);
    Draw();
    EndShaderMode();
}

void MyGrid::Update() {

    if (IsKeyPressed(KEY_N) && slices > 1)
        slices--;
    if (IsKeyPressed(KEY_M))
        slices++;
    if (IsKeyPressed(KEY_COMMA) && bendsPerEdge > 0)
        bendsPerEdge--;
    if (IsKeyPressed(KEY_PERIOD))
        bendsPerEdge++;
    if (IsKeyDown(KEY_J))
        width /= 1.02;
    if (IsKeyDown(KEY_K))
        width *= 1.02;
}
