/*********************************************************************************
 * Copyright (C) 2020  Jia Lihong
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ********************************************************************************/

#include "view.h"

#include <QKeyEvent>
#include <QDebug>

View::View(QWidget *parent)
    : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    setMouseTracking(true);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void View::showImage(const QImage &image)
{
    m_scene->clear();
    int imageWidth = image.width();
    int imageHeight = image.height();
    m_scene->setSceneRect(0, 0, imageWidth, imageHeight);
    m_scene->addPixmap(QPixmap::fromImage(image));
}

void View::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        if (event->delta() > 0)
        {
            m_scale *= 1.1;
        }
        else
        {
            m_scale *= 0.9;
            m_scale = std::max(m_scale, 0.1);
        }

        QTransform t;
        t.scale(m_scale, m_scale);
        setTransform(t);
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    setFocus();
    QGraphicsView::mouseMoveEvent(event);
}
