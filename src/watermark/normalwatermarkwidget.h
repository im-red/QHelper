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
#include <QLineEdit>

namespace Ui {
class NormalWatermarkWidget;
}

class NormalWatermarkWidget : public QWidget
{
    Q_OBJECT

    enum Tab
    {
        TextTab = 0,
        ImageTab = 1
    };

public:
    explicit NormalWatermarkWidget(QWidget *parent = nullptr);
    ~NormalWatermarkWidget();

private slots:
    void on_openButton_clicked();
    void on_saveButton_clicked();
    void on_colorButton_clicked();
    void on_addWatermarkButton_clicked();
    void on_openWatermarkButton_clicked();

private:
    void loadImage(const QString &path);
    void saveImage(const QString &path);
    void loadWatermarkImage(const QString &path);
    bool updateWatermark(const QString &text, int size, int thickness, int opacity, const QColor &color, int angle);
    bool updateWatermark(double scale, int thickness, int opacity, int angle);
    void onColorChanged(const QColor &color);
    void setTextLineEditWarn(QLineEdit *widget, bool warn, const QString &msg = QString());

private:
    Ui::NormalWatermarkWidget *ui;
    QColorDialog *m_colorDialog;

    QImage m_originalImage;
    QImage m_watermarkedImage;

    QImage m_watermarkImage;

    QString m_text;
    int m_size = 0;
    int m_thickness = 0;

    QColor m_color;
    int m_angle = 0;
};

