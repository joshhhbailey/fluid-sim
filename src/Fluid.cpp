#include "Fluid.h"

#include <iostream>

Fluid::Fluid(int _cellSize, int _gridDimensions, float _timeStep, float _diffusion, float _viscosity)
{
    m_cellSize = _cellSize;
    m_gridDimensions = _gridDimensions;
    m_timeStep = _timeStep;
    m_diffusion = _diffusion;
    m_viscosity = _viscosity;

    Reset();
}

void Fluid::AddDensity(int _xPos, int _yPos, float _amount)
{
    m_density[GetGridIndex(_xPos, _yPos)] += _amount;

    // Constrain density to avoid overflow of RGBA values
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
    LinearSolve(_b, _x, _xPrev, a, 1 + 4 * a, _iterations, _gridDimensions);
}

void Fluid::LinearSolve(int _b, std::vector<float>& _x, std::vector<float>& _xPrev, float _a, float _c, int _iterations, int _gridDimensions)
{
    // More iterations = more accuracy
    for (int k = 0; k < _iterations; ++k)
    {
        // Loop all cells (excluding boundaries)
        for (int j = 1; j < _gridDimensions - 1; ++j)
        {
            for (int i = 1; i < _gridDimensions - 1; ++i)
            {
                // Each cells diffusion amount is a product of itself and its direct surrounding neighbours using Gauss-Seidel relaxtion
                _x[GetGridIndex(i, j)] = (_xPrev[GetGridIndex(i, j)] + _a *
                                             (_x[GetGridIndex(i + 1, j)] +      // Right
                                              _x[GetGridIndex(i - 1, j)] +      // Left
                                              _x[GetGridIndex(i, j + 1)] +      // Down
                                              _x[GetGridIndex(i, j - 1 )]))     // Up
                                              / _c;
                }
            }
        SetBounds(_b, _x, _gridDimensions);
    }
}

void Fluid::Project(std::vector<float>& _xVel, std::vector<float>& _yVel, std::vector<float>& _p, std::vector<float>& _div, int _iterations, int _gridDimensions)
{
    // Hodge decomposition (incompressible field = current velocities - gradient field)
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
            // Product of left and right neighbour
            _xVel[GetGridIndex(i, j)] -= 0.5f * (_p[GetGridIndex(i + 1, j)] -
                                                 _p[GetGridIndex(i - 1, j)]) * _gridDimensions;
            // Product of top and bottom neighbour
            _yVel[GetGridIndex(i, j)] -= 0.5f * (_p[GetGridIndex(i, j + 1)] -
                                                 _p[GetGridIndex(i, j - 1)]) * _gridDimensions;
        }
    }
    SetBounds(1, _xVel, _gridDimensions);
    SetBounds(2, _yVel, _gridDimensions);
}

void Fluid::Advect(int _b, std::vector<float>& _d, std::vector<float>& _d0,  std::vector<float>& _xVel, std::vector<float>& _yVel, float _timeStep, int _gridDimensions)
{
    // Linear backtracing
    // X
    float x, s0, s1;
    int i0, i1;

    // Y
    float y, t0, t1;
    int j0, j1;

    float timeStep = _timeStep * (_gridDimensions - 2);
    
    // Loop all cells (excluding boundaries)
    for (int j = 1; j < _gridDimensions - 1; ++j)
    { 
        for (int i = 1; i < _gridDimensions - 1; ++i)
        {
            // X
            x = i - (timeStep * _xVel[GetGridIndex(i, j)]);
            if (x < 0.5f)
            {
                x = 0.5f;
            }
            if (x > _gridDimensions + 0.5f)
            {
                x = _gridDimensions + 0.5f;
            }

            i0 = int(x);
            i1 = i0 + 1;
            s1 = x - i0;
            s0 = 1.0f - s1;

            // Y
            y = j - (timeStep * _yVel[GetGridIndex(i, j)]);
            if (y < 0.5f)
            {
                y = 0.5f;
            }
            if (y > _gridDimensions + 0.5f)
            {
                y = _gridDimensions + 0.5f;
            }

            j0 = int(y);
            j1 = j0 + 1;
            t1 = y - j0;
            t0 = 1.0f - t1;
                
            // Cell is a product of itself and its surrounding neighbours
            _d[GetGridIndex(i, j)] = s0 * (t0 * _d0[GetGridIndex(i0, j0)] + t1 * _d0[GetGridIndex(i0, j1)]) +
                                     s1 * (t0 * _d0[GetGridIndex(i1, j0)] + t1 * _d0[GetGridIndex(i1, j1)]);   
        }
    }
    SetBounds(_b, _d, _gridDimensions);
}

