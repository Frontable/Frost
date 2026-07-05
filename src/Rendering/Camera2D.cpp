#include "Camera2D.h"

namespace Frost
{
    Camera2D::Camera2D(int width, int height)
        : m_Width(width), m_Height(height)
    {
        m_Projection = ortho(0.0f, (float)width, (float)height, 0.0f);
        Recalculate();
    }

    const mat4& Camera2D::GetViewProjection()
    {
        if (m_Dirty) Recalculate();
        return m_ViewProjection;
    }

    void Camera2D::Follow(const vec2& target, float lerpFactor)
    {
        vec2 newPos = lerp(m_Pos, target, lerpFactor);
        SetPosition(newPos);
    }

    void Camera2D::Recalculate()
    {
        mat4 translation = translate(vec3{ -m_Pos.x, -m_Pos.y, 0.0f });
        mat4 scaling = scale(vec3{ m_Scale, m_Scale, 1.0f });
        m_ViewProjection = m_Projection * translation * scaling;
        m_Dirty = false;
    }
}