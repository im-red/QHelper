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

#include "normalwatermarkwidget.h"
#include "ui_normalwatermarkwidget.h"
#include "util.h"

#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>

NormalWatermarkWidget::NormalWatermarkWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NormalWatermarkWidget)
    , m_colorDialog(new QColorDialog(this))
    , m_color(Qt::white)
{
    ui->setupUi(this);

    onColorChanged(m_color);
    m_colorDialog->setCurrentColor(m_color);

    connect(m_colorDialog, &QColorDialog::currentColorChanged, this, &NormalWatermarkWidget::onColorChanged);
}

NormalWatermarkWidget::~NormalWatermarkWidget()
{
    delete ui;
}

void NormalWatermarkWidget::on_openButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.bmp *.png *.jpg)"));
    qDebug() << "path:" << path;
    if (path.size() == 0)
    {
        return;
    }
    loadImage(path);
}

void NormalWatermarkWidget::on_saveButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png)"));
    qDebug() << "path:" << path;
    if (path.size() == 0)
    {
        return;
    }
    saveImage(path);
}

void NormalWatermarkWidget::on_colorButton_clicked()
{
    m_colorDialog->show();
}

void NormalWatermarkWidget::on_addWatermarkButton_clicked()
{
    int thickness = ui->thicknessSpinBox->value();
    int opacity = ui->opacitySpinBox->value();
    int angle = (630 - ui->angleDial->value()) % 360;

    if (ui->tabWidget->currentIndex() == TextTab)
    {
        QString text = ui->textLineEdit->text();
        if (text.size() == 0)
        {
            setTextLineEditWarn(ui->textLineEdit, true, tr("Please input watermark text"));
            return;
        }

        setTextLineEditWarn(ui->textLineEdit, false);

        int size = ui->sizeSpinBox->value();
        QColor color = m_colorDialog->currentColor();

        if (updateWatermark(text, size, thickness, opacity, color, angle))
        {
            ui->saveButton->setEnabled(true);
            util::showTip(tr("Add watermark succeeded"));
        }
        else
        {
            ui->saveButton->setEnabled(false);
            util::showTip(tr("Add watermark failed"));
        }
    }
    else
    {
        QString path = ui->watermarkImageLineEdit->text();
        if (path.size() == 0)
        {
            setTextLineEditWarn(ui->watermarkImageLineEdit, true, tr("Please select watermark image"));
            return;
        }

        setTextLineEditWarn(ui->watermarkImageLineEdit, false);

        double scale = ui->scaleSpinBox->value();

        if (updateWatermark(scale, thickness, opacity, angle))
        {
            ui->saveButton->setEnabled(true);
            util::showTip(tr("Add watermark succeeded"));
        }
        else
        {
            ui->saveButton->setEnabled(false);
            util::showTip(tr("Add watermark failed"));
        }
    }
}

void NormalWatermarkWidget::on_openWatermarkButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.bmp *.png *.jpg)"));
    qDebug() << "path:" << path;
    if (path.size() == 0)
    {
        return;
    }
    loadWatermarkImage(path);
}

void NormalWatermarkWidget::loadImage(const QString &path)
{
    if (!QFile::exists(path))
    {
        util::showTip(tr("Invalid path: ") + path);
        return;
    }
    QImage img = QImage(path);
    if (img.format() == QImage::Format_Invalid)
    {
        util::showTip(tr("Invalid image"));
        return;
    }

    util::showTip(tr("Open succeeded: ") + path);

    m_originalImage.swap(img);
    ui->imageView->showImage(m_originalImage);

    ui->addWatermarkButton->setEnabled(true);
    ui->saveButton->setEnabled(false);
}

void NormalWatermarkWidget::saveImage(const QString &path)
{
    if (m_watermarkedImage.save(path, "png"))
    {
        util::showTip(tr("Save succeeded: ") + path);
    }
    else
    {
        util::showTip(tr("Save failed: ") + path);
    }
}

