#version 330

// od vertex shadera, kierunek w który wypuszczamy promień z kamery
in vec3 direction;

// cube mapa z teksturą nieba
uniform samplerCube enviromentMap;
uniform float exposure;
uniform vec3 WorldCoords; // (x, y, z)
uniform vec3 WorldCoordsSpherical; // (r,th,phi)
uniform float rs; // Shwarzchild radius

uniform bool useSpherical = true;
// output ostateczny kolor
out vec4 finalColor;

//
struct State {
        float r;
        float th;
        float phi;
        float dr; // pochodna r
        float dth; // pochodna th
};

State add(State s1, State s2)
{
        return State(s1.r + s2.r, s1.th + s2.th, s1.phi + s2.phi, s1.dr + s2.dr, s1.dth + s2.dth);
}
State scale(State s, float c)
{
        return State(s.r * c, s.th * c, s.phi * c, s.dr * c, s.dth * c);
}
State LinearComb(State s1, float a1, State s2, float a2)
{
        return add(scale(s1, a1), scale(s2, a2));
}

State Slope(State state, float E, float L)
{
        float r = state.r;
        float th = state.th;
        float phi = state.phi;
        float dr = state.dr;
        float dth = state.dth;

        // pomocnicze stałe dla przyspieszenia obliczen
        float invR = 1.0 / r;
        float invR2 = invR * invR;
        float cosTh = cos(th);
        float sinTh = sin(th);
        float invSinTh2 = 1.0 / (sinTh * sinTh);

        float f = 1.0 - rs * invR;
        float df = rs * invR2;

        // definiuje funkję pochodną stanu potrzebną do metody RK
        float fr = state.dr;
        float fth = state.dth;
        float fphi = L * invR2 * invSinTh2;
        float fphi2 = fphi * fphi;

        float fdr = 0.5 * df / f * (-E * E + dr * dr) + r * f * (dth * dth + sinTh * sinTh * fphi2);
        float fdth = -2. * invR * dr * dth + cosTh * sinTh * fphi2;

        return State(fr, fth, fphi, fdr, fdth);
}

State RK4Step(State state, float E, float L, float stepSize)
{
        State k1 = scale(Slope(state, E, L), stepSize);
        State k2 = scale(Slope(add(state, scale(k1, 0.5)), E, L), stepSize);
        State k3 = scale(Slope(add(state, scale(k2, 0.5)), E, L), stepSize);
        State k4 = scale(Slope(add(state, k3), E, L), stepSize);
        return add(state, scale(add(k1, add(scale(add(k2, k3), 2.0), k4)), 1.0 / 6.0));
}

// tablica współczynników RK45
const float B21 = 1.0 / 4.0;

const float B31 = 3.0 / 32.0;
const float B32 = 9.0 / 32.0;

const float B41 = 1932.0 / 2197.0;
const float B42 = -7200.0 / 2197.0;
const float B43 = 7296.0 / 2197.0;

const float B51 = 439.0 / 216.0;
const float B52 = -8.0;
const float B53 = 3680.0 / 513.0;
const float B54 = -845.0 / 4104.0;

const float B61 = -8.0 / 27.0;
const float B62 = 2.0;
const float B63 = -3544.0 / 2565.0;
const float B64 = 1859.0 / 4104.0;
const float B65 = -11.0 / 40.0;

const float c1 = 25. / 216.;
const float c3 = 1408. / 2565.;
const float c4 = 2197. / 4104.;
const float c5 = -1. / 5.;

const float C1 = 16. / 135.;
const float C3 = 6656. / 12825.;
const float C4 = 28561. / 56430.;
const float C5 = -9. / 50.;
const float C6 = 2. / 55.;
void RK45Step(State state, float E, float L, float h, out State state4, out State state5) {

        // metoda RK45 robi kroki metodą RK4 i RK5, następnie porównuje ich różnicę.
        // jeśli jest odpowiednio mała to akceptuje wynik i zwiększa rozmiar kroku.
        // jeśli różnica jest zbyt duża to wynik odrzucamy, zmniejszamy krok powtarzamy.

        // oblicz k1 ... k6
        State k1 = scale(Slope(state, E, L), h);
        State k2 = scale(Slope(
                                add(state, scale(k1, B21)), E, L), h);
        State k3 = scale(Slope(
                                add(state, LinearComb(k1, B31, k2, B32)), E, L), h);
        State k4 = scale(Slope(
                                add(state, add(scale(k1, B41), LinearComb(k2, B42, k3, B43))), E, L), h);
        State k5 = scale(Slope(
                                add(state, add(LinearComb(k1, B51, k2, B52), LinearComb(k3, B53, k4, B54))), E, L), h);
        State k6 = scale(Slope(
                                add(state, add(scale(k1, B61), add(LinearComb(k2, B62, k3, B63), LinearComb(k4, B64, k5, B65)))), E, L), h);

        // oblicz state4
        state4 = add(state,
                        add(LinearComb(k1, c1, k3, c3), LinearComb(k4, c4, k5, c5))
                );

        // oblicz state5
        state5 = add(state,
                        add(
                                add(LinearComb(k1, C1, k3, C3), LinearComb(k4, C4, k5, C5)),
                                scale(k6, C6)
                        )
                );
}

