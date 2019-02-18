//
// Created by Gabriel on 2/8/2019.
//
#ifndef LOGO_H
#define LOGO_H

#include <qopengl.h>
#include <QVector>
#include <QVector3D>
#include "vertex.h"

class Logo
{
public:
    Logo();
    const GLfloat *constData() const { return m_core.constData(); }
    int count() const { return  m_data.count() * 9; }
    int vertexCount() const { return m_data.count(); }

private:
    void quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4);
    void extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void add(const QVector3D &v, const QVector3D &n);

    QVector<GLfloat> m_core;
    QVector<Vertex> m_data;
    int m_count;
};

#endif // LOGO_H
