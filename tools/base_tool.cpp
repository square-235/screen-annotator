#include "base_tool.h"

BaseTool::BaseTool(const QColor &color, int thickness)
    : m_color(color)
    , m_thickness(thickness)
    , m_annotation(nullptr)
    , m_isDrawing(false)
{
}

void BaseTool::setColor(const QColor &color)
{
    m_color = color;
}

QColor BaseTool::getColor() const
{
    return m_color;
}

void BaseTool::setThickness(int thickness)
{
    m_thickness = thickness;
}

int BaseTool::getThickness() const
{
    return m_thickness;
}

bool BaseTool::isDrawing() const
{
    return m_isDrawing;
}

void BaseTool::onPress(const QPoint &point)
{
    m_isDrawing = true;
    m_annotation = std::make_shared<Annotation>(toolName(), m_color, m_thickness);
    handlePress(point);
}

void BaseTool::onMove(const QPoint &point)
{
    if (!m_isDrawing)
        return;
    handleMove(point);
}

AnnotationPtr BaseTool::onRelease(const QPoint &point)
{
    if (!m_isDrawing)
        return nullptr;
    handleRelease(point);
    m_isDrawing = false;
    AnnotationPtr result = m_annotation;
    m_annotation = nullptr;
    return result;
}
