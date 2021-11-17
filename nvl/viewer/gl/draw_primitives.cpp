#include "draw_primitives.h"

#include <nvl/viewer/gl/opengl_headers.h>
#include <nvl/viewer/gl/draw_gl.h>

namespace nvl {

NVL_INLINE void drawPoint(
        const Point3d& point,
        const Color& color,
        int pointSize)
{
    glPointSize(pointSize);

    glBegin(GL_POINTS);

    glColor(color);
    glVertex(point);

    glEnd();
}

NVL_INLINE void drawLine(
        const Point3d& start,
        const Point3d& end,
        const Color& color,
        int lineSize)
{
    glLineWidth(lineSize);

    glBegin(GL_LINES);

    glColor(color);
    glVertex(start);
    glVertex(end);

    glEnd();
}

NVL_INLINE void drawSphere(
        const Point3d& center,
        double radius,
        const Color& color,
        int slices,
        int stacks)
{
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glColor(color);

    glPushMatrix();

    glTranslate(center);

    GLUquadric *sphere = gluNewQuadric();

    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
    gluSphere(sphere, radius, slices, stacks);

    glPopMatrix();

    gluDeleteQuadric(sphere);
}

NVL_INLINE void drawCylinder(
        const Point3d& a,
        const Point3d& b,
        double startRadius,
        double endRadius,
        const Color &color,
        int slices,
        int stacks)
{
    Vector3d dir = b - a;
    dir.normalize();

    Point3d z = Point3d(0,0,1);

    Vector3d normal = dir.cross(z);
    double angle = acos(dir.dot(z)) * 180 / M_PI;

    bool cull = glIsEnabled(GL_CULL_FACE);

    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glColor(color);

    glPushMatrix();

    glTranslate(a);
    glRotate(-angle, normal);

    GLUquadric *cylinder = gluNewQuadric();
    gluQuadricNormals(cylinder, GLU_SMOOTH);
    gluQuadricOrientation(cylinder, GLU_OUTSIDE);
    gluCylinder(cylinder, startRadius, endRadius, (a-b).norm(), slices, stacks);

    GLUquadric *disk1 = gluNewQuadric();
    gluQuadricNormals(disk1, GLU_SMOOTH);
    gluQuadricOrientation(disk1, GLU_INSIDE);
    gluDisk(disk1, 0, startRadius, slices, stacks);

    glPopMatrix();    

    gluDeleteQuadric(cylinder);
    gluDeleteQuadric(disk1);

    if (cull)
        glEnable(GL_CULL_FACE);
}

NVL_INLINE void drawArrow(
        const Point3d& start,
        const Point3d& end,
        double endRadius,
        const Color& color,
        int slices,
        int stacks)
{
    Point3d midPoint = (start * 1 + end * 9) / 10;
    drawCylinder(start, midPoint, endRadius, endRadius, color, slices, stacks);
    drawCylinder(midPoint, end, endRadius*2, 0, color, slices, stacks);
}

}
