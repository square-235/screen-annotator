#ifndef BASE_TOOL_H
#define BASE_TOOL_H

#include <QPoint>
#include <QPainter>
#include <QColor>
#include <memory>

#include "../models/annotation.h"

class BaseTool {
public:
    BaseTool(const QColor &color, int thickness);
    virtual ~BaseTool() = default;

    void setColor(const QColor &color);
    QColor getColor() const;
    void setThickness(int thickness);
    int getThickness() const;
    bool isDrawing() const;

    void onPress(const QPoint &point);
    void onMove(const QPoint &point);
    AnnotationPtr onRelease(const QPoint &point);

    virtual void drawPreview(QPainter &painter) = 0;
    virtual void drawAnnotation(QPainter &painter, const AnnotationPtr &annotation) = 0;
    virtual QString toolName() const = 0;

protected:
    virtual void handlePress(const QPoint &point) = 0;
    virtual void handleMove(const QPoint &point) = 0;
    virtual void handleRelease(const QPoint &point) = 0;

    QColor m_color;
    int m_thickness;
    AnnotationPtr m_annotation;
    bool m_isDrawing;
};

typedef std::shared_ptr<BaseTool> BaseToolPtr;

#endif
