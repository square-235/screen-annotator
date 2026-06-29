#include "annotation_overlay.h"
#include "../config/config.h"
#include "../tools/tool_factory.h"
#include "../tools/eraser_tool.h"

#include <QPainter>
#include <QGuiApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QApplication>
#include <QTouchEvent>

AnnotationOverlay::AnnotationOverlay(QWidget *parent)
    : QWidget(parent)
    , m_currentToolName("pen")
    , m_currentColor(Config::DEFAULT_COLOR)
    , m_currentThickness(Config::DEFAULT_THICKNESS)
    , m_currentTool(nullptr)
    , m_history(nullptr)
    , m_toolbar(nullptr)
    , m_cacheDirty(true)
    , m_toolbarParent(true)
{
    initWindow();
    m_history = new HistoryManager(this);
    connect(m_history, &HistoryManager::historyChanged, this, &AnnotationOverlay::onHistoryChanged);

    createTool();
    createToolbar();
}

void AnnotationOverlay::initWindow()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_AcceptTouchEvents);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect geometry = screen->virtualGeometry();
    setGeometry(geometry);
}

void AnnotationOverlay::createTool()
{
    m_currentTool = ToolFactory::createTool(m_currentToolName, m_currentColor, m_currentThickness);
}

void AnnotationOverlay::createToolbar()
{
    m_toolbar = new FloatingToolbar(this);
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int toolbarX = screenGeometry.center().x() - Config::TOOLBAR_WIDTH / 2;
    int toolbarY = screenGeometry.bottom() - Config::TOOLBAR_HEIGHT - Config::TOOLBAR_MARGIN - 20;
    m_toolbar->move(toolbarX, toolbarY);
    m_toolbar->show();
    m_toolbar->raise();

    connect(m_toolbar, &FloatingToolbar::toolChanged, this, &AnnotationOverlay::setTool);
    connect(m_toolbar, &FloatingToolbar::colorChanged, this, &AnnotationOverlay::setColor);
    connect(m_toolbar, &FloatingToolbar::thicknessChanged, this, &AnnotationOverlay::setThickness);
    connect(m_toolbar, &FloatingToolbar::undoRequested, this, &AnnotationOverlay::undo);
    connect(m_toolbar, &FloatingToolbar::redoRequested, this, &AnnotationOverlay::redo);
    connect(m_toolbar, &FloatingToolbar::clearRequested, this, &AnnotationOverlay::clearAll);
    connect(m_toolbar, &FloatingToolbar::minimizeRequested, this, &AnnotationOverlay::minimizeToTray);
    connect(m_toolbar, &FloatingToolbar::exitRequested, this, &AnnotationOverlay::onExitApp);
}

void AnnotationOverlay::minimizeToTray()
{
    m_savedToolbarPos = m_toolbar->mapToGlobal(QPoint(0, 0));
    hide();
    m_toolbar->hide();
    m_toolbarParent = false;
}

void AnnotationOverlay::restoreFromTray()
{
    show();

    QPoint localPos;
    if (!m_savedToolbarPos.isNull()) {
        localPos = mapFromGlobal(m_savedToolbarPos);
    } else {
        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        localPos = QPoint(
            screenGeometry.center().x() - Config::TOOLBAR_WIDTH / 2,
            screenGeometry.bottom() - Config::TOOLBAR_HEIGHT - Config::TOOLBAR_MARGIN - 20
        );
    }

    if (localPos.y() + m_toolbar->height() > height()) {
        localPos.setY(height() - m_toolbar->height() - Config::TOOLBAR_MARGIN);
    }

    m_toolbar->setParent(this);
    m_toolbar->move(localPos);
    m_toolbar->show();
    m_toolbar->raise();

    m_toolbarParent = true;
}

void AnnotationOverlay::setTool(const QString &toolName)
{
    if (m_currentToolName == toolName)
        return;
    m_currentToolName = toolName;
    createTool();
}

void AnnotationOverlay::setColor(const QColor &color)
{
    m_currentColor = color;
    if (m_currentTool) {
        m_currentTool->setColor(color);
    }
}

void AnnotationOverlay::setThickness(int thickness)
{
    m_currentThickness = thickness;
    if (m_currentTool) {
        m_currentTool->setThickness(thickness);
    }
}

void AnnotationOverlay::undo()
{
    m_history->undo();
    m_cacheDirty = true;
    update();
    updateUndoRedoState();
}

void AnnotationOverlay::redo()
{
    m_history->redo();
    m_cacheDirty = true;
    update();
    updateUndoRedoState();
}

void AnnotationOverlay::clearAll()
{
    m_history->clear();
    m_cacheDirty = true;
    update();
    updateUndoRedoState();
}

bool AnnotationOverlay::canUndo() const
{
    return m_history->canUndo();
}

bool AnnotationOverlay::canRedo() const
{
    return m_history->canRedo();
}

void AnnotationOverlay::updateUndoRedoState()
{
    m_toolbar->setUndoEnabled(m_history->canUndo());
    m_toolbar->setRedoEnabled(m_history->canRedo());
}

void AnnotationOverlay::onHistoryChanged()
{
    m_cacheDirty = true;
    update();
}