void NormalWatermarkWidget::loadWatermarkImage(const QString &path)
{
    if (!QFile::exists(path))
    {
        util::showTip(tr("Invalid path: ") + path);
        return;
    }
    QImage img = QImage(path);
    if (img.format() == QImage::Format_Invalid)
    {
        util::showTip(tr("Invalid image"));
        return;
    }

    util::showTip(tr("Open succeeded: ") + path);
    m_watermarkImage.swap(img);

    ui->watermarkImageLineEdit->setText(path);
    ui->watermarkImageLineEdit->setToolTip(path);
    ui->watermarkImageLineEdit->setCursorPosition(0);
}

bool NormalWatermarkWidget::updateWatermark(const QString &text, int size, int thickness, int opacity, const QColor &color, int angle)
{
    qDebug() << "text:" << text
             << "size:" << size
             << "thickness:" << thickness
             << "opacity:" << opacity
             << "color:" << color
             << "angle:" << angle;

    m_watermarkedImage = m_originalImage;
    QPainter painter(&m_watermarkedImage);

    QFont font = qApp->font();
    font.setPixelSize(size);
    font.setBold(true);
    painter.setFont(font);

    QColor actualColor = color;
    actualColor.setAlpha(opacity);
    painter.setPen(actualColor);

    int imageWidth = m_originalImage.width();
    int imageHeight = m_originalImage.height();
    painter.translate(imageWidth / 2, imageHeight / 2);

    painter.rotate(-angle);

    QFontMetrics fm(font);
    int textWidth = fm.width(text);
    int textHeight = fm.height();
    int gridWidth = textWidth * 100 / thickness;
    int gridHeight = textHeight * 100 / thickness;
    if (!gridWidth * gridHeight)
    {
        return false;
    }
    int gridColumn = std::max(imageWidth, imageHeight) * 2 / gridWidth + 2;
    int gridRow = std::max(imageWidth, imageHeight) * 2 / gridHeight + 2;
    for (int i = -gridRow / 2; i < gridRow / 2; i++)
    {
        for (int j = -gridColumn / 2; j < gridColumn / 2; j++)
        {
            painter.drawText(j * gridWidth, i * gridHeight, text);
        }
    }

    ui->imageView->showImage(m_watermarkedImage);
    return true;
}

bool NormalWatermarkWidget::updateWatermark(double scale, int thickness, int opacity, int angle)
{
    qDebug() << "scale: " << scale
             << "thickness: " << thickness
             << "opacity: " << opacity
             << "angle: " << angle;

    m_watermarkedImage = m_originalImage;
    QPainter painter(&m_watermarkedImage);

    painter.scale(scale, scale);
    painter.setOpacity(opacity / 255.0);

    int imageWidth = m_originalImage.width() / scale;
    int imageHeight = m_originalImage.height() / scale;
    painter.translate(imageWidth / 2, imageHeight / 2);

    painter.rotate(-angle);

    int watermarkWidth = m_watermarkImage.width();
    int watermarkHeight = m_watermarkImage.height();
    int gridWidth = watermarkWidth * 100 / thickness;
    int gridHeight = watermarkHeight * 100 / thickness;
    if (!gridWidth * gridHeight)
    {
        return false;
    }
    int gridColumn = std::max(imageWidth, imageHeight) * 2 / gridWidth + 2;
    int gridRow = std::max(imageWidth, imageHeight) * 2 / gridHeight + 2;
    for (int i = -gridRow / 2; i < gridRow / 2; i++)
    {
        for (int j = -gridColumn / 2; j < gridColumn / 2; j++)
        {
            painter.drawImage(j * gridWidth, i * gridHeight, m_watermarkImage);
        }
    }

    ui->imageView->showImage(m_watermarkedImage);
    return true;
}

void NormalWatermarkWidget::onColorChanged(const QColor &color)
{
    QString qss = QString("background-color: rgb(%1, %2, %3)")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue());
    ui->colorButton->setStyleSheet(qss);
}

void NormalWatermarkWidget::setTextLineEditWarn(QLineEdit *widget, bool warn, const QString &msg)
{
    QString qss;
    if (warn)
    {
        qss = QString("border: 2px solid rgb(255, 0, 0)");
        widget->setFocus();
        util::showTip(msg);
    }
    else
    {
        util::showTip("");
    }
    widget->setStyleSheet(qss);
}
