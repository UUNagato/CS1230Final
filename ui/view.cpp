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


View::View(QWidget *parent) :
    QGLWidget(ViewFormat(), parent),
    m_time(),
    m_timer(),
    m_captureMouse(false),
    m_toon_levels(5.0f),
    m_toon_kd(1.0f),
    m_toon_ks(0.2f),
    m_toon_shiny(0.5f),
    m_toon_diffuse_color(glm::vec3(0.3f, 0.8f, 0.1f))
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


    glm::mat4 view = glm::lookAt(glm::vec3(0.8f, 3.5f, -0.2f), glm::vec3(0.f, 0.f, -1.25f),
                                 glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 1024.f / 768.f, 0.1f, 10.f);

    glm::mat4 lightView = glm::lookAt(glm::vec3(1.5f, 2.f, -1.5f), glm::vec3(0.f, 0.f, -1.5f),
                                      glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 lightProj = glm::ortho(-3.f, 3.f, -3.f, 3.f, 0.1f, 5.f);
    glm::mat4 lightSpaceMat = lightProj * lightView;


    m_toon_shader = std::make_unique<Shader>(":/shaders/toon.vert", ":/shaders/toon.frag");
    m_toon_shader->bind();
    m_toon_shader->setUniform("view", view);
    m_toon_shader->setUniform("projection", projection);

    // set texture
    m_toon_shader->setUniform("mainTex", 0);
    m_toon_shader->setUniform("shadowMap", 2);
    m_toon_shader->setUniform("lightSpaceMatrix", lightSpaceMat);

    m_toon_shader->unbind();

    m_water_shader = std::make_unique<Shader>(":/shaders/water.vert", ":/shaders/water.frag");
    m_water_shader->bind();
    m_water_shader->setUniform("view", view);
    m_water_shader->setUniform("projection", projection);

    m_water_shader->setUniform("lightSpaceMatrix", lightSpaceMat);
    m_water_shader->setUniform("shadowMap", 2);
    m_water_shader->setUniform("surfaceNoise", 0);
    m_water_shader->setUniform("surfaceDistortion", 1);
    m_water_shader->setUniform("cameraDepthTexture", 3);
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
    m_swimming_ring = std::make_unique<ObjModel>();
    m_swimming_ring_pattern = std::make_unique<ObjModel>();
    m_toon_diffuse = std::make_shared<Texture2D>();
    m_shadow_map = std::make_shared<ShadowMapping>();

    m_surface_noise = std::make_shared<Texture2D>();
    m_surface_distortion = std::make_shared<Texture2D>();

    std::cout << "Load character Model:" << m_character->load(":/models/giraffe.obj") << "\n";
    std::cout << "Read character texture map:" << m_toon_diffuse->open(":/models/Giraffe_txtr.png") << '\n';
    std::cout << "Load simming ring model:" << m_swimming_ring->load(":/models/swimming_ring.obj") << "\n";
    std::cout << "Load simming ring pattern:" << m_swimming_ring_pattern->load(":/models/swimming_pattern.obj") << "\n";


    m_surface_noise->open(":/models/PerlinNoise.png");
    m_surface_distortion->open(":/models/WaterDistortion.png");
}

