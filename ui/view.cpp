#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>
#include <QDir>
//#include <QMouseEvent>
//#include <sstream>
#include <QElapsedTimer>

#include "shapes/sphere.h"
//#include "shapes/cube.h"
//#include "camera/orbitingcamera.h"
#include "lib/resourceloader.h"
//#include "uniforms/varsfile.h"
#include "gl/shaders/shaderattriblocations.h"
//#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(), m_timer(), m_captureMouse(false)
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


    glm::mat4 view = glm::lookAt(glm::vec3(0.5f, 3.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
                                 glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 1024.f / 768.f, 0.1f, 10.f);

    glm::mat4 lightView = glm::lookAt(glm::vec3(1.5f, 2.f, -1.5f), glm::vec3(0.f, 0.f, -1.f),
                                      glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 lightProj = glm::ortho(-3.f, 3.f, -3.f, 3.f, 0.1f, 5.f);
    glm::mat4 lightSpaceMat = lightProj * lightView;


    m_toon_shader = std::make_unique<Shader>(":/shaders/toon.vert", ":/shaders/toon.frag");
    m_toon_shader->bind();
    m_toon_shader->setUniform("view", view);
    m_toon_shader->setUniform("projection", projection);

    // set texture
    m_toon_shader->setUniform("mainTex", 0);

    m_toon_shader->unbind();

    m_water_shader = std::make_unique<Shader>(":/shaders/water.vert", ":/shaders/water.frag");
    m_water_shader->bind();
    m_water_shader->setUniform("view", view);
    m_water_shader->setUniform("projection", projection);

    m_water_shader->setUniform("lightSpaceMatrix", lightSpaceMat);
    m_water_shader->setUniform("shadowMap", 2);
    m_water_shader->setUniform("cameraDepthTexture", 0);
    m_water_shader->setUniform("cameraNormalsTexture", 1);
    m_water_shader->unbind();

    m_outline_shader = std::make_unique<Shader>(":/shaders/outline.vert", ":/shaders/outline.frag");
    m_outline_shader->bind();
    m_outline_shader->setUniform("view", view);
    m_outline_shader->setUniform("projection", projection);
    m_outline_shader->unbind();

    m_shadowmap_shader = std::make_unique<Shader>(":/shaders/shadowmap.vert", ":/shaders/shadowmap.frag");
    m_shadowmap_shader->bind();
    m_shadowmap_shader->setUniform("lightSpaceMatrix", lightSpaceMat);
    m_shadowmap_shader->unbind();


    //std::vector<GLfloat> sphereData = SPHERE_VERTEX_POSITIONS;
    //m_sphere = std::make_unique<OpenGLShape>();
    //m_sphere->setVertexData(&sphereData[0], sphereData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_SPHERE_VERTICES);
    //m_sphere->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    //m_sphere->setAttribute(ShaderAttrib::NORMAL, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    //m_sphere->buildVAO();

    std::cout << QDir::currentPath().toStdString();

    m_quad = std::make_unique<Quad>();
    m_character = std::make_unique<ObjModel>();
    m_toon_diffuse = std::make_shared<Texture2D>();
    m_shadow_map = std::make_shared<ShadowMapping>();

    std::cout << m_character->load(":/models/giraffe.obj") << std::endl;
    std::cout << "Read character texture map:" << m_toon_diffuse->open(":/models/Giraffe_txtr.png") << std::endl;
}

void View::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw character, make it lay down
    glm::mat4 chara_model(1.f);
    chara_model = glm::scale(chara_model, glm::vec3(0.5f));
    chara_model = glm::rotate(chara_model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));

    glm::mat4 water_model(1.f);
    water_model = glm::translate(water_model, glm::vec3(0.f, -0.5f, -1.f));
    water_model = glm::scale(water_model, glm::vec3(5.f));

    // first pass shadowmapping
    glViewport(0, 0, ShadowMapping::SHADOW_MAPPING_WIDTH, ShadowMapping::SHADOW_MAPPING_HEIGHT);
    m_shadowmap_shader->bind();
    m_shadow_map->setup();
    glClear(GL_DEPTH_BUFFER_BIT);
    m_shadowmap_shader->setUniform("model", chara_model);
    m_character->draw();
    m_shadow_map->finish();
    m_shadowmap_shader->unbind();

    // TODO: Implement the demo rendering here
    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_toon_shader->bind();

    m_toon_shader->setUniform("model", chara_model);
    glActiveTexture(GL_TEXTURE0);
    m_toon_diffuse->bind();

    m_character->draw();
    m_toon_diffuse->unbind();
    m_toon_shader->unbind();

    m_outline_shader->bind();
    m_outline_shader->setUniform("model", chara_model);
    glCullFace(GL_FRONT);
    m_character->draw();
    glCullFace(GL_BACK);
    m_outline_shader->unbind();

    // Water part ========================================
    m_water_shader->bind();
    m_water_shader->setUniform("model", water_model);
    glActiveTexture(GL_TEXTURE2);
    m_shadow_map->bindShadowMapping();
    m_water_shader->setUniform("time", static_cast<float>(m_time.elapsed()));
    m_quad->draw();
    m_shadow_map->unbindShadowMapping();
    m_water_shader->unbind();
}

void View::resizeGL(int w, int h) {
    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
    ratio = 1; // use 1 if the scene is fucking smaller on your computer for no fucking reason.s
    w = static_cast<int>(w / ratio);
    h = static_cast<int>(h / ratio);
    glViewport(0, 0, w, h);

    m_viewportWidth = w;
    m_viewportHeight = h;
}

void View::mousePressEvent(QMouseEvent *event) {

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

        // TODO: Handle mouse movements here
    }
}

void View::mouseReleaseEvent(QMouseEvent *event) {

}

void View::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
}

void View::keyReleaseEvent(QKeyEvent *event) {

}

void View::tick() {
    // Get the number of seconds since the last tick (variable update rate)
//    float seconds = m_time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}
