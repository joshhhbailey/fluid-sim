#include <SDL2/SDL.h>

#include "Texture.h"

#include <vector>

class Fluid
{
    public:
        Fluid(int _cellSize, int _gridDimensions, float _timeStep, float _diffusion, float _viscosity, SDL_Renderer* _renderer);

        void AddDensity(int _xPos, int _yPos, float _amount);
        void AddVelocity(int _xPos, int _yPos, float _amountX, float _amountY);

        // The following functions have been implemented and ported from:
        // Stam, Jos., 2003. Real-Time Fluid Dynamics for Games. [online]
        // Available from: http://graphics.cs.cmu.edu/nsp/course/15-464/Fall09/papers/StamFluidforGames.pdf
        // Accessed [18 March 2021]
        void Diffuse(int b, std::vector<float>& x, std::vector<float>& xPrev, float _amount, float _timestep, int _iterations, int _gridDimensions);
        void LinearSolve(int _b, std::vector<float>& _x, std::vector<float>& _xPrev, float a, float c, int _iterations, int _gridDimensions);
        void Project(std::vector<float>& _xVel, std::vector<float>& _yVel, std::vector<float>& _p, std::vector<float>& _div, int _iterations, int _gridDimensions);
        void Advect(int _b, std::vector<float>& _d, std::vector<float>& _d0,  std::vector<float>& _xVel, std::vector<float>& _yVel, float _timeStep, int _gridDimensions);
        void SetBounds(int _b, std::vector<float>& _x, int _gridDimensions);
        // [End of reference]

        void Fade(float _fadeRate);
        void ShowGrid();
        void ShowVelocity();

        void Update();
        void Draw();
        void Reset();
        void Destroy();

        int GetGridIndex(int _xPos, int _yPos);

    private:
        int m_cellSize;
        int m_gridDimensions;
        float m_timeStep;
        float m_diffusion;
        float m_viscosity;
        
        // Density
        std::vector<float> m_prevDensity;
        std::vector<float> m_density;
        
        // Velocity
        std::vector<float> m_xVelPrev;
        std::vector<float> m_yVelPrev;
        std::vector<float> m_xVel;
        std::vector<float> m_yVel;

        SDL_Renderer* m_renderer;
        Texture m_arrow;
};