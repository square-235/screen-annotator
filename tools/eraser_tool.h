#ifndef ERASER_TOOL_H
#define ERASER_TOOL_H

#include "base_tool.h"
#include <QVector>

class EraserTool : public BaseTool {
public:
    EraserTool(const QColor &color, int thickness);
    ~EraserTool() override = default;

    QString toolName() const override;
    void setAnnotations(const QVector<AnnotationPtr> &annotations);
    QVector<AnnotationPtr> getErasedAnnotations() const;

    void drawPreview(QPainter &painter) override;
    void drawAnnotation(QPainter &painter, const AnnotationPtr &annotation) override;

    AnnotationPtr onRelease(const QPoint &point);

protected:
    void handlePress(const QPoint &point) override;
    void handleMove(const QPoint &point) override;
    void handleRelease(const QPoint &point) override;

private:
    int m_eraserSize;
    QVector<int> m_erasedIndices;
    QVector<AnnotationPtr> m_allAnnotations;
    QPoint m_currentPoint;
    bool m_hasCurrentPoint;

    void checkErase(const QPoint &point);
    bool intersects(const QPoint &point, const AnnotationPtr &annotation, double radius);
    double pointToLineDistance(const QPoint &point, const QPoint &lineStart, const QPoint &lineEnd);
};

#endif
