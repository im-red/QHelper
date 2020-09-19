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

#include "mainwindow.h"

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time yyyy-MM-dd h:mm:ss.zzz} [%{type}] (%{file}:%{line}) %{function} - %{message}");

    QApplication a(argc, argv);

    QFontDatabase fd;
    fd.addApplicationFont(":/SourceHanSansCN-Normal.ttf");
    QFont font("Source Han Sans CN", 9);
    a.setFont(font);

    QTranslator translator;
    translator.load(":/QHelper_zh_CN.qm");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
