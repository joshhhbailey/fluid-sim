#include "Fluid.h"

#include <iostream>

Fluid::Fluid(int _cellSize, int _gridDimensions, float _timeStep, float _diffusion, float _viscosity)
{
    m_cellSize = _cellSize;
    m_gridDimensions = _gridDimensions;
    m_timeStep = _timeStep;
    m_diffusion = _diffusion;
    m_viscosity = _viscosity;

    // Initialise grid cells
    m_prevDensity = std::vector<float>(_gridDimensions * _gridDimensions, 0);
    m_density = std::vector<float>(_gridDimensions * _gridDimensions, 0);
    m_xVelPrev = std::vector<float>(_gridDimensions * _gridDimensions, 0);
    m_yVelPrev = std::vector<float>(_gridDimensions * _gridDimensions, 0);
    m_xVel = std::vector<float>(_gridDimensions * _gridDimensions, 0);
    m_yVel = std::vector<float>(_gridDimensions * _gridDimensions, 0);
}

void Fluid::AddDensity(int _xPos, int _yPos, float _amount)
{
    m_density[GetGridIndex(_xPos, _yPos)] += _amount;

    // Cap density to avoid overflow
    if (m_density[GetGridIndex(_xPos, _yPos)] > 255)
    {
        m_density[GetGridIndex(_xPos, _yPos)] = 255;
    }
}

void Fluid::AddVelocity(int _xPos, int _yPos, float _amountX, float _amountY)
{
    m_xVel[GetGridIndex(_xPos, _yPos)] += _amountX;
    m_yVel[GetGridIndex(_xPos, _yPos)] += _amountY;
}

void Fluid::Diffuse(int _b, std::vector<float>& _x, std::vector<float>& _xPrev, float _amount, float _timestep, int _iterations, int _gridDimensions)
{
    float a = _timestep * _amount * (_gridDimensions - 2) * (_gridDimensions - 2);
    LinearSolve(_b, _x, _xPrev, a, 1 + 6 * a, _iterations, _gridDimensions);
}

void Fluid::LinearSolve(int _b, std::vector<float>& _x, std::vector<float>& _xPrev, float _a, float _c, int _iterations, int _gridDimensions)
{
    float cRecip = 1.0f / _c;
    for (int k = 0; k < _iterations; ++k)
    {
        for (int j = 1; j < _gridDimensions - 1; ++j)
        {
            for (int i = 1; i < _gridDimensions - 1; ++i)
            {
                // Cell is a product of itself and its surrounding neighbours
                _x[GetGridIndex(i, j)] = (_xPrev[GetGridIndex(i, j)] + _a *
                                             (_x[GetGridIndex(i + 1, j)] +      // Right
                                              _x[GetGridIndex(i - 1, j)] +      // Left
                                              _x[GetGridIndex(i, j + 1)] +      // Down
                                              _x[GetGridIndex(i, j - 1 )]))     // Up
                                              * cRecip;
                }
            }
        SetBounds(_b, _x, _gridDimensions);
    }
}

void Fluid::Project(std::vector<float>& _xVel, std::vector<float>& _yVel, std::vector<float>& _p, std::vector<float>& _div, int _iterations, int _gridDimensions)
{
    for (int j = 1; j < _gridDimensions - 1; ++j)
    {
        for (int i = 1; i < _gridDimensions - 1; ++i)
        {
            // Cell is a product of itself and its surrounding neighbours
            _div[GetGridIndex(i, j)] = -0.5f * (_xVel[GetGridIndex(i + 1, j)] -
                                                _xVel[GetGridIndex(i - 1, j)] +
                                                _yVel[GetGridIndex(i, j + 1)] -
                                                _yVel[GetGridIndex(i, j - 1)])
                                                / _gridDimensions;
            _p[GetGridIndex(i, j)] = 0;
        }
    }
    SetBounds(0, _div, _gridDimensions); 
    SetBounds(0, _p, _gridDimensions);
    LinearSolve(0, _p, _div, 1, 4, _iterations, _gridDimensions);
    
    for (int j = 1; j < _gridDimensions - 1; ++j)
    {
        for (int i = 1; i < _gridDimensions - 1; ++i)
        {
            _xVel[GetGridIndex(i, j)] -= 0.5f * (_p[GetGridIndex(i + 1, j)] -
                                                 _p[GetGridIndex(i - 1, j)]) * _gridDimensions;
            _yVel[GetGridIndex(i, j)] -= 0.5f * (_p[GetGridIndex(i, j + 1)] -
                                                 _p[GetGridIndex(i, j - 1)]) * _gridDimensions;
        }
    }
    SetBounds(1, _xVel, _gridDimensions);
    SetBounds(2, _yVel, _gridDimensions);
}

