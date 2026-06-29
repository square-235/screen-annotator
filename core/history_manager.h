#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H

#include <QObject>
#include <QVector>
#include "../models/annotation.h"

class HistoryManager : public QObject {
    Q_OBJECT
public:
    explicit HistoryManager(QObject *parent = nullptr);

    QVector<AnnotationPtr> annotations() const;
    bool canUndo() const;
    bool canRedo() const;

public slots:
    void addAnnotation(const AnnotationPtr &annotation);
    AnnotationPtr undo();
    AnnotationPtr redo();
    void clear();

signals:
    void historyChanged();

private:
    QVector<AnnotationPtr> m_undoStack;
    QVector<AnnotationPtr> m_redoStack;
    int m_maxSize;
};

#endif
