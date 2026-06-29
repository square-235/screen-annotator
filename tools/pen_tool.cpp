#include "pen_tool.h"
#include <QPen>
#include <QPainterPath>

PenTool::PenTool(const QColor &color, int thickness)
    : BaseTool(color, thickness)
{
}

QString PenTool::toolName() const
{
    return "pen";
}

void PenTool::handlePress(const QPoint &point)
{
    m_annotation->points.append(point);
}

void PenTool::handleMove(const QPoint &point)
{
    if (m_annotation->points.isEmpty())
        return;
    QPoint lastPoint = m_annotation->points.last();
    if ((point - lastPoint).manhattanLength() < 2)
        return;
    m_annotation->points.append(point);
}

void PenTool::handleRelease(const QPoint &point)
{
    if (m_annotation->points.size() > 0) {
        QPoint lastPoint = m_annotation->points.last();
        if ((point - lastPoint).manhattanLength() >= 2) {
            m_annotation->points.append(point);
        }
    }
}

void PenTool::drawPreview(QPainter &painter)
{
    if (!m_annotation || m_annotation->points.size() < 2)
        return;
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(m_color, m_thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    drawLines(painter, m_annotation->points);
    painter.restore();
}

void PenTool::drawAnnotation(QPainter &painter, const AnnotationPtr &annotation)
{
    if (annotation->points.size() < 2)
        return;
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(annotation->color, annotation->thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    drawLines(painter, annotation->points);
    painter.restore();
}

void PenTool::drawLines(QPainter &painter, const QVector<QPoint> &points)
{
    if (points.size() < 2)
        return;

    QPainterPath path;
    path.moveTo(points[0]);
    for (int i = 1; i < points.size(); ++i) {
        path.lineTo(points[i]);
    }
    painter.drawPath(path);
}