float StateError(State s1, State s2)
{
        // różnica między stanami
        // na razie prosty wzór
        return max(abs((s1.phi - s2.phi) * s1.r * sin(s1.th)),
                max(
                        max(abs(s1.r - s2.r), abs(s1.dr - s2.dr)),
                        max(abs(s1.th - s2.th) * s1.r, abs(s1.dth - s2.dth) * s1.r)
                ));
}
State EulerStep(State state, float E, float L, float stepSize)
{
        float r = state.r;
        float th = state.th;
        float phi = state.phi;
        float dr = state.dr;
        float dth = state.dth;

        float f = 1 - rs / r;
        float df = rs / r / r;

        state.r += dr * stepSize;
        state.th += dth * stepSize;
        state.phi += L / pow(r * sin(th), 2.0) * stepSize;

        // drugie pochodne
        float ddr = 0.5 * df / f * (-E * E + dr * dr) + r * f * (dth * dth + pow(L / sin(th) / r / r, 2.0));
        float ddth = -2. / r * dr * dth + cos(th) * sin(th) * pow(L / pow(r * sin(th), 2.0), 2.0);

        state.dr += ddr * stepSize;
        state.dth += ddth * stepSize;

        return state;
}
// vec3 SphericalCoords(vec3 WorldCoords){
//
// 	float r = length(WorldCoords);
// 	float x = WorldCoords.x;
// 	float y = WorldCoords.y;
// 	float z = WorldCoords.z;
// 	float th = acos(z/r);
// 	float phi = atan(y , x);// + 3.141592;
//
// 	return vec3(r,th,phi);
// }
// oblicza współrzędne kontrawariantne wektora w nowej bazie
vec3 ConvertToSpherical(vec3 WorldCoords, vec3 vector)
{
        float r = WorldCoords.x;
        float th = WorldCoords.y;
        float phi = WorldCoords.z;

        mat3 M = mat3(r * sin(th) * sin(th) * cos(phi), sin(th) * cos(th) * cos(phi), -sin(phi),
                        r * sin(th) * sin(th) * sin(phi), sin(th) * cos(th) * sin(phi), cos(phi),
                        r * sin(th) * cos(th), -sin(th) * sin(th), 0.0) / (r * sin(th));
        return M * vector;
}
// tylko dla wektorów kontrawariantnych
vec3 ConvertToCartesian(vec3 WorldCoords, vec3 vector)
{
        float r = WorldCoords.x;
        float th = WorldCoords.y;
        float phi = WorldCoords.z;

        mat3 M = mat3(sin(th) * cos(phi), sin(th) * sin(phi), cos(th),
                        r * cos(th) * cos(phi), r * cos(th) * sin(phi), -r * sin(th),
                        -r * sin(th) * sin(phi), r * sin(th) * cos(phi), 0.0);
        return M * vector;
}

vec3 CastRaySpherical(vec3 direction /*cartesian*/ , out bool passedHorison)
{
        float r = WorldCoordsSpherical.x;
        float th = WorldCoordsSpherical.y;
        float phi = WorldCoordsSpherical.z;

        vec3 dirSpher = ConvertToSpherical(WorldCoordsSpherical, direction);
        float dr = dirSpher.x;
        float dth = dirSpher.y;
        float dphi = dirSpher.z;

        // oblicz E i L
        float f = 1. - rs / r;
        float L = r * r * sin(th) * sin(th) * dphi;
        float E = sqrt(dr * dr + f * r * r * (dth * dth + sin(th) * sin(th) * dphi * dphi));

        passedHorison = (r <= rs);

        State state = State(
                        r, th, phi, dr, dth
                );

        State state4, state5;
        float eps = 0.5 * 1e-3; // error tolerance
        float h = 0.03; // first timestep

        for (int i = 0; i < 600; i++) {
                if (passedHorison) break;
                // state = EulerStep(state, E, L, h);
                // state = RK4Step(state, E, L, h + (state.r-rs)/800. * 0.0);

                RK45Step(state, E, L, h, state4, state5);

                float error = StateError(state4, state5);

                float factor = 0.9 * pow(eps / max(error, 1e-10), 0.2);
                factor = clamp(factor, 0.2, 2.0);

                h *= factor;

                if (error < eps) {
                        // akceptacja
                        state = state5;
                }

                if (state.r <= rs) passedHorison = true;
                if (state.r > 20) break;
        }

        vec3 DirSphr = vec3(state.dr, state.dth, L / pow(state.r * sin(state.th), 2.0));
        vec3 FinalCoordsSphr = vec3(state.r, state.th, state.phi);
        direction = ConvertToCartesian(FinalCoordsSphr, DirSphr);

        return direction;
}
struct StateCart {
        vec3 xyz;
        vec3 dxyz;
};
StateCart CartSlopeIncr(StateCart state, float h) {
        float InvR = 1.0 / length(state.xyz);
        float s = dot(normalize(state.xyz), state.dxyz);
        float a = 3.0 / 2.0 * rs * (InvR * InvR * InvR) * (-dot(state.dxyz, state.dxyz) + s * s);

        return StateCart(state.dxyz * h, a * state.xyz * h);
}

