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

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <iostream>
#include <QImage>

using namespace std;

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector3D color;
};

//! [0]
GeometryEngine::GeometryEngine(QVector3D terrainColor)
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    //initCubeGeometry();

    // Initializes plane geometry and transfers it to VBOs
    initPlaneGeometry();

    // Initializes the height map geometry with the given color
    initHeightMapGeometry(terrainColor);
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::updateTerrainColor(QVector3D color)
{
    terrainColor = color;
    initHeightMapGeometry(terrainColor);
}

void GeometryEngine::initCubeGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
//! [1]
}

//! [2]
void GeometryEngine::drawCubeGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
}
//! [2]

void GeometryEngine::initPlaneGeometry()
{
    unsigned short nbVertecesInRow = 16;
    VertexData* vertices = new VertexData[nbVertecesInRow*nbVertecesInRow];

    int index = 0;
    for(int x=0; x<nbVertecesInRow; x++){
        for(int y=0; y<nbVertecesInRow; y++){
            vertices[index] = {QVector3D((float)x/nbVertecesInRow,(float)y/nbVertecesInRow,(y%2)/10.0f), QVector2D((float)x/(nbVertecesInRow-1),(float)y/(nbVertecesInRow-1))};
            index++;
        }
    }

    unsigned short nbIndexes = (nbVertecesInRow*2+1)*2+(nbVertecesInRow*2+2)*(nbVertecesInRow-3);
    index = 0;
    GLushort* indices = new GLushort[nbIndexes];
    unsigned short j=0;
    for(unsigned short i=0; i<nbVertecesInRow-1; i++){
        // Duplique l'indice en début de ligne, sauf pour la première
        if(i!=0){
            indices[index] = i*nbVertecesInRow+j;
            index++;
        }
        for(j=0; j<nbVertecesInRow; j++){
            indices[index] = i*nbVertecesInRow+j;
            index++;
            indices[index] = (i+1)*nbVertecesInRow+j;
            index++;
        }
        // Duplique l'indice en fin de ligne, sauf pour la dernière
        if(i!=nbVertecesInRow-2){
            indices[index] = (i+1)*nbVertecesInRow+(j-1);
            index++;
            // Réinitialisation nécessaire pour le premier if
            j=0;
        }
    }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nbVertecesInRow*nbVertecesInRow * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, nbIndexes * sizeof(GLushort));
}

void GeometryEngine::drawPlaneGeometry(QOpenGLShaderProgram *program)
{
    unsigned short nbVertecesInRow = 16;
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Offset for color coordinate
    offset += sizeof(QVector2D);

    int colorAttribute = program->attributeLocation("a_color");
    program->enableAttributeArray(colorAttribute);
    program->setAttributeBuffer(colorAttribute, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, (nbVertecesInRow*2+1)*2 + (nbVertecesInRow*2+2)*(nbVertecesInRow-3), GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::initHeightMapGeometry(QVector3D terrainColor){
    QImage img = QImage("../TP1/heightmap-1.png");
    unsigned short nbVertecesInRow = 16;
    VertexData* vertices = new VertexData[nbVertecesInRow*nbVertecesInRow];

    int index = 0;
    for(int x=0; x<nbVertecesInRow; x++){
        for(int y=0; y<nbVertecesInRow; y++){
            // La composante z du vecteur de position est calculée en fonction du niveau de gris/noir de la heightmap
            float z = img.pixelColor(x*img.width()/(float)nbVertecesInRow,
                                     y*img.height()/(float)nbVertecesInRow).black()/512.f;
            vertices[index] = {QVector3D((float)(x-nbVertecesInRow/2)/nbVertecesInRow,(float)(y-nbVertecesInRow/2)/nbVertecesInRow,z),
                               QVector2D((float)x/(nbVertecesInRow-1),(float)y/(nbVertecesInRow-1)),
                               terrainColor};
            index++;
        }
    }

    unsigned short nbIndexes = (nbVertecesInRow*2+1)*2+(nbVertecesInRow*2+2)*(nbVertecesInRow-3);
    index = 0;
    GLushort* indices = new GLushort[nbIndexes];
    unsigned short j=0;
    for(unsigned short i=0; i<nbVertecesInRow-1; i++){
        // Duplique l'indice en début de ligne, sauf pour la première
        if(i!=0){
            indices[index] = i*nbVertecesInRow+j;
            index++;
        }
        for(j=0; j<nbVertecesInRow; j++){
            indices[index] = i*nbVertecesInRow+j;
            index++;
            indices[index] = (i+1)*nbVertecesInRow+j;
            index++;
        }
        // Duplique l'indice en fin de ligne, sauf pour la dernière
        if(i!=nbVertecesInRow-2){
            indices[index] = (i+1)*nbVertecesInRow+(j-1);
            index++;
            // Réinitialisation nécessaire pour le premier if
            j=0;
        }
    }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nbVertecesInRow*nbVertecesInRow * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, nbIndexes * sizeof(GLushort));
}
