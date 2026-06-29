#ifndef PEN_TOOL_H
#define PEN_TOOL_H

#include "base_tool.h"

class PenTool : public BaseTool {
public:
    PenTool(const QColor &color, int thickness);
    ~PenTool() override = default;

    QString toolName() const override;
    void drawPreview(QPainter &painter) override;
    void drawAnnotation(QPainter &painter, const AnnotationPtr &annotation) override;

protected:
    void handlePress(const QPoint &point) override;
    void handleMove(const QPoint &point) override;
    void handleRelease(const QPoint &point) override;

    void drawLines(QPainter &painter, const QVector<QPoint> &points);
};

#endif
