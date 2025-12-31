#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>
#include <iostream>

using std::vector;

/* =======================
   Marching Cubes Tables
   ======================= */

#include "edgeTable.h"
#include "triTable.h"

/* =======================
   Math
   ======================= */

struct Vec3 {
    float x, y, z;
};

Vec3 operator+(const Vec3& a, const Vec3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 operator-(const Vec3& a, const Vec3& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 operator*(const Vec3& a, float s) {
    return {a.x * s, a.y * s, a.z * s};
}

/* =======================
   Scalar Field (Sphere)
   ======================= */

float scalarField(float x, float y, float z) {
    float r = 0.6f;
    return x*x + y*y + z*z - r*r;
}

/* =======================
   Interpolation
   ======================= */

Vec3 interpolate(const Vec3& p1, const Vec3& p2, float v1, float v2) {
    float t = v1 / (v1 - v2);
    return p1 + (p2 - p1) * t;
}

/* =======================
   Marching Cubes
   ======================= */

void marchingCubes(vector<Vec3>& vertices) {

    const int N = 30;
    const float step = 2.0f / (N - 1);

    Vec3 cubeVerts[8];
    float cubeValues[8];
    Vec3 edgeVerts[12];

    for (int x = 0; x < N - 1; x++) {
        for (int y = 0; y < N - 1; y++) {
            for (int z = 0; z < N - 1; z++) {

                int idx = 0;
                for (int dx = 0; dx <= 1; dx++)
                    for (int dy = 0; dy <= 1; dy++)
                        for (int dz = 0; dz <= 1; dz++) {
                            float px = -1.0f + (x + dx) * step;
                            float py = -1.0f + (y + dy) * step;
                            float pz = -1.0f + (z + dz) * step;

                            cubeVerts[idx] = {px, py, pz};
                            cubeValues[idx] = scalarField(px, py, pz);
                            idx++;
                        }

                int cubeIndex = 0;
                for (int i = 0; i < 8; i++)
                    if (cubeValues[i] < 0) cubeIndex |= (1 << i);

                if (edgeTable[cubeIndex] == 0)
                    continue;

                auto interp = [&](int a, int b, int e) {
                    if (edgeTable[cubeIndex] & (1 << e))
                        edgeVerts[e] = interpolate(
                            cubeVerts[a], cubeVerts[b],
                            cubeValues[a], cubeValues[b]);
                };

                interp(0,1,0); interp(1,2,1); interp(2,3,2); interp(3,0,3);
                interp(4,5,4); interp(5,6,5); interp(6,7,6); interp(7,4,7);
                interp(0,4,8); interp(1,5,9); interp(2,6,10); interp(3,7,11);

                for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {
                    vertices.push_back(edgeVerts[triTable[cubeIndex][i]]);
                    vertices.push_back(edgeVerts[triTable[cubeIndex][i+1]]);
                    vertices.push_back(edgeVerts[triTable[cubeIndex][i+2]]);
                }
            }
        }
    }
}

/* =======================
   Shaders
   ======================= */

const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform float angle;
void main() {
    float c = cos(angle);
    float s = sin(angle);
    mat3 rotY = mat3(
        c, 0, s,
        0, 1, 0,
       -s, 0, c
    );
    gl_Position = vec4(rotY * aPos, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.8, 0.9, 1.0, 1.0);
}
)";

/* =======================
   Main
   ======================= */

int main() {

    if (!glfwInit()) {
        std::cerr << "Erro ao iniciar GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Marching Cubes", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();

    vector<Vec3> vertices;
    marchingCubes(vertices);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fs);

    GLuint shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        float t = (float)glfwGetTime();
        glUniform1f(glGetUniformLocation(shader, "angle"), t * 0.6f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
