#pragma once
#include <Windows.h>
#include <corecrt_math.h>
#define _SILENCE_ALL_CXX17_DEPRECATION_WARININGS
class Color
{
public:
	float  a = 255.f; float  r = 0; float g = 0; float  b = 0;
	Color(float r, float g, float b) { this->r = r; this->g = g; this->b = b; this->a = 255.f; };
	Color(float a, float r, float g, float b) { this->r = r; this->g = g; this->b = b; this->a = a; };
	Color() {};
	static DWORD convert(Color color);//color -> d3dcolor
	 DWORD convert();//color -> d3dcolor
     template<typename T> static inline void ImSwap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
     static inline float  ImFabs(float x) { return fabsf(x); }
     static inline float  ImFmod(float x, float y) { return fmodf(x, y); }
     void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
     {
         float K = 0.f;
         if (g < b)
         {
             ImSwap(g, b);
             K = -1.f;
         }
         if (r < g)
         {
             ImSwap(r, g);
             K = -2.f / 6.f - K;
         }

         const float chroma = r - (g < b ? g : b);
         out_h = ImFabs(K + (g - b) / (6.f * chroma + 1e-20f));
         out_s = chroma / (r + 1e-20f);
         out_v = r;
     } 
     float clamp(float c)
     {
         if (c < 0)c = 0;
         if (c > 255)c = 255;
         return c;
     }
     float clamp(float c) const
     {
         if (c < 0)c = 0;
         if (c > 255)c = 255;
         return c;
     };
     static void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
     {
         if (s == 0.0f)
         {
             // gray
             out_r = out_g = out_b = v;
             return;
         }

         h = ImFmod(h, 1.0f) / (60.0f / 360.0f);
         int   i = (int)h;
         float f = h - (float)i;
         float p = v * (1.0f - s);
         float q = v * (1.0f - s * f);
         float t = v * (1.0f - s * (1.0f - f));

         switch (i)
         {
         case 0: out_r = v; out_g = t; out_b = p; break;
         case 1: out_r = q; out_g = v; out_b = p; break;
         case 2: out_r = p; out_g = v; out_b = t; break;
         case 3: out_r = p; out_g = q; out_b = v; break;
         case 4: out_r = t; out_g = p; out_b = v; break;
         case 5: default: out_r = v; out_g = p; out_b = q; break;
         }
     }
     Color operator-(int a)
     {
         return Color(255.f, clamp(this->r - a), clamp(this->g - a), clamp(this->b - a));
     }
     Color operator-(int a) const
     {
         return Color(255.f, clamp(this->r - a), clamp(this->g - a), clamp(this->b - a));
     }
     Color operator+(int a)
     {
         return Color(255.f, clamp(this->r + a), clamp(this->g + a), clamp(this->b + a));
     }
     float* operator[](int i)
     {
         if (i == 0)return &this->r;
         if (i == 1)return &this->g;
         if (i == 2)return &this->b;
         if (i == 3)return &this->a;
     }
     Color operator/(float a)
     {
         return Color(255.f, clamp(this->r / a), clamp(this->g / a), clamp(this->b / a));
     }
     Color operator*(float a)
     {
         return Color(255.f, clamp(this->r * a), clamp(this->g * a), clamp(this->b * a));
     }
private:
};

