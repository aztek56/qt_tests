//
// Created by Gabriel on 2/16/2019.
//

#ifndef QT_TESTS_VERTEX_H
#define QT_TESTS_VERTEX_H

#include <QVector3D>
#include <qopengl.h>

class Vertex
{
public:
    // Constructors
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector3D &color, const QVector3D &normal);
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector3D &color);
    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D &position);
    Q_DECL_CONSTEXPR Vertex();

    // Accessors / Mutators
    Q_DECL_CONSTEXPR const QVector3D& position() const;
    Q_DECL_CONSTEXPR const QVector3D& color() const;
    Q_DECL_CONSTEXPR const QVector3D& normal() const;
    void setPosition(const QVector3D& position);
    void setColor(const QVector3D& color);
    void setNormal(const QVector3D& normal);

    // OpenGL Helpers
    static const int PositionTupleSize = 3;
    static const int ColorTupleSize = 3;
    static const int NormalTupleSize = 3;
    static Q_DECL_CONSTEXPR int positionOffset();
    static Q_DECL_CONSTEXPR int colorOffset();
    static Q_DECL_CONSTEXPR int normalOffset();
    static Q_DECL_CONSTEXPR int stride();

private:
    QVector3D m_position;
    QVector3D m_color;
    QVector3D m_normal;
};

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position, const QVector3D &color, const QVector3D &normal)
    : m_position(position), m_color(color), m_normal(normal) { }
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position, const QVector3D &color) : Vertex(position, color, QVector3D(0.0f, 0.0f, 0.0f)) {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position) : Vertex(position, QVector3D(0.0f, 0.0f, 0.0f)) {}
Q_DECL_CONSTEXPR inline Vertex::Vertex() : Vertex(QVector3D(0.0f, 0.0f, 0.0f)) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::position() const { return m_position; }
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::color() const { return m_color; }
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::normal() const { return m_normal; }
void inline Vertex::setPosition(const QVector3D& position) { m_position = position; }
void inline Vertex::setColor(const QVector3D& color) { m_color = color; }
void inline Vertex::setNormal(const QVector3D& normal) { m_normal = normal; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int Vertex::positionOffset() { return offsetof(Vertex, m_position); }
Q_DECL_CONSTEXPR inline int Vertex::colorOffset() { return offsetof(Vertex, m_color); }
Q_DECL_CONSTEXPR inline int Vertex::normalOffset() { return offsetof(Vertex, m_normal); }
Q_DECL_CONSTEXPR inline int Vertex::stride() { return 9 * sizeof(GLfloat); }

#endif //QT_TESTS_VERTEX_H