void Fluid::Advect(int _b, std::vector<float>& _d, std::vector<float>& _d0,  std::vector<float>& _xVel, std::vector<float>& _yVel, float _timeStep, int _gridDimensions)
{
    float i0, i1, j0, j1;
    
    float dtx = _timeStep * (_gridDimensions - 2);
    float dty = _timeStep * (_gridDimensions - 2);
    
    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;
    
    float Nfloat = float(_gridDimensions);
    float ifloat, jfloat;
    int i, j;
    
    for (j = 1, jfloat = 1; j < _gridDimensions - 1; ++j, ++jfloat)
    { 
        for (i = 1, ifloat = 1; i < _gridDimensions - 1; ++i, ++ifloat)
        {
            tmp1 = dtx * _xVel[GetGridIndex(i, j)];
            tmp2 = dty * _yVel[GetGridIndex(i, j)];
            x = ifloat - tmp1;
            y = jfloat - tmp2;
                
            if (x < 0.5f)
            {
                x = 0.5f;
            }

            if (x > Nfloat + 0.5f)
            {
                x = Nfloat + 0.5f;
            }

            i0 = floorf(x);
            i1 = i0 + 1.0f;

            if (y < 0.5f)
            {
                y = 0.5f;
            }

            if (y > Nfloat + 0.5f)
            {
                y = Nfloat + 0.5f;
            }

            j0 = floorf(y);
            j1 = j0 + 1.0f; 
                
            s1 = x - i0; 
            s0 = 1.0f - s1; 
            t1 = y - j0; 
            t0 = 1.0f - t1;
                
            int i0i = int(i0);
            int i1i = int(i1);
            int j0i = int(j0);
            int j1i = int(j1);
                
            _d[GetGridIndex(i, j)] = s0 * (t0 * _d0[GetGridIndex(i0i, j0i)] + t1 * _d0[GetGridIndex(i0i, j1i)]) +
                                     s1 * (t0 * _d0[GetGridIndex(i1i, j0i)] + t1 * _d0[GetGridIndex(i1i, j1i)]);   
        }
    }
    SetBounds(_b, _d, _gridDimensions);
}

void Fluid::SetBounds(int _b, std::vector<float>& _x, int _gridDimensions)
{
    for (int i = 1; i < _gridDimensions - 1; ++i)
    {
        _x[GetGridIndex(i, 0)] = _b == 2 ? -_x[GetGridIndex(i, 1)] : _x[GetGridIndex(i, 1)];          // Top
        _x[GetGridIndex(i, _gridDimensions - 1)] = _b == 2 ? -_x[GetGridIndex(i, _gridDimensions - 2)] : _x[GetGridIndex(i, _gridDimensions - 2)];    // Bottom
    }
    for (int j = 1; j < _gridDimensions - 1; ++j)
    {
        _x[GetGridIndex(0, j)] = _b == 1 ? -_x[GetGridIndex(1, j)] : _x[GetGridIndex(1, j)];    // Left
        _x[GetGridIndex(_gridDimensions - 1, j)] = _b == 1 ? -_x[GetGridIndex(_gridDimensions - 2, j)] : _x[GetGridIndex(_gridDimensions - 2, j)];    // Right
    }
    
    _x[GetGridIndex(0, 0)] = 0.5f * (_x[GetGridIndex(1, 0)] + _x[GetGridIndex(0, 1)]);               // Top left
    _x[GetGridIndex(0, _gridDimensions - 1)] = 0.5f * (_x[GetGridIndex(1, _gridDimensions - 1)] + _x[GetGridIndex(0, _gridDimensions - 2)]);         // Top right
    _x[GetGridIndex(_gridDimensions - 1, 0)] = 0.5f * (_x[GetGridIndex(_gridDimensions - 2, 0)] + _x[GetGridIndex(_gridDimensions - 1, 1)]);         // Bottom left
    _x[GetGridIndex(_gridDimensions - 1, _gridDimensions - 1)] = 0.5f * (_x[GetGridIndex(_gridDimensions - 2, _gridDimensions - 1)] + _x[GetGridIndex(_gridDimensions - 1, _gridDimensions - 2)]);   // Bottom right
}

void Fluid::Update()
{
    float& timeStep = m_timeStep;
    int& gridDimensions = m_gridDimensions;
    float& diffusion = m_diffusion;
    float& viscosity = m_viscosity;

    // Density
    std::vector<float>& prevDensity = m_prevDensity;
    std::vector<float>& density = m_density;

    // Velocity
    std::vector<float>& xVelPrev = m_xVelPrev;
    std::vector<float>& yVelPrev = m_yVelPrev;
    std::vector<float>& xVel = m_xVel;
    std::vector<float>& yVel = m_yVel;
    
    Diffuse(1, xVelPrev, xVel, viscosity, timeStep, 4, gridDimensions);
    Diffuse(2, yVelPrev, yVel, viscosity, timeStep, 4, gridDimensions);
    
    Project(xVelPrev, yVelPrev, xVel, yVel, 4, gridDimensions);
    
    Advect(1, xVel, xVelPrev, xVelPrev, yVelPrev, timeStep, gridDimensions);
    Advect(2, yVel, yVelPrev, xVelPrev, yVelPrev, timeStep, gridDimensions);
    
    Project(xVel, yVel, xVelPrev, yVelPrev, 4, gridDimensions);
    
    Diffuse(0, prevDensity, density, diffusion, timeStep, 4, gridDimensions);
    Advect(0, density, prevDensity, xVel, yVel, timeStep, gridDimensions);
}

void Fluid::Render(SDL_Renderer* _renderer)
{
    for (int y = 0; y < m_gridDimensions; ++y)
    {
        for (int x = 0; x < m_gridDimensions; ++x)
        {
            int xGridPos = x * m_cellSize;
            int yGridPos = y * m_cellSize;
            int density = m_density[GetGridIndex(x, y)];
                
            // Draw cell
            SDL_Rect cell = {xGridPos, yGridPos, m_cellSize, m_cellSize};
            SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_ADD);
			SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, density);	
			SDL_RenderFillRect(_renderer, &cell);
        }
    }
}

int Fluid::GetGridIndex(int _xPos, int _yPos)
{
    // Constrain index (std::clamp is horrendously slow)
    if (_xPos > m_gridDimensions - 1)
    {
        _xPos = m_gridDimensions - 1;
    }
    else if (_xPos < 0)
    {
        _xPos = 0;
    }
    if (_yPos > m_gridDimensions - 1)
    {
        _yPos = m_gridDimensions - 1;
    }
    else if (_yPos < 0)
    {
        _yPos = 0;
    }

    return _xPos + (_yPos * m_gridDimensions);
}