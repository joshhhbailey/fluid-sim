#include <SDL2/SDL.h>

#include <vector>

class Fluid
{
    public:
        Fluid(int _cellSize, int _gridDimensions, float _timeStep, float _diffusion, float _viscosity);

        void AddDensity(int _xPos, int _yPos, float _amount);
        void AddVelocity(int _xPos, int _yPos, float _amountX, float _amountY);
        void Diffuse(int b, std::vector<float>& x, std::vector<float>& xPrev, float _amount, float _timestep, int _iterations, int _gridDimensions);
        void LinearSolve(int _b, std::vector<float>& _x, std::vector<float>& _xPrev, float a, float c, int _iterations, int _gridDimensions);
        void Project(std::vector<float>& _xVel, std::vector<float>& _yVel, std::vector<float>& _p, std::vector<float>& _div, int _iterations, int _gridDimensions);
        void Advect(int _b, std::vector<float>& _d, std::vector<float>& _d0,  std::vector<float>& _xVel, std::vector<float>& _yVel, float _timeStep, int _gridDimensions);
        void SetBounds(int _b, std::vector<float>& _x, int _gridDimensions);

        void Fade(float _fadeRate);
        void ShowGrid(SDL_Renderer* _renderer);

        void Update();
        void Draw(SDL_Renderer* _renderer);
        void Reset();

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
};