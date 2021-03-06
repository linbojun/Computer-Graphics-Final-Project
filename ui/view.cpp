#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(), m_timer(), m_captureMouse(false), m_defaultOrbitingCamera(new OrbitingCamera()), m_frame_count(0)
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor
    if (m_captureMouse) {
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

}

View::~View()
{
}

void View::initializeGL() {
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
    }
    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    m_scene = std::make_unique<SceneviewScene>();
    //m_terrain = std::make_unique<Terrain>();
    //m_terrain->init();

}

void View::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Implement the demo rendering here
    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
    glViewport(0, 0, width() * ratio, height() * ratio);
    m_defaultOrbitingCamera.get()->setAspectRatio(static_cast<float>(width()) / static_cast<float>(height()));
    m_scene->render(getCamera(), m_time.second(), m_time.msec());
    //m_terrain->render(getCamera());
}

void View::resizeGL(int w, int h) {
    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
    w = static_cast<int>(w / ratio);
    h = static_cast<int>(h / ratio);
    glViewport(0, 0, w, h);
}

void View::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        m_defaultOrbitingCamera.get()->mouseDown(event->x(), event->y());
        m_isDragging = true;
        update();
    }
}

void View::mouseMoveEvent(QMouseEvent *event) {
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    if(m_captureMouse) {
        int deltaX = event->x() - width() / 2;
        int deltaY = event->y() - height() / 2;
        if (!deltaX && !deltaY) return;
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

    }
    if (m_isDragging) {
        m_defaultOrbitingCamera.get()->mouseDragged(event->x(), event->y());
        update();
    }
}

void View::mouseReleaseEvent(QMouseEvent *event) {
    if (m_isDragging && event->button() == Qt::RightButton) {
        m_defaultOrbitingCamera.get()->mouseUp(event->x(), event->y());
        m_isDragging = false;
        update();
    }
}

void View::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
}

void View::wheelEvent(QWheelEvent *event) {
    m_defaultOrbitingCamera.get()->mouseScrolled(event->delta());
    update();
}

void View::keyReleaseEvent(QKeyEvent *event) {

}

void View::tick() {
    // Get the number of seconds since the last tick (variable update rate)
    m_frame_count += 1;
    float seconds = m_time.restart() * 0.001f;
    if (m_frame_count == 60) {
        std::cout << "Current FPS: " << 1.f / seconds << std::endl;
        m_frame_count = 0;
    }

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}

Camera *View::getCamera() {
    return m_defaultOrbitingCamera.get();
}
