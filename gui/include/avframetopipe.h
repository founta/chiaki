/*
 * This file is part of Chiaki.
 *
 * Chiaki is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chiaki is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Chiaki.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CHIAKI_AVFRAMETOPIPE_H
#define CHIAKI_AVFRAMETOPIPE_H

#include <QObject>
#include <QOpenGLWidget>

class AVOpenGLWidget;
class VideoDecoder;
class QSurface;

//gets frames and pushes them to a named pipe for consumption by a
//python program
class AVFrameToPipe: public QObject
{
	Q_OBJECT

	private:
		VideoDecoder *decoder;

	private slots:
		void FrameToPipe();

	public:
		AVFrameToPipe(VideoDecoder *decoder);
};

#endif // CHIAKI_AVFRAMETOPIPE_H