StateCart EulerStepCart(StateCart state, float h) {
        StateCart slope = CartSlopeIncr(state, h);
        state.xyz += slope.xyz;
        state.dxyz += slope.dxyz;
        return state;
}
float supremum(vec3 v) {
        return max(max(abs(v.x), abs(v.y)), abs(v.z));
}
float StateCartError(StateCart s1, StateCart s2) {
        return max(supremum(s1.xyz - s2.xyz), supremum(s1.dxyz - s2.dxyz));
        // return max(distance(s1.xyz, s2.xyz), distance(s1.dxyz, s2.dxyz));
}
void RK45StepCart(StateCart state, float h, out StateCart state4, out StateCart state5)
{
        StateCart k1 = CartSlopeIncr(state, h);
        StateCart k2 = CartSlopeIncr(StateCart(
                                state.xyz + k1.xyz * B21,
                                state.dxyz + k1.dxyz * B21
                        ), h);
        StateCart k3 = CartSlopeIncr(StateCart(
                                state.xyz + k1.xyz * B31 + k2.xyz * B32,
                                state.dxyz + k1.dxyz * B31 + k2.dxyz * B32
                        ), h);
        StateCart k4 = CartSlopeIncr(StateCart(
                                state.xyz + k1.xyz * B41 + k2.xyz * B42 + k3.xyz * B43,
                                state.dxyz + k1.dxyz * B41 + k2.dxyz * B42 + k3.dxyz * B43
                        ), h);
        StateCart k5 = CartSlopeIncr(StateCart(
                                state.xyz + k1.xyz * B51 + k2.xyz * B52 + k3.xyz * B53 + k4.xyz * B54,
                                state.dxyz + k1.dxyz * B51 + k2.dxyz * B52 + k3.dxyz * B53 + k4.dxyz * B54
                        ), h);
        StateCart k6 = CartSlopeIncr(StateCart(
                                state.xyz + k1.xyz * B61 + k2.xyz * B62 + k3.xyz * B63 + k4.xyz * B64 + k5.xyz * B65,
                                state.dxyz + k1.dxyz * B61 + k2.dxyz * B62 + k3.dxyz * B63 + k4.dxyz * B64 + k5.dxyz * B65
                        ), h);

        state4 = StateCart(
                        state.xyz + k1.xyz * c1 + k3.xyz * c3 + k4.xyz * c4 + k5.xyz * c5,
                        state.dxyz + k1.dxyz * c1 + k3.dxyz * c3 + k4.dxyz * c4 + k5.dxyz * c5
                );
        state5 = StateCart(
                        state.xyz + k1.xyz * C1 + k3.xyz * C3 + k4.xyz * C4 + k5.xyz * C5 + k6.xyz * C6,
                        state.dxyz + k1.dxyz * C1 + k3.dxyz * C3 + k4.dxyz * C4 + k5.dxyz * C5 + k6.dxyz * C6
                );
}

vec3 CastRayCartesian(vec3 direction, out bool passedHorison)
{
        float r = length(WorldCoords);
        passedHorison = r <= rs;
        StateCart state = StateCart(WorldCoords, direction);

        StateCart state4, state5;
        float eps = 0.5 * 1e-3;

        float h = 0.03;
        for (int i = 0; i < 600; i++) {
                // state = EulerStepCart(state, h);

                RK45StepCart(state, h, state4, state5);
                float error = StateCartError(state4, state5);
                float factor = 0.9 * pow(eps / max(error, 1e-10), 0.2);
                factor = clamp(factor, 0.2, 2.0);

                h *= factor;

                if (error < eps) {
                        // akceptacja
                        state = state5;
                }

                r = length(state.xyz);
                if (r <= rs) {
                        passedHorison = true;
                        break;
                }
                if (r > 20) break;
        }

        return state.dxyz;
}
void main()
{
        // odczytujemy kolor z kostki
        bool passedHorison;
        vec3 NewDir;
        if (useSpherical) {
                NewDir = CastRaySpherical(normalize(direction), passedHorison);
        }
        else {
                NewDir = CastRayCartesian(normalize(direction), passedHorison);
        }
        if (passedHorison) { // wpadł za horyzont
                finalColor = vec4(0., 0., 0., 1.);
                return;
        }
        vec3 color = texture(enviromentMap, NewDir).rgb;

        color *= pow(2.0, exposure);
        finalColor = vec4(color, 1.0);
}
