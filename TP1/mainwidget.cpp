/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"

#include <QMouseEvent>

#include <math.h>

#include <iostream>

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(1.0),
    cam(-0.25,-0.5,0),
    rotationAxis(0,0,1)
{
}

// On surchage le constructeur pour pouvoir prendre en paramètre les fps
MainWidget::MainWidget(int season, int fps, QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(1.0),
    cam(-0.25,-0.5,0),
    rotationAxis(0,0,1),
    fps(fps),
    season(season)
{
    updateSeasonColor();
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}

void MainWidget::updateSeasonColor(){
    switch (season) {
        //Winter
        case 0:
            terrainColor = QVector3D(0.75, 0.75, 0.75);
            break;
        //Spring
        case 1:
            terrainColor = QVector3D(0.25, 1, 0.0);
            break;
        //Summer
        case 2:
            terrainColor = QVector3D(1.0, 0.9, 0.0);
            break;
        //Fall
        default:
            terrainColor = QVector3D(1.0, 0.5, 0.0);
            break;
    }
    program.setUniformValue("texture", 0);
}

void MainWidget::changeSeason(){
    season = (season+1)%4;
    std::cout << "Season " << season << std::endl;
    updateSeasonColor();
    geometries->updateTerrainColor(terrainColor);
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position, QWidget *parent = 0
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    //QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
    QVector3D n = QVector3D(0.0,0.0,1.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]

void MainWidget::wheelEvent(QWheelEvent *event){
    int numDegrees = event->delta() / 8;
    cam.setZ(cam.z()+numDegrees*0.01f);
    update();
}

void MainWidget::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_Left :
            cam.setX(cam.x()-1.0f/10);
        break;
        case Qt::Key_Right :
            cam.setX(cam.x()+1.0f/10);
        break;
        case Qt::Key_Up :
            //cam.setY(cam.y()+1.0f/10);
            angularSpeed += 0.05;
        break;
        case Qt::Key_Down :
            //.setY(cam.y()-1.0f/10);
            angularSpeed -= 0.05;
        break;
    }
    std::cerr << "angular speed : " << angularSpeed << std::endl;
    update();
}

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    // On veut une vitesse constante, pas besoin de diminuer la vitesse
    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    update();
}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]

    geometries = new GeometryEngine(terrainColor);

    // Fixe le nombre de fois que l'event timerEvent est appelé
    // et donc le taux de rafraichissement, en millisecondes
    timer.start(1000/fps, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load heightmap image
    texture = new QOpenGLTexture(QImage("../TP1/heightmap-1.png"));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 1.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    // On change l'angle pour regarder sous un angle de 45 degrés
    QQuaternion framing = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),-45.0);
    matrix.rotate(framing);
    // On applique les modifications apportées par l'utilisateur
    matrix.translate(cam);
    // Rotation à une vitesse constante
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    //geometries->drawCubeGeometry(&program);

    // Draw plane geometry
    geometries->drawPlaneGeometry(&program);
}
