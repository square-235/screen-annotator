#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <QPoint>
#include <QColor>
#include <QVector>
#include <QString>
#include <memory>

class Annotation {
public:
    Annotation();
    Annotation(const QString &toolType, const QColor &color, int thickness);
    Annotation(const QString &toolType, const QColor &color, int thickness, const QVector<QString> &erasedIds);

    QString id;
    QString toolType;
    QColor color;
    int thickness;
    QVector<QPoint> points;
    QPoint startPoint;
    QPoint endPoint;
    bool hasStartPoint;
    bool hasEndPoint;

    QVector<QString> erasedIds;

    bool operator==(const Annotation &other) const;
};

typedef std::shared_ptr<Annotation> AnnotationPtr;

#endif
