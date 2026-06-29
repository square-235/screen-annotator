#include "history_manager.h"
#include "../config/config.h"

HistoryManager::HistoryManager(QObject *parent)
    : QObject(parent)
    , m_maxSize(Config::UNDO_LIMIT)
{
}

QVector<AnnotationPtr> HistoryManager::annotations() const
{
    return m_undoStack;
}

bool HistoryManager::canUndo() const
{
    return !m_undoStack.isEmpty();
}

bool HistoryManager::canRedo() const
{
    return !m_redoStack.isEmpty();
}

void HistoryManager::addAnnotation(const AnnotationPtr &annotation)
{
    m_undoStack.append(annotation);
    if (m_undoStack.size() > m_maxSize) {
        m_undoStack.removeFirst();
    }
    m_redoStack.clear();
    emit historyChanged();
}

AnnotationPtr HistoryManager::undo()
{
    if (!canUndo())
        return nullptr;
    AnnotationPtr annotation = m_undoStack.last();
    m_undoStack.removeLast();
    m_redoStack.append(annotation);
    emit historyChanged();
    return annotation;
}

AnnotationPtr HistoryManager::redo()
{
    if (!canRedo())
        return nullptr;
    AnnotationPtr annotation = m_redoStack.last();
    m_redoStack.removeLast();
    m_undoStack.append(annotation);
    emit historyChanged();
    return annotation;
}

void HistoryManager::clear()
{
    m_undoStack.clear();
    m_redoStack.clear();
    emit historyChanged();
}
