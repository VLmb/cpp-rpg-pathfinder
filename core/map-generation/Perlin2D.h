#pragma once

#include <vector>
#include <cmath>
#include <numeric>   
#include <random>    
#include <algorithm> 

class Perlin2D {
private:
    std::vector<int> p; 

    static float Grad(int hash, float x, float y) {

        int h = hash & 7; 
        
        switch (h) {
            case 0: return  x + y; // ( 1,  1)
            case 1: return -x + y; // (-1,  1)
            case 2: return  x - y; // ( 1, -1)
            case 3: return -x - y; // (-1, -1)
            case 4: return  x;     // ( 1,  0)
            case 5: return -x;     // (-1,  0)
            case 6: return  y;     // ( 0,  1)
            default: return -y;    // ( 0, -1)
        }
    }

    // кривая сглаживания: 6t^5 - 15t^4 + 10t^3
    static float quinticCurve(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static float lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

public:

    Perlin2D(unsigned int seed = 0) {
        p.resize(512);

        for (int i = 0; i < 256; i++) {
            p[i] = i;
        }

        std::mt19937 engine(seed); // вихрь Мерсенна
        std::shuffle(p.begin(), p.begin() + 256, engine);

        for (int i = 0; i < 256; ++i) {
            p[256 + i] = p[i];
        }
    }

    float noise(float fx, float fy) const {

        // находим координаты клетки сетки (0..255)
        int X = static_cast<int>(std::floor(fx)) & 255;
        int Y = static_cast<int>(std::floor(fy)) & 255;

        // относительные координаты внутри клетки
        fx -= std::floor(fx);
        fy -= std::floor(fy);

        // Вычисляем кривые сглаживания
        float u = quinticCurve(fx);
        float v = quinticCurve(fy);

        // хэширование углов квадрата через таблицу перестановок
        int A = p[X] + Y;
        int B = p[X + 1] + Y;

        // скалярные произведения для углов
        float aa = Grad(p[A], fx, fy);
        float ab = Grad(p[A + 1], fx, fy - 1);
        float ba = Grad(p[B], fx - 1, fy);
        float bb = Grad(p[B + 1], fx - 1, fy - 1);

        // интерполяция
        float res = lerp(
            lerp(aa, ba, u),
            lerp(ab, bb, u),
            v
        );

        return res;
    }

    // шум с октавами (Фрактальный броуновское движение)
    float noise(float fx, float fy, int octaves, float persistence = 0.5f) const {
        float amplitude = 1.0f;
        float max = 0.0f;
        float result = 0.0f;

        while (octaves-- > 0) {
            max += amplitude;
            result += noise(fx, fy) * amplitude;
            amplitude *= persistence;
            fx *= 2.0f;
            fy *= 2.0f;
        }

        return result / max;
    }
};