void Fluid::SetBounds(int _b, std::vector<float>& _x, int _gridDimensions)
{
    // Sets the velocity of the boundary cells, equal to the reverse incoming velocity (repelling the fluid)

    // Top and bottom cases
    for (int i = 1; i < _gridDimensions - 1; ++i)
    {
        _x[GetGridIndex(i, 0)] = _b == 2 ? -_x[GetGridIndex(i, 1)] : _x[GetGridIndex(i, 1)];
        _x[GetGridIndex(i, _gridDimensions - 1)] = _b == 2 ? -_x[GetGridIndex(i, _gridDimensions - 2)] : _x[GetGridIndex(i, _gridDimensions - 2)];
    }
    // Left and right cases
    for (int j = 1; j < _gridDimensions - 1; ++j)
    {
        _x[GetGridIndex(0, j)] = _b == 1 ? -_x[GetGridIndex(1, j)] : _x[GetGridIndex(1, j)];
        _x[GetGridIndex(_gridDimensions - 1, j)] = _b == 1 ? -_x[GetGridIndex(_gridDimensions - 2, j)] : _x[GetGridIndex(_gridDimensions - 2, j)];
    }
    
    // Corner cases (TL, TR, BL, BR)
    _x[GetGridIndex(0, 0)] = 0.5f * (_x[GetGridIndex(1, 0)] + _x[GetGridIndex(0, 1)]);
    _x[GetGridIndex(0, _gridDimensions - 1)] = 0.5f * (_x[GetGridIndex(1, _gridDimensions - 1)] + _x[GetGridIndex(0, _gridDimensions - 2)]);
    _x[GetGridIndex(_gridDimensions - 1, 0)] = 0.5f * (_x[GetGridIndex(_gridDimensions - 2, 0)] + _x[GetGridIndex(_gridDimensions - 1, 1)]);
    _x[GetGridIndex(_gridDimensions - 1, _gridDimensions - 1)] = 0.5f * (_x[GetGridIndex(_gridDimensions - 2, _gridDimensions - 1)] + _x[GetGridIndex(_gridDimensions - 1, _gridDimensions - 2)]);
}

void Fluid::Fade(float _fadeRate)
{
    for (int i = 0; i < m_density.size(); ++i)
    {
        m_density[i] -= _fadeRate;
        // Constrain density to avoid overflow of RGBA values
        if (m_density[i] < 0)
        {
            m_density[i] = 0;
        }
        else if (m_density[i] > 255)
        {
            m_density[i] = 255;
        }
    }
}

void Fluid::ShowGrid(SDL_Renderer* _renderer)
{
    // Set line colour (red)
    SDL_SetRenderDrawColor(_renderer, 0xFF, 0x00, 0x00, 0xFF);
    for (int x = 0; x < m_gridDimensions; x += m_cellSize)
    {
        // Horizontal
        SDL_RenderDrawLine(_renderer, x * m_cellSize, 0, x * m_cellSize, m_gridDimensions * m_cellSize);
        // Vertical
        SDL_RenderDrawLine(_renderer, 0, x * m_cellSize, m_gridDimensions * m_cellSize, x * m_cellSize);
    }
}

void Fluid::Update()
{
    // Update velocity
    Diffuse(1, m_xVelPrev, m_xVel, m_viscosity, m_timeStep, 4, m_gridDimensions);           // Diffuse velocity
    Diffuse(2, m_yVelPrev, m_yVel, m_viscosity, m_timeStep, 4, m_gridDimensions);           // ...
    Project(m_xVelPrev, m_yVelPrev, m_xVel, m_yVel, 4, m_gridDimensions);                   // Make incompressible
    Advect(1, m_xVel, m_xVelPrev, m_xVelPrev, m_yVelPrev, m_timeStep, m_gridDimensions);    // Trace back original position
    Advect(2, m_yVel, m_yVelPrev, m_xVelPrev, m_yVelPrev, m_timeStep, m_gridDimensions);    // ...
    Project(m_xVel, m_yVel, m_xVelPrev, m_yVelPrev, 4, m_gridDimensions);                   // Make incompressible
    
    // Update density
    Diffuse(0, m_prevDensity, m_density, m_diffusion, m_timeStep, 4, m_gridDimensions);     // Diffuse density
    Advect(0, m_density, m_prevDensity, m_xVel, m_yVel, m_timeStep, m_gridDimensions);      // Trace back original position
}

void Fluid::Draw(SDL_Renderer* _renderer)
{
    for (int y = 0; y < m_gridDimensions; ++y)
    {
        for (int x = 0; x < m_gridDimensions; ++x)
        {
            int xGridPos = x * m_cellSize;
            int yGridPos = y * m_cellSize;
            float density = m_density[GetGridIndex(x, y)];
                
            // Draw cell
            SDL_Rect cell = {xGridPos, yGridPos, m_cellSize, m_cellSize};
            SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_ADD);
			SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, Uint8(density));
			SDL_RenderFillRect(_renderer, &cell);
        }
    }
}

void Fluid::Reset()
{
    // Reset all fluids values back to 0
    m_prevDensity = std::vector<float>(m_gridDimensions * m_gridDimensions, 0);
    m_density = std::vector<float>(m_gridDimensions * m_gridDimensions, 0);
    m_xVelPrev = std::vector<float>(m_gridDimensions * m_gridDimensions, 0);
    m_yVelPrev = std::vector<float>(m_gridDimensions * m_gridDimensions, 0);
    m_xVel = std::vector<float>(m_gridDimensions * m_gridDimensions, 0);
    m_yVel = std::vector<float>(m_gridDimensions * m_gridDimensions, 0);
}

int Fluid::GetGridIndex(int _xPos, int _yPos)
{
    // Strange vector out of bounds happening...
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