void AnnotationOverlay::rebuildCache()
{
    if (m_cachedPixmap.isNull() || m_cachedPixmap.size() != size()) {
        m_cachedPixmap = QPixmap(size());
    }
    m_cachedPixmap.fill(Qt::transparent);

    QPainter painter(&m_cachedPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QVector<AnnotationPtr> annotations = m_history->annotations();
    QVector<QString> erasedIds;

    for (const AnnotationPtr &annotation : annotations) {
        if (annotation->toolType == "eraser") {
            for (const QString &id : annotation->erasedIds) {
                erasedIds.append(id);
            }
        }
    }

    for (const AnnotationPtr &annotation : annotations) {
        if (annotation->toolType == "eraser") {
            continue;
        }
        if (erasedIds.contains(annotation->id)) {
            continue;
        }
        BaseToolPtr tool = ToolFactory::createTool(annotation->toolType, annotation->color, annotation->thickness);
        if (tool) {
            tool->drawAnnotation(painter, annotation);
        }
    }
    painter.end();
    m_cacheDirty = false;
}

void AnnotationOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    if (m_cacheDirty) {
        rebuildCache();
    }

    if (!m_cachedPixmap.isNull()) {
        painter.drawPixmap(0, 0, m_cachedPixmap);
    }

    if (m_currentTool && m_currentTool->isDrawing()) {
        m_currentTool->drawPreview(painter);
    }

    painter.end();
}

bool AnnotationOverlay::isOnToolbar(const QPoint &pos) const
{
    if (!m_toolbarParent)
        return false;
    QRect toolbarRect = m_toolbar->geometry();
    return toolbarRect.contains(pos);
}

void AnnotationOverlay::mousePressEvent(QMouseEvent *event)
{
    if (!m_currentTool)
        return;

    QPoint pos = event->position().toPoint();
    if (isOnToolbar(pos))
        return;

    if (event->button() == Qt::LeftButton) {
        if (m_currentToolName == "eraser") {
            EraserTool *eraserTool = dynamic_cast<EraserTool*>(m_currentTool.get());
            if (eraserTool) {
                eraserTool->setAnnotations(m_history->annotations());
            }
        }

        m_currentTool->onPress(pos);
        update();
    }
}

void AnnotationOverlay::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_currentTool)
        return;

    if (m_currentTool->isDrawing()) {
        QPoint pos = event->position().toPoint();
        m_currentTool->onMove(pos);
        update();
    }
}

void AnnotationOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_currentTool)
        return;

    if (event->button() == Qt::LeftButton && m_currentTool->isDrawing()) {
        QPoint pos = event->position().toPoint();
        AnnotationPtr result = m_currentTool->onRelease(pos);

        if (result) {
            if (m_currentToolName == "eraser") {
                handleEraserResult();
            } else {
                m_history->addAnnotation(result);
                m_cacheDirty = true;
                updateUndoRedoState();
            }
        }

        update();
    }
}

void AnnotationOverlay::handleEraserResult()
{
    EraserTool *eraserTool = dynamic_cast<EraserTool*>(m_currentTool.get());
    if (eraserTool) {
        QVector<AnnotationPtr> erased = eraserTool->getErasedAnnotations();
        if (!erased.isEmpty()) {
            QVector<QString> erasedIds;
            for (const AnnotationPtr &e : erased) {
                erasedIds.append(e->id);
            }
            AnnotationPtr eraseAnnotation = std::make_shared<Annotation>(
                "eraser",
                QColor(0, 0, 0),
                Config::ERASER_SIZE,
                erasedIds
            );
            m_history->addAnnotation(eraseAnnotation);
            m_cacheDirty = true;
            updateUndoRedoState();
        }
    }
}

void AnnotationOverlay::resizeEvent(QResizeEvent *event)
{
    m_cacheDirty = true;
    QWidget::resizeEvent(event);
}

void AnnotationOverlay::onExitApp()
{
    QApplication::instance()->quit();
}

void AnnotationOverlay::closeEvent(QCloseEvent *event)
{
    QApplication::instance()->quit();
    QWidget::closeEvent(event);
}

bool AnnotationOverlay::event(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin ||
        event->type() == QEvent::TouchUpdate ||
        event->type() == QEvent::TouchEnd) {

        QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
        if (!m_currentTool)
            return QWidget::event(event);

        const QList<QEventPoint> &touchPoints = touchEvent->points();
        if (touchPoints.isEmpty())
            return QWidget::event(event);

        const QEventPoint &touchPoint = touchPoints.first();
        QPoint pos = touchPoint.position().toPoint();

        if (isOnToolbar(pos))
            return QWidget::event(event);

        switch (touchPoint.state()) {
        case QEventPoint::Pressed: {
            if (m_currentToolName == "eraser") {
                EraserTool *eraserTool = dynamic_cast<EraserTool*>(m_currentTool.get());
                if (eraserTool) {
                    eraserTool->setAnnotations(m_history->annotations());
                }
            }
            m_currentTool->onPress(pos);
            update();
            break;
        }
        case QEventPoint::Updated: {
            if (m_currentTool->isDrawing()) {
                m_currentTool->onMove(pos);
                update();
            }
            break;
        }
        case QEventPoint::Released: {
            if (m_currentTool->isDrawing()) {
                AnnotationPtr result = m_currentTool->onRelease(pos);
                if (result) {
                    if (m_currentToolName == "eraser") {
                        handleEraserResult();
                    } else {
                        m_history->addAnnotation(result);
                        m_cacheDirty = true;
                        updateUndoRedoState();
                    }
                }
                update();
            }
            break;
        }
        default:
            break;
        }

        event->accept();
        return true;
    }

    return QWidget::event(event);
}
