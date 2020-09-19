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

#pragma once

#include <QWidget>
#include <QColorDialog>

namespace Ui {
class AddWatermarkWidget;
}

class AddWatermarkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddWatermarkWidget(QWidget *parent = nullptr);
    ~AddWatermarkWidget();

private slots:
    void on_openButton_clicked();
    void on_saveButton_clicked();
    void on_colorButton_clicked();
    void on_addWatermarkButton_clicked();

private:
    void loadImage(const QString &path);
    void saveImage(const QString &path);
    bool updateWatermark(const QString &text, int size, int thickness, int opacity, const QColor &color, int angle);
    void onColorChanged(const QColor &color);
    void setTextLineEditWarn(bool warn);

private:
    Ui::AddWatermarkWidget *ui;
    QColorDialog *m_colorDialog;

    QImage m_originalImage;
    QImage m_watermarkedImage;

    QString m_text;
    int m_size = 0;
    int m_thickness = 0;

    QColor m_color;
    int m_angle = 0;
};