void View::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw character, make it lay down
    glm::mat4 chara_model(1.f), ring_model(1.f);
    chara_model = glm::scale(chara_model, glm::vec3(0.5f));
    chara_model = glm::rotate(chara_model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
    chara_model = glm::translate(chara_model, glm::vec3(0.f, 0.f, m_chara_position));
    ring_model = glm::translate(ring_model, glm::vec3(0.f, m_chara_position - 0.7f, -0.85f));
    ring_model = glm::scale(ring_model, glm::vec3(0.06f));

    glm::mat4 water_model(1.f), water_scale_model(1.f);
    water_model = glm::translate(water_model, glm::vec3(0.f, 0.1f, -1.f));
    water_model = glm::scale(water_model, glm::vec3(5.f));
    water_scale_model = glm::scale(water_scale_model, glm::vec3(4.f, 1.5f, 1.f));

    // first pass shadowmapping ==============================================================
    glViewport(0, 0, ShadowMapping::SHADOW_MAPPING_WIDTH, ShadowMapping::SHADOW_MAPPING_HEIGHT);
    m_shadowmap_shader->bind();
    m_shadow_map->setup();
    glClear(GL_DEPTH_BUFFER_BIT);
    m_shadowmap_shader->setUniform("model", chara_model);
    m_character->draw();
    m_shadowmap_shader->setUniform("model", ring_model);
    m_swimming_ring->draw();
    m_swimming_ring_pattern->draw();
    m_shadow_map->finish();
    m_shadowmap_shader->unbind();

    // TODO: Implement the demo rendering here
    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Toon shader ==============================================================
    m_toon_shader->bind();

    m_toon_shader->setUniform("model", chara_model);
    m_toon_shader->setUniform("levels", m_toon_levels);
    m_toon_shader->setUniform("ambientColor", glm::vec3(0.01f));

    m_toon_shader->setUniform("material_kd", m_toon_kd);
    m_toon_shader->setUniform("material_ks", m_toon_ks);
    m_toon_shader->setUniform("material_shininess", m_toon_shiny);
    m_toon_shader->setUniform("useTex", true);
    m_toon_shader->setUniform("diffuseColor", m_toon_diffuse_color);

    glActiveTexture(GL_TEXTURE0);
    m_toon_diffuse->bind();

    glActiveTexture(GL_TEXTURE2);
    m_shadow_map->bindShadowMapping();

    m_character->draw();
    // m_toon_diffuse->unbind();

    // draw swimming ring
    m_toon_shader->setUniform("model", ring_model);
    m_toon_shader->setUniform("useTex", false);
    m_toon_shader->setUniform("mainColor", glm::vec3(0.75f, 0.75f, 0.75f));
    m_swimming_ring->draw();

    m_toon_shader->setUniform("mainColor", glm::vec3(0.8f, 0.2f, 0.2f));
    m_swimming_ring_pattern->draw();

    m_toon_shader->unbind();

    m_outline_shader->bind();
    m_outline_shader->setUniform("model", chara_model);
    glCullFace(GL_FRONT);
    m_character->draw();
    m_outline_shader->setUniform("model", ring_model);
    m_swimming_ring->draw();
    glCullFace(GL_BACK);
    m_outline_shader->unbind();

    // Water part ========================================
    m_water_shader->bind();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_water_shader->setUniform("model", water_model);
    m_water_shader->setUniform("scaleMatrix", water_scale_model);

    glActiveTexture(GL_TEXTURE0);
    m_surface_noise->bind();


    glActiveTexture(GL_TEXTURE1);
    m_surface_distortion->bind();

    glActiveTexture(GL_TEXTURE2);
    m_shadow_map->bindShadowMapping();
    m_water_shader->setUniform("time", static_cast<float>(m_time.elapsed()));
    m_quad->draw();
    m_shadow_map->unbindShadowMapping();
    m_surface_noise->unbind();
    m_surface_distortion->unbind();
    glDisable(GL_BLEND);
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

    float time = m_time.elapsed()/500.f;

    // TODO: Implement the demo update here
    m_chara_position = 0.75f + sin(time) * 0.05f;

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}

void View::updateToonLayer(int value) {
    m_toon_levels = static_cast<float>(value);
}

void View::updateToonKD(int value) {
    m_toon_kd = static_cast<float>(value) / 10.f;
}

void View::updateToonKS(int value) {
    m_toon_ks = static_cast<float>(value) / 10.f;
}

void View::updateToonShiny(int value) {
    m_toon_shiny = static_cast<float>(value) / 10.f;
}

void View::updateDiffuseR(int value) {
    float m_diffuse_r = static_cast<float>(value) / 10.f;
    m_toon_diffuse_color = glm::vec3(m_diffuse_r, m_toon_diffuse_color.y, m_toon_diffuse_color.z);
}

void View::updateDiffuseG(int value) {
    float m_diffuse_g = static_cast<float>(value) / 10.f;
    m_toon_diffuse_color = glm::vec3(m_toon_diffuse_color.x, m_diffuse_g, m_toon_diffuse_color.z);
}

void View::updateDiffuseB(int value) {
    float m_diffuse_b = static_cast<float>(value) / 10.f;
    m_toon_diffuse_color = glm::vec3(m_toon_diffuse_color.x, m_toon_diffuse_color.y, m_diffuse_b);
}
