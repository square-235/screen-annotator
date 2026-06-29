#include "eraser_tool.h"
#include "../config/config.h"
#include <QPen>

EraserTool::EraserTool(const QColor &color, int thickness)
    : BaseTool(color, thickness)
    , m_eraserSize(Config::ERASER_SIZE)
    , m_hasCurrentPoint(false)
{
}

QString EraserTool::toolName() const
{
    return "eraser";
}

void EraserTool::setAnnotations(const QVector<AnnotationPtr> &annotations)
{
    m_allAnnotations = annotations;
}

QVector<AnnotationPtr> EraserTool::getErasedAnnotations() const
{
    QVector<AnnotationPtr> result;
    for (int i : m_erasedIndices) {
        if (i >= 0 && i < m_allAnnotations.size()) {
            result.append(m_allAnnotations[i]);
        }
    }
    return result;
}

void EraserTool::handlePress(const QPoint &point)
{
    m_erasedIndices.clear();
    m_currentPoint = point;
    m_hasCurrentPoint = true;
    checkErase(point);
}

void EraserTool::handleMove(const QPoint &point)
{
    m_currentPoint = point;
    m_hasCurrentPoint = true;
    checkErase(point);
}

void EraserTool::handleRelease(const QPoint &point)
{
    m_hasCurrentPoint = false;
    checkErase(point);
}

void EraserTool::checkErase(const QPoint &point)
{
    double radius = m_eraserSize / 2.0;
    for (int i = 0; i < m_allAnnotations.size(); ++i) {
        if (m_erasedIndices.contains(i))
            continue;
        if (intersects(point, m_allAnnotations[i], radius)) {
            m_erasedIndices.append(i);
        }
    }
}

bool EraserTool::intersects(const QPoint &point, const AnnotationPtr &annotation, double radius)
{
    if (!annotation->points.isEmpty()) {
        for (const QPoint &p : annotation->points) {
            if ((p - point).manhattanLength() < radius + annotation->thickness) {
                return true;
            }
        }
    }
    if (annotation->hasStartPoint && annotation->hasEndPoint) {
        if (pointToLineDistance(point, annotation->startPoint, annotation->endPoint) < radius + annotation->thickness) {
            return true;
        }
    }
    return false;
}

double EraserTool::pointToLineDistance(const QPoint &point, const QPoint &lineStart, const QPoint &lineEnd)
{
    QPoint lineVec = lineEnd - lineStart;
    QPoint pointVec = point - lineStart;
    double lineLenSq = lineVec.x() * lineVec.x() + lineVec.y() * lineVec.y();
    if (lineLenSq == 0) {
        return pointVec.manhattanLength();
    }
    double t = qMax(0.0, qMin(1.0, (pointVec.x() * lineVec.x() + pointVec.y() * lineVec.y()) / lineLenSq));
    QPoint projection(
        lineStart.x() + t * lineVec.x(),
        lineStart.y() + t * lineVec.y()
    );
    return (point - projection).manhattanLength();
}

void EraserTool::drawPreview(QPainter &painter)
{
    if (m_hasCurrentPoint && m_isDrawing) {
        double halfSize = m_eraserSize / 2.0;
        double rectX = m_currentPoint.x() - halfSize;
        double rectY = m_currentPoint.y() - halfSize;

        QPen pen(QColor(255, 255, 255, 150), 2);
        pen.setStyle(Qt::DashLine);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(QRectF(rectX, rectY, m_eraserSize, m_eraserSize));

        QPen penInner(QColor(255, 0, 0, 80), 1);
        penInner.setStyle(Qt::SolidLine);
        painter.setPen(penInner);
        painter.drawRect(QRectF(rectX + 2, rectY + 2, m_eraserSize - 4, m_eraserSize - 4));
    }
}

void EraserTool::drawAnnotation(QPainter &painter, const AnnotationPtr &annotation)
{
    Q_UNUSED(painter);
    Q_UNUSED(annotation);
}

AnnotationPtr EraserTool::onRelease(const QPoint &point)
{
    if (!m_isDrawing)
        return nullptr;
    handleRelease(point);
    m_isDrawing = false;
    if (m_erasedIndices.size() > 0) {
        AnnotationPtr result = std::make_shared<Annotation>(
            "eraser",
            QColor(0, 0, 0),
            m_eraserSize
        );
        return result;
    }
    return nullptr;
